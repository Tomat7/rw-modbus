#pragma once
// value_class.h ----------------------------
// Copyright 2025 Tomat7 (star0413@gmail.com)

#include <float.h>
#include <string.h>

//#include <mutex>
#include <map>
#include <string>
#include <typeindex>
#include <typeinfo>

#define UA_TYPES_BOOLEAN 0
#define UA_TYPES_SBYTE 1
#define UA_TYPES_BYTE 2
#define UA_TYPES_INT16 3
#define UA_TYPES_UINT16 4
#define UA_TYPES_INT32 5
#define UA_TYPES_UINT32 6
#define UA_TYPES_INT64 7
#define UA_TYPES_UINT64 8
#define UA_TYPES_FLOAT 9
#define UA_TYPES_DOUBLE 10
#define UA_TYPES_STRING 11

#define STR_SIZE 50

union value_uu {
  int16_t i16;
  int32_t i32;
  int64_t i64;
  uint16_t ui16;
  uint32_t ui32;
  uint64_t ui64 = 0;
  float fl;
  double dbl;
};

using std::string;
using std::mutex;
using std::map;
using std::type_index;

#define LOGx LOGA

class Value_c
{
public:
  Value_c(Value_c &V);
  ~Value_c() {};

// ======= Constructors Templates =======
  Value_c& operator= (Value_c &V);

  template <typename T> Value_c& operator= (T x)
  {
    if (!init(type_index(typeid(x)), sizeof(x), &x))
      LOGA("Value_c: = TYPE not supported");
    LOGx("xValue_C: = Tx type %i", _type_ua);
    return *this;
  };

  template <typename T> Value_c(T x)
  {
    if (!init(type_index(typeid(x)), sizeof(x), &x))
      LOGA("Value_c: new TYPE not supported");
    LOGx("xValue_C: new Tx type - %i", _type_ua);
  };

  template <typename T> Value_c(value_uu v, T x)
  {
    if (!init(type_index(typeid(x)), sizeof(x), &x));
    else
      LOGA("Value_c: new VALUE not supported");
    LOGx("xValue_C: new value_u ");
  };

// ======= TYPE() Templates =======

  operator char*() { return c_str(); }
  operator string() { return string(c_str()); }
  template <typename T> operator T()
  {
    if (same_type(type_index(typeid(T))))
      return (*static_cast<T*>(_ptr));
    else {
      // static_assert(sizeof(T) < 8);
      return (static_cast<T>(_to_dbl(sizeof(T))));
    }
  }

// ======= COMPARE Operators =================

  bool operator<(Value_c &v2);
  bool operator>(Value_c &v2);
  bool operator==(Value_c &v2);
  bool operator!=(Value_c &v2);

  bool operator==(float &x);
  bool operator==(double &x);
  bool operator!=(float &x);
  bool operator!=(double &x);

// ======= COMPARE Operators Templates =======

  template <typename T> bool operator== (T x)
  {
    if (same_type(type_index(typeid(T))) && _type_is_int)
      return (*static_cast<T*>(_ptr) == x);
    else
      return fabs(*(double*)(_ptr) - x) <= DBL_EPSILON;
  }

  template <typename T> bool operator!= (T x)
  {
    if (same_type(type_index(typeid(T))) && _type_is_int)
      return (*static_cast<T*>(_ptr) != x);
    else
      return fabs(*(double*)(_ptr) - x) > DBL_EPSILON;
  }

  template <typename T> bool operator< (T x)
  {
    if (same_type(type_index(typeid(T))))
      return (*static_cast<T*>(_ptr) < x);
    else
      return (*(double*)(_ptr) < x);
  }

  template <typename T> bool operator> (T x)
  {
    if (same_type(type_index(typeid(T))))
      return (*static_cast<T*>(_ptr) > x);
    else
      return (*(double*)(_ptr) > x);
  }

// =======================================

  char* c_str(const char* fmt = nullptr);

  int16_t &i16 = _value.i16;
  int32_t &i32 = _value.i32;
  int64_t &i64 = _value.i64;
  uint16_t &ui16 = _value.ui16;
  uint32_t &ui32 = _value.ui32;
  uint64_t &ui64 = _value.ui64;
  float &fl = _value.fl;
  double &dbl = _value.dbl;

// =======================================
// =======================================

private:
  value_uu _value;
  void* _ptr = &_value;

  type_index _type_index = type_index(typeid(bool));
  int _type_ua = 0;
  size_t _type_size = 0;
  bool _type_is_int = false;
  const char* _type_fmt = nullptr;
  char _str[STR_SIZE + 1];

  bool same_type(const type_index &_ti);
  bool init(const type_index &_ti, const size_t &_sz, void* _psrc);
  // void init_var(void* _psrc);

  char* _c_str(const char* fmt);
  double _dbl();
  double _to_dbl();
  double _to_dbl(size_t _sz);
  double _fabs(double &x);

  static map<type_index, int> type_map;
  static map<const int, const char*> format_map;

};


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

//friend bool operator<(const Value_c &v1, const Value_c &v2) { return v1.ui64 < v2.ui64; }
//friend bool operator>(const Value_c &v1, const Value_c &v2) { return v1.ui64 > v2.ui64; }
//friend bool operator!=(const Value_c &v1, const Value_c &v2) { return v1.ui64 != v2.ui64; }
//friend bool operator==(const Value_c &v1, const Value_c &v2) { return v1.ui64 == v2.ui64; }

/*
  Value_c(int16_t x);
  Value_c(int32_t x);
  Value_c(int64_t x);
  Value_c(uint16_t x);
  Value_c(uint32_t x);
  Value_c(uint64_t x);
  Value_c(float x);
  Value_c(double x);
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
