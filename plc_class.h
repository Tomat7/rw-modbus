#pragma once

// plc_class.h ----------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// Other (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <modbus/modbus.h>

#include <ctime>
#include <iostream>
#include <vector>

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

struct reg_t {
  int raddr;
  int rstatus;
  const char* rname;
  const char* rmode;
  const char* rtype;
  uint16_t rvalue;
};

class PLC {
 public:
  PLC();    // { LOGINFO("+ New PLC created."); }
  ~PLC();   // { deinit(); }

  int init(const char* _ip = "", int _port = 0);
  int set_timeout();
  int connect();
  int read();
  void deinit();
  uint64_t millis();

  const char* dev_title = nullptr;
  const char* dev_desc = nullptr;
  const char* dev_name = nullptr;
  const char* ip_addr = nullptr;
  int tcp_port = 0;

  uint64_t mb_timestamp_ms = 0;     // milliseconds since the Epoch on last read
  uint32_t mb_interval_ms = 0;      // milliseconds between read request
  uint32_t mb_timeout_us = 0;       // miCRo seconds (!!) Modbus respose timeout
  uint32_t mb_errors = 0;           // counter of any ERROR
  uint32_t mb_status = 0;           // rc value of last func (init/connect/read)

  int reg_min = 0;  // minimal address of reg
  int reg_max = 0;  // maximal address of reg
  int reg_qty = 0;  // number of regs
  std::vector<reg_t> regs;

 private:
  int rc = 0;
  int read_mb();
  modbus_t* ctx = nullptr;
};
