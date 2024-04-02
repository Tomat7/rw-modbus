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
#include <mutex>
#include <string>
#include <vector>
#include <map>

#define MB_SLAVE_CONN_MAX 5
#define USE_SYSLOG

using namespace std;

struct reg_t {
  int raddr = 0;
  uint16_t rvalue = 0;
  int rstatus = 0; // -1 mean ERROR, any positive - is OK
  int rerrors = 0; // number of errors on MB func (init/connect/read)
  int rmode = 0;   // 1 - mean RW
  int rtype = 0;   // 1 - mean FLOAT
  int rupdate = 0; // 1 - need to write/update remote register
  string fullname;
  string str_name;
  string str_mode;
  string str_type;
  const char* ch_name = nullptr;
};

struct rdata_t {
  uint16_t rvalue = 0;
  int rstatus = 0; // -1 mean ERROR, any positive - is OK
  int rerrors = 0; // number of errors on MB func (init/connect/read)
  int rmode = 0;   // 1 - mean RW
  int rtype = 0;   // 1 - mean FLOAT
};

struct rmap_t {
  int fd = -1;                // descriptor of SHARED MEMORY
  rdata_t rdata;              // the COPY of PLC data (for memcpy() to SHM)
  rdata_t* ptr_shm = nullptr; // ptr to SHARED MEMORY data
  reg_t* ptr_reg = nullptr;   // ptr to PLC data
};

struct mbdata_t {
  int status = 0;            // rc value of last func (init/connect/read)
  uint64_t timestamp_ms = 0; // milliseconds since the Epoch on last read
  uint32_t interval_ms = 0;  // milliseconds between read request
  uint32_t timeout_us = 0;   // miCRo seconds (!!) Modbus respose timeout
  uint32_t errors = 0;       // counter of any current ERRORS (reset if OK)
  uint32_t errors_rd = 0;    // counter of READ errors (summ from start)
  uint32_t errors_wr = 0;    // counter of WRITE errors (summ from start)
  uint32_t errors_cn = 0;    // counter of CONNECT errors (summ from start)
};

class PLC
{
public:
  PLC(string _ip = "none", string _name = "Master"); // for Master
  PLC(int _port, int _m = 1, string _name = "Slave");            // for Slave
  ~PLC();

  void mb_deinit();
  int get_rc();
  uint64_t millis();

  void init_master();  // for Master only (mandatory!)
  int read_master();   // for Master only
  int write_master();  // for Master only
  int update_master(); // for Master only

  int handle_slave(int usec = 10000); // for Slave only. Need to call very often!
  int write_raw(int r, uint16_t val); // for Slave only
  uint16_t read_raw(int r);           // for Slave only

  string str_title;
  string str_desc;
  string str_dev_name;
  string str_ip_addr;
  const char* dev_name = nullptr;
  const char* ip_addr = nullptr;
  int tcp_port = 0;
  int attempts = 2; // number of attempts
  int reg_min = 0; // minimal address of reg
  int reg_max = 0; // maximal address of reg
  int reg_qty = 0; // number of regs
  mbdata_t mb;
  std::map<int, reg_t> regs;

private:
  modbus_t* ctx = nullptr;
  modbus_mapping_t* mbm = nullptr;
  static mutex logger_mux;

  bool is_slave = false;
  int rc = -1;
  int att = 0;

  fd_set refset;
  fd_set rdset;
  uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
  int server_socket = -1;
  int master_socket;
  int fdmax;

  int mb_ctx();
  int mb_connect();
  int read_allregs();
  int write_reg(reg_t &);
  int set_timeout();

  int renew_mapping();  // for Slave only
  int renew_listen();   // for Slave only
  int check_slave();    // for Slave only
  void new_client();    // for Slave only
  void work_client();   // for Slave only
  void logger(int prio, const char*, ...);
};
