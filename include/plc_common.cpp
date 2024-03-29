// plc_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include "./plc_class.h"

#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>
#include <chrono>
#include <mutex>

// ANSI color codes
#define KNRM "\x1B[0m"
#define KRED "\x1B[91m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[94m"

mutex PLC::logger_mux;

PLC::~PLC()
{
  deinit();
}

void PLC::deinit()
{

  if (server_socket != -1)
  {
//    logger(LOG_INFO, "- PLC ready to close socket: %s %s.", ip_addr, dev_name);
    close(server_socket);
    server_socket = -1;
  }

  if (mb_mapping != nullptr)
  {
//    logger(LOG_INFO, "- PLC ready to free mapping: %s %s.", ip_addr, dev_name);
    modbus_mapping_free(mb_mapping);
    mb_mapping = nullptr;
  }

//  logger(LOG_INFO, "- PLC ready to close ctx & free: %s %s.", ip_addr, dev_name);
  modbus_close(ctx);
  modbus_free(ctx);
  ctx = nullptr;
  logger(LOG_INFO, "- PLC closed, free and deleted: %s %s.", ip_addr, dev_name);
  
  return;
}

void PLC::logger(int prio, const char *format, ...)
{
  logger_mux.lock();
  FILE *fout = stdout;
  if (prio == LOG_ERR)
  {
    fout = stderr;
    fprintf(fout, KRED);
  }

  if (is_slave)
    openlog("MB_slave", LOG_NDELAY, LOG_LOCAL1);
  else
    openlog("MB_master", LOG_NDELAY, LOG_LOCAL1);

  va_list arg1;
  va_list arg2;

  va_start(arg1, format);
  va_copy(arg2, arg1);
  vfprintf(fout, format, arg1);
  vsyslog(prio, format, arg2);
  va_end(arg1);
  va_end(arg2);

  fprintf(fout, "%s\n", KNRM);
  closelog();
  logger_mux.unlock();
}

int PLC::mb_ctx()
{
  rc = 0;
  // logger(LOG_INFO, "%s:%d %s try to close/free.", ip_addr, tcp_port,
  // dev_name);
  modbus_close(ctx);
  modbus_free(ctx);
  ctx = nullptr;

  ctx = modbus_new_tcp(ip_addr, tcp_port);
  if (ctx == nullptr)
  {
    rc = -1;
    logger(LOG_ERR, "%s:%d %s CTX allocate error.", ip_addr, tcp_port,
           dev_name);
  }
  else
    logger(LOG_INFO, "%s:%d %s CTX allocate OK.", ip_addr, tcp_port, dev_name);

  return rc;
}

uint64_t PLC::millis()
{
#define CAST_MILLIS std::chrono::duration_cast<std::chrono::milliseconds>
  uint64_t t;
  t = CAST_MILLIS(std::chrono::system_clock::now().time_since_epoch()).count();
  return t;
}

int PLC::get_rc() { return rc; }

// eof
