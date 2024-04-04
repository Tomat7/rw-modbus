// plc_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <chrono>
#include <mutex>
#include <map>

#include "./plc_class.h"
#include "./logger.h"
#include "./shmem.h"

RegMap_c::RegMap_c() {}

RegMap_c::~RegMap_c() {}

RegMap_c::RegMap_c(int _fd, regdata_t* _shm, regdata_t* _plc, reg_t* _reg)
{
  fd = _fd;
  ptr_data_shm = _shm;
  ptr_data_plc = _plc;
  ptr_reg = _reg;
  sync();
  //  logger(LOG_INFO, "+ New RegMap created.");
}

RegMap_c::RegMap_c(reg_t* _reg)
{
  ptr_reg = _reg;
  ptr_data_plc = &(ptr_reg->data);

  regdata_t* _shm;
  int _fd = create_shm_fd(ptr_reg->fullname.c_str());
  if (fd != -1) {
    _shm = (regdata_t*)create_shm_addr(fd, sizeof(regdata_t));
    if (_shm != nullptr) {
      fd = _fd;
      ptr_data_shm = _shm;
      sync();
      //  logger(LOG_INFO, "+ New RegMap created.");
    }
  }
}


uint16_t RegMap_c::get_plc_val()
{
  if (ptr_data_plc != nullptr)
    return ptr_data_plc->rvalue;
  return 0;
}

uint16_t RegMap_c::get_shm_val()
{
  if (ptr_data_shm != nullptr && fd != -1)
    return ptr_data_shm->rvalue;
  return 0;
}

uint16_t RegMap_c::get_local()
{
  if (ptr_data_shm != nullptr && fd != -1) {
    regdata_t mem;
    memcpy(&mem, ptr_data_shm, sizeof(regdata_t));
    return mem.rvalue;
  }
  return 0;
}

void RegMap_c::set_plc_val(uint16_t _val)
{
  if (ptr_data_plc != nullptr) {
    ptr_data_plc->rvalue = _val;
    ptr_data_plc->rupdate = 1;
  }
}

void RegMap_c::set_shm_val(uint16_t _val)
{
  if (ptr_data_shm != nullptr)
    ptr_data_shm->rvalue = _val;
}

void RegMap_c::set_local(uint16_t _val)
{
  if (ptr_data_shm != nullptr) {
    regdata_t mem;
    mem.rvalue = _val;
    memcpy(ptr_data_shm, &mem, sizeof(regdata_t));
  }
}

void RegMap_c::sync(uint16_t _val)
{
  if (ptr_data_plc != nullptr && ptr_data_shm != nullptr) {
    regdata_t mem;
    value = _val;
    mem.rvalue = _val;
    mem.rerrors = ptr_data_plc->rerrors;
    mem.rstatus = ptr_data_plc->rstatus;
    mem.rmode = ptr_data_plc->rmode;
    mem.rtype = ptr_data_plc->rtype;
    memcpy(ptr_data_shm, &mem, sizeof(regdata_t));
  }
  return;
}

void RegMap_c::sync()
{
  sync(get_plc_val());
  return;
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

