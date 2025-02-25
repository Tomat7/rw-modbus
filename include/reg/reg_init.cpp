// reg_init.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include "reg_class.h"

#include <fcntl.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <syslog.h>
#include <unistd.h>

#include <chrono>
#include <map>
#include <mutex>
#include <string>

#include "include/logger.h"
#include "include/plc/plc_class.h"
#include "include/shmem/shmem.h"

#ifdef SYSLOG_NAME
#undef SYSLOG_NAME
#endif
#define SYSLOG_NAME "REG-class"

/* map<string, pair<regtype_t, byteorder_t>> type_map { */

map<string, regprop_t> type_map {
  // See reg_datatype.h for details & comments
  {"*", TYPE_REFERENCED}, // "Referenced" registers
  {"-", TYPE_NOTHING},    // ??
  {"u", TYPE_U16},        {"i", TYPE_I16},        {"f", TYPE_F100},
  {"u16", TYPE_U16},      {"i16", TYPE_I16},      {"f10", TYPE_F10},
  {"hex", TYPE_HEX},      {"bin", TYPE_BINARY},   {"f100", TYPE_F100},

  // High byte first, high word first (Big-endian) as default
  {"u32", TYPE_U32_HH},     {"i32", TYPE_I32_HH},
  {"u64", TYPE_U64_HH},     {"i64", TYPE_I64_HH},
  {"float", TYPE_FLOAT_HH}, {"double", TYPE_DOUBLE_HH},

  // Other types and word/byte orders.  (longnames)
  {"u32hh", TYPE_U32_HH},   {"i32hh", TYPE_I32_HH},
  {"u32hl", TYPE_U32_HL},   {"i32hl", TYPE_I32_HL},
  {"u32lh", TYPE_U32_LH},   {"i32lh", TYPE_I32_LH},
  {"u32ll", TYPE_U32_LL},   {"i32ll", TYPE_I32_LL},

  {"u64hh", TYPE_U64_HH},   {"i64hh", TYPE_I64_HH},
  {"u64hl", TYPE_U64_HL},   {"i64hl", TYPE_I64_HL},
  {"u64lh", TYPE_U64_LH},   {"i64lh", TYPE_I64_LH},
  {"u64ll", TYPE_U64_LL},   {"i64ll", TYPE_I64_LL},

  {"fhh", TYPE_FLOAT_HH},   {"float_abcd", TYPE_FLOAT_HH},
  {"fhl", TYPE_FLOAT_HL},   {"float_cdab", TYPE_FLOAT_HL},
  {"flh", TYPE_FLOAT_LH},   {"float_badc", TYPE_FLOAT_LH},
  {"fll", TYPE_FLOAT_LL},   {"float_dcba", TYPE_FLOAT_LL},

  {"f_abcd", TYPE_FLOAT_HH}, {"d_abcd", TYPE_DOUBLE_HH},
  {"f_cdab", TYPE_FLOAT_HL}, {"d_cdab", TYPE_DOUBLE_HL},
  {"f_badc", TYPE_FLOAT_LH}, {"d_badc", TYPE_DOUBLE_LH},
  {"f_dcba", TYPE_FLOAT_LL}, {"d_dcba", TYPE_DOUBLE_LL},

  {"dhh", TYPE_DOUBLE_HH},  {"double_abcd", TYPE_DOUBLE_HH},
  {"dhl", TYPE_DOUBLE_HL},  {"double_cdab", TYPE_DOUBLE_HL},
  {"dlh", TYPE_DOUBLE_LH},  {"double_badc", TYPE_DOUBLE_LH},
  {"dll", TYPE_DOUBLE_LL},  {"double_dcba", TYPE_DOUBLE_LL},

  {"2nd", TYPE_2ND}, {"2", TYPE_2ND},
  {"3rd", TYPE_3RD}, {"3", TYPE_3RD},
  {"4th", TYPE_4TH}, {"4", TYPE_4TH},

};


Reg_c::~Reg_c() { /* LOGD("DEstruct! %x %s", this, this->rn); */ }

