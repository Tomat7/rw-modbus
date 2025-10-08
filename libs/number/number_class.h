#pragma once
// Number_class.h ----------------------------
// Copyright 2025 Tomat7 (star0413@gmail.com)

#include <float.h>
#include <string.h>

//#include <mutex>
#include <map>
#include <string>
#include <typeindex>
#include <typeinfo>

#include "include/numeric.h"
#include "include/logger.h"

#define STR_SIZE 50

using std::string;
using std::map;
using std::type_index;

using float32=float;
using float64=double;
using float128=long double;
//using float128=__float128;

//#define DEBUG_NUMBER
#undef LOGb
#undef LOGr
#undef LOGg
#undef LOGx

#ifdef DEBUG_NUMBER
#define LOGr LOG_RED
#define LOGb LOG_BLU
#define LOGg LOG_GRN
#define LOGx LOG_CYN
#else
#define LOGr(...)
#define LOGb(...)
#define LOGg(...)
#define LOGx(...)
#endif // DEBUG_NUMBER
/*
  struct type_properties_t {
  int ua_type = 0; // UA_type
  int size_bytes = 0; // Size in BYTES!
  type_index type_idx = type_index(typeid(bool));  // for lookup
  const char* c_format = nullptr; // for printf(...)
  };
*/
struct type_attribute_t {
  string name_type = "";
  int g_type = 0; // GLOBAL type not always == UA_type (ex. f100)
  int ua_type = 0; // UA_type
  int size_bytes = 0; // Size in BYTES!
  type_index type_idx = type_index(typeid(bool));  // for lookup
  const char* c_format = nullptr; // for printf(...)
};

using type_a = type_attribute_t;

class Number_c
{
public:
  numeric_u value;

  Number_c();
  Number_c(const Number_c &V);
  ~Number_c() {LOGD("--Number_c! %x ", this);}

// ======= Constructors Templates =======

  Number_c &operator= (const Number_c &V);

  template <typename T> Number_c &operator= (T x)
  {
    if ((_type_bytes == 0) || (_type_ua == 0)) {
      if (!set(type_index(typeid(x)), &x))
        LOGA("Number_c: new=Tx not supported");
    } else {
      if (!_set_value((float128)x))
        LOGA("Number_c: existing=Tx not supported");
    }

    LOGx("+Number_c: %x (= Tx) type %i", this, _type_ua);
    return *this;
  };

  template <typename T> Number_c(T x)
  {
    if (!set(type_index(typeid(x)), &x))
      LOGA("Number_c: new (Tx) not supported");
    LOGx("+Number_c: %x new (Tx) type - %i", this, _type_ua);
  };

  template <typename T> Number_c(numeric_u v, T x)
  {
    if (!set(type_index(typeid(x)), &x))
      LOGA("Number_c: new (value_u, Tx) not supported");
    LOGx("+Number_c: %x new (value_u) ", this);
  };

// ======= TYPE() Templates =======

  operator char* () { return c_str(); }
  operator string() { return string(c_str()); }

  template <typename T> operator T()
  {
    if (_same_type(type_index(typeid(T))))
      return (*static_cast<T*>(_ptr));
    else
      return (static_cast<T>(_as_f128()));
  }

// ======= COMPARE Operators =======

  bool operator<(Number_c &v2);
  bool operator>(Number_c &v2);
  bool operator==(Number_c &v2);
  bool operator!=(Number_c &v2);

// ======= COMPARE Operators Templates =======

  template <typename T> bool operator== (T x)
  {
    if (_same_type(type_index(typeid(T))) && _type_is_int)
      return (*static_cast<T*>(_ptr) == x);
    else
      return fabsl(_as_f128() - x) <= DBL_EPSILON;
  }

  template <typename T> bool operator!= (T x)
  {
    if (_same_type(type_index(typeid(T))) && _type_is_int)
      return (*static_cast<T*>(_ptr) != x);
    else
      return fabsl(_as_f128() - x) > DBL_EPSILON;
  }

  template <typename T> bool operator< (T x)
  {
    if (_same_type(type_index(typeid(T))))
      return (*static_cast<T*>(_ptr) < x);
    else
      return (_as_f128() < x);
  }

  template <typename T> bool operator> (T x)
  {
    if (_same_type(type_index(typeid(T))))
      return (*static_cast<T*>(_ptr) > x);
    else
      return (_as_f128() > x);
  }

// =======================================
//  bool set_type(int _sz_byte = 2, int _uatype = UA_TYPES_UINT16);
  bool set(const Number_c &V);
  bool set(int gtype, const void* ptr = nullptr, bool isok = true);
  bool set(type_index ti, const void* ptr = nullptr, bool isok = true);

  void set_status(int stcode, const char* stchars, bool isok = true);
  void set_status(int stcode, string stname, bool isok = true);
  bool get_status(int &stcode, string& stname);
  string status_name();  // Get StatusCode Name (string)
  int status_code();  // Get StatusCode (int)

  char* c_str(const char* fmt = nullptr);

  bool isgood = true; // false if something wrong with value/type or other
//  bool &isgood = status;

  int16_t &i16 = value.i16;
  int32_t &i32 = value.i32;
  int64_t &i64 = value.i64;
  uint16_t &ui16 = value.ui16;
  uint32_t &ui32 = value.ui32;
  uint64_t &ui64 = value.ui64;
  float &fl = value.fl;
  double &dbl = value.dbl;

// =======================================
protected:
  int _type_ua = 0;
  size_t _type_bytes = 0;
  type_index _type_index = type_index(typeid(bool));
  bool _type_is_int = false;

private:

  int _status_code = 0;
  string _status_name = "";
  const char* _type_fmt = nullptr;
  void* _ptr = &value;
  char _str[STR_SIZE + 1];

  float128 _as_f128();
  bool _set(type_attribute_t* ta, const void* psrc, bool status);
  bool _set_attributes(type_attribute_t* ta);
  void _init(const void* _psrc = nullptr, bool _status = true);

  char* _c_str(const char* fmt);
  bool _set_value(float128 f128);
  bool _same_type(const type_index &_ti);

//  bool _init__(const type_index &_ti, const void* _psrc);


  type_attribute_t* _get_type_ptr(const int &_global_t);
  type_attribute_t* _get_type_ptr(const type_index& _ti);

  static type_attribute_t type_attr[];
  static size_t type_attr_sz;

};


// eof
