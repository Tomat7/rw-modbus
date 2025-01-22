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

#include "include/regs.h"

#define MB_SLAVE_CONN_MAX 5
//#define USE_SYSLOG

#define TYPE_U16 0
#define TYPE_I16 1
#define TYPE_F100 2
#define TYPE_F10 3

#define TYPE_FLOAT_ABCD 21
#define TYPE_FLOAT_CDAB 22

using namespace std;


class Reg_c
{
public:
  Reg_c(int _fd, regdata_t* _shm, regdata_t* _plc, reg_t* _reg);
  Reg_c(reg_t* _reg);      // for PLC master
  Reg_c(const char* _rn, string src_ref);  // for Scada regs.
  Reg_c(string _rn, string src_ref);       // for Scada regs.
  Reg_c();
  ~Reg_c();

  uint16_t get_plc_val();
  uint16_t get_shm_val();
  uint16_t get_local();

  void set_plc_val(uint16_t _val);
  void set_shm_val(uint16_t _val);
  void set_local(uint16_t _val);

  void sync();
  void sync(uint16_t _val);
  void sync_regdata(regdata_t* prt_data);

  int get_mode(); // 1 = "rw"
  int get_type();
  bool is_shm();
  bool is_MB();
  bool is_Scada();  // Calculated
  bool has_Ref();    // Referenced to Modbus

  // set MODBUS value and return LOCAL
  template <typename T> T set_value(T _val);

  // read LOCAL saved value
  template <typename T> T read_value();

  // ask MODBUS for current value
  template <typename T> T get_value();


  string src_reference;  // keeps "reference" to MB-reg (Tdef = DEF.Temp3)
  // "-" mean no reference - Scada calculated reg
  // "" mean no reference - Modbus reg only

  int fd = -1;                        // descriptor of SHARED MEMORY
  const char* rn = nullptr;           // just for FUN! (copy)
  value_u value;                 // just for FUN! (to print with PLC & SHM)
  regdata_t* ptr_data_shm = nullptr;  // ptr to SHARED MEMORY (local) data
  regdata_t* ptr_data_plc = nullptr;  // ptr to SHARED MEMORY (PLC/MB) data
  reg_t* ptr_reg = nullptr;           // ptr to PLC reg
};

// eof
