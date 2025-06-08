// reg_init.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

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
#include "reg_class.h"
//#include "include/shmem/shmem.h"

#ifdef SYSLOG_NAME
#undef SYSLOG_NAME
#endif
#define SYSLOG_NAME "REG-class"

/* map<string, pair<regtype_t, byteorder_t>> type_map { */

map<string, regprop_t> type_map {
  // See reg_datatype.h for details & comments
  {"*", TYPE_REFERENCED},  // "Referenced" registers
  {"-", TYPE_NOTHING},     // ??
  {"u", TYPE_U16},
  {"i", TYPE_I16},
  {"f", TYPE_F100},
  {"u16", TYPE_U16},
  {"i16", TYPE_I16},
  {"f10", TYPE_F10},
  {"hex", TYPE_HEX},
  {"bin", TYPE_BINARY},
  {"f100", TYPE_F100},

  // High byte first, high word first (Big-endian) as default
  {"u32", TYPE_U32_BE},
  {"i32", TYPE_I32_BE},
  {"u64", TYPE_U64_BE},
  {"i64", TYPE_I64_BE},
  {"float", TYPE_FLOAT_BE},
  {"double", TYPE_DOUBLE_BE},

  // Other types and word/byte orders.  (longnames)
  {"u32be", TYPE_U32_BE},
  {"i32be", TYPE_I32_BE},
  {"u32les", TYPE_U32_LES},
  {"i32les", TYPE_I32_LES},
  {"u32bes", TYPE_U32_BES},
  {"i32bes", TYPE_I32_BES},
  {"u32le", TYPE_U32_LE},
  {"i32le", TYPE_I32_LE},

  {"u64hh", TYPE_U64_BE},
  {"i64hh", TYPE_I64_BE},
  {"u64hl", TYPE_U64_LES},
  {"i64hl", TYPE_I64_LES},
  {"u64lh", TYPE_U64_BES},
  {"i64lh", TYPE_I64_BES},
  {"u64ll", TYPE_U64_LE},
  {"i64ll", TYPE_I64_LE},

  {"fhh", TYPE_FLOAT_BE}, // Big-endian
  {"fbe", TYPE_FLOAT_BE},
  {"float_be", TYPE_FLOAT_BE},
  {"float_abcd", TYPE_FLOAT_BE},

  {"flh", TYPE_FLOAT_BES}, // Big-endian Swap
  {"fbes", TYPE_FLOAT_BES},
  {"float_bes", TYPE_FLOAT_BES},
  {"float_badc", TYPE_FLOAT_BES},


  {"fll", TYPE_FLOAT_LE}, // Little-endian
  {"fle", TYPE_FLOAT_LE},
  {"float_le", TYPE_FLOAT_LE},
  {"float_dcba", TYPE_FLOAT_LE},

  {"fhl", TYPE_FLOAT_LES}, // Little-endian Swap
  {"fles", TYPE_FLOAT_LES},
  {"float_les", TYPE_FLOAT_LES},
  {"float_cdab", TYPE_FLOAT_LES},


  {"f_abcd", TYPE_FLOAT_BE},
  {"d_abcd", TYPE_DOUBLE_BE},

  {"f_cdab", TYPE_FLOAT_LES},
  {"d_cdab", TYPE_DOUBLE_LES},

  {"f_badc", TYPE_FLOAT_BES},
  {"d_badc", TYPE_DOUBLE_BES},

  {"f_dcba", TYPE_FLOAT_LE},
  {"d_dcba", TYPE_DOUBLE_LE},

  {"dhh", TYPE_DOUBLE_BE},
  {"dbe", TYPE_DOUBLE_BE},
  {"double_be", TYPE_DOUBLE_BE},
  {"double_abcd", TYPE_DOUBLE_BE},

  {"dhl", TYPE_DOUBLE_LES},
  {"dles", TYPE_DOUBLE_LES},
  {"double_les", TYPE_DOUBLE_LES},
  {"double_cdab", TYPE_DOUBLE_LES},

  {"dlh", TYPE_DOUBLE_BES},
  {"dbes", TYPE_DOUBLE_BES},
  {"double_bes", TYPE_DOUBLE_BES},
  {"double_badc", TYPE_DOUBLE_BES},

  {"dll", TYPE_DOUBLE_LE},
  {"dle", TYPE_DOUBLE_LE},
  {"double_le", TYPE_DOUBLE_LE},
  {"double_dcba", TYPE_DOUBLE_LE},

  {"2nd", TYPE_2ND},
  {"2", TYPE_2ND},
  {"3rd", TYPE_3RD},
  {"3", TYPE_3RD},
  {"4th", TYPE_4TH},
  {"4", TYPE_4TH},

};

