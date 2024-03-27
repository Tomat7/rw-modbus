#pragma once

// plc_class.h ----------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// Other (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <modbus/modbus.h>
#include <string.h>
#include <stdarg.h>

#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <mutex>

#define USE_SYSLOG

//#include "./macros.h"
/*
    #ifdef USE_SYSLOG
    #define LOGERR(...) (fprintf(stderr, __VA_ARGS__), syslog(LOG_ERR,
    __VA_ARGS__)) #define LOGINFO(...) (printf(__VA_ARGS__), syslog(LOG_INFO,
    __VA_ARGS__)) #else #define LOGERR(...) fprintf(stderr, __VA_ARGS__) #define
    LOGINFO(...) printf(__VA_ARGS__) #endif
*/

using namespace std;

struct reg_t {
  uint16_t rvalue = 0;
  int raddr = 0;
  int rstatus = 0;  // -1 mean ERROR, any positive - is OK
  int rerrors = 0;  // number of errors on MB func (init/connect/read)
  int rmode = 0;    // 1 - mean RW
  int rtype = 0;    // 1 - mean FLOAT
  int rupdate = 0;  // 1 - need to write/update remote register
  string fullname;
  string str_name;
  string str_mode;
  string str_type;
  const char* ch_name = nullptr;
};

struct rdata_t {
  uint16_t rvalue = 0;
  int rstatus = 0;  // -1 mean ERROR, any positive - is OK
  int rerrors = 0;  // number of errors on MB func (init/connect/read)
  int rmode = 0;    // 1 - mean RW
  int rtype = 0;    // 1 - mean FLOAT
};

struct rmap_t {
  int fd = -1;                 // descriptor of SHARED MEMORY
  rdata_t rdata;               // the COPY of PLC data (for memcpy() to SHM)
  rdata_t* ptr_shm = nullptr;  // ptr to SHARED MEMORY data
  reg_t* ptr_reg = nullptr;    // ptr to PLC data
};

struct mbdata_t {
  int status = 0;             // rc value of last func (init/connect/read)
  uint64_t timestamp_ms = 0;  // milliseconds since the Epoch on last read
  uint32_t interval_ms = 0;   // milliseconds between read request
  uint32_t timeout_us = 0;    // miCRo seconds (!!) Modbus respose timeout
  uint32_t errors = 0;        // counter of any current ERRORS (reset if OK)
  uint32_t errors_rd = 0;     // counter of READ errors (summ from start)
  uint32_t errors_wr = 0;     // counter of WRITE errors (summ from start)
  uint32_t errors_cn = 0;     // counter of CONNECT errors (summ from start)
};

class PLC
{
public:
  PLC(string _ip = "X", string _name = "Master");   // for Master
  PLC(int _port, string _name = "Slave");   // for Slave
  ~PLC();  //

  void init_master();   // for Master only (mandatory!)
  int read_master();    // for Master only
  int write_master();   // for Master only
  int update_master();  // for Master only
  int check_slave();   // for Slave only. Need to call very often!
  int get_rc();
  void deinit();
  uint64_t millis();

  string str_title;
  string str_desc;
  string str_dev_name;
  string str_ip_addr;
  const char* dev_name = nullptr;
  const char* ip_addr = nullptr;

  mbdata_t mb;
  int tcp_port = 0;
  int attempts = 2;  // number of attempts

  int reg_min = 0;  // minimal address of reg
  int reg_max = 0;  // maximal address of reg
  int reg_qty = 0;  // number of regs

  std::vector<reg_t> regs;

private:
  modbus_t* ctx = nullptr;
  bool is_slave = false;
  int rc = -1;
  int mb_new_master();
  int mb_new_slave();
  int mb_connect();
  int read_allregs();
  int write_reg(reg_t &);
  int set_timeout();
  void logger(int prio, const char*, ...);
  void loginfo(const char*, ...);
  static mutex logger_mux;
};
