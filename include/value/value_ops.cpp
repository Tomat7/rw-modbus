// value_ops.cpp ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <math.h>

#include <map>
#include <mutex>
#include <set>
#include <thread>
#include <typeindex>
#include <typeinfo>

#include "include/console.h"
#include "include/logger.h"
#include "value_class.h"

#define DEBUG(a) if(isDebug){a}

using std::set;
using std::to_string;
/*
  bool Value_c::operator<(Value_c &v2) {
  if (this->_type == v2._type)
    return this->ui64 < v2.ui64;
  else
    return (atof(this->c_str()) < atof(v2.c_str()));
  }

  bool operator>(Value_c &v1, Value_c &v2) {
  if (v1._type == v2._type)
    return v1.ui64 > v2.ui64;
  else
    return (atof(v1.c_str()) > atof(v2.c_str()));
  }
*/

Value_c& Value_c::operator= (const Value_c &V)
{
  _value.ui64 = V._value.ui64;
  _type_index = V._type_index;
  _type = V._type;
  LOG_("Value_C: op=*THIS %s", c_str());
  return *this;
}

char* Value_c::c_str()
{
  return c_str(_str);
}


// ===============================================================
#pragma GCC diagnostic push // Save current diagnostic warning set
// Suppress GCC warnings on "var_format" which is nonliteral
#pragma GCC diagnostic ignored "-Wformat-nonliteral"

#define NB_CHARS 19

char* Value_c::c_str(char* retch)
{
  if (format_map.count(_type)) {
    switch (_type) {
    case UA_TYPES_INT16:
      snprintf(retch, NB_CHARS, format_map[_type], i16);
      return retch;
      break;
    case UA_TYPES_INT32:
      snprintf(retch, NB_CHARS, format_map[_type], i32);
      return retch;
      break;
    case UA_TYPES_INT64:
      snprintf(retch, NB_CHARS, format_map[_type], i64);
      return retch;
      break;
    case UA_TYPES_UINT16:
      snprintf(retch, NB_CHARS, format_map[_type], ui16);
      return retch;
      break;
    case UA_TYPES_UINT32:
      snprintf(retch, NB_CHARS, format_map[_type], ui32);
      return retch;
      break;
    case UA_TYPES_UINT64:
      snprintf(retch, NB_CHARS, format_map[_type], ui64);
      return retch;
      break;
    case UA_TYPES_FLOAT:
      snprintf(retch, NB_CHARS, format_map[_type], fl);
      return retch;
      break;
    case UA_TYPES_DOUBLE:
      snprintf(retch, NB_CHARS, format_map[_type], dbl);
      return retch;
      break;
    default:
      LOGE("Type: %i not supported", _type);
    }
  } else
    LOGE("Type_format wrong - %i", _type);

  return retch;
}
#pragma GCC diagnostic pop  // Restore diagnostic warning set
// ==========================================================

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