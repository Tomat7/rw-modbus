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

  modbus_close(ctx);
  modbus_free(ctx);
  ctx = nullptr;
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
