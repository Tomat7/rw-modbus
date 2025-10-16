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

// ===============================================================

type_attribute_t* Number_c::_get_type_ptr(const int &gtype)
{
  for (size_t i = 0; i < type_attr_sz; i++)
    if (gtype == type_attr[i].g_type)
      return &type_attr[i];

  LOGA("gtype %i - not found", gtype);
  return nullptr;
}

type_attribute_t* Number_c::_get_type_ptr(const type_index &_ti)
{
  for (size_t i = 0; i < type_attr_sz; i++)
    if (_ti == type_attr[i].type_idx)
      return &type_attr[i];
  LOGA("_ti - not found");
  return nullptr;
}

// ==================================================================

void Number_c::set_status(int stcode, string stname, bool isok)
{
  isgood = isok;
  _status_code = stcode;
  _status_name = stname;
}

void Number_c::set_status(int stcode, const char* stchars, bool isok)
{
  set_status(stcode, string(stchars), isok);
}

bool Number_c::get_status(int &stcode, string &stname)
{
  stcode = _status_code;
  stname = _status_name;
  return isgood;
}

string Number_c::status_name()
{
  return _status_name;
}

int Number_c::status_code()
{
  return _status_code;
}
// eof