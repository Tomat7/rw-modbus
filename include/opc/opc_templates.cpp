
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
  return getVar(s, x);
}

template <>
int16_t OpcServer_c::getValue(string s)
{
  int16_t x = bad_value.i16;
  return getVar(s, x);
}

template <>
uint32_t OpcServer_c::getValue(string s) { return getVarUnion(s).ui32; }
template <>
int32_t OpcServer_c::getValue(string s) { return getVarUnion(s).i32; }
template <>
uint64_t OpcServer_c::getValue(string s) { return getVarUnion(s).ui64; }

template <>
int64_t OpcServer_c::getValue(string s)
{
  int64_t x = bad_value.i64;
  return getVar(s, x);
}

template <>
float OpcServer_c::getValue(string s)
{
  float x = bad_value.fl;
  return getVar(s, x);
}

template <>
double OpcServer_c::getValue(string s)
{
  double x = bad_value.dbl;
  return getVar(s, x);
}

// eof
