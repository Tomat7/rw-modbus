
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <map>
#include <mutex>
#include <set>
#include <thread>
#include <typeindex>
#include <typeinfo>

#include "include/logger.h"
#include "opc_class.h"

#define DEBUG(a) if (isDebug) {a}


template <>
uint16_t OpcServer_c::getValue(string s)
{
  uint16_t x = bad_value.ui16;
  if (!getVariableValue(s, x))  // if OK, return new value (x)
    if (isVariable(s))     // check for existing
      x = vars[s].value.ui16;   // if var exist - return last good value
  return x;                     // else - the "BAD" value will return
}

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

// ========================================

template <>
uint16_t OpcServer_c::readValue(string s)
{
  uint16_t x = bad_value.ui16;
  if (isVariable(s))     // check for existing
    x = vars[s].value.ui16;   // if var exist - return last good value
  return x;                     // else - the "BAD" value will return
}

template <>
int16_t OpcServer_c::readValue(string s)
{
  int16_t x = bad_value.i16;
  if (isVariable(s))
    x = vars[s].value.i16;
  return x;
}

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

template <>
float OpcServer_c::readValue(string s)
{
  float x = bad_value.fl;
  if (isVariable(s))
    x = vars[s].value.fl;
  return x;
}

template <>
double OpcServer_c::readValue(string s)
{
  double x = bad_value.dbl;
  if (isVariable(s))
    x = vars[s].value.dbl;
  return x;
}


// eof
