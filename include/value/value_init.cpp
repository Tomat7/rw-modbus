/*
  #include <open62541/plugin/log_stdout.h>
  #include <open62541/server.h>
  #include <open62541/server_config_default.h>
*/
//#include "include/open62541/open62541.h"

#include <map>
#include <mutex>
#include <set>
#include <thread>
#include <typeindex>
#include <typeinfo>

#include "include/console.h"
#include "include/logger.h"
#include "value_class.h"

#define _TYPE_INDEX(_XNUM) type_index(typeid(_XNUM))
#define _UA_TYPE(_XNUM) ua_types[_TYPE_INDEX(_XNUM)]

#define DEBUG(a) if(isDebug){a}

using std::set;
using std::to_string;


Value_c::Value_c(Value_c &V)
{
  //_value.ui64 = V._value.ui64;
  //_type_index = V._type_index;
  //_type = V._type;
  if (init_type(V._type_index))
    init_var(&V._value.ui64);
  else
    LOGA("Value_c: TYPE& not supported");
  LOGx("Value_C: COPY %u", ui64);
};


Value_c& Value_c::operator= (Value_c &V)
{
  //_value.ui64 = V._value.ui64;
  //_type_index = V._type_index;
  //_type = V._type;
  if (init_type(V._type_index))
    init_var(&V._value.ui64);
  else
    LOGA("Value_c: TYPE= not supported");
  LOGx("Value_C: op=V %s", c_str());
  return *this;
}


bool Value_c::init_type(type_index _ti)
{
  bool rc = false;
  if (type_map.count(_ti)) {
    _type_index = _ti;
    _type = type_map[_ti];
    rc = true;
  } else
    LOGA("Value_C: wrong TYPE used!");
  return rc;
}

void Value_c::init_var(void* _ptr)
{

  switch (_type) {
  case UA_TYPES_INT16:
    _value.i16 = *static_cast<int16_t*>(_ptr);
    break;
  case UA_TYPES_INT32:
    _value.i32 = *static_cast<int32_t*>(_ptr);
    break;
  case UA_TYPES_INT64:
    _value.i64 = *static_cast<int64_t*>(_ptr);
    break;
  case UA_TYPES_UINT16:
    _value.ui16 = *static_cast<uint16_t*>(_ptr);
    break;
  case UA_TYPES_UINT32:
    _value.ui32 = *static_cast<uint32_t*>(_ptr);
    break;
  case UA_TYPES_UINT64:
    _value.ui64 = *static_cast<uint64_t*>(_ptr);
    break;
  case UA_TYPES_FLOAT:
    _value.fl = *static_cast<float*>(_ptr);
    break;
  case UA_TYPES_DOUBLE:
    _value.dbl = *static_cast<double*>(_ptr);
    break;
  default:
    LOGE("Type: %i not supported", _type);
  }

}


/*
  Value_c::Value_c(int16_t x)
  {
  _value.i16 = x;
  _type_index = type_index(typeid(x));
  _type = type_map[type_index(typeid(x))];
  LOGx("Value_C: i16 %i", i16);
  };

  Value_c::Value_c(int32_t x)
  {
  _value.i32 = x;
  _type_index = type_index(typeid(x));
  _type = type_map[type_index(typeid(x))];
  LOGx("Value_C: i32 %i", i32);
  };

  Value_c::Value_c(int64_t x)
  {
  _value.i64 = x;
  _type_index = type_index(typeid(x));
  _type = type_map[type_index(typeid(x))];
  LOGx("Value_C: i64 %i", i64);
  };

  Value_c::Value_c(uint16_t x)
  {
  _value.ui16 = x;
  _type_index = type_index(typeid(x));
  _type = type_map[type_index(typeid(x))];
  LOGx("Value_C: ui16 %u %i", ui16, _type);
  };

  Value_c::Value_c(uint32_t x)
  {
  _value.ui32 = x;
  _type_index = type_index(typeid(x));
  _type = type_map[type_index(typeid(x))];
  LOGx("Value_C: ui32 %u", ui32);
  };

  Value_c::Value_c(uint64_t x)
  {
  _value.ui64 = x;
  init_type(type_index(typeid(x)));
  LOGx("Value_C: ui64 %u", ui64);
  };

  Value_c::Value_c(float x)
  {
  _value.fl = x;
  init_type(type_index(typeid(x)));
  LOGx("Value_C: float %f %s", fl, format_map[_type]);
  };

  Value_c::Value_c(double x)
  {
  _value.dbl = x;
  init_type(type_index(typeid(x)));
  LOGx("Value_C: double %f", dbl);
  };
*/

// eof