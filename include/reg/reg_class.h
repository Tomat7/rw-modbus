#pragma once

// reg_class.h ----------------------------------
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

#define MB_SLAVE_CONN_MAX 5
//#define USE_SYSLOG

using namespace std;

struct regdata_t {
  uint16_t rvalue = 0;
  uint16_t rerrors = 0;  // number of errors on MB func (init/connect/read)
  int rupdate = 0;  // 1 - need to write/update remote register, 0 - no update
  int rstatus = 0;  // -1 mean ERROR, any positive - is OK
  int rmode = 0;    // 1 - mean RW, 0 - Read-only
  int rtype = 0;    // 0 - uint16_t, 1 - int16_t, 2 - float (enum?),
};

struct reg_t {
  int raddr = 0;
  regdata_t data;
  string fullname;  // Master and Slave (reference of): PLC_name.reg_name
  string str_name;  // reg_name
  string str_mode;
  string str_type;
  const char* ch_name = nullptr;
};

class Reg_c
{
public:
  Reg_c(int _fd, regdata_t* _shm, regdata_t* _plc, reg_t* _reg);
  Reg_c(reg_t* _reg);      // for PLC master
  Reg_c(const char* _rn);  // for Slave. PLC regs not initialized!
  Reg_c(string _rn);       // for Slave. PLC regs not initialized!
  Reg_c();
  ~Reg_c();

  uint16_t get_plc_val();
  uint16_t get_shm_val();
  uint16_t get_local();
  const char* rn = nullptr;

  void set_plc_val(uint16_t _val);
  void set_shm_val(uint16_t _val);
  void set_local(uint16_t _val);

  void sync(uint16_t _val);
  void sync();
  int get_mode();
  int get_type();
  bool is_shm();

  int fd = -1;                        // descriptor of SHARED MEMORY
  uint16_t value = 0;                 // just for FUN! (to print with PLC & SHM)
  regdata_t* ptr_data_shm = nullptr;  // ptr to SHARED MEMORY (local) data
  regdata_t* ptr_data_plc = nullptr;  // ptr to SHARED MEMORY (PLC/MB) data
  reg_t* ptr_reg = nullptr;           // ptr to PLC reg
};
