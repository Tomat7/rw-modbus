// plc_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>

#include <chrono>
#include <mutex>
#include <thread>

#include "include/logger.h"
#include "plc_class.h"

// mutex PLC_c::logger_mux;

/* map<string, pair<regtype_t, byteorder_t>> type_map { */

/*
  // See plc_datatype.h for details & comments
  // {"*", TYPE_REFERENCED, OTHER},
  {"-", TYPE_NOTHING},    // "Referenced" registers
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
*/

PLC_c::~PLC_c()
{
  // lock_now();
  LOGD("- PLC destructor: try_lock(): %s %s.", ip_addr, dev_name);
  while (!(lock_mux->try_lock()) && !is_slave)
    std::this_thread::yield();

  LOGD("- PLC destructor: mb_deinit()) done: %s %s.", ip_addr, dev_name);
  mb_deinit();
  //  unlock_now();
  delete lock_mux;
  LOGN("- PLC destructor: closed, unmapped and free: %s %s.", ip_addr,
       dev_name);
}


void PLC_c::init_regs()  // Master only
{
  for (auto &[a, r] : regs) {
    // init_type(r);
    // init_str(r);

    r.ch_name = r.str_name.c_str();
    r.data.rmode = (r.str_mode == "rw") ? 1 : 0;

    if (!is_slave) {
      if (str_dev_name == MB_NO_DEV_NAME) // Scada!
        r.fullname = r.str_name;
      else
        r.fullname = str_dev_name + "." + r.str_name;
    }

    if (r.raddr < reg_min)
      reg_min = r.raddr;
    if (r.raddr > reg_max)
      reg_max = r.raddr;

    r.data.rvalue = 777;  // TODO: remove for production
  }

// RECODE!! add 64-bit regg
  for (auto &[a, r] : regs) {
    int ru = r.data.rsize;

    if (ru > 1) {
      r.r_next = &regs[r.raddr + 1];
      regs[r.raddr + 1].data.rmode = r.data.rmode;
      regs[r.raddr + 1].data.rtype = NOTUA_TYPES_2ND;
    }

    if (ru > 2) {
      regs[r.raddr + 1].r_next = &regs[r.raddr + 2];
      regs[r.raddr + 2].data.rmode = r.data.rmode;
      regs[r.raddr + 2].data.rtype = NOTUA_TYPES_3RD;
      regs[r.raddr + 2].r_next = &regs[r.raddr + 3];
      regs[r.raddr + 3].data.rmode = r.data.rmode;
      regs[r.raddr + 3].data.rtype = NOTUA_TYPES_4TH;
    }

    LOGN("+ REG init: %-9s %2d %2s %3d [%s]", r.ch_name, r.raddr,
         r.str_mode.c_str(), regs[r.raddr].data.rtype, r.fullname.c_str());
  }
}


void PLC_c::init_str(reg_t &r)
{
  return;
}


void PLC_c::init_type(reg_t &R)
{
  string st_ = to_lower(R.str_type);
  /*   if (type_map.count(st_))
      R.data.rtype = type_map[st_].first;
    else
      LOGA("Error REG: %s, type: %s\n", R.str_name.c_str(), R.str_type.c_str());
  */
  /*
    auto &rd = R.data;
    if (R.str_type == "u")
      rd.rtype = 0;
    else if (R.str_type == "i")
      rd.rtype = 1;
    else if (R.str_type == "f")
      rd.rtype = 2;
    else if (R.str_source == "")
      LOGA("Error REG type: %s\n", R.str_name.c_str());
  */
  return;
}

/*
  void PLC_c::init_str(reg_t &R)
  {
  //  R.str_title = str_title;
  //  R.str_opcname = "/" + R.str_title + "/";

  if (!is_slave) {
    if (str_dev_name == MB_NO_DEV_NAME) { // Scada!
      if (R.str_rfolder == MB_NO_FOLDER || R.str_rfolder == "")
        R.fullname = R.str_name;
  //      else {
  //        R.fullname = R.str_rfolder + "." + R.str_name;
  //        R.str_opcname += R.str_folder + "/";
      }
    } else {
      R.fullname = str_dev_name + "." + R.str_name;
  //      R.str_opcname += str_dev_name + "/";
  //      if (!(R.str_folder == MB_NO_FOLDER || R.str_folder == ""))
  //        R.str_opcname += R.str_folder + "/";
  //    }
  }

  //  R.str_opcname += R.fullname;
  R.ch_name = R.str_name.c_str();

  return;
  }
*/

// eof
