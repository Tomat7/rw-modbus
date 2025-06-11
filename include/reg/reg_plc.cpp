// reg_plc.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

//#include <unistd.h>

//#include <string>

#include "include/logger.h"
#include "include/plc/plc_class.h"
#include "reg_class.h"
//#include "include/shmem/shmem.h"

#ifdef SYSLOG_NAME
#undef SYSLOG_NAME
#endif
#define SYSLOG_NAME "REG-class"

reg_t* Reg_c::get_ptr(int x) { return ptr_reg[x]; }

uint16_t Reg_c::get_plc_reg(reg_t* rptr)  // Set reg's local value != read PLC.
{
  uint16_t rval = bad_value.ui16;
  if ((is_modbus || is_ref) && (rptr != nullptr)) {
    var_errors = rptr->data.rerrors;
    if (var_errors)
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
  value_u val;
  if (is_scada) {
    LOGE("Not Modbus get-value: %s", str_fullname.c_str());
    return val;
  }

  bool has_errors = get_plc_errors();

  if (var_size == 1) {
    if (var_type == UA_TYPES_UINT16)
      val.ui16 = (has_errors) ? bad_value.ui16 : (uint16_t)get_plc_reg();
    else if (var_type == UA_TYPES_INT16)
      val.i16 = (has_errors) ? bad_value.i16 : (int16_t)get_plc_reg();
    else if (byte_order == BO_F100)
      val.fl = (has_errors) ? bad_value.fl : (int16_t)get_plc_reg() * 0.01f;
    else if (byte_order == BO_F10)
      val.fl = (has_errors) ? bad_value.fl : (int16_t)get_plc_reg() * 0.1f;
  } else if (var_size == 2) {
    //val = pull_plc_regs_by_order(byte_order);
    val = pull_plc_value32();
    if (var_type == UA_TYPES_UINT32)
      val.ui32 = (has_errors) ? bad_value.ui32 : val.ui32;
    else if (var_type == UA_TYPES_INT32)
      val.i32 = (has_errors) ? bad_value.i32 : val.i32;
    else if (var_type == UA_TYPES_FLOAT)
      val.fl = (has_errors) ? bad_value.fl : val.fl;
  } else if (var_size == 4) {
    //val = pull_plc_regs_by_order(byte_order);
    val = pull_plc_value64();
    if (var_type == UA_TYPES_UINT64)
      val.ui64 = (has_errors) ? bad_value.ui64 : val.ui64;
    else if (var_type == UA_TYPES_INT64)
      val.i64 = (has_errors) ? bad_value.i64 : val.i64;
    else if (var_type == UA_TYPES_DOUBLE)
      val.dbl = (has_errors) ? bad_value.dbl : val.dbl;
  } else
    val.ui64 = 1111111;

  value = val;

  return value;
}

// =======================================

void Reg_c::set_plc_reg(uint16_t _val, reg_t* rptr)
{
  if ((is_modbus || is_ref) && (rptr != nullptr))
    PLC_c::set_reg(_val, rptr);
  else
    LOGE("Not Modbus set-reg: %s", str_fullname.c_str());
}


// Set reg's local value != read PLC.
void Reg_c::set_plc_reg(uint16_t _val, int x)
{
  set_plc_reg(_val, ptr_reg[x]);
  return;
}

void Reg_c::set_plc_value(value_u v)
{
  // byte-order not supported yet :-(
  if (is_modbus || is_ref)
    for (int i = 0; i < var_size; i++)
      set_plc_reg(v.mb64u[i], i);
  else
    LOGE("Not Modbus set-value: %s", str_fullname.c_str());

  return;
}

// ========================================

int Reg_c::get_plc_errors()  // Get reg's local value != read PLC.
{
  int err = 0;

  for (int i = 0; i < var_size; i++) {
    if ((ptr_reg[i] != nullptr) && !is_scada)
      err += ptr_reg[i]->data.rerrors;
    else
      LOGE("NULL pointer on reg or Scada: %s", str_fullname.c_str());
  }

  return err;
}

// eof
