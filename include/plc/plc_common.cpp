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
  //lock_now();
  while (!(lock_mux->try_lock()) && !is_slave)
    std::this_thread::yield();
  LOGD("- PLC destructor: try_lock() done: %s %s.", ip_addr, dev_name);

  mb_deinit();
  //  unlock_now();
  delete lock_mux;
  LOGN("- PLC destructor: closed, unmapped and free: %s %s.", ip_addr, dev_name);
}

int PLC_c::get_rc() { return rc; }

void PLC_c::init_regs()  // Master only
{
  ip_addr = str_ip_addr.c_str();
  dev_name = str_dev_name.c_str();
  LOGN("+ PLC init: %s %-7s %-7s %-20s", ip_addr, dev_name, str_title.c_str(),
       str_desc.c_str());

  for (auto &[a, R] : regs) {
    auto &rd = R.data;

    if (!is_slave)
      R.fullname = str_dev_name + "." + R.str_name;

    R.ch_name = R.str_name.c_str();
    rd.rmode = (R.str_mode == "rw") ? 1 : 0;

    if (R.str_type == "u")
      rd.rtype = 0;
    else if (R.str_type == "i")
      rd.rtype = 1;
    else if (R.str_type == "f")
      rd.rtype = 2;
    else
      LOGA("Error REG init: %s\n", R.str_name.c_str());

    if (R.raddr < reg_min)
      reg_min = R.raddr;

    if (R.raddr > reg_max)
      reg_max = R.raddr;

    rd.rvalue = 777;  // TODO: remove for production
    LOGI("+ REG init: %-9s %2d %2s [%s]", R.ch_name, R.raddr,
         R.str_mode.c_str(), R.fullname.c_str());
  }
}

void PLC_c::mb_deinit()
{
  //  LOCK_GUARD(network_mux);
  if (server_socket != -1) {
    close(server_socket);
    server_socket = -1;
  }

  if (mbm != nullptr) {
    modbus_mapping_free(mbm);
    mbm = nullptr;
  }

  if (ctx != nullptr) {
    modbus_close(ctx);
    modbus_free(ctx);
    ctx = nullptr;
  }
  LOGD("- MB deinit: closed, unmapped and free: %s %s.", ip_addr, dev_name);

  return;
}

int PLC_c::mb_ctx()
{
  rc = 0;
  mb_deinit();

  ctx = modbus_new_tcp(ip_addr, tcp_port);
  if (ctx == nullptr) {
    rc = -1;
    LOGE("- %s:%d %s CTX allocate error.", ip_addr, tcp_port, dev_name);
  } else
    LOGI("+ %s:%d %s CTX allocate OK.", ip_addr, tcp_port, dev_name);

  return rc;
}

int PLC_c::set_reg(int raddr, uint16_t rval)  // Set reg locally != write PLC.
{
  rc = -1;
  if (raddr <= reg_max) {
    regdata_t &rd = regs[raddr].data;
    if (rd.rmode == 1) {
      if (rd.rvalue != rval) {
        rd.rvalue = rval;
        rd.rupdate = 1;
        LOGD("%d %d", raddr, rval);
        rc = 1;  // Update is ok
      } else
        rc = 0;  // No update necessary
    }
  }
  return rc;
}

int PLC_c::set_reg(string rname, uint16_t rval)
{
  rc = -2;  // rname not found

  for (auto &[a, r] : regs) {
    if (r.str_name == rname) {
      rc = set_reg(a, rval);
      break;
    }
  }

  return rc;
}

uint16_t PLC_c::get_reg(int raddr)  // Set reg's local value != read PLC.
{
  uint16_t rval = 0;
  rc = -1;
  if (regs.count(raddr)) {
    rval = regs[raddr].data.rvalue;
    rc = 1;
  }
  return rval;
}

uint16_t PLC_c::get_reg(string rname)  // Set reg's local value != read PLC.
{
  uint16_t rval = 0;
  rc = -1;

  for (auto &[a, r] : regs) {
    if (r.str_name == rname) {
      rval = r.data.rvalue;
      rc = 1;
      break;
    }
  }

  return rval;
}

uint64_t PLC_c::millis()
{
#define CAST_MILLIS std::chrono::duration_cast<std::chrono::milliseconds>
  uint64_t t;
  t = CAST_MILLIS(std::chrono::system_clock::now().time_since_epoch()).count();
  return t;
}

// eof
