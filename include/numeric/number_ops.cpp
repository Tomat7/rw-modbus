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

//using std::set;
using std::to_string;
/*
  bool Number_c::operator==(float &x) { return (fabs(_as_dbl() - x) <= FLT_EPSILON); }
  bool Number_c::operator==(double &x) { return (fabs(_as_dbl() - x) <= DBL_EPSILON); }
  bool Number_c::operator!=(float &x) { return (fabs(_as_dbl() - x) > FLT_EPSILON); }
  bool Number_c::operator!=(double &x) { return (fabs(_as_dbl() - x) > DBL_EPSILON); }
*/
// ==========================================================================

// The next is very bad with (u)int64_t - already good with 'long double'
bool Number_c::operator<(Number_c &v2) { return ((float128)*this < (float128)v2); }
bool Number_c::operator>(Number_c &v2) { return ((float128)*this > (float128)v2); }

bool Number_c::operator==(Number_c &v2)
{
  if ((this->_type_ua == v2._type_ua) && _type_is_int)
    return this->ui64 == v2.ui64;
  else
    return fabsl((float128)*this - (float128)v2) <= DBL_EPSILON;
}

bool Number_c::operator!=(Number_c &v2)
{
  if ((this->_type_ua == v2._type_ua) && _type_is_int)
    return this->ui64 != v2.ui64;
  else
    return fabsl((float128)*this - (float128)v2) > DBL_EPSILON;
}

// =============================================================

char* Number_c::c_str(const char* _fmt)
{
  if (_fmt == nullptr)
    return _c_str(_type_fmt);
  else
    return _c_str(_fmt);
}

// double Number_c::_fabs(double &x) { return fabs(*(double*)(_ptr) - x); }

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

type_index Number_c::_get_typeidx()
{
  switch (_type_ua) {
  case UA_TYPES_INT16:
    return type_index(typeid(int16_t));
  case UA_TYPES_INT32:
    return type_index(typeid(int32_t));
  case UA_TYPES_INT64:
    return type_index(typeid(int64_t));
  case UA_TYPES_UINT16:
    return type_index(typeid(uint16_t));
  case UA_TYPES_UINT32:
    return type_index(typeid(uint32_t));
  case UA_TYPES_UINT64:
    return type_index(typeid(uint64_t));
  case UA_TYPES_FLOAT:
    return type_index(typeid(float));
  case UA_TYPES_DOUBLE:
    return type_index(typeid(double));
  case NOTUA_TYPES_F10:
    return type_index(typeid(float));
  case NOTUA_TYPES_F100:
    return type_index(typeid(float));
  default:
    LOGE("Type: %i not supported", _type_ua);
    return type_index(typeid(bool));
  }
}

bool Number_c::set_value(float128 f128)
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

/*
  double Number_c::_as_chars_to_dbl()
  {
  const char* _fmt = _type_fmt;
  if (_type_ua == UA_TYPES_FLOAT)
    _fmt = "%.6f";
  else if (_type_ua == UA_TYPES_DOUBLE)
    _fmt = "%.15f";

  return atof(_c_str(_fmt));
  }
*/

/*
  double Number_c::_as_dbl()
  {
  switch (_type_ua) {
  case UA_TYPES_INT16:
    return (double)_value.i16; // ok
  case UA_TYPES_INT32:
    return (double)_value.i32; // ok
  case UA_TYPES_INT64:
    LOGA("Conversation from 'int64_t' to ANY other impossible");
    return (double)_value.i64; // bad!
  case UA_TYPES_UINT16:
    return (double)_value.ui16; // ok
  case UA_TYPES_UINT32:
    return (double)_value.ui32; // ok
  case UA_TYPES_UINT64:
    LOGA("Conversation from 'uint64_t' to ANY other impossible");
    return (double)_value.ui64; // bad!
  case UA_TYPES_FLOAT:
    return (double)_value.fl; // ok
  case UA_TYPES_DOUBLE:
    return _value.dbl;        // ok, but useless
  default:
    LOGE("Type: %i not supported", _type_ua);
    return _value.ui16;
  }
  }
*/


/*
  bool Number_c::operator==(Number_c &v2)
  {
  if ((this->_type_ua == v2._type_ua) && (_type_ua < UA_TYPES_FLOAT))
    return this->ui64 == v2.ui64;
  else
    return fabs(this->_to_dbl() - v2._to_dbl()) < DBL_EPSILON;
  }
*/
/*
  bool Number_c::operator!=(Number_c &v2)
  {
  if ((this->_type_ua == v2._type_ua) && (_type_ua < UA_TYPES_FLOAT))
    return this->ui64 != v2.ui64;
  else
    return fabs(this->_to_dbl() - v2._to_dbl()) > DBL_EPSILON;
  }
*/


/*
  bool Number_c::operator<(Number_c &v2)
  {
  if ((this->_type_ua == v2._type_ua) && (_type_ua < UA_TYPES_FLOAT))
    return this->ui64 < v2.ui64;
  else
    return (this->_to_dbl() < v2._to_dbl());
  }
*/
/*
  bool Number_c::operator>(Number_c &v2)
  {
  if ((this->_type_ua == v2._type_ua) && (_type_ua < UA_TYPES_FLOAT))
    return this->ui64 > v2.ui64;
  else
    return (this->_to_dbl() > v2._to_dbl());
  }
*/


/*
  Number_c::operator uint16_t()
  {
  uint16_t x;
  if (_type == type_map[type_index(typeid(x))])
    x = _value.ui16;
  else
    x = static_cast<float>(atof(c_str()));
  return x;
  };


  Number_c::operator float()
  {
  float x;
  if (_type == type_map[type_index(typeid(x))])
    x = _value.fl;
  else
    x = static_cast<float>(atof(c_str()));
  return x;
  };

*/

/*
  double Number_c::_to_dbl(size_t _sz)
  {
  //  assert(_sz < 8);
  const char* _fmt = _type_fmt;
  if (_type_ua == UA_TYPES_FLOAT)
    _fmt = "%.6f";
  else if (_type_ua == UA_TYPES_DOUBLE)
    _fmt = "%.15f";

  return atof(_c_str(_fmt));
  }
*/

// eof