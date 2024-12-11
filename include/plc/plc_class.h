#pragma once

// plc_class.h ----------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// Other (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <modbus/modbus.h>
#include <string.h>

#include <atomic>
#include <ctime>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <vector>

#include "include/macros.h"
#include "include/reg/reg_class.h"

//#define LOCK_GUARD(lg) const std::lock_guard<std::mutex> lock(lg)
#define MB_SLAVE_CONN_MAX 5
// #define USE_SYSLOG
//#define LOCK_INIT(m) m = new mutex

using namespace std;

struct mbdata_t {
  int status = 0;                 // rc value of last func (init/connect/read)
  uint64_t timestamp_try_ms = 0;  // milliseconds since the Epoch on last TRY
  uint64_t timestamp_ok_ms = 0;   // ms since the Epoch on last GOOD read
  uint32_t polling_ms = 0;        // milliseconds between read request
  uint32_t timeout_us = 0;        // miCRo seconds (!!) Modbus respose timeout
  uint16_t errors = 0;            // counter of any current ERRORS (reset if OK)
  uint16_t errors_rd = 0;         // counter of READ errors (summ from start)
  uint16_t errors_wr = 0;         // counter of WRITE errors (summ from start)
  uint16_t errors_cn = 0;         // counter of CONNECT errors (summ from start)
};

class PLC_c
{
public:
  // for Master
  PLC_c(string _ip = "none", string _name = "Master");
  PLC_c(string _devname, string _ip, string _title, string _desc, int _port,
        int _atm, int _ms, int _us);

  // for Slave
  PLC_c(int _port = 502, int _m = 1, string _name = "Slave");
  ~PLC_c();

  void mb_deinit();
  int get_rc();
  uint64_t millis();

  // For Master only
  void init_regs();     // for Master only (mandatory!)
  int read_master();    // for Master only
  int write_master();   // for Master only
  int update_master();  // for Master only

  // For Slave only.
  int handle_slave(int usec = 10000);  // for Slave only. Call very often!
  int write_raw(int r, uint16_t val);  // for Slave only
  uint16_t read_raw(int r);            // for Slave only
  int update_slave();  // for Slave only. Copy raw to regs (map).

  // Set/Get local values. NO real update to PLC.
  int set_reg(int raddr, uint16_t rval);
  int set_reg(string rname, uint16_t rval);
  uint16_t get_reg(string rname);
  uint16_t get_reg(int raddr);

  // Common properties
  string str_title;
  string str_desc;
  string str_dev_name;
  string str_ip_addr;
  const char* dev_name = nullptr;
  const char* ip_addr = nullptr;
  int tcp_port = 0;
  int attempts = 2;  // number of attempts
  int reg_min = 0;   // minimal address of reg
  int reg_max = 0;   // maximal address of reg
  int reg_qty = 0;   // number of regs
  mbdata_t mb;
  map<int, reg_t> regs;  // All regs here.

private:
  modbus_t* ctx = nullptr;
  mutex* lock_mux = nullptr;

  // ============
  bool is_slave = false;
  int rc = -1;
  int att = 0;

  int mb_ctx();
  int mb_connect();
  int read_allregs();
  int write_reg(reg_t &);
  int set_timeout();

  // For Slave only
  modbus_mapping_t* mbm = nullptr;
  uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
  fd_set refset;
  fd_set rdset;

  int server_socket = -1;
  int master_socket;
  int fdmax;

  int renew_mapping();  // for Slave only
  int renew_listen();   // for Slave only
  int check_slave();    // for Slave only
  void new_client();    // for Slave only
  void work_client();   // for Slave only
};

//  mutable std::mutex network_mux;
// volatile  atomic <bool> lock { false };
// std::atomic_flag lock = ATOMIC_FLAG_INIT;
/*   volatile atomic_flag* lockflag = nullptr;

  inline __attribute__((always_inline)) void lock_init()
  {
    lockflag = new atomic_flag(ATOMIC_FLAG_INIT);
  };

  inline __attribute__((always_inline)) void lock_now()
  {
    lockflag->test_and_set(std::memory_order_acquire);
  };

  inline __attribute__((always_inline)) void unlock_now()
  {
    lockflag->clear(std::memory_order_release);
  }; */
// ============

// eof
