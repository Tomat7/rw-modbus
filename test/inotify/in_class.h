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
  INotify(const char *fn, uint32_t mask = IN_MODIFY);
  //  ~PLC();

  //  int init();
  int check();
  //  int read();
  //  int write();
  //  int update();
  //  int get_rc();
  //  void deinit();
  //  uint64_t millis();

  uint32_t evt_mask = 0;
  nfds_t nfds = 2;
  struct pollfd fds[2];

  const char *fname = nullptr;

  //  const char *dev_desc = nullptr;
  //  const char *dev_name = nullptr;
  //  const char *ip_addr = nullptr;
  //  int tcp_port = 0;
  //  int attempts = 2; // number of attempts

  //  mbdata_t mb;
  int fd = 0; // file description
  int wd = 0; // watch description
  //  int reg_qty = 0; // number of regs

  //  std::vector<reg_t> regs;

private:
  int rc = -1;
  int get_event();
  //  int write_reg(reg_t &);
  //  modbus_t *ctx = nullptr;
};
