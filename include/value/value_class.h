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

/*
  struct badvalue_t {
  int16_t i16 = INT16_MIN; //-32000;
  int32_t i32 = INT32_MIN; //-99999;
  int64_t i64 = INT64_MIN; //-99999999;
  uint16_t ui16 = UINT16_MAX; //65333;
  uint32_t ui32 = UINT32_MAX; // 123456789; //99999;
  uint64_t ui64 = UINT64_MAX; // 1234567890123456789;// 9999999;
  float fl = FLT_MIN; // 123456.00f; //-99.54f;
  double dbl = DBL_MIN; //123456789.00; //-999.8765;
  };
*/
using std::string;
using std::mutex;
using std::map;
using std::type_index;

#define LOG_ LOGA

class Value_c
{
public:
  Value_c(value_uu v) { _value.ui64 = v.ui64; LOG_("Value_C: value");};
  Value_c(int16_t x) { _value.i16 = x; LOG_("Value_C: i16");};
  Value_c(int32_t x) { _value.i32 = x; LOG_("Value_C: i32");};
  Value_c(int64_t x) { _value.i64 = x; LOG_("Value_C: i64");};
  Value_c(uint16_t x) { _value.ui16 = x; LOG_("Value_C: ui16");};
  Value_c(uint32_t x) { _value.ui32 = x; LOG_("Value_C: ui32");};
  Value_c(uint64_t x) { _value.ui64 = x; LOG_("Value_C: ui64");};
  Value_c(float x) { _value.fl = x; LOG_("Value_C: float");};
  Value_c(double x) { _value.dbl = x; LOG_("Value_C: double");};

  Value_c(const Value_c &V) { _value.ui64 = V._value.ui64; LOG_("Value_C: COPY");};
  Value_c &operator= (const Value_c &V)
  {
    _value.ui64 = V._value.ui64;
    LOG_("Value_C: COPY");
    return *this;
  };
  //Value_c(const Value_c ©) : ui64(copy.ui64) {LOGD("Value_C: COPY");};

  ~Value_c() {};

  operator int16_t() { return _value.i16; }
  operator int32_t() { return _value.i32; }
  operator int64_t() { return _value.i64; }
  operator uint16_t() { return _value.ui16; }
  operator uint32_t() { return _value.ui32; }
  operator uint64_t() { return _value.ui64; }
  operator float() { return _value.fl; }
  operator double() { return _value.dbl; }
  /*
    friend bool operator<(const Value_c &v1, const Value_c &v2) { return v1.ui64 < v2.ui64; }
    friend bool operator>(const Value_c &v1, const Value_c &v2) { return v1.ui64 > v2.ui64; }
    friend bool operator!=(const Value_c &v1, const Value_c &v2) { return v1.ui64 != v2.ui64; }
    friend bool operator==(const Value_c &v1, const Value_c &v2) { return v1.ui64 == v2.ui64; }
  */
private:
  value_uu _value;
  /*
    int16_t &i16 = _value.i16;
    int32_t &i32 = _value.i32;
    int64_t &i64 = _value.i64;
    uint16_t &ui16 = _value.ui16;
    uint32_t &ui32 = _value.ui32;
    uint64_t &ui64 = _value.ui64;
    float &fl = _value.fl;
    double &dbl = _value.dbl;

  */  int _type;
//  type_index _type_idx = ;
//  badvalue_t bad_value;
//  map<type_index, int> ua_types;  // UA types coding (as CPP type_index)
};

// ======== Definition of TEMPLATEs =========

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
