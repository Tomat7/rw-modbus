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
#include <map>

// ANSI color codes
#define KNRM "\x1B[0m"
#define KRED "\x1B[91m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[94m"

// mutex PLC_c::logger_mux;

RegMap_c::RegMap_c(int _fd, regdata_t *_shm, regdata_t *_plc, reg_t *_reg)
{
  fd = _fd;
  ptr_data_shm = _shm;
  ptr_data_plc = _plc;
  ptr_reg = _reg;
  //  logger(LOG_INFO, "+ New RegMap created.");
}

RegMap_c::RegMap_c() {}

RegMap_c::~RegMap_c()
{
  //  mb_deinit();
  //  logger(LOG_INFO, "- RegMap unmapped: %s %s.", ip_addr, dev_name);
}

uint16_t RegMap_c::get_remote()
{
  if (ptr_data_plc != nullptr)
    return ptr_data_plc->rvalue;
  return 0;
}

uint16_t RegMap_c::get_local()
{
  if (ptr_data_shm != nullptr && fd != -1)
  {
    regdata_t mem;
    memcpy(&mem, ptr_data_shm, sizeof(regdata_t));
    return mem.rvalue;
  }
  return 0;
}

void RegMap_c::set_remote(uint16_t _val)
{
  if (ptr_data_plc != nullptr)
  {
    ptr_data_plc->rvalue = _val;
    ptr_data_plc->rupdate = 1;
  }
}

void RegMap_c::set_local(uint16_t _val)
{
  if (ptr_data_shm != nullptr)
  {
    regdata_t mem;
    mem.rvalue = _val;
    memcpy(ptr_data_shm, &mem, sizeof(regdata_t));
  }
}

void RegMap_c::sync_local(uint16_t _val)
{
  if (ptr_data_plc != nullptr && ptr_data_shm != nullptr)
  {
    regdata_t mem;
    mem.rvalue = _val;
    mem.rerrors = ptr_data_plc->rerrors;
    mem.rstatus = ptr_data_plc->rstatus;
    mem.rmode = ptr_data_plc->rmode;
    mem.rtype = ptr_data_plc->rtype;
    memcpy(ptr_data_shm, &mem, sizeof(regdata_t));
  }
}

int RegMap_c::get_mode()
{
  if (ptr_data_plc != nullptr)
    return ptr_data_plc->rmode;
  return -1;
}

int RegMap_c::get_type()
{
  if (ptr_data_plc != nullptr)
    return ptr_data_plc->rtype;
  return -1;
}

/*
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
//  logger(LOG_INFO, "- PLC closed, unmapped and free: %s %s.", ip_addr, dev_name);

  return;
}


int PLC_c::mb_ctx()
{
  rc = 0;
  mb_deinit();

  ctx = modbus_new_tcp(ip_addr, tcp_port);
  if (ctx == nullptr) {
    rc = -1;
    logger(LOG_ERR, "- %s:%d %s CTX allocate error.", ip_addr, tcp_port,
           dev_name);
  } else
    logger(LOG_INFO, "+ %s:%d %s CTX allocate OK.", ip_addr, tcp_port, dev_name);

  return rc;
}

uint64_t PLC_c::millis()
{
#define CAST_MILLIS std::chrono::duration_cast<std::chrono::milliseconds>
  uint64_t t;
  t = CAST_MILLIS(std::chrono::system_clock::now().time_since_epoch()).count();
  return t;
}



void PLC_c::logger(int prio, const char* format, ...)
{
  logger_mux.lock();
  FILE* fout = stdout;

  if (prio == LOG_ERR) {
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
*/
// eof
