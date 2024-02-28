#pragma once

// plc_class.h ----------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// Other (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <modbus/modbus.h>
#include <string.h>

#include <ctime>
#include <iostream>
#include <string>
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
  int raddr = 0;
  int rstatus = 0; // -1 - mean ERROR, any positive - is OK
  uint16_t rvalue = 0;
  int rmode = 0;   // 1 - mean RW
  int rtype = 0;   // 1 - mean FLOAT
  int rupdate = 0; // 1 - need to write/update remote register
  const char *fullname = nullptr;
  const char *ch_name = nullptr;
  const char *ch_mode = nullptr;
  const char *ch_type = nullptr;
};

struct rdata_t {
  int rstatus = 0;
  int rmode = 0;
  int rtype = 0;
  uint16_t rvalue = 0;
};

struct rmap_t {
  int fd = -1;              // descriptor of SHARED MEMORY
  rdata_t *p_shm = nullptr; // ptr to SHARED MEMORY data
  reg_t *p_reg = nullptr;   // ptr to PLC data
  rdata_t rdata;            // the COPY of PLC data (for memcpy() to SHM)
};

class PLC {
public:
  PLC();  // { LOGINFO("+ New PLC created."); }
  ~PLC(); // { deinit(); }

  int init(const char *_ip = "", int _port = 0);
  int set_timeout();
  int connect();
  int read();
  int write();
  void deinit();
  uint64_t millis();

  const char *dev_title = nullptr;
  const char *dev_desc = nullptr;
  const char *dev_name = nullptr;
  const char *ip_addr = nullptr;
  int tcp_port = 0;

  uint64_t mb_timestamp_ms = 0; // milliseconds since the Epoch on last read
  uint32_t mb_interval_ms = 0;  // milliseconds between read request
  uint32_t mb_timeout_us = 0;   // miCRo seconds (!!) Modbus respose timeout
  uint32_t mb_errors = 0;       // counter of any ERROR (reset after OK)
  uint32_t mb_errors_total = 0; // counter of any ERROR (total from start)
  uint32_t mb_status = 0;       // rc value of last func (init/connect/read)

  int reg_min = 0; // minimal address of reg
  int reg_max = 0; // maximal address of reg
  int reg_qty = 0; // number of regs
  std::vector<reg_t> regs;

private:
  int rc = -1;
  int read_allregs();
  int write_reg(reg_t &);
  modbus_t *ctx = nullptr;
};
