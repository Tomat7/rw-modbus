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

/* map<string, regprop_t> type_map {
  {"*", TYPE_REFERENCED }}; */

Reg_c::~Reg_c() { /* LOGD("DEstruct! %x %s", this, this->rn); */ }

Reg_c::Reg_c() { /* LOGD("Construct! %x", this);  */}

Reg_c::Reg_c(string _rn, string src_ref) { Reg_c(_rn.c_str(), src_ref); }

Reg_c::Reg_c(const char* _rn, string src_ref)
{
  rn = get_new_char(_rn);
  src_reference = src_ref;
  // LOGD("try to open %s", rn);

  /*   fd = create_shm_fd(rn);
    if (fd != -1) {
      ptr_data_shm = (regdata_t*)create_shm_addr(fd, sizeof(regdata_t));
      if (ptr_data_shm != nullptr) {
        sync();
        LOGI("created- %s, REF: %s, FD: %d, SHM: %x, this: %x",
             rn, src_reference.c_str(), ptr_data_shm, this);
      }
    }
  */
  /*   if (is_shm())
      LOGD("Open %s, FD: %d, SHM: %x, this: %x", rn, fd, ptr_data_shm, this);
    else
      LOGE("Error open %s", rn); */
}

Reg_c::Reg_c(reg_t* _reg, PLC_c* _dev) // For Modbus regs only
{
// init PLC vars first
// init_type(r);
// init_str(r);

// init locals
  ptr_reg = _reg;
  src_reference = ptr_reg->str_source;

  str_fullname = ptr_reg->fullname;
  rn = str_fullname.c_str();

  str_topfolder = _dev->str_folder;
  str_opcname = "/" + str_topfolder + "/";

  string &_dname = _dev->str_dev_name;
  if (_dname != MB_NO_DEV_NAME)
    str_opcname += _dname + "/";

  string &_rfolder = _reg->str_rfolder;
  if ((_rfolder != MB_NO_FOLDER) && (_rfolder != ""))
    str_opcname += _rfolder + "/";

  str_opcname += _reg->fullname;

  init_types(_reg);   // init types
  string st_ = to_lower(_reg->str_type);
  var_type = type_map[st_].rtype;
  var_size = type_map[st_].rsize;
  byte_order = type_map[st_].rbyteorder;

  auto &rbo = type_map[st_].rbyteorder;
  visible = (rbo != BO_2ND) && (rbo != BO_3RD) && (rbo != BO_4TH);

//  LOGD("- %s %d - done, sizeof(value): %d", __func__, 5, sizeof(value));
}

void Reg_c::init_types(reg_t* _reg) // !! STATIC FUNCTION !!
{
  string st_ = _reg->str_type;
  for (auto &c : st_)
    c = static_cast<char>(tolower(c));

  _reg->data.rtype = type_map[st_].rtype;
  _reg->data.rsize = type_map[st_].rsize;
  _reg->data.rbyteorder = type_map[st_].rbyteorder;
}

string Reg_c::to_lower(string str)
{
  for (auto &c : str)
    c = static_cast<char>(tolower(c));
  return str;
}
// eof
