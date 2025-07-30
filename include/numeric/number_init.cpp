// Number_class.cpp --------------------------
// Copyright 2025 Tomat7 (star0413@gmail.com)

#include <map>
#include <typeindex>
#include <typeinfo>
//#include <set>
//#include <mutex>
//#include <thread>
//#include "include/console.h"

#include "include/logger.h"
#include "number_class.h"

#define _TYPE_INDEX(_XNUM) type_index(typeid(_XNUM))
#define _UA_TYPE(_XNUM) ua_types[_TYPE_INDEX(_XNUM)]

#define DEBUG(a) if(isDebug){a}


map<type_index, int> Number_c::typeidx_ua_map {
  {type_index(typeid(int16_t)),  UA_TYPES_INT16 },
  {type_index(typeid(uint16_t)), UA_TYPES_UINT16},
  {type_index(typeid(int32_t)),  UA_TYPES_INT32 },
  {type_index(typeid(uint32_t)), UA_TYPES_UINT32},
  {type_index(typeid(int64_t)),  UA_TYPES_INT64 },
  {type_index(typeid(uint64_t)), UA_TYPES_UINT64},
  {type_index(typeid(float)),    UA_TYPES_FLOAT },
  {type_index(typeid(double)),   UA_TYPES_DOUBLE}
};

map<const int, const char*> Number_c::format_map {
  {UA_TYPES_INT16, "%i"  },
  {UA_TYPES_INT32, "%i"  },
  {UA_TYPES_INT64, "%li" },
  {UA_TYPES_UINT16, "%u" },
  {UA_TYPES_UINT32, "%u" },
  {UA_TYPES_UINT64, "%lu"},
  {UA_TYPES_DOUBLE, "%.5lf"},
  {UA_TYPES_FLOAT, "%.4f"},
  {NOTUA_TYPES_F100, "%.2f"},
  {NOTUA_TYPES_F10, "%.1f" }
};


/*
  map<int, type_index> Number_c::ua_typeidx_map {
  {UA_TYPES_INT16,  type_index(typeid(int16_t)) },
  {UA_TYPES_INT32,  type_index(typeid(int32_t)) },
  {UA_TYPES_INT64,  type_index(typeid(int64_t)) },
  {UA_TYPES_UINT16, type_index(typeid(uint16_t))},
  {UA_TYPES_UINT32, type_index(typeid(uint32_t))},
  {UA_TYPES_UINT64, type_index(typeid(uint64_t))},
  {UA_TYPES_FLOAT,  type_index(typeid(float))   },
  {UA_TYPES_DOUBLE, type_index(typeid(double))  },
  {NOTUA_TYPES_F100, type_index(typeid(float))  },
  {NOTUA_TYPES_F10, type_index(typeid(float))   }
  };
*/
// ===============================================================

Number_c::Number_c(int _sz_byte, int _uatype)
{
  if (!set_type(_sz_byte, _uatype))
    LOGA("Number_c: set_type() TYPE: %i not supported, _uatype");
  LOGx("+Number_c: %x new (_sz_byte, _uatype) %u", this, ui64);
}

bool Number_c::set_type(int _sz_byte, int _uatype)
{
  bool rc = false;
  _type_ua = _uatype;
  _type_index = _get_typeidx(); //   ua_typeidx_map[_type_ua];
  if ((_sz_byte != 0) && (_type_index != type_index(typeid(bool))) /* ua_typeidx_map.count(_uatype) */) {
    _type_size_bytes = _sz_byte;
    //  _type_ua = _uatype;
    //  _type_index = _get_typeidx(); //   ua_typeidx_map[_type_ua];
    _type_fmt = format_map[_type_ua];
    _type_is_int = (_type_ua < UA_TYPES_FLOAT);
    value.ui64 = 0;
    rc = true;
  } else {
    _type_size_bytes = 0;
    _type_index = type_index(typeid(bool));
    _type_ua = 0;
    _type_fmt = "Number_c: TYPE not supported";
    _type_is_int = false;
    value.ui64 = 0;
    LOGx("Number_c: _uatype %i not supported", _uatype);
  }
  LOGA("xNumber_c: size %i type %i", _type_size_bytes, _type_ua);
  return rc;
};

Number_c::Number_c(const Number_c &V)
{
  if (!set_type((int)V._type_size_bytes, V._type_ua)) //init(V._type_index, V._type_size_bytes, &V.value.ui64))
    LOGA("Number_c: TYPE& not supported");
  else
    ui64 = V.ui64;
  LOGx("+Number_c: %x new (V) COPY %u", this, ui64);
};

Number_c &Number_c::operator= (const Number_c &V)
{
  if (!set_type((int)V._type_size_bytes, V._type_ua)) //(!init(V._type_index, V._type_size_bytes, &V.value.ui64))
    LOGA("Number_c: = TYPE not supported");
  else
    ui64 = V.ui64;
  LOGx("+Number_c: %x (= V) Number_c %s", this, c_str());
  return *this;
}

