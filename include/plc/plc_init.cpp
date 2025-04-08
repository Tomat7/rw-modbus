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
    r.ch_name = r.str_rname.c_str();
    r.data.rmode = (r.str_mode == "rw") ? 1 : 0;

    if (!is_slave)
      r.rfullname = str_dev_name + "." + r.str_rname;

    if (r.raddr < reg_min)
      reg_min = r.raddr;
    if (r.raddr > reg_max)
      reg_max = r.raddr;

    r.data.rvalue = 777;  // TODO: remove for production
  }

  // add pointer to next reg (for each!)
  for (auto &[a, r] : regs) {
    if (regs.count(r.raddr + 1))
      r.r_next = &regs[r.raddr + 1];
    LOGN("+ REG init: %-9s %2d %2s %4d %2d %3d [%s]", r.ch_name, r.raddr,
         r.str_mode.c_str(), regs[r.raddr].data.rtype, regs[r.raddr].data.rsize,
         regs[r.raddr].data.rbyteorder, r.rfullname.c_str());
  }
}

/*
  void PLC_c::init_str(reg_t &r)
  {
  return;
  }
*/

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