Reg_c::Reg_c() { /* LOGD("Construct! %x", this);  */}

Reg_c::Reg_c(reg_t* _reg, PLC_c* _dev) // For Modbus regs only
{
  string st_ = to_lower(_reg->str_type);
  st_ = "u16";
  if (type_map.count(st_)) {
    var_type = type_map[st_].rtype;
    var_size = type_map[st_].rsize;
    byte_order = type_map[st_].rbyteorder;
    var_mode = _reg->data.rmode;
//    auto &rbo = type_map[st_].rbyteorder;
//    visible = (rbo != BO_2ND) && (rbo != BO_3RD) && (rbo != BO_4TH);
  } else
    LOGE("Wrong type: %s, reg: %s", _reg->str_type.c_str(), rn);

  ptr_reg[0] = _reg;
//  for (int i = 0; i < var_size; i++)
//    ptr_reg[i] = &(_dev->regs[(_reg->raddr) + i]);

  str_source = "";  // flag of Modbus register/tag!
  is_modbus = true; // flag of Modbus register/tag!
  str_fullname = _reg->rfullname;
  rn = str_fullname.c_str();

  str_opcname = "/" + _dev->str_top_folder + "/";
  str_opcname += _dev->str_dev_name + "/";
  str_opcname += _reg->str_rfolder + "/";
  str_opcname += _reg->rfullname;

  for (int i = 0; i < 4; i++)
    remove_dbl_slashes(str_opcname);

  LOGD(" type:%4d sz:%2d bo:%3d rw:%d [%s]", var_type, var_size,
       byte_order, var_mode, str_opcname.c_str());
}


Reg_c::Reg_c(reg_t* _reg, string _opc_base) // For SCADA regs only
{
  if (_reg->str_source == "" || _reg->str_source == "-") {
    str_source = "-";   // flag of SCADA register/tag!
    is_scada = true;
  } else {
    str_source = _reg->str_source;
    is_ref = true;
  }

  str_fullname = _reg->rfullname;
  rn = str_fullname.c_str();
  str_opcname = _opc_base + _reg->str_rfolder + "/" + str_fullname;

  for (int i = 0; i < 4; i++)
    remove_dbl_slashes(str_opcname);

  string st_ = to_lower(_reg->str_type);
  if (type_map.count(st_)) {
    var_type = type_map[st_].rtype;
    var_size = type_map[st_].rsize;         // for SCADA ??
    byte_order = type_map[st_].rbyteorder;  // for SCADA ??
    var_mode = _reg->data.rmode;
    visible = true;
  } else
    LOGE("Wrong type: '%s', reg: '%s'", _reg->str_type.c_str(), rn);

  if (is_ref) {
    for (int i = 0; i < var_size; i++) {
      ptr_reg[i] = _reg;
      _reg = _reg->r_next;
    }
  }

  LOGD(" type:%4d sz:%2d bo:%3d rw:%d [%s]", var_type, var_size,
       byte_order, var_mode, str_opcname.c_str());
}


bool Reg_c::init_types(reg_t* _reg) // !! STATIC FUNCTION !!
{
  bool isOK = false;
  string st_ = _reg->str_type;
  for (auto &c : st_)
    c = static_cast<char>(tolower(c));

  if (type_map.count(st_)) {
    _reg->data.rtype = type_map[st_].rtype;
    _reg->data.rsize = type_map[st_].rsize;
    _reg->data.rbyteorder = type_map[st_].rbyteorder;
    isOK = true;
  } else
    LOGE("Wrong type: '%s', reg: '%s' - IGNORED!",
         st_.c_str(), _reg->str_rname.c_str());

  return isOK;
}

string Reg_c::to_lower(string str)
{
  for (auto &c : str)
    c = static_cast<char>(tolower(c));
  return str;
}

void Reg_c::remove_dbl_slashes(string &str)
{
  auto dbl_slash = str.find("//");
  if (dbl_slash != std::string::npos)
    str.erase(dbl_slash);
}

// eof
