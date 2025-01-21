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
       str_title.c_str(), str_desc.c_str());

  for (auto &[a, R] : regs) {
    init_str(R);
    init_type(R);

    R.data.rmode = (R.str_mode == "rw") ? 1 : 0;

    if (R.raddr < reg_min)
      reg_min = R.raddr;

    if (R.raddr > reg_max)
      reg_max = R.raddr;

    R.data.rvalue = 777;  // TODO: remove for production
    LOGI("+ REG init: %-9s %2d %2s [%s]", R.ch_name, R.raddr,
         R.str_mode.c_str(), R.fullname.c_str());
  }
}


void PLC_c::init_str(reg_t &R)
{

  R.str_title = str_title;
  R.str_opcname = "/" + R.str_title + "/";

  if (!is_slave) {
    if (str_dev_name == MB_NO_DEV_NAME) { // Scada!
      if (R.str_folder == MB_NO_FOLDER)
        R.fullname = R.str_name;
      else {
        R.fullname = R.str_folder + "." + R.str_name;
        R.str_opcname += R.str_folder + "/";
      }
    } else {
      R.fullname = str_dev_name + "." + R.str_name;
      R.str_opcname += str_dev_name + "/";
      if (!(R.str_folder == MB_NO_FOLDER || R.str_folder == ""))
        R.str_opcname += R.str_folder + "/";
    }
  }

  R.str_opcname += R.fullname;
  R.ch_name = R.str_name.c_str();

  return;
}


void PLC_c::init_type(reg_t &R)
{
  auto &rd = R.data;
  if (R.str_type == "u")
    rd.rtype = 0;
  else if (R.str_type == "i")
    rd.rtype = 1;
  else if (R.str_type == "f")
    rd.rtype = 2;
  else if (R.str_source == "")
    LOGA("Error REG init: %s\n", R.str_name.c_str());
  return;
}

// eof
