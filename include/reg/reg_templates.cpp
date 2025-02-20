
#include "reg_class.h"

#include <fcntl.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <syslog.h>
#include <unistd.h>

#include <chrono>
#include <map>
#include <mutex>
#include <cmath>

#include "include/logger.h"
#include "include/plc/plc_class.h"
#include "include/shmem/shmem.h"

#ifdef SYSLOG_NAME
#undef SYSLOG_NAME
#endif
#define SYSLOG_NAME "REG-class"

#define DEBUG(a) if (isDebug) { a }

regdata_t* ptr_data_plc = nullptr;

//    ptr_data_plc->rupdate = 1;
/*
        if (ptr_data_plc != nullptr) {

          if (ptr_data_plc->rtype == TYPE_F100)
            ptr_data_plc->rvalue = (uint16_t)(_val * 100);
          else if (ptr_data_plc->rtype == TYPE_FLOAT_ABCD) {
            value.fl = _val;
            ptr_data_plc->rvalue = value.fl2u[1];
            (ptr_reg->r_next)->data.rvalue = value.fl2u[0];
          } else if (ptr_data_plc->rtype == TYPE_FLOAT_CDAB) {
            value.fl = _val;
            ptr_data_plc->rvalue = value.fl2u[0];
            (ptr_reg->r_next)->data.rvalue = value.fl2u[1];
          }
        }
*/


template <>
uint16_t Reg_c::set_value(uint16_t _val)
{
  uint16_t _ui16 = value.ui16;
  value.ui16 = _val;
  /*   if (ptr_data_plc != nullptr) {
      if (ptr_data_plc->rvalue != _val) {
        ptr_data_plc->rvalue = _val;
        ptr_data_plc->rupdate = 1;
      }
    } */
  return _ui16;
}

template <>
int16_t Reg_c::set_value(int16_t _val)
{
  int16_t _i16 = value.i16;
  value.i16 = _val;
  /*   if (ptr_data_plc != nullptr)
      if (ptr_data_plc->rvalue != (uint16_t)_val) {
        ptr_data_plc->rvalue = (uint16_t)_val;
        ptr_data_plc->rupdate = 1;
      } */
  return _i16;
}

template <>
float Reg_c::set_value(float _val)
{
  float _fl = value.fl;

  if (fabs(_fl - _val) > 0.01)
    value.fl = _val;

  return _fl;
}


template <typename T>
T Reg_c::set_value(T _val)
{
  return T();
}

// =============================================

template <>
uint16_t Reg_c::get_value()
{
  if (ptr_data_plc != nullptr) {
    value.ui16 = ptr_data_plc->rvalue;
    return value.ui16;
  }
  return (uint16_t)0;
}

template <>
int16_t Reg_c::get_value()
{
  if (ptr_data_plc != nullptr) {
    value.i16 = (int16_t)(ptr_data_plc->rvalue);
    return value.i16;
  }
  return (int16_t)0;
}


template <>
float Reg_c::get_value()
{
  value_u v;
  v.ui64 = 0;
  v.fl = bad_value.fl;

  for (int i = 0; i < var_size; i++)
    v.dbl2u[i] = get_plc_reg(ptr_reg[i]);

  if (var_type == NOTUA_TYPES_F100)
    value.fl = v.i16 * (float)0.01;
  else if (var_type == UA_TYPES_FLOAT) {
    if (byte_order == BO_HH) {
      value.fl2u[1] = v.dbl2u[0];
      value.fl2u[0] = v.dbl2u[1];;
    } else if (byte_order == BO_HL) {
      value.fl2u[0] = v.dbl2u[0];;
      value.fl2u[1] = v.dbl2u[0];;
    }
  } else
    value.fl = -99.12f;

  return value.fl;
}

/*
  template <>
  int16_t OpcServer_c::getValue(string s)
  {
  int16_t x = bad_value.i16;
  if (!getVariableValue(s, x))
    if (isVariable(s))
      x = vars[s].value.i16;
  return x;
  }

  template <>
  uint32_t OpcServer_c::getValue(string s)
  {
  uint32_t x = bad_value.ui32;
  if (!getVariableValue(s, x))
    if (isVariable(s))
      x = vars[s].value.ui32;
  return x;
  }

  template <>
  int32_t OpcServer_c::getValue(string s)
  {
  int32_t x = bad_value.i32;
  if (!getVariableValue(s, x))
    if (isVariable(s))
      x = vars[s].value.i32;
  return x;
  }

  template <>
  uint64_t OpcServer_c::getValue(string s)
  {
  uint64_t x = bad_value.ui64;
  if (!getVariableValue(s, x))
    if (isVariable(s))
      x = vars[s].value.ui64;
  return x;
  }

  template <>
  int64_t OpcServer_c::getValue(string s)
  {
  int64_t x = bad_value.i64;
  if (!getVariableValue(s, x))
    if (isVariable(s))
      x = vars[s].value.i64;
  return x;
  }

  template <>
  float OpcServer_c::getValue(string s)
  {
  float x = bad_value.fl;
  if (!getVariableValue(s, x))
    if (isVariable(s))
      x = vars[s].value.fl;
  return x;
  }

  template <>
  double OpcServer_c::getValue(string s)
  {
  double x = bad_value.dbl;
  if (!getVariableValue(s, x))
    if (isVariable(s))
      x = vars[s].value.dbl;
  return x;
  }
*/
// ========================================

template <>
uint16_t Reg_c::read_value()
{
  return value.ui16; // else - the "BAD" value will return
}

template <>
int16_t Reg_c::read_value()
{
  return value.i16;
}

template <>
float Reg_c::read_value()
{
  return value.fl;
}


/*
  template <>
  uint32_t OpcServer_c::readValue(string s)
  {
  uint32_t x = bad_value.ui32;
  if (isVariable(s))
    x = vars[s].value.ui32;
  return x;
  }

  template <>
  int32_t OpcServer_c::readValue(string s)
  {
  int32_t x = bad_value.i32;
  if (isVariable(s))
    x = vars[s].value.i32;
  return x;
  }

  template <>
  uint64_t OpcServer_c::readValue(string s)
  {
  uint64_t x = bad_value.ui64;
  if (isVariable(s))
    x = vars[s].value.ui64;
  return x;
  }

  template <>
  int64_t OpcServer_c::readValue(string s)
  {
  int64_t x = bad_value.i64;
  if (isVariable(s))
    x = vars[s].value.i64;
  return x;
  }
*/

/*
  template <>
  double OpcServer_c::readValue(string s)
  {
  double x = bad_value.dbl;
  if (isVariable(s))
    x = vars[s].value.dbl;
  return x;
  }
*/
// eof
