
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
uint16_t OpcServer_c::ReadOpcChannel(string s) { return getValue(s).ui16; }
template <>
int16_t OpcServer_c::ReadOpcChannel(string s) { return getValue(s).i16; }
template <>
uint32_t OpcServer_c::ReadOpcChannel(string s) { return getValue(s).ui32; }
template <>
int32_t OpcServer_c::ReadOpcChannel(string s) { return getValue(s).i32; }
template <>
uint64_t OpcServer_c::ReadOpcChannel(string s) { return getValue(s).ui64; }
template <>
int64_t OpcServer_c::ReadOpcChannel(string s) { return getValue(s).i64; }
template <>
float OpcServer_c::ReadOpcChannel(string s)
{
  /*   float fl = -999;
    getVar(s, fl);
    return fl;
  */  return getValue(s).fl;
}
template <>
double OpcServer_c::ReadOpcChannel(string s) { return getValue(s).dbl; }
// eof