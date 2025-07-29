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
  #include <variant>
*/
#include <map>
#include <vector>
#include <string>

//#include "include/opc_server/opcs_class.h"
#include "include/numeric.h"
#include "include/numeric/number_class.h"
#include "include/plc/plc_class.h"
#include "reg_datatype.h"

#define MB_SLAVE_CONN_MAX 5
//#define USE_SYSLOG

using std::string;
using std::vector;
using std::map;

/*
  using variant_t = std::variant<
                  int16_t, uint16_t,
                  int32_t, uint32_t,
                  int64_t, uint64_t,
                  double, float >;
*/
/*
  struct regprop_t {
  int rtype;
  int rsize;
  byteorder_t rbyteorder;
  };
*/
struct regprop_t : typeprop_t {
  byteorder_t rbyteorder;
};

class Reg_c : public mbreg_t /* , public Number_c */
{
public:
  Reg_c();
  ~Reg_c();
  //  Reg_c(int _fd, regdata_t* _shm, regdata_t* _plc, reg_t* _reg);
  //  Reg_c(const char* _rn, string src_ref);  // for Scada regs.
  //  Reg_c(string _rn, string src_ref);       // for Scada regs.

  // for PLC master
  Reg_c(mbreg_t* _reg, PLC_c* _dev);

  // for Scada regs.
  Reg_c(mbreg_t* _reg, mbreg_t* _src, string _str_source,
        string _str_type, string _opc_base);

  static bool init_types(mbreg_t* _reg);
  static bool check_type(string _st);

  mbreg_t* get_ptr(int x = 0);

  int get_plc_errors();
  numeric_u get_plc_value();
  void set_plc_value(numeric_u _value);

  numeric_u get_local_value();
  void set_local_value(numeric_u _value);

  //string get_local_value_string();
  char* get_local_value_chars(char* retch);
  //variant_t get_local_variant();

  const char* rn = nullptr;  // copy from MB-reg
  string str_fullname = "";  // copy from MB-reg
  string str_opcname = "";   // /PLC/folder/PLC_name/rfolder/PLC_name.reg_name
  // str_source - name of "referenced" MB-reg (ex. DEF.Temp3)
  // "-" mean no reference - Scada calculated reg!
  // "" mean no reference - Modbus reg only!
  string str_source = "";
  // str_type - "name" of variable type: "i", "f", "u", "f100"
  // see "type_map" in reg_init.cpp
  string str_type = "*";

  int var_errors = 0;    // regdata_t.rerrors
  int var_mode_rw = 0;   // 1 - "rw", 0 - "readonly"
  int var_type_ua = 0;   // = Number._type_ua; // for OPC UA server (ex. UA_TYPES_FLOAT)
  int var_size_word = 0; // 1 word = 2 bytes for multiply Modbus registers (ex. 32-bit Float)

//  int var_type = Number._type_ua;   // for OPC UA server (ex. UA_TYPES_FLOAT)
//  size_t &var_size = Number._type_size; // 1;  // for multiply Modbus registers (ex. 32-bit Float)

  const char* var_format = nullptr;
  byteorder_t byte_order = BO_SNGL;// for 32/64-bit Modbus register

  bool visible = false;  // try to hide 2nd/3rd/4th word of multiply MB regs
  bool is_modbus = false; // Modebus register
  bool is_scada = false;  // SCADA-only (local) var/reg
  bool is_ref = false;    // Referenced to Modbus reg(s)


private:

  Number_c Number = (uint16_t)0;
//  numeric_u value; // = *_value; // = Number.value;  // union of values (by type)
  numeric_u* _value = nullptr; //&Number.value;
//  variant_t variant_value;
  badvalue_t bad_value;
  mbreg_t* ptr_reg[4] = {nullptr};  // ptr to Modbus PLC regs

  //void init_mbreg(mbreg_t* _reg, PLC_c* _dev);
  uint16_t get_plc_reg(mbreg_t* rptr);
  uint16_t get_plc_reg(int x = 0);

  void set_plc_reg(uint16_t _val, mbreg_t* rptr);
  void set_plc_reg(uint16_t _val, int x = 0);

  numeric_u pull_plc_regs_by_order(byteorder_t _bo);
  numeric_u pull_plc_value32();
  numeric_u pull_plc_value64();

  bool has_Str(string SS, string fs);  // Look for fs within SS
  char* get_new_char(const char* _oldch);
  void remove_dbl_slashes(string &str);
  string to_lower(string str);

};

//class localreg_t : public mbreg_t
//{
//public:
//   int raddr = 0;
//   mbregdata_t data;
//   mbreg_t* r_next = nullptr;  // keep pointer to next "connected" reg in chain
//   string str_mode = "*";    // "rw", "r", "w" - (read from Config)
// all next params is optional for raw/pure Modbus
// Register name is necessary for naming and good look
//   string str_rname = "-";         // reg_name
//   string rfullname = "";          // PLC_name.reg_name
//   const char* ch_name = nullptr;  // str_rname.c_str()
// OPC name of folder (optional)
// .../PLC_name/rfolder/PLC_name.reg_name (optional)
//   string str_rfolder = "";
// For SCADA/OPC: reg which/where referenced/pointed to Modbus reg
// Reference to external register (optional)
// string str_source = "";
// Type of variable "i", "f", "u", "f100", see "type_map" in reg_init.cpp
//string str_type = "*";
//};

// eof
