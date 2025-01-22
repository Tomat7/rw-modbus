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

int PLC_c::set_reg(int raddr, float fl)  // Set reg locally != write PLC.
{
  float2uint_u fl2u;
  fl2u.fl = fl;
  rc = -1;  // Addr out of range

  if ((raddr + 1) <= reg_max) {
    regdata_t &rd0 = regs[raddr].data;
    regdata_t &rd1 = regs[raddr+1].data;
    if (rd0.rmode == 1) {
      if ((rd0.rvalue != fl2u.ui[0]) || (rd1.rvalue != fl2u.ui[1])) {
        rd0.rvalue = fl2u.ui[0];
        rd0.rvalue = fl2u.ui[0];
        rd0.rupdate = 1;
        rd1.rvalue = fl2u.ui[1];
        rd1.rvalue = fl2u.ui[1];
        rd1.rupdate = 1;
        LOGD("%d %f", raddr, fl);
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

int PLC_c::set_reg(string rname, float fl)
{
  rc = -2;  // rname not found

  for (auto &[a, r] : regs) {
    if (r.str_name == rname) {
      rc = set_reg(a, fl);
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

int PLC_c::is_float(int raddr)  // 0 - 16 bit reg, 1 - 1st reg of 32-bits reg
{
  int x = 0;
  if (regs.count(raddr)) {
    auto &rt = regs[raddr].data.rtype;
    if (rt > 20) {
      x = 1;
      if (rt > 100)
        x = 2;
    }
  }
  return x;
}

int PLC_c::is_float(string rname) // 0 - 16 bit reg, 1 - 1st reg of 32-bits reg
{
  int x = 0;
  for (auto &[a, r] : regs) {
    if (r.str_name == rname) {
      x = is_float(a);
      break;
    }
  }
  return x;
}

uint64_t PLC_c::millis()
{
#define CAST_MILLIS std::chrono::duration_cast<std::chrono::milliseconds>
  uint64_t t;
  t = CAST_MILLIS(std::chrono::system_clock::now().time_since_epoch()).count();
  return t;
}

string PLC_c::to_lower(string str)
{
  for (auto &c : str)
    c = static_cast<char>(tolower(c));
  return str;
}

// eof
