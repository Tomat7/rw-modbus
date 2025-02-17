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

#include "include/plc/plc_class.h"
#include "include/opc/opc_class.h"

#include "reg_datatype.h"

#define MB_SLAVE_CONN_MAX 5
//#define USE_SYSLOG

struct regprop_t {
  int rtype;
  int rsize;
  int rbyteorder;
};


using namespace std;


class Reg_c
{
public:
//  Reg_c(int _fd, regdata_t* _shm, regdata_t* _plc, reg_t* _reg);
  Reg_c(reg_t* _reg, PLC_c* _dev);      // for PLC master
//  Reg_c(const char* _rn, string src_ref);  // for Scada regs.
//  Reg_c(string _rn, string src_ref);       // for Scada regs.
  Reg_c(reg_t* _reg, string _opc_base); // for Scada regs.
  Reg_c();
  ~Reg_c();

  static void init_types(reg_t* _reg);
  /*   uint16_t get_plc_val();
    uint16_t get_shm_val(); */
  uint16_t get_local();

  void set_plc_val(uint16_t _val);
//  void set_shm_val(uint16_t _val);
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
  bool has_Str(string SS, string fs); // Look for fs within SS
  string to_lower(string str);
  void remove_dbl_slashes(string &str);

  // set MODBUS value and return LOCAL
  template <typename T> T set_value(T _val);

  // read LOCAL saved value
  template <typename T> T read_value();

  // ask MODBUS for current value
  template <typename T> T get_value();

//  regdata_t* ptr_data_shm = nullptr;  // ptr to SHARED MEMORY (local) data
//  regdata_t* ptr_data_plc = nullptr;  // ptr to SHARED MEMORY (PLC/MB) data
//  int fd = -1;                        // descriptor of SHARED MEMORY

  const char* rn = nullptr;   // just for FUN! (copy)
  string str_fullname;        // just for FUN! (copy)

  value_u value;                 // union of values (by type)
  int var_type;     // for OPC UA server (ex. UA_TYPES_FLOAT)
  int var_size;     // for multiply Modbus registers (ex. 32-bit Float)
  int byte_order;   // for 32/64-bit Modbus register (Big-Endian & other)
  bool visible = false; // try to hide 2nd/3rd/4th word of multiply MB regs

//  string str_topfolder = "";  // "PLC" or "SCADA" top folder-name
  string str_opcname = "";    // /PLC/folder/PLC_name/rfolder/PLC_name.reg_name
  string src_reference;       // name of "referenced" MB-reg (ex. DEF.Temp3)
  // "-" mean no reference - Scada calculated reg!
  // "" mean no reference - Modbus reg only!

  reg_t* ptr_reg = nullptr;   // ptr to Modbus PLC reg

};

// eof
