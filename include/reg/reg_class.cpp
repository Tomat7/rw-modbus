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
#include <string>

#include "include/logger.h"
#include "include/plc/plc_class.h"
#include "include/shmem/shmem.h"

#ifdef SYSLOG_NAME
#undef SYSLOG_NAME
#endif
#define SYSLOG_NAME "REG-class"


bool Reg_c::is_MB() { return (src_reference == ""); }
bool Reg_c::is_Scada() { return (src_reference == "-"); }
bool Reg_c::has_Ref() { return !(is_MB() || is_Scada()); }

bool Reg_c::has_Str(string SS, string fs) { return SS.find(fs) != std::string::npos; }

bool Reg_c::is_shm()
{
  bool ret = true;
  /*
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
  */
  return ret;
}


void Reg_c::sync(uint16_t _val)
{
  /*   value.ui16 = _val;
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
    return; */
}

void Reg_c::sync()
{
  /*   sync(get_plc_val()); */
  return;
}

void Reg_c::sync_regdata(regdata_t* ptr_data)
{
  /*   if (ptr_data != nullptr) {
      value.ui16 = ptr_data->rvalue;

      value_u vu;
      value = vu;

      if (ptr_data_shm != nullptr) {
        regdata_t mem;
        mem.rvalue = value.ui16;

        mem.rerrors = ptr_data->rerrors;
        mem.rstatus = ptr_data->rstatus;
        mem.rmode = ptr_data->rmode;
        mem.rtype = ptr_data->rtype;

        memcpy(ptr_data_shm, &mem, sizeof(regdata_t));
      }
    }
    return; */
}

/* uint16_t Reg_c::get_plc_val()
  {
  if (ptr_data_plc != nullptr)
    return ptr_data_plc->rvalue;
  return 0;
  }
*/
/* uint16_t Reg_c::get_shm_val()
  {
  if (is_shm())
    return ptr_data_shm->rvalue;
  return 0;
  }
*/
uint16_t Reg_c::get_local()
{
  /*   if (is_shm()) {
      regdata_t mem;
      memcpy(&mem, ptr_data_shm, sizeof(regdata_t));
      return mem.rvalue;
    } */
  return 0;
}
/*
  void Reg_c::set_plc_val(uint16_t _val)
  {
  if (ptr_data_plc != nullptr)
    if (ptr_data_plc->rvalue != _val) {
      ptr_data_plc->rvalue = _val;
      ptr_data_plc->rupdate = 1;
    }
  }
*/
/* void Reg_c::set_shm_val(uint16_t _val)
  {
  if (is_shm())
    ptr_data_shm->rvalue = _val;
  }
*/

void Reg_c::set_local(uint16_t _val)
{
  /*   LOGD("%d %s %x.", _val, rn, this);
    if (is_shm()) {
      regdata_t mem;
      mem.rvalue = _val;
      memcpy(ptr_data_shm, &mem, sizeof(regdata_t));
    } */
}

int Reg_c::get_mode() // 1 = "rw"
{
  /*   if (ptr_data_plc != nullptr)
      return ptr_data_plc->rmode; */
  return -1;
}

int Reg_c::get_type()
{
  /*   if (ptr_data_plc != nullptr)
      return ptr_data_plc->rtype; */
  return -1;
}
