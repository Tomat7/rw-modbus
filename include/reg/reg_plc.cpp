// reg_plc.cpp ----------------------------
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



uint16_t Reg_c::get_plc_reg(reg_t* rptr)  // Set reg's local value != read PLC.
{
  uint16_t rval = bad_value.ui16;
  if (is_modbus && (rptr != nullptr)) {
    if (rptr->data.rerrors)
      rval = bad_value.ui16;
    else
      rval = PLC_c::get_reg(rptr);
  } else
    LOGE("Not Modbus get-reg: %s", str_fullname.c_str());
  return rval;
}

uint16_t Reg_c::get_plc_reg(int x)  // Get reg's local value != read PLC.
{
  return get_plc_reg(ptr_reg[x]);
}

value_u Reg_c::get_plc_value()
{
  value_u v;
  for (int i = 0; i < var_size; i++)
    v.dbl2u[i] = get_plc_reg(i);
  return v;
}

// =======================================


void Reg_c::set_plc_reg(uint16_t _val, reg_t* rptr)
{
  if (is_modbus && (rptr != nullptr))
    PLC_c::set_reg(_val, rptr);
  else
    LOGE("Not Modbus set-reg: %s", str_fullname.c_str());
}

void Reg_c::set_plc_reg(uint16_t _val, int x)  // Set reg's local value != read PLC.
{
  set_plc_reg(_val, ptr_reg[x]);
  return;
}

void Reg_c::set_plc_value(value_u v)
{
  for (int i = 0; i < var_size; i++)
    set_plc_reg(v.dbl2u[i], i);
  return;
}

// ========================================

int Reg_c::get_plc_errors()  // Get reg's local value != read PLC.
{
  int err = 0;
  if (is_modbus) {
    for (int i = 0; i < var_size; i++)
      if (ptr_reg[i] != nullptr)
        err += ptr_reg[i]->data.rerrors;
      else
        LOGE("NULL pointer on reg: %s", str_fullname.c_str());
  }
  return err;
}


// eof

