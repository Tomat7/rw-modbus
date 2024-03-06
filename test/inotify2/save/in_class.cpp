// plc_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// Other (C) https://ru.manpages.org/inotify/7
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <chrono>
#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <syslog.h>
#include <unistd.h>

#include "./in_class.h"

INotify::~INotify() { deinit(); }

INotify::INotify(const char *fn, uint32_t mask) {

  openlog("INotify", LOG_NDELAY, LOG_LOCAL1);

  if (fn != nullptr) {
    fname = fn;
    evt_mask = mask;
  }

  fd = inotify_init1(IN_NONBLOCK);
  if (fd == -1) {
    LOGERR("FD error: %s\n", fname);
    perror("inotify_init1");
    exit(EXIT_FAILURE);
  }

  wd = inotify_add_watch(fd, fname, evt_mask);
  if (wd == -1) {
    LOGERR("WD error: %s\n", fname);
    perror("inotify_add_watch");
    exit(EXIT_FAILURE);
  } else {
    LOGINFO("New watching file: %s \n", fname);
  }

  /*
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    fds[1].fd = fd;
    fds[1].events = POLLIN;
  */
  return;
}

int INotify::check() {

  rc = 0;
  //  char buf;
  //  printf(".\n");
  //  sleep(10);
  struct pollfd fds = {fd, POLLIN, 0};
  int poll_num = poll(&fds, 1, -1);

  if (poll_num == -1) {
    if (errno == EINTR)
      return rc;
    perror("poll");
    exit(EXIT_FAILURE);
  }

  if (poll_num > 0) {
    /*
        if (fds[0].revents & POLLIN) { // доступен ввод с консоли:
          while (read(STDIN_FILENO, &buf, 1) > 0 && buf != '\n') {
            printf("!\n"); // опустошаем stdin и выходим
            continue;
          }
          return -1;
        }
    */
    if (fds.revents & POLLIN) { // доступны события inotify
      rc = get_event();
    }
  }

  return rc;
}

int INotify::get_event() {

  char buf[4096] __attribute__((aligned(__alignof__(struct inotify_event))));
  const struct inotify_event *event;
  ssize_t length;
  char *ptr;
  rc = 0;

  //  printf(".\n");
  /* проходим по всем событиям, которые можем прочитать
     из файлового дескриптора inotify */
  for (;;) {
    // Читаем несколько событий. Если неблокирующий read()
    // не найдёт событий для чтения, то вернёт -1
    // с errno равным EAGAIN.
    // В этом случае выходим из цикла.

    length = read(fd, buf, sizeof buf);
    if (length == -1 && errno != EAGAIN) {
      LOGERR("Error watching: %s \n", fname);
      perror("read");
      exit(EXIT_FAILURE);
    }
    if (length <= 0)
      break;

    /* проходим по всем событиям в буфере */
    for (ptr = buf; ptr < buf + length;
         ptr += sizeof(struct inotify_event) + event->len) {

      event = (const struct inotify_event *)ptr;

      if (event->mask & evt_mask) { // печатаем тип события
        LOGINFO("File changed: %s \n", fname);
        rc = 1;
      }

      //      if (event->len)
      //        printf("%s", event->name);  // the filename if watching
      //        directory
    }
  }

  return rc;
}

void INotify::deinit() {
  inotify_rm_watch(fd, wd);
  close(fd);
  LOGINFO("Stop watching: %s\n", fname);
  return;
}

/*
int PLC::read() {

  rc = read_allregs();
  mb.status = rc;

  for (auto &r : regs) {
    r.rstatus = rc;
    r.rerrors = mb.errors;
  }

  mb.timestamp_ms = millis();

  return rc;
}

int PLC::read_allregs() {

  int nb_regs = reg_max - reg_min + 1; // WARNING!! May be too much!
  uint16_t *mbregs = new uint16_t[nb_regs];
  rc = 0;

  for (int i = 0; i < attempts && rc <= 0; i++) {
    connect();
    rc = modbus_read_registers(ctx, reg_min, nb_regs, mbregs);
    mb.errors++;
  }

  if (rc == -1) {
    mb.errors++;
    mb.errors_rd++;
    LOGERR("%s %s read error: %s \n", ip_addr, dev_name,
           modbus_strerror(errno));
  } else if (rc != nb_regs) {
    mb.errors++;
    mb.errors_rd++;
    rc = -2;
    LOGERR("%s %s qty regs mismatch: expect %d, got %d\n", ip_addr, dev_name,
           nb_regs, rc);
  } else {
    mb.errors = 0;
    for (auto &r : regs)
      r.rvalue = mbregs[r.raddr - reg_min];
  }

  delete[] mbregs;
  return rc;
}

int PLC::write() {

  for (auto &r : regs)
    rc = write_reg(r);

  return rc;
}

int PLC::write_reg(reg_t &r) {

  if (r.rmode && r.rupdate) {
    rc = 0;
    for (int i = 0; i < attempts && rc <= 0; i++) {
      connect();
      rc = modbus_write_register(ctx, r.raddr, r.rvalue);
      mb.errors++;
    }

    if (rc == -1) {
      mb.errors++;
      mb.errors_wr++;
      LOGERR("%s %s write reg: %s error: %s \n", ip_addr, dev_name, r.ch_name,
             modbus_strerror(errno));
    } else {
      mb.errors = 0;
      r.rupdate = 0;
    }

    r.rstatus = rc;
    r.rerrors = mb.errors;
  }

  return rc;
}

int PLC::update() {

  rc = 0;
  if (millis() - mb.timestamp_ms > mb.interval_ms) {
    rc = write();
    rc = read();
  }
  return rc;
}

int PLC::set_timeout() {

  if (ctx == nullptr)
    init();

  rc = modbus_set_response_timeout(ctx, 0, mb.timeout_us);
  if (rc == -1) {
    LOGERR("%s %s set timeout failed: %s\n", ip_addr, dev_name,
           modbus_strerror(errno));
  }

  return rc;
}

void PLC::deinit() {
  if (ctx != nullptr) {
    LOGINFO("%s %s close and free. \n", ip_addr, dev_name);
    modbus_close(ctx);
    modbus_free(ctx);
  }

  LOGINFO("- PLC deleted: %s. \n", dev_name);
}

uint64_t PLC::millis() {
#define CAST_MILLIS std::chrono::duration_cast<std::chrono::milliseconds>
  uint64_t t;
  t = CAST_MILLIS(std::chrono::system_clock::now().time_since_epoch()).count();
  return t;
}

int PLC::get_rc() { return rc; }
*/