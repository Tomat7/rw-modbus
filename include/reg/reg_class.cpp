// reg_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include "reg_class.h"

#include <fcntl.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <syslog.h>
#include <unistd.h>

#include <chrono>
#include <map>
#include <mutex>

#include "include/logger.h"
#include "include/plc/plc_class.h"
#include "include/shmem/shmem.h"

#ifdef SYSLOG_NAME
#undef SYSLOG_NAME
#endif
#define SYSLOG_NAME "REG-class"

Reg_c::~Reg_c() { LOGD("DEstruct! %x %s", this, this->rn); }

Reg_c::Reg_c() { LOGD("Construct! %x", this); }

/*
  Reg_c::Reg_c(const char* _rn) { Reg_c((string)_rn); }

  Reg_c::Reg_c(string _rn)
  {
  rn = get_new_char(_rn.c_str());
  LOGD("try to open %s", rn);

  if (is_shm())
    LOGD("Open %s, FD: %d, SHM: %x, this: %x", rn, fd, ptr_data_shm, this);
  else
    LOGE("Error open %s", rn);
  }
*/

Reg_c::Reg_c(reg_t* _reg)
{
  ptr_reg = _reg;
  ptr_data_plc = &(ptr_reg->data);
  rn = ptr_reg->fullname.c_str();
  // LOGD("try to create %s", rn);

  fd = create_shm_fd(rn);
  if (fd != -1) {
    ptr_data_shm = (regdata_t*)create_shm_addr(fd, sizeof(regdata_t));
    if (ptr_data_shm != nullptr) {
      sync();
      LOGI("created %s, FD: %d, SHM: %x, this: %x", rn, fd, ptr_data_shm, this);
    }
  }
}

Reg_c::Reg_c(int _fd, regdata_t* _shm, regdata_t* _plc, reg_t* _reg)
{
  fd = _fd;
  ptr_data_shm = _shm;
  ptr_data_plc = _plc;
  ptr_reg = _reg;
  rn = ptr_reg->fullname.c_str();
  sync();
  //  LOGI("+ New RegMap created.");
}

bool Reg_c::is_shm()
{
  bool ret = false;

  int _fd = get_shm_fd(rn);
  D(printf(" ^%2d_~%2d ", _fd, fd);)

  if (_fd == -1)  // No SHM found
    close_shm(fd, ptr_data_shm, sizeof(regdata_t));
  else if (fd != -1) {  // SHM - Ok && FD already Ok
    close_fd(_fd);      // All - Ok, close _fd
    ret = true;
  } else {  // SHM - Ok, but FD not ready yet
    fd = _fd;
    ptr_data_shm = (regdata_t*)get_shm_addr(fd, sizeof(regdata_t));
    if (ptr_data_shm == nullptr)
      close_fd(fd);
    else
      ret = true;
  }

  return ret;
}

void Reg_c::sync(uint16_t _val)
{
  value = _val;
  if (ptr_data_shm != nullptr) {
    regdata_t mem;
    mem.rvalue = _val;

    if (ptr_data_plc != nullptr) {
      mem.rerrors = ptr_data_plc->rerrors;
      mem.rstatus = ptr_data_plc->rstatus;
      mem.rmode = ptr_data_plc->rmode;
      mem.rtype = ptr_data_plc->rtype;
    }

    memcpy(ptr_data_shm, &mem, sizeof(regdata_t));
  }
  return;
}

void Reg_c::sync()
{
  sync(get_plc_val());
  return;
}

uint16_t Reg_c::get_plc_val()
{
  if (ptr_data_plc != nullptr)
    return ptr_data_plc->rvalue;
  return 0;
}

uint16_t Reg_c::get_shm_val()
{
  if (is_shm())
    return ptr_data_shm->rvalue;
  return 0;
}

uint16_t Reg_c::get_local()
{
  if (is_shm()) {
    regdata_t mem;
    memcpy(&mem, ptr_data_shm, sizeof(regdata_t));
    return mem.rvalue;
  }
  return 0;
}

void Reg_c::set_plc_val(uint16_t _val)
{
  if (ptr_data_plc != nullptr)
    if (ptr_data_plc->rvalue != _val) {
      ptr_data_plc->rvalue = _val;
      ptr_data_plc->rupdate = 1;
    }
}

void Reg_c::set_shm_val(uint16_t _val)
{
  if (is_shm())
    ptr_data_shm->rvalue = _val;
}

void Reg_c::set_local(uint16_t _val)
{
  LOGD("%d %s %x.", _val, rn, this);
  if (is_shm()) {
    regdata_t mem;
    mem.rvalue = _val;
    memcpy(ptr_data_shm, &mem, sizeof(regdata_t));
  }
}

int Reg_c::get_mode()
{
  if (ptr_data_plc != nullptr)
    return ptr_data_plc->rmode;
  return -1;
}

int Reg_c::get_type()
{
  if (ptr_data_plc != nullptr)
    return ptr_data_plc->rtype;
  return -1;
}