Reg_c::~Reg_c() { /* LOGD("DEstruct! %x %s", this, this->rn); */ }

Reg_c::Reg_c() { /* LOGD("Construct! %x", this);  */ }

Reg_c::Reg_c(reg_t* _reg, PLC_c* _dev)  // For Modbus regs only
{
  string st_ = to_lower(_reg->str_type);
  st_ = "u16";
  if (type_map.count(st_)) {
    var_mode = _reg->data.rmode;
    var_type = type_map[st_].rtype;
    var_size = type_map[st_].rsize;
    byte_order = type_map[st_].rbyteorder;
  } else
    LOGE("Wrong type: %s, reg: %s", _reg->str_type.c_str(), rn);

  ptr_reg[0] = _reg;
  //  for (int i = 0; i < var_size; i++)
  //    ptr_reg[i] = &(_dev->regs[(_reg->raddr) + i]);

  str_source = "";   // flag of Modbus register/tag!
  is_modbus = true;  // flag of Modbus register/tag!
  str_fullname = _reg->rfullname;
  rn = get_new_char(str_fullname.c_str());

  str_opcname = "/" + _dev->str_top_folder + "/";
  str_opcname += _dev->str_dev_name + "/";
  str_opcname += _reg->str_rfolder + "/";
  str_opcname += _reg->rfullname;

  for (int i = 0; i < 4; i++)
    remove_dbl_slashes(str_opcname);

  set_local_value(get_plc_value());

  LOGI("%s type:%4d sz:%2d bo:%3d rw:%d val: %u [%s]", rn, var_type, var_size,
       byte_order, var_mode, value.ui16, str_opcname.c_str());
}

Reg_c::Reg_c(reg_t* _reg, reg_t* _src, string _opc_base)  // For SCADA regs only
{
  if (_reg->str_source == "" || _reg->str_source == "-") {
    str_source = "-";  // flag of SCADA register/tag!
    is_scada = true;
  } else {
    str_source = _reg->str_source;
    is_ref = true;
  }

  str_fullname = _reg->rfullname;
  rn = get_new_char(str_fullname.c_str());
  str_opcname = _opc_base + _reg->str_rfolder + "/" + str_fullname;

  for (int i = 0; i < 4; i++)
    remove_dbl_slashes(str_opcname);

  string st_ = to_lower(_reg->str_type);
  if (type_map.count(st_)) {
    var_mode = (_reg->str_mode == "rw") ? 1 : 0;
    var_type = type_map[st_].rtype;
    var_size = type_map[st_].rsize;         // for SCADA ??
    byte_order = type_map[st_].rbyteorder;  // for SCADA ??
    visible = true;
  } else
    LOGE("Wrong type: '%s', reg: '%s'", _reg->str_type.c_str(), rn);

  if (is_ref) {
    if (_src != nullptr) {
      for (int i = 0; i < var_size; i++) {
        ptr_reg[i] = _src;
        _src = _src->r_next;
      }
    } else
      LOGE("Wrong pointer to 'source' on REF reg: %s", rn);
  }

  if (is_ref)
    set_local_value(get_plc_value());
  else
    value.ui16 = _reg->data.rvalue;

  char ch[50];
  LOGI("%-7s type:%4d sz:%2d bo:%3d rw:%d val: %14s [%s]", rn, var_type,
       var_size, byte_order, var_mode, get_local_value_chars(ch),
       str_opcname.c_str());
}

bool Reg_c::init_types(reg_t* _reg)  // !! STATIC FUNCTION !!
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
    LOGE("Wrong type: '%s', reg: '%s' - IGNORED!", st_.c_str(),
         _reg->str_rname.c_str());

  return isOK;
}

char* Reg_c::get_new_char(const char* _oldch)
{
  char* _newch = new char[1 + strlen(_oldch)];
  strcpy(_newch, _oldch);
  return _newch;
}

// eof
