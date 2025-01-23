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
map<string, int> var_type {
  {"u", TYPE_U16},       {"i", TYPE_I16},       {"f", TYPE_F100},
  {"*", 254},            {"-", 255},      /* Referenced registers */
  {"u16", TYPE_U16},     {"i16", TYPE_I16},     {"f10", TYPE_F10},
  {"uint16", TYPE_U16},  {"int16", TYPE_I16},   {"f100", TYPE_F100},
  {"fabcd", 21},  {"fcdab", 22},  {"fbadc", 23},  {"fdcba", 24},
  {"f_abcd", 21}, {"f_cdab", 22}, {"f_badc", 23}, {"f_dcba", 24},
  {"float_abcd", 21},  {"float_abcd_1", 21}, {"float_abcd_2", 121},
  {"float_cdab", 22},  {"float_cdab_1", 22}, {"float_cdab_2", 122},
  {"float_badc", 23},  {"float_badc_1", 23}, {"float_badc_2", 123},
  {"float_dcba", 24},  {"float_dcba_1", 24}, {"float_dcba_2", 124}
};

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
  ip_addr = str_ip_addr.c_str();
  dev_name = str_dev_name.c_str();
  LOGN("+ PLC init: %s:%i %-7s %-7s %-20s", ip_addr, tcp_port, dev_name,
       str_folder.c_str(), str_desc.c_str());

  for (auto &[a, r] : regs) {
    init_type(r);
    init_str(r);

    r.data.rmode = (r.str_mode == "rw") ? 1 : 0;

    if (r.raddr < reg_min)
      reg_min = r.raddr;
    if (r.raddr > reg_max)
      reg_max = r.raddr;

    r.data.rvalue = 777;  // TODO: remove for production
  }

//
  for (auto &[a, r] : regs) {
    if (is_float(a) == 1) {
      r.r_next = &regs[r.raddr + 1];
      regs[r.raddr + 1].data.rmode = r.data.rmode;
      regs[r.raddr + 1].data.rtype = r.data.rtype + 100;
    }
    LOGI("+ REG init: %-9s %2d %2s %3d [%s]", r.ch_name, r.raddr,
         r.str_mode.c_str(), regs[r.raddr].data.rtype, r.fullname.c_str());
  }

}


void PLC_c::init_str(reg_t &r)
{
  if (!is_slave) {
    if (str_dev_name == MB_NO_DEV_NAME) // Scada!
      r.fullname = r.str_name;
    else
      r.fullname = str_dev_name + "." + r.str_name;
  }
  r.ch_name = r.str_name.c_str();

  return;
}


void PLC_c::init_type(reg_t &R)
{
  string st_ = to_lower(R.str_type);
  if (var_type.count(st_))
    R.data.rtype = var_type[st_];
  else
    LOGA("Error REG: %s, type: %s\n", R.str_name.c_str(), R.str_type.c_str());

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
