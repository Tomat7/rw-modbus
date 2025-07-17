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


Value_c::Value_c(const Value_c &V)
{
  _value.ui64 = V._value.ui64;
  _type_index = V._type_index;
  _type = V._type;
  LOG_("Value_C: COPY %u", ui64);
};


Value_c::Value_c(int16_t x)
{
  _value.i16 = x;
  _type_index = type_index(typeid(x));
  _type = type_map[type_index(typeid(x))];
  LOG_("Value_C: i16 %i", i16);
};

Value_c::Value_c(int32_t x)
{
  _value.i32 = x;
  _type_index = type_index(typeid(x));
  _type = type_map[type_index(typeid(x))];
  LOG_("Value_C: i32 %i", i32);
};

Value_c::Value_c(int64_t x)
{
  _value.i64 = x;
  _type_index = type_index(typeid(x));
  _type = type_map[type_index(typeid(x))];
  LOG_("Value_C: i64 %i", i64);
};

Value_c::Value_c(uint16_t x)
{
  _value.ui16 = x;
  _type_index = type_index(typeid(x));
  _type = type_map[type_index(typeid(x))];
  LOG_("Value_C: ui16 %u %i", ui16, _type);
};

Value_c::Value_c(uint32_t x)
{
  _value.ui32 = x;
  _type_index = type_index(typeid(x));
  _type = type_map[type_index(typeid(x))];
  LOG_("Value_C: ui32 %u", ui32);
};

Value_c::Value_c(uint64_t x)
{
  _value.ui64 = x;
  _type_index = type_index(typeid(x));
  _type = type_map[type_index(typeid(x))];
  LOG_("Value_C: ui64 %u", ui64);
};

Value_c::Value_c(float x)
{
  _value.fl = x;
  _type_index = type_index(typeid(x));
  _type = type_map[type_index(typeid(x))];
  LOG_("Value_C: float %f %s", fl, format_map[_type]);
};

Value_c::Value_c(double x)
{
  _value.dbl = x;
  _type_index = type_index(typeid(x));
  _type = type_map[type_index(typeid(x))];
  LOG_("Value_C: double %f", dbl);
};


// eof