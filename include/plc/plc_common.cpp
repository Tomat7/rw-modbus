// plc_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//

#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>

#include <chrono>
#include <mutex>
#include <thread>

#include "include/logger.h"
#include "plc_class.h"

int PLC_c::get_rc_read() { return rc_read; }

int PLC_c::get_rc_write() { return rc_write; }

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

  if (tcp_port != 0)
    ctx = modbus_new_tcp(ip_addr, tcp_port);
  else
    return 0;

  if (ctx == nullptr) {
    rc = -1;
    LOGE("- %s:%d %s CTX allocate error.", ip_addr, tcp_port, dev_name);
  } else
    LOGI("+ %s:%d %s CTX allocate OK.", ip_addr, tcp_port, dev_name);

  return rc;
}

int PLC_c::set_reg(uint16_t rval, mbreg_t* rptr)  // !! STATIC !!
{
  int ret = -1;
  if (rptr != nullptr) {
    mbregdata_t &rd = rptr->data;
    if (rd.rmode == 1) {
      if (rd.rvalue != rval) {
        rd.rvalue = rval;
        rd.rupdate = 1;
        LOGD("%d %d", rptr->raddr, rval);
        ret = 1;  // Update is ok
      } else
        ret = 0;  // No update necessary
    }
  } else
    LOGE("Error set reg: %s", rptr->rfullname.c_str());

  return ret;
}

int PLC_c::set_reg(uint16_t rval, int raddr)  // Set reg locally != write PLC.
{
  rc = -1;
  if (raddr <= reg_max) {
    mbregdata_t &rd = regs[raddr].data;
    if (rd.rmode == 1) {
      if (rd.rvalue != rval) {
        rd.rvalue = rval;
        rd.rupdate = 1;
        LOGD("%d %d", raddr, rval);
        rc = 1;  // Update is ok
      } else
        rc = 0;  // No update necessary
    }
  } else
    LOGE("Error set reg: %s", regs[raddr].ch_name);

  return rc;
}

int PLC_c::set_reg_by_name(uint16_t rval, string rname)
{
  rc = -2;  // rname not found

  for (auto &[a, r] : regs) {
    if (r.str_rname == rname) {
      rc = set_reg(rval, a);
      break;
    }
  }

  return rc;
}

int PLC_c::set_reg_by_fullname(uint16_t rval, string rname)
{
  rc = -2;  // rname not found

  for (auto &[a, r] : regs) {
    if (r.rfullname == rname) {
      rc = set_reg(rval, a);
      break;
    }
  }

  return rc;
}

// =============================================

uint16_t PLC_c::get_reg(mbreg_t* rptr)  // !! STATIC !!
{
  uint16_t rval = 0;
  if (rptr != nullptr)
    rval = rptr->data.rvalue;
  return rval;
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

uint16_t PLC_c::get_reg_by_name(
  string rname)  // Set reg's local value != read PLC.
{
  uint16_t rval = 0;
  rc = -1;
  for (auto &[a, r] : regs) {
    if (r.str_rname == rname) {
      rval = r.data.rvalue;
      rc = 1;
      break;
    }
  }
  return rval;
}

uint16_t PLC_c::get_reg_by_fullname(
  string rfname)  // Set reg's local value != read PLC.
{
  uint16_t rval = 0;
  rc = -1;
  for (auto &[a, r] : regs) {
    if (r.str_rname == rfname) {
      rval = r.data.rvalue;
      rc = 1;
      break;
    }
  }
  return rval;
}


uint64_t PLC_c::millis()
{
  using namespace std::chrono;
#define CAST_MILLIS duration_cast<milliseconds>
  return CAST_MILLIS(system_clock::now().time_since_epoch()).count();
}

string PLC_c::to_lower(string str)
{
  for (auto &c : str)
    c = static_cast<char>(tolower(c));
  return str;
}

// eof
