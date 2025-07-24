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
#include "value_class.h"

#define DEBUG(a) if(isDebug){a}

//using std::set;
using std::to_string;

bool Value_c::operator==(float &x) { return (fabs(_dbl() - x) <= DBL_EPSILON); }
bool Value_c::operator==(double &x) { return (fabs(_dbl() - x) <= DBL_EPSILON); }

bool Value_c::operator!=(float &x) { return (fabs(_dbl() - x) > DBL_EPSILON); }
bool Value_c::operator!=(double &x) { return (fabs(_dbl() - x) > DBL_EPSILON); }

// ==========================================================================

bool Value_c::operator<(Value_c &v2) { return ((double)*this < (double)v2); }

bool Value_c::operator>(Value_c &v2) { return ((double)*this > (double)v2); }

bool Value_c::operator==(Value_c &v2)
{
  if ((this->_type_ua == v2._type_ua) && _type_is_int)
    return this->ui64 == v2.ui64;
  else
    return fabs((double)*this - (double)v2) <= DBL_EPSILON;
}

bool Value_c::operator!=(Value_c &v2)
{
  if ((this->_type_ua == v2._type_ua) && _type_is_int)
    return this->ui64 != v2.ui64;
  else
    return fabs((double)*this - (double)v2) > DBL_EPSILON;
}

// =============================================================

char* Value_c::c_str(const char* _fmt)
{
  if (_fmt != nullptr)
    _type_fmt = _fmt; // Will remember format for next use!
  return _c_str(_type_fmt);
}

double Value_c::_fabs(double &x) { return fabs(*(double*)(_ptr) - x); }

double Value_c::_dbl() { return *(double*)_ptr; }

double Value_c::_to_dbl()
{
  const char* _fmt = _type_fmt;
  if (_type_ua == UA_TYPES_FLOAT)
    _fmt = "%.6f";
  else if (_type_ua == UA_TYPES_DOUBLE)
    _fmt = "%.15f";

  return atof(_c_str(_fmt));
}

double Value_c::_to_dbl(size_t _sz)
{
//  assert(_sz < 8);
  const char* _fmt = _type_fmt;
  if (_type_ua == UA_TYPES_FLOAT)
    _fmt = "%.6f";
  else if (_type_ua == UA_TYPES_DOUBLE)
    _fmt = "%.15f";

  return atof(_c_str(_fmt));
}

// ===============================================================
#pragma GCC diagnostic push // Save current diagnostic warning set
// Suppress GCC warnings on "var_format" which is nonliteral
#pragma GCC diagnostic ignored "-Wformat-nonliteral"

#define NB_CHARS STR_SIZE

char* Value_c::_c_str(const char* fmt)
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
  default:
    LOGE("Type: %i not supported", _type_ua);
  }


  return _str;
}
#pragma GCC diagnostic pop  // Restore diagnostic warning set
// ==========================================================

/*
  bool Value_c::operator==(Value_c &v2)
  {
  if ((this->_type_ua == v2._type_ua) && (_type_ua < UA_TYPES_FLOAT))
    return this->ui64 == v2.ui64;
  else
    return fabs(this->_to_dbl() - v2._to_dbl()) < DBL_EPSILON;
  }
*/
/*
  bool Value_c::operator!=(Value_c &v2)
  {
  if ((this->_type_ua == v2._type_ua) && (_type_ua < UA_TYPES_FLOAT))
    return this->ui64 != v2.ui64;
  else
    return fabs(this->_to_dbl() - v2._to_dbl()) > DBL_EPSILON;
  }
*/


/*
  bool Value_c::operator<(Value_c &v2)
  {
  if ((this->_type_ua == v2._type_ua) && (_type_ua < UA_TYPES_FLOAT))
    return this->ui64 < v2.ui64;
  else
    return (this->_to_dbl() < v2._to_dbl());
  }
*/
/*
  bool Value_c::operator>(Value_c &v2)
  {
  if ((this->_type_ua == v2._type_ua) && (_type_ua < UA_TYPES_FLOAT))
    return this->ui64 > v2.ui64;
  else
    return (this->_to_dbl() > v2._to_dbl());
  }
*/


/*
  Value_c::operator uint16_t()
  {
  uint16_t x;
  if (_type == type_map[type_index(typeid(x))])
    x = _value.ui16;
  else
    x = static_cast<float>(atof(c_str()));
  return x;
  };


  Value_c::operator float()
  {
  float x;
  if (_type == type_map[type_index(typeid(x))])
    x = _value.fl;
  else
    x = static_cast<float>(atof(c_str()));
  return x;
  };

*/

// eof