// plc_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// Other (C) https://ru.manpages.org/inotify/7
// https://stackoverflow.com/questions/4664975/monitoring-file-using-inotify
//

#include "include/inotify/in_class.h"

#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <syslog.h>
#include <unistd.h>

#include <chrono>

INotify::~INotify() { deinit(); }

INotify::INotify(const char* fn, uint32_t mask)
{
  openlog("INotify", LOG_NDELAY, LOG_LOCAL1);

  if (fn != nullptr) {
    fname = fn;
    evt_mask = mask;
  }

  fd = inotify_init1(IN_NONBLOCK);
  if (fd == -1) {
    LOGE("FD error: %s", fname);
    perror("inotify_init1");
    exit(EXIT_FAILURE);
  }

  const char* wobj = "directory";
  wd = inotify_add_watch(fd, fname, evt_mask | IN_ONLYDIR);
  if ((wd == -1) && (errno == ENOTDIR)) {
    is_file = true;
    wobj = "file";
    wd = inotify_add_watch(fd, fname, evt_mask);
  }

  if (wd == -1) {
    LOGE("WD error: %s", fname);
    perror("inotify_add_watch");
    exit(EXIT_FAILURE);
  } else
    LOGI("New watching %s: %s ", wobj, fname);

  return;
}

int INotify::check()
{
  rc = 0;
  struct pollfd fds = {fd, POLLIN, 0};
  int poll_num = poll(&fds, 1, -1);

  if (poll_num == -1) {
    if (errno == EINTR)
      return rc;
    LOGE("Error pollig: %s ", fname);
    perror("poll");
    exit(EXIT_FAILURE);
  }

  if (poll_num > 0) {
    if (fds.revents & POLLIN)  // доступны события inotify
      rc = get_event();
  }

  return rc;
}

int INotify::get_event()
{
  char buf[4096] __attribute__((aligned(__alignof__(struct inotify_event))));
  const struct inotify_event* event;
  ssize_t length;
  char* ptr;
  rc = 0;
  // проходим по всем событиям, которые прочитаем из файлового дескриптора IN
  for (;;) {
    // Читаем несколько событий. Если неблокирующий read() не найдёт
    // событий для чтения, то вернёт -1 с errno равным EAGAIN.
    length = read(fd, buf, sizeof buf);

    if (length == -1 && errno != EAGAIN) {
      LOGE("Error watching: %s ", fname);
      perror("read");
      exit(EXIT_FAILURE);
    }
    if (length <= 0)
      break;  // ничего не нашли

    // проходим по всем событиям в буфере
    for (ptr = buf; ptr < buf + length;
         ptr += sizeof(struct inotify_event) + event->len) {
      event = (const struct inotify_event*)ptr;

      if (event->mask & evt_mask) {
        rc = 1;
        LOGW("+ %d ", evt_mask);

        if (is_file)
          LOGN("File changed: %s.", fname);

        if (event->len) {
          if (event->mask & IN_ISDIR)  // печатаем тип события
            LOGN("Directory changed: %s ", event->name);
          else
            LOGN("File changed: %s ", event->name);
        }
      }
    }
  }
  return rc;
}

void INotify::deinit()
{
  inotify_rm_watch(fd, wd);
  close(fd);
  LOGN("Stop watching: %s", fname);
  closelog();
  return;
}
