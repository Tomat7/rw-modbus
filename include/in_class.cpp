// plc_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// Other (C) https://ru.manpages.org/inotify/7
// https://stackoverflow.com/questions/4664975/monitoring-file-using-inotify
//

#include "./in_class.h"

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
    LOGERR("FD error: %s\n", fname);
    perror("inotify_init1");
    exit(EXIT_FAILURE);
  }

  wd = inotify_add_watch(fd, fname, evt_mask);
  if (wd == -1) {
    LOGERR("WD error: %s\n", fname);
    perror("inotify_add_watch");
    exit(EXIT_FAILURE);
  } else
    LOGINFO("New watching: %s \n", fname);

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
    LOGERR("Error pollig: %s \n", fname);
    perror("poll");
    exit(EXIT_FAILURE);
  }

  if (poll_num > 0) {
    if (fds.revents & POLLIN) // доступны события inotify
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
      LOGERR("Error watching: %s \n", fname);
      perror("read");
      exit(EXIT_FAILURE);
    }
    if (length <= 0)
      break; // ничего не нашли

    // проходим по всем событиям в буфере
    for (ptr = buf; ptr < buf + length;
         ptr += sizeof(struct inotify_event) + event->len) {
      event = (const struct inotify_event*)ptr;

      if ((event->mask & evt_mask) && (event->len)) {
        rc = 1;
        if (event->mask & IN_ISDIR) // печатаем тип события
          LOGINFO("Directory changed: %s \n", event->name);
        else
          LOGINFO("File changed: %s \n", event->name);
      }
    }
  }
  return rc;
}

void INotify::deinit()
{
  inotify_rm_watch(fd, wd);
  close(fd);
  LOGINFO("Stop watching: %s\n", fname);
  closelog();
  return;
}
