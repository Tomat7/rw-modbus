// plc_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include "./plc_class.h"

#include <chrono>
#include <mutex>
#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>

#include "./logger.h"

// ANSI color codes
#define KNRM "\x1B[0m"
#define KRED "\x1B[91m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[94m"

// mutex PLC_c::logger_mux;

PLC_c::~PLC_c()
{
  mb_deinit();
  LOGN("- PLC closed, unmapped and free: %s %s.", ip_addr, dev_name);
}

int PLC_c::get_rc() { return rc; }

void PLC_c::init_regs() // Master only
{
  ip_addr = str_ip_addr.c_str();
  dev_name = str_dev_name.c_str();
  LOGI("+ PLC init: %s %-7s %-7s %-20s", ip_addr, dev_name, str_title.c_str(),
       str_desc.c_str());

  for (auto &[a, R] : regs) {
    auto &rd = R.data;

    if (!is_slave)
      R.fullname = str_dev_name + "." + R.str_name;
    R.ch_name = R.str_name.c_str();

    rd.rmode = (R.str_mode == "rw") ? 1 : 0;
    rd.rtype = (R.str_type == "f") ? 1 : 0;

    if (R.raddr < reg_min)
      reg_min = R.raddr;

    if (R.raddr > reg_max)
      reg_max = R.raddr;

    rd.rvalue = 777; // TODO: remove for production
    LOGI("+ REG init: %-7s %2d %2s [%s]", R.ch_name, R.raddr,
         R.str_mode.c_str(), R.fullname.c_str());
  }
}


void PLC_c::mb_deinit()
{

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
  //  LOGI("- PLC closed, unmapped and free: %s %s.", ip_addr, dev_name);

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
    LOGN("+ %s:%d %s CTX allocate OK.", ip_addr, tcp_port, dev_name);

  return rc;
}

uint64_t PLC_c::millis()
{
#define CAST_MILLIS std::chrono::duration_cast<std::chrono::milliseconds>
  uint64_t t;
  t = CAST_MILLIS(std::chrono::system_clock::now().time_since_epoch()).count();
  return t;
}

// eof
