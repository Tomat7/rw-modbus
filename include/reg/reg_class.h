#pragma once

// reg_class.h ----------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// Other (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <modbus/modbus.h>
#include <string.h>

// #include <atomic>
/*
  #include <ctime>
  #include <iostream>
  #include <map>
  #include <mutex>
*/
#include <map>
#include <vector>
#include <string>
#include <variant>

#include "include/opc_server/opcs_class.h"
#include "include/plc/plc_class.h"
#include "reg_datatype.h"

#define MB_SLAVE_CONN_MAX 5
//#define USE_SYSLOG

using std::string;
using std::vector;
using std::map;

using variant_t = std::variant< int16_t, uint16_t,
      int32_t, uint32_t,
      int64_t, uint64_t,
      double, float >;

struct regprop_t {
  int rtype;
  int rsize;
  byteorder_t rbyteorder;
};



class Reg_c
{
public:
  //  Reg_c(int _fd, regdata_t* _shm, regdata_t* _plc, reg_t* _reg);
  //  Reg_c(const char* _rn, string src_ref);  // for Scada regs.
  //  Reg_c(string _rn, string src_ref);       // for Scada regs.
  Reg_c(reg_t* _reg, PLC_c* _dev);                    // for PLC master
  Reg_c(reg_t* _reg, reg_t* _src, string _opc_base);  // for Scada regs.
  Reg_c();
  ~Reg_c();

  static bool init_types(reg_t* _reg);

  reg_t* get_ptr(int x = 0);

  int get_plc_errors();
  value_u get_plc_value();
  void set_plc_value(value_u _value);

  value_u get_local_value();
  void set_local_value(value_u _value);

  //string get_local_value_string();
  char* get_local_value_chars(char* retch);
  variant_t get_local_variant();

  const char* rn = nullptr;  // copy from MB-reg
  string str_fullname = "";  // copy from MB-reg
  string str_opcname = "";   // /PLC/folder/PLC_name/rfolder/PLC_name.reg_name
  string str_source = "";    // name of "referenced" MB-reg (ex. DEF.Temp3)
  // "-" mean no reference - Scada calculated reg!
  // "" mean no reference - Modbus reg only!

  int var_errors = 0;              // regdata_t.rerrors
  int var_mode = 0;                // 1 - "rw", 0 - "readonly"
  int var_type = UA_TYPES_UINT16;  // for OPC UA server (ex. UA_TYPES_FLOAT)
  int var_size = 1;  // for multiply Modbus registers (ex. 32-bit Float)

  const char* var_format = nullptr;
  byteorder_t byte_order = BO_SNGL;// for 32/64-bit Modbus register

  bool visible = false;  // try to hide 2nd/3rd/4th word of multiply MB regs
  bool is_modbus = false; // Modebus register
  bool is_scada = false;  // SCADA-only (local) var/reg
  bool is_ref = false;    // Referenced to Modbus reg(s)

private:
  uint16_t get_plc_reg(reg_t* rptr);
  uint16_t get_plc_reg(int x = 0);

  void set_plc_reg(uint16_t _val, reg_t* rptr);
  void set_plc_reg(uint16_t _val, int x = 0);

  value_u pull_plc_regs_by_order(byteorder_t _bo);
  value_u pull_plc_value32();
  value_u pull_plc_value64();

  bool has_Str(string SS, string fs);  // Look for fs within SS
  char* get_new_char(const char* _oldch);
  void remove_dbl_slashes(string &str);
  string to_lower(string str);

  value_u value;  // union of values (by type)
  variant_t variant_value;
  badvalue_t bad_value;
  reg_t* ptr_reg[4] = {nullptr};  // ptr to Modbus PLC regs

};

// eof
