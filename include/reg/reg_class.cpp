// reg_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//

#include "reg_class.h"

#include <fcntl.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <syslog.h>
#include <unistd.h>

#include <chrono>
#include <cstdio>
#include <map>
#include <mutex>
#include <string>

#include "include/logger.h"
#include "include/plc/plc_class.h"
//#include "include/shmem/shmem.h"

#ifdef SYSLOG_NAME
#undef SYSLOG_NAME
#endif
#define SYSLOG_NAME "REG-class"

// ===============================================================

value_u Reg_c::get_local_value() { return value; }

void Reg_c::set_local_value(value_u _val) { value = _val; }

string Reg_c::to_lower(string str)
{
  for (auto &c : str)
    c = static_cast<char>(tolower(c));
  return str;
}

void Reg_c::remove_dbl_slashes(string &str)
{
  auto dbl_slash = str.find("//");
  if (dbl_slash != std::string::npos)
    str.erase(dbl_slash, 1);
}

bool Reg_c::has_Str(string SS, string fs)
{
  return SS.find(fs) != std::string::npos;
}

variant_t Reg_c::get_local_variant()
{
  switch (var_type) {
  case UA_TYPES_INT16:
    return value.i16;
    break;
  case UA_TYPES_UINT16:
    return value.ui16;
    break;
  case UA_TYPES_INT32:
    return value.i32;
    break;
  case UA_TYPES_UINT32:
    return value.ui32;
    break;
  case UA_TYPES_INT64:
    return value.i64;
    break;
  case UA_TYPES_UINT64:
    return value.ui64;
    break;
  case UA_TYPES_FLOAT:
    return value.fl;
    break;
  case UA_TYPES_DOUBLE:
    return value.dbl;
    break;
  case NOTUA_TYPES_F100:
    return value.fl;
    break;
  case NOTUA_TYPES_F10:
    return value.fl;
    break;
  default:
    LOGE("Type: %i not supported: %s", var_type, str_opcname.c_str());
  }

  return value.i16;
}

char* Reg_c::get_local_value_chars(char* retch)
{
  if (var_format == nullptr)
    LOGE("Type_format wrong: %s", var_format);
  else
    snprintf(retch, 49, var_format, get_local_variant());

  return retch;
}

/*
  // ===============================================================
  #pragma GCC diagnostic push // Save current diagnostic warning set
  // Suppress GCC warnings on "var_format" which is nonliteral
  #pragma GCC diagnostic ignored "-Wformat-nonliteral"

  char* Reg_c::get_local_value_chars(char* retch)
  {
  if (var_format == nullptr)
    LOGE("Reg: %s, type_format wrong: %s", rn, var_format);

  else if (var_type == UA_TYPES_INT16)
    snprintf(retch, 49, var_format, value.i16);
  else if (var_type == UA_TYPES_UINT16)
    snprintf(retch, 49, var_format, value.ui16);
  else if (var_type == UA_TYPES_INT32)
    snprintf(retch, 49, var_format, value.i32);
  else if (var_type == UA_TYPES_UINT32)
    snprintf(retch, 49, var_format, value.ui32);
  else if (var_type == UA_TYPES_INT64)
    snprintf(retch, 49, var_format, value.i64);
  else if (var_type == UA_TYPES_UINT64)
    snprintf(retch, 49, var_format, value.ui64);
  else if (var_type == UA_TYPES_FLOAT)
    snprintf(retch, 49, var_format, value.fl);
  else if (var_type == UA_TYPES_DOUBLE)
    snprintf(retch, 49, var_format, value.dbl);
  else if (var_type == NOTUA_TYPES_F100)
    snprintf(retch, 49, var_format, value.fl);
  else if (var_type == NOTUA_TYPES_F10)
    snprintf(retch, 49, var_format, value.fl);

  else
    LOGE("Reg: %s, type not supported: %i", rn, var_type);

  return retch;
  }
  #pragma GCC diagnostic pop  // Restore diagnostic warning set
  // ==========================================================
*/

/*
  char* Reg_c::get_local_value_chars(char* retch)
  {

  if (var_format == nullptr)
    LOGE("Type_format wrong: %s", var_format);
  else if (var_type == UA_TYPES_DOUBLE)
    snprintf(retch, 49, var_format, value.dbl);
  else if (var_type == UA_TYPES_FLOAT)
    snprintf(retch, 49, "%10.3f", value.fl);
  else if (var_type == NOTUA_TYPES_F100)
    snprintf(retch, 49, "%7.2f", value.fl);
  else if (var_type == NOTUA_TYPES_F10)
    snprintf(retch, 49, "%7.1f", value.fl);
  else if (var_type == UA_TYPES_UINT16)
    snprintf(retch, 49, "%u", value.ui16);
  else if (var_type == UA_TYPES_INT16)
    snprintf(retch, 49, "%i", value.i16);
  else if (var_type == UA_TYPES_UINT32)
    snprintf(retch, 49, "%#010x", value.ui32);
  else if (var_type == UA_TYPES_INT32)
    snprintf(retch, 49, "%#010x", value.i32);
  else if (var_type == UA_TYPES_UINT64)
    snprintf(retch, 49, "%#018lx", value.ui64);
  else if (var_type == UA_TYPES_INT64)
    snprintf(retch, 49, "%#018lx", value.i64);
  else
    LOGE("Type not supported: %i", var_type);

  return retch;
  }
*/

/*
  string Reg_c::get_local_value_string()
  {
  string rets;
  if (var_type == UA_TYPES_FLOAT)
    rets= to_string(value.fl);
  else if (var_type == NOTUA_TYPES_F100)
    rets= to_string(value.fl);
  else if (var_type == NOTUA_TYPES_F10)
    rets= to_string(value.fl);
  else if (var_type == UA_TYPES_UINT16)
    rets= to_string(value.ui16);
  else if (var_type == UA_TYPES_INT16)
    rets= to_string(value.i16);
  return rets;
  }
*/

// eof