/*
  Number_c::Number_c(const Number_c &V)
  {
  if (!init(V._type_index, V._type_size_bytes, &V.value.ui64))
    LOGA("Number_c: TYPE& not supported");
  LOGx("xNumber_c: new COPY %u", ui64);
  };
*/

/*
  Number_c &Number_c::operator= (const Number_c &V)
  {
  if (!init(V._type_index, V._type_size_bytes, &V.value.ui64))
    LOGA("Number_c: = TYPE not supported");
  LOGx("+Number_c: %x (= V) Number_c %s", this, c_str());
  return *this;
  }
*/


bool Number_c::same_type(const type_index &_ti)
{
  return (_type_index == _ti);
}

bool Number_c::init(const type_index &_ti, const size_t &_sz, const void* _psrc)
{
  bool rc = false;
  if (typeidx_ua_map.count(_ti)) {
    _type_size_bytes = _sz;
    _type_index = _ti;
    _type_ua = typeidx_ua_map[_ti];
    _type_fmt = format_map[_type_ua];
    _type_is_int = (_type_ua < UA_TYPES_FLOAT);
    value.ui64 = 0;
    memcpy(_ptr, _psrc, _type_size_bytes);
    rc = true;
  } else {
    _type_size_bytes = 0;
    _type_index = type_index(typeid(bool));
    _type_ua = 0;
    _type_fmt = "Number_c: TYPE not supported";
    _type_is_int = false;
    value.ui64 = 0;
    LOGx("Number_c::init wrong TYPE!");
  }
  return rc;
}

// ======================================================================

/*
  void Number_c::init_var(void* _psrc)
  {
  _value.ui64 = 0;
  memcpy(_ptr, _psrc, _type_size);
  }
*/
/*
  void Number_c::init_var(void* _psrc)
  {

  switch (_type_ua) {
  case UA_TYPES_INT16:
    _value.i16 = *static_cast<int16_t*>(_psrc);
    break;
  case UA_TYPES_INT32:
    _value.i32 = *static_cast<int32_t*>(_psrc);
    break;
  case UA_TYPES_INT64:
    _value.i64 = *static_cast<int64_t*>(_psrc);
    break;
  case UA_TYPES_UINT16:
    _value.ui16 = *static_cast<uint16_t*>(_psrc);
    break;
  case UA_TYPES_UINT32:
    _value.ui32 = *static_cast<uint32_t*>(_psrc);
    break;
  case UA_TYPES_UINT64:
    _value.ui64 = *static_cast<uint64_t*>(_psrc);
    break;
  case UA_TYPES_FLOAT:
    _value.fl = *static_cast<float*>(_psrc);
    break;
  case UA_TYPES_DOUBLE:
    _value.dbl = *static_cast<double*>(_psrc);
    break;
  default:
    LOGE("Type: %i not supported", _type_ua);
  }

  }
*/

/*
  Number_c::Number_c(int16_t x)
  {
  _value.i16 = x;
  _type_index = type_index(typeid(x));
  _type = type_map[type_index(typeid(x))];
  LOGx("Number_c: i16 %i", i16);
  };

  Number_c::Number_c(int32_t x)
  {
  _value.i32 = x;
  _type_index = type_index(typeid(x));
  _type = type_map[type_index(typeid(x))];
  LOGx("Number_c: i32 %i", i32);
  };

  Number_c::Number_c(int64_t x)
  {
  _value.i64 = x;
  _type_index = type_index(typeid(x));
  _type = type_map[type_index(typeid(x))];
  LOGx("Number_c: i64 %i", i64);
  };

  Number_c::Number_c(uint16_t x)
  {
  _value.ui16 = x;
  _type_index = type_index(typeid(x));
  _type = type_map[type_index(typeid(x))];
  LOGx("Number_c: ui16 %u %i", ui16, _type);
  };

  Number_c::Number_c(uint32_t x)
  {
  _value.ui32 = x;
  _type_index = type_index(typeid(x));
  _type = type_map[type_index(typeid(x))];
  LOGx("Number_c: ui32 %u", ui32);
  };

  Number_c::Number_c(uint64_t x)
  {
  _value.ui64 = x;
  init_type(type_index(typeid(x)));
  LOGx("Number_c: ui64 %u", ui64);
  };

  Number_c::Number_c(float x)
  {
  _value.fl = x;
  init_type(type_index(typeid(x)));
  LOGx("Number_c: float %f %s", fl, format_map[_type]);
  };

  Number_c::Number_c(double x)
  {
  _value.dbl = x;
  init_type(type_index(typeid(x)));
  LOGx("Number_c: double %f", dbl);
  };
*/

// eof