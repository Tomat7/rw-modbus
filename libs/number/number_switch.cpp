// value_ops.cpp ----------------------------
// Copyright 2025 Tomat7 (star0413@gmail.com)

#include <math.h>

#include <map>
//#include <mutex>
//#include <set>
//#include <thread>
#include <typeindex>
#include <typeinfo>
#include <cassert>

//#include "include/console.h"
#include "include/logger.h"
#include "number_class.h"

#define DEBUG(a) if(isDebug){a}

float128 Number_c::_as_f128()
{
  switch (_type_ua) {
  case UA_TYPES_INT16:
    return (float128)value.i16; // ok
  case UA_TYPES_INT32:
    return (float128)value.i32; // ok
  case UA_TYPES_INT64:
    return (float128)value.i64; // bad! - good with float128
  case UA_TYPES_UINT16:
    return (float128)value.ui16; // ok
  case UA_TYPES_UINT32:
    return (float128)value.ui32; // ok
  case UA_TYPES_UINT64:
    return (float128)value.ui64; // bad! - good with float128
  case UA_TYPES_FLOAT:
    return (float128)value.fl; // ok
  case UA_TYPES_DOUBLE:
    return (float128)value.dbl; // ok, but useless
  case NOTUA_TYPES_F10:
    return (float128)value.fl; // ok
  case NOTUA_TYPES_F100:
    return (float128)value.fl; // ok
  default:
    LOGE("Type: %i not supported", _type_ua);
    return (float128)value.ui16;
  }
}

// ===============================================================

// Suppress GCC warnings on "fmt" which is nonliteral
#pragma GCC diagnostic push // Save current diagnostic warning set
#pragma GCC diagnostic ignored "-Wformat-nonliteral" // Suppressed here

#define NB_CHARS STR_SIZE

char* Number_c::_c_str(const char* fmt)
{
  switch (_type_ua) {
  case UA_TYPES_INT16:
    snprintf(_str, NB_CHARS, fmt, i16);
    break;
  case UA_TYPES_INT32:
    snprintf(_str, NB_CHARS, fmt, i32);
    break;
  case UA_TYPES_INT64:
    snprintf(_str, NB_CHARS, fmt, i64);
    break;
  case UA_TYPES_UINT16:
    snprintf(_str, NB_CHARS, fmt, ui16);
    break;
  case UA_TYPES_UINT32:
    snprintf(_str, NB_CHARS, fmt, ui32);
    break;
  case UA_TYPES_UINT64:
    snprintf(_str, NB_CHARS, fmt, ui64);
    break;
  case UA_TYPES_FLOAT:
    snprintf(_str, NB_CHARS, fmt, fl);
    break;
  case UA_TYPES_DOUBLE:
    snprintf(_str, NB_CHARS, fmt, dbl);
    break;
  case NOTUA_TYPES_F10:
    snprintf(_str, NB_CHARS, fmt, fl);
    break;
  case NOTUA_TYPES_F100:
    snprintf(_str, NB_CHARS, fmt, fl);
    break;
  default:
    LOGE("Type: %i not supported", _type_ua);
  }

  return _str;
}
#pragma GCC diagnostic pop  // Restore diagnostic warning set

// ==========================================================

bool Number_c::_set_value(float128 f128)
{
  switch (_type_ua) {
  case UA_TYPES_INT16:
    i16 = (int16_t)f128;
    return true;
  case UA_TYPES_INT32:
    i32 = (int32_t)f128;
    return true;
  case UA_TYPES_INT64:
    i64 = (int64_t)f128;
    return true;
  case UA_TYPES_UINT16:
    ui16 = (uint16_t)f128;
    return true;
  case UA_TYPES_UINT32:
    ui32 = (uint32_t)f128;
    return true;
  case UA_TYPES_UINT64:
    ui64 = (uint64_t)f128;
    return true;
  case UA_TYPES_FLOAT:
    fl = (float)f128;
    return true;
  case UA_TYPES_DOUBLE:
    dbl = (double)f128;
    return true;
  case NOTUA_TYPES_F10:
    fl = (float)f128;
    return true;
  case NOTUA_TYPES_F100:
    fl = (float)f128;
    return true;
  default:
    LOGE("Type: %i not supported", _type_ua);
    return false;
  }
}

// eof