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

class Number_c
{
public:
  numeric_u value;

  //Number_c(int _sz_byte = 2, int _uatype = UA_TYPES_UINT16);
  Number_c(int _uatype = UA_TYPES_UINT16);
  Number_c(const Number_c &V);
  ~Number_c() {LOGD("--Number_c! %x ", this);}

// ======= Constructors Templates =======

  Number_c &operator= (const Number_c &V);
  /*
    template <typename T> Number_c& operator= (T x)
    {
      if (!init(type_index(typeid(x)), sizeof(x), &x))
        LOGA("Number_c: = TYPE not supported");
      LOGx("xNumber_c: = Tx type %i", _type_ua);
      return *this;
    };
  */

  template <typename T> Number_c &operator= (T x)
  {
    if ((_type_size_bytes == 0) || (_type_ua == 0)) {
      if (!init(type_index(typeid(x)), sizeof(x), &x))
        LOGA("Number_c: new= TYPE not supported");
    } else {
      if (!set_value((float128)x))
        LOGA("Number_c: set= TYPE not supported");
    }

    LOGx("+Number_c: %x (= Tx) type %i", this, _type_ua);
    return *this;
  };

  template <typename T> Number_c(T x)
  {
    if (!init(type_index(typeid(x)), sizeof(x), &x))
      LOGA("Number_c: new TYPE not supported");
    LOGx("+Number_c: %x new (Tx) type - %i", this, _type_ua);
  };

  template <typename T> Number_c(numeric_u v, T x)
  {
    if (!init(type_index(typeid(x)), sizeof(x), &x))
      LOGA("Number_c: new VALUE not supported");
    LOGx("+Number_c: %x new (value_u) ", this);
  };

// ======= TYPE() Templates =======

  operator char* () { return c_str(); }
  operator string() { return string(c_str()); }

  template <typename T> operator T()
  {
    if (same_type(type_index(typeid(T))))
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
    if (same_type(type_index(typeid(T))) && _type_is_int)
      return (*static_cast<T*>(_ptr) == x);
    else
      return fabsl(_as_f128() - x) <= DBL_EPSILON;
  }

  template <typename T> bool operator!= (T x)
  {
    if (same_type(type_index(typeid(T))) && _type_is_int)
      return (*static_cast<T*>(_ptr) != x);
    else
      return fabsl(_as_f128() - x) > DBL_EPSILON;
  }

  template <typename T> bool operator< (T x)
  {
    if (same_type(type_index(typeid(T))))
      return (*static_cast<T*>(_ptr) < x);
    else
      return (_as_f128() < x);
  }

  template <typename T> bool operator> (T x)
  {
    if (same_type(type_index(typeid(T))))
      return (*static_cast<T*>(_ptr) > x);
    else
      return (_as_f128() > x);
  }

// =======================================
//  bool set_type(int _sz_byte = 2, int _uatype = UA_TYPES_UINT16);
  bool set_type(int _uatype);
  bool set(int _uatype, void* _ptr, bool _status = true);
  char* c_str(const char* fmt = nullptr);

  bool status = true; // false if something wrong with value/type or other
  bool &ok = status;

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
  size_t _type_size_bytes = 0;
  type_index _type_index = type_index(typeid(bool));
  bool _type_is_int = false;

private:

  const char* _type_fmt = nullptr;
  void* _ptr = &value;
  char _str[STR_SIZE + 1];

  float128 _as_f128();
  bool set_value(float128 f128);
  bool same_type(const type_index &_ti);
  bool init(const type_index &_ti, const size_t &_sz, const void* _psrc);
  char* _c_str(const char* fmt);

  type_index _get_typeidx();
  int _get_typesize();

  static map<type_index, int> map_ti_ua;
  static map<const int, const char*> map_ua_fmt;
//  static map<int, type_index> ua_typeidx_map;

};

// =================================================================

//double _as_chars_to_dbl();
//double _fabs(double &x);
//double _as_dbl();

//  bool operator==(float &x);
//  bool operator==(double &x);
//  bool operator!=(float &x);
//  bool operator!=(double &x);


/*
  template <typename T> Number_c& operator= (T x)
  {
    if ((_type_size != 0) && typeidx_ua_map.count(type_index(typeid(x)))) {
      printf("Number_c: size != 0, TYPE is OK\n");
       this = x;
    }
    else
      LOGA("Number_c: = TYPE not supported");
    LOGx("xNumber_c: = Tx type %i", _type_ua);
    return *this;
  };
*/

/*
  template <typename T> operator T()
  {
    if (_type_index == type_index(typeid(T)))
      //return (*static_cast<T*>((void*)&_value));
      return (*static_cast<T*>(_ptr));
    else
      //return (static_cast<T>(atof(c_str())));
      return (static_cast<T>(_to_dbl()));
  }
*/


