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

RegMap_c::~RegMap_c() {}

RegMap_c::RegMap_c() { LOGD("Construct! %x", this); }

RegMap_c::RegMap_c(int _fd, regdata_t* _shm, regdata_t* _plc, reg_t* _reg)
{
  fd = _fd;
  ptr_data_shm = _shm;
  ptr_data_plc = _plc;
  ptr_reg = _reg;
  rn = ptr_reg->fullname.c_str();
  sync();
  //  LOGI("+ New RegMap created.");
}

RegMap_c::RegMap_c(reg_t* _reg)
{
  ptr_reg = _reg;
  ptr_data_plc = &(ptr_reg->data);
  rn = ptr_reg->fullname.c_str();
  LOGF("try to create %s", rn);

  fd = create_shm_fd(rn);
  if (fd != -1) {
    ptr_data_shm = (regdata_t*)create_shm_addr(fd, sizeof(regdata_t));
    if (ptr_data_shm != nullptr) {
      sync();
      LOGF("created %s, FD: %d, addr: %x", rn, fd, ptr_data_shm);
    }
  }
}

bool RegMap_c::is_shm()
{
  bool ret = true;

  int _fd = get_shm_fd(rn);
  if (_fd == -1) {
    ret = false;
    if (fd != -1) {
      close_shm(fd, ptr_data_shm, sizeof(regdata_t));
      ptr_data_shm = nullptr;
      fd = -1;
    }
  } else if (fd == -1) {
    fd = _fd;
    ptr_data_shm = (regdata_t*)get_shm_addr(fd, sizeof(regdata_t));
    if (ptr_data_shm != nullptr) {
      sync();
      LOGI("SHM: created %s, FD: %d, addr: %x", rn, fd, ptr_data_shm);
    } else {
      ret = false;
      close_fd(fd);
      fd = -1;
    }
  } else
    close_fd(_fd);

  D(printf(" ^%d_~%d ", _fd, fd);)

  return ret;
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

uint16_t RegMap_c::get_plc_val()
{
  if (ptr_data_plc != nullptr)
    return ptr_data_plc->rvalue;
  return 0;
}

uint16_t RegMap_c::get_shm_val()
{
  if (is_shm())
    return ptr_data_shm->rvalue;
  return 0;
}

uint16_t RegMap_c::get_local()
{
  if (is_shm()) {
    regdata_t mem;
    memcpy(&mem, ptr_data_shm, sizeof(regdata_t));
    return mem.rvalue;
  }
  return 0;
}

void RegMap_c::set_plc_val(uint16_t _val)
{
  if (ptr_data_plc != nullptr) {
    if (ptr_data_plc->rvalue != _val)
      ptr_data_plc->rvalue = _val;
    ptr_data_plc->rupdate = 1;
  }
}

void RegMap_c::set_shm_val(uint16_t _val)
{
  if (is_shm())
    ptr_data_shm->rvalue = _val;
}

void RegMap_c::set_local(uint16_t _val)
{
  LOGD("%s(): %d %s %x.", __func__, _val, rn, this);
  if (is_shm()) {
    regdata_t mem;
    mem.rvalue = _val;
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
