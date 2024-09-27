#pragma once

// in_class.h ----------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// Other (c)
// https://ru.manpages.org/inotify/7
//

#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <syslog.h>
#include <unistd.h>

#define USE_SYSLOG

#ifdef USE_SYSLOG
#define LOGERR(...)                                                            \
  fprintf(stderr, __VA_ARGS__);                                                \
  syslog(LOG_ERR, __VA_ARGS__)
#define LOGINFO(...)                                                           \
  printf(__VA_ARGS__);                                                         \
  syslog(LOG_INFO, __VA_ARGS__)
#else
#define LOGERR(...) fprintf(stderr, __VA_ARGS__)
#define LOGINFO(...) printf(__VA_ARGS__)
#endif

// using namespace std;

class INotify {
public:
  INotify(const char *fn = nullptr, uint32_t mask = IN_MODIFY);
  ~INotify();

  void deinit();
  int check();

  //  nfds_t nfds = 2;
  //  struct pollfd fds[2];

  const char *fname = nullptr;
  int fd = 0; // file description
  int wd = 0; // watch description

private:
  int rc = -1;
  int get_event();
  uint32_t evt_mask = 0;
};
