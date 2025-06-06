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

bool Reg_c::has_Str(string SS, string fs)
{
  return SS.find(fs) != std::string::npos;
}

// ========================================

value_u Reg_c::get_local_value() { return value; }

void Reg_c::set_local_value(value_u _val) { value = _val; }


char* Reg_c::get_local_value_chars(char* retch)
{
  if (var_type == UA_TYPES_DOUBLE)
    snprintf(retch, 49, "%14.4f", value.dbl);
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
  else
    LOGE("Type not supported: %i", var_type);

  return retch;
}

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
