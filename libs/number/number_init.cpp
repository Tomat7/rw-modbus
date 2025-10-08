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


type_attribute_t Number_c::type_attr[] = {
  {"i16", UA_TYPES_INT16, UA_TYPES_INT16,  2, type_index(typeid(int16_t)), "%i" },
  {"i32", UA_TYPES_INT32, UA_TYPES_INT32,  4, type_index(typeid(int32_t)), "%i"},
  {"i64", UA_TYPES_INT64, UA_TYPES_INT64,  8, type_index(typeid(int64_t)), "%li"},
  {"u16", UA_TYPES_UINT16, UA_TYPES_UINT16, 2, type_index(typeid(uint16_t)), "%u"},
  {"u32", UA_TYPES_UINT32, UA_TYPES_UINT32, 4, type_index(typeid(uint32_t)), "%u"},
  {"u64", UA_TYPES_UINT16, UA_TYPES_UINT16, 8, type_index(typeid(uint64_t)), "%lu"},
  {"dbl", UA_TYPES_DOUBLE,   UA_TYPES_DOUBLE, 8, type_index(typeid(double)), "%.5lf"},
  {"fl",  UA_TYPES_FLOAT,    UA_TYPES_FLOAT,  4, type_index(typeid(float)), "%.3f"},
  {"f100", NOTUA_TYPES_F100, UA_TYPES_FLOAT,  4, type_index(typeid(float)), "%.2f"},
  {"f10",  NOTUA_TYPES_F10,  UA_TYPES_FLOAT,  4, type_index(typeid(float)), "%.1f"}
};

size_t Number_c::type_attr_sz = sizeof(type_attr) / sizeof(type_attribute_t);

// ==================================================================

Number_c::Number_c()
{
  if (!set(UA_TYPES_UINT16))
    LOGA("Number_c:: new (UINT16) type %i not supported");
  LOGx("+Number_c:: %x new (UINT16) %u", this, ui64);
}


Number_c::Number_c(const Number_c &V)
{
  if (!set(V))
    LOGA("Number_c:: new (&V) not supported");
  LOGx("+Number_c:: %x new (&V) COPY %u", this, ui64);
};

Number_c &Number_c::operator= (const Number_c &V)
{
  if (!set(V))
    LOGA("Number_c: (=&V) not supported");
  LOGx("+Number_c: %x (=&V) Number_c %s", this, c_str());
  return *this;
}

// ==================================================================


bool Number_c::set(const Number_c &V)
{
  if (set(V._type_ua, (const void*)&V.value, V.isgood)) {
    _status_code = V._status_code;
    _status_name = V._status_name;
    return true;
  }
  return false;
}

bool Number_c::set(int gtype, const void* psrc, bool isok)
{
  return _set(_get_type_ptr(gtype), psrc, isok);
}

bool Number_c::set(type_index ti, const void* psrc, bool isok)
{
  return _set(_get_type_ptr(ti), psrc, isok);
}

bool Number_c::_set(type_attribute_t* ta, const void* psrc, bool isok)
{
  bool rc = _set_attributes(ta);

  if (rc)
    _init(psrc, isok);
  else
    LOGA("Number_c::_set() TYPE: not supported");

  return rc;
}

void Number_c::_init(const void* psrc, bool isok)
{
  isgood = isok;
  value.ui64 = 0;
  if (psrc != nullptr)
    memcpy(_ptr, psrc, _type_bytes);
}

bool Number_c::_set_attributes(type_attribute_t* ta)
{
  bool rc = false;

  if (ta != nullptr) {
    _type_ua = ta->ua_type;
    _type_index = ta->type_idx;
    _type_bytes = ta->size_bytes;
    _type_fmt = ta->c_format;
    _type_is_int = (_type_ua < UA_TYPES_FLOAT);
    isgood = true;
    value.ui64 = 0;
    rc = true;
  } else {
    _type_ua = 0;
    _type_index = type_index(typeid(bool));
    _type_fmt = "Number_c: TYPE not supported";
    _type_is_int = false;
    isgood = false;
    value.ui64 = 0;
    LOGx("Number_c::set_attributes: TYPE not supported");
  }
  LOGb("xNumber_c::set_attributes: size %i type %i", _type_bytes, _type_ua);
  return rc;
};


bool Number_c::_same_type(const type_index &_ti)
{
  return (_type_index == _ti);
}

// ======================================================================

/*
  bool Number_c::_init__(const type_index &_ti, const void* _psrc)
  {
  bool rc = false;
  type_a *ta = _get_type_ptr(_ti);

  if (ta != nullptr) {
    _type_ua = ta->ua_type;
    _type_index = _ti;
    _type_bytes = ta->size_bytes;
    _type_fmt = ta->c_format;
    _type_is_int = (_type_ua < UA_TYPES_FLOAT);
    status = true;
    value.ui64 = 0;
    memcpy(_ptr, _psrc, _type_bytes);
    rc = true;
  } else {
    _type_bytes = 0;
    _type_index = type_index(typeid(bool));
    _type_ua = 0;
    _type_fmt = "Number_c: TYPE not supported";
    _type_is_int = false;
    status = false;
    value.ui64 = 0;
    LOGx("Number_c::init wrong TYPE!");
  }
  return rc;
  }
*/


// eof