#pragma once
// var_class.cpp ----------------------------------
// Copyright 2025 Tomat7 (star0413@gmail.com)

#include <float.h>
#include <string.h>

//#include <iostream>
#include <mutex>
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

union value_uu {
  int16_t i16;
  int32_t i32;
  int64_t i64;
  uint16_t ui16;
  uint32_t ui32;
  uint64_t ui64 = 0;
  int64_t dt;
  float fl;
  double dbl;
};

using std::string;
using std::mutex;
using std::map;
using std::type_index;

#define LOG_ LOGA

class Value_c
{
public:

  template <typename T> Value_c(value_uu v, T x)
  {
    _value.ui64 = v.ui64;
    _type_index = type_index(typeid(x));
    LOG_("Value_C: value");
  };

  Value_c(const Value_c &V);
  Value_c(int16_t x);
  Value_c(int32_t x);
  Value_c(int64_t x);
  Value_c(uint16_t x);
  Value_c(uint32_t x);
  Value_c(uint64_t x);
  Value_c(float x);
  Value_c(double x);
  Value_c& operator= (const Value_c &V);

  ~Value_c() {};

  char* c_str(char* retch);
  char* c_str();

  template <typename T> operator T() { return static_cast<T>(atof(c_str())); }

  operator char*() { return c_str(); }
  operator string() { return string(c_str()); }

  friend bool operator<(const Value_c &v1, const Value_c &v2) { return v1.ui64 < v2.ui64; }
  friend bool operator>(const Value_c &v1, const Value_c &v2) { return v1.ui64 > v2.ui64; }
  friend bool operator!=(const Value_c &v1, const Value_c &v2) { return v1.ui64 != v2.ui64; }
  friend bool operator==(const Value_c &v1, const Value_c &v2) { return v1.ui64 == v2.ui64; }

  int16_t &i16 = _value.i16;
  int32_t &i32 = _value.i32;
  int64_t &i64 = _value.i64;
  uint16_t &ui16 = _value.ui16;
  uint32_t &ui32 = _value.ui32;
  uint64_t &ui64 = _value.ui64;
  float &fl = _value.fl;
  double &dbl = _value.dbl;

private:
  value_uu _value;
  type_index _type_index = type_index(typeid(uint16_t));
  int _type;
  char _str[20];

  map<type_index, int> type_map {
    {type_index(typeid(int16_t)),  UA_TYPES_INT16 },
    {type_index(typeid(uint16_t)), UA_TYPES_UINT16},
    {type_index(typeid(int32_t)),  UA_TYPES_INT32 },
    {type_index(typeid(uint32_t)), UA_TYPES_UINT32},
    {type_index(typeid(int64_t)),  UA_TYPES_INT64 },
    {type_index(typeid(uint64_t)), UA_TYPES_UINT64},
    {type_index(typeid(float)),    UA_TYPES_FLOAT },
    {type_index(typeid(double)),   UA_TYPES_DOUBLE}
  };

  map<int, const char*> format_map {
    {type_map[type_index(typeid(int16_t))], "%i" },
    {type_map[type_index(typeid(int32_t))], "%i" },
    {type_map[type_index(typeid(int64_t))], "%i" },
    {type_map[type_index(typeid(uint16_t))], "%u"},
    {type_map[type_index(typeid(uint32_t))], "%u"},
    {type_map[type_index(typeid(uint64_t))], "%u"},
    {type_map[type_index(typeid(float))], "%-10.4f"},
    {type_map[type_index(typeid(double))], "%-14.6f"}
  };

};

// ======== Definition of TEMPLATEs =========

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
