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
#include "plc_datatype.h"

//#define LOCK_GUARD(lg) const std::lock_guard<std::mutex> lock(lg)
#define MB_SLAVE_CONN_MAX 5
#define MB_NO_DEV_NAME "-"
#define MB_NO_FOLDER "."
// #define USE_SYSLOG

//using namespace std;

using std::string;
using std::mutex;

class PLC_c
{
public:
  // for Master
  // PLC_c(string _ip = "none", string _name = "Master");
  PLC_c(string _devname, string _ip, string _title, string _desc,
        int _port, int _atm, int _ms, int _us);

  // for Slave
  PLC_c(int _port = 502, int _m = 1, string _name = "Slave");
  ~PLC_c();

  void mb_deinit();
  int get_rc_read();
  int get_rc_write();
  uint64_t millis();

  // For Master only
  void init_regs();     // for Master only (mandatory!)
  int read_master();    // for Master only
  int write_master();   // for Master only
  int update_master();  // for Master only
  int refresh_master() { return update_master(); }  // for Master only

  // For Slave only.
  int handle_slave(int usec = 10000);  // for Slave only. Call very often!
  int write_raw(int r, uint16_t val);  // for Slave only
  uint16_t read_raw(int r);            // for Slave only
  int update_slave();  // for Slave only. Copy raw to regs (map).

  // Set/Get local values. NO real update to PLC.
  static uint16_t get_reg(reg_t* rptr);
  uint16_t get_reg(int raddr);
  uint16_t get_reg_by_name(string rname);
  uint16_t get_reg_by_fullname(string rfname);

  static int set_reg(uint16_t rval, reg_t* rptr);
  int set_reg(uint16_t rval, int raddr);
  int set_reg_by_name(uint16_t rval, string rname);
  int set_reg_by_fullname(uint16_t rval, string rfname);

  /*
    int set_reg(int raddr, uint32_t rval);
    int set_by_regname(string rname, uint32_t rval);
    int set_by_fullname(string rname, uint32_t rval);

    int set_reg(int raddr, uint64_t rval);
    int set_by_regname(string rname, uint64_t rval);
    int set_by_fullname(string rname, uint64_t rval);
  */
  // int set_reg(int raddr, float rval);
  // int set_reg(string rname, float rval);

  // float get_reg(string rname);
  // float get_reg(int raddr);

  int regs_used(string rname);  // 0 - 16-bit reg, 1 - 1st reg of 32-bits reg
  int regs_size(int raddr);     // 2 - 2nd reg of 32-bits register

  // Common properties
  bool Enabled = false;
  bool is_slave = false;
  string str_desc;
  string str_top_folder;
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
  std::map<int, reg_t> regs;  // All regs here.

private:
  modbus_t* ctx = nullptr;
  mutex* lock_mux = nullptr;

  // ============
  int rc = -1;
  int rc_read = -1;
  int rc_write = -1;
  int att = 0;

  int mb_ctx();
  int mb_connect();
  int read_allregs();
  int write_reg(reg_t &);
  int set_timeout();
  void init_type(reg_t &);
  void init_str(reg_t &);

  // For Slave only
  modbus_mapping_t* mbm = nullptr;
  uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
  fd_set refset;
  fd_set rdset;

  int server_socket = -1;
  int master_socket;
  int fdmax;

  string to_lower(string str);
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