/*  {
  {type_index(typeid(int16_t)),  UA_TYPES_INT16 },
  {type_index(typeid(uint16_t)), UA_TYPES_UINT16},
  {type_index(typeid(int32_t)),  UA_TYPES_INT32 },
  {type_index(typeid(uint32_t)), UA_TYPES_UINT32},
  {type_index(typeid(int64_t)),  UA_TYPES_INT64 },
  {type_index(typeid(uint64_t)), UA_TYPES_UINT64},
  {type_index(typeid(float)),    UA_TYPES_FLOAT },
  {type_index(typeid(double)),   UA_TYPES_DOUBLE}
  }; */

/* {
  {type_map[type_index(typeid(int16_t))], "%i" },
  {type_map[type_index(typeid(int32_t))], "%i" },
  {type_map[type_index(typeid(int64_t))], "%li" },
  {type_map[type_index(typeid(uint16_t))], "%u"},
  {type_map[type_index(typeid(uint32_t))], "%u"},
  {type_map[type_index(typeid(uint64_t))], "%lu"},
  {type_map[type_index(typeid(float))], "%f"}, //"%-10.4f"},
  {type_map[type_index(typeid(double))], "%lf"} //"%-14.6f"}
  }; */

/*
  template <typename T> bool operator< (T x)
  {
    if (_type_index == type_index(typeid(T)))
      //return (*static_cast<T*>((void*)&_value) < x);
      return (*static_cast<T*>(_ptr) < x);
    else
      return (static_cast<T>(_to_dbl()) < x);
  }

  template <typename T> bool operator> (T x)
  {
    if (_type_index == type_index(typeid(T)))
      //return (*static_cast<T*>((void*)&_value) > x);
      return (*static_cast<T*>(_ptr) > x);
    else
      return (static_cast<T>(_to_dbl()) > x);
  }

  template <typename T> bool operator== (T x)
  {
    if (_type_index == type_index(typeid(T))) {
      LOGA("CMP==: %s _ %10.7lf _ %10.7lf", c_str(), *static_cast<T*>((void*)&_value), x);
      //return fabs(*static_cast<T*>((void*)&_value) - x) < DBL_EPSILON;
      return fabs(*static_cast<T*>(_ptr) - x) < DBL_EPSILON;
    } else
      return fabs(static_cast<T>(_to_dbl())  - x) < DBL_EPSILON;
  }

  template <typename T> bool operator!= (T x)
  {
    if (_type_index == type_index(typeid(T)))
      return fabs(*static_cast<T*>(_ptr) - x) > DBL_EPSILON;
    else
      return fabs(static_cast<T>(_to_dbl()) - x) > DBL_EPSILON;
  }
*/

// ======== Definition of TEMPLATEs =========

//friend bool operator<(const Number_c &v1, const Number_c &v2) { return v1.ui64 < v2.ui64; }
//friend bool operator>(const Number_c &v1, const Number_c &v2) { return v1.ui64 > v2.ui64; }
//friend bool operator!=(const Number_c &v1, const Number_c &v2) { return v1.ui64 != v2.ui64; }
//friend bool operator==(const Number_c &v1, const Number_c &v2) { return v1.ui64 == v2.ui64; }

/*
  Number_c(int16_t x);
  Number_c(int32_t x);
  Number_c(int64_t x);
  Number_c(uint16_t x);
  Number_c(uint32_t x);
  Number_c(uint64_t x);
  Number_c(float x);
  Number_c(double x);
*/


/*
  template <typename T>
  operator T()
  {
    //T x;
    //    if (type_map.count(type_index(typeid(T))))
    //x = static_cast<T>(atof(c_str()));
    return static_cast<T>(atof(c_str())); //x;
  }; */
/*
    operator int16_t() { return _value.i16; }
    operator int32_t() { return _value.i32; }
    operator int64_t() { return _value.i64; }
    operator uint16_t(); // { return _value.ui16; }
    operator uint32_t() { return _value.ui32; }
    operator uint64_t() { return _value.ui64; }
    operator float(); // { return _value.fl; }
    operator double() { return _value.dbl; }
*/
/*
  struct ReadValue {
    string _s;  // Full path to variable
    ReadValue(string svar)
    {
      //_s = OPCs.GetVarFullName(svar);  // Try to find fullpath-name
    }
    template <typename T>
    operator T()
    {
      T x;
      // OPCs.ReadNumber(_s, x);
      return x;
    }
  };
*/


//#include "opcs_templates.h"

/*
  int getType(string s);
  int getStatus(string s);    // 0 - is OK, any other (1 or -1) is BAD
*/
/*
  // ask OPC server for current value (refresh)
  template <typename T>
  T GetNumber(string s);
*/
/*
  // read value saved on previous/last correct getValue
  template <typename T>
  T ReadValue(string s);
*/
/*
  template <typename T>
  bool ReadValue(string s, T &x);
  //  Definition at the bottom of THIS file
*/

// eof
