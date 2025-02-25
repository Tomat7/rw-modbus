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
//  Reg_c(const char* _rn, string src_ref);  // for Scada regs.
//  Reg_c(string _rn, string src_ref);       // for Scada regs.
  Reg_c(reg_t* _reg, PLC_c* _dev);      // for PLC master
  Reg_c(reg_t* _reg, string _opc_base); // for Scada regs.
  Reg_c();
  ~Reg_c();

  static bool init_types(reg_t* _reg);

  uint16_t get_plc_reg(reg_t* rptr);
  uint16_t get_plc_reg(int x = 0);

  void set_plc_reg(uint16_t _val, reg_t* rptr);
  void set_plc_reg(uint16_t _val, int x = 0);

  int get_plc_errors();

  value_u get_plc_value();
  void set_plc_value(value_u _value);

  value_u get_local_value();
  void set_local_value(value_u _value);

  value_u get_opc_value();
  void set_opc_value(value_u _value);

  //value_u get_scada_value();
  /*  uint16_t get_plc_val();
      uint16_t get_shm_val(); */
//  uint16_t get_local();


//  void set_shm_val(uint16_t _val);
//  void set_local(uint16_t _val);

  //void sync();
  //void sync(value_u _val);
  //void sync_regdata(regdata_t* prt_data);

//  int get_mode(); // 1 = "rw"
//  int get_type();

  //bool is_shm();
//  bool is_MB();
//  bool is_Scada();  // Calculated
//  bool has_Ref();    // Referenced to Modbus

  bool has_Str(string SS, string fs); // Look for fs within SS

  void remove_dbl_slashes(string &str);
  string to_lower(string str);

  // set MODBUS value and return LOCAL
  template <typename T> T set_value(T _val);

  // read LOCAL saved value
  template <typename T> T read_value();

  // ask MODBUS for current value
  template <typename T> T get_value();

//  regdata_t* ptr_data_shm = nullptr;  // ptr to SHARED MEMORY (local) data
//  regdata_t* ptr_data_plc = nullptr;  // ptr to SHARED MEMORY (PLC/MB) data
//  int fd = -1;                        // descriptor of SHARED MEMORY

  const char* rn = nullptr;   // copy from MB-reg
  string str_fullname = "";   // copy from MB-reg
  string str_opcname = "";    // /PLC/folder/PLC_name/rfolder/PLC_name.reg_name
  string str_source = "";  // name of "referenced" MB-reg (ex. DEF.Temp3)
// "-" mean no reference - Scada calculated reg!
// "" mean no reference - Modbus reg only!
//string str_topfolder = "";  // "PLC" or "SCADA" top folder-name

  badvalue_t bad_value;
  value_u value;                 // union of values (by type)
  int var_errors;   // regdata_t.rerrors
  int var_mode;     // 1 - "rw", 0 - "readonly"
  int var_type;     // for OPC UA server (ex. UA_TYPES_FLOAT)
  int var_size;     // for multiply Modbus registers (ex. 32-bit Float)
  int byte_order;   // for 32/64-bit Modbus register (Big-Endian & other)
  bool visible = false; // try to hide 2nd/3rd/4th word of multiply MB regs
  bool is_modbus = false;
  bool is_scada = false;
  bool is_ref = false;  // variable Referenced to Modbus reg(s)

  reg_t* ptr_reg[4] = { nullptr };   // ptr to Modbus PLC reg

};

// eof
