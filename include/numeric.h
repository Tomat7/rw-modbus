#pragma once
// numeric.h ---------------------------------
// Copyright 2025 Tomat7 (star0413@gmail.com)

#include <float.h>

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
#define NOTUA_TYPES_F10 1010   // floatVar = ModbusVar_int16 / 10.0;
#define NOTUA_TYPES_F100 1100  // floatVar = ModbusVar_int16 / 100.0;

union numeric_u {
  int16_t i16;
  int32_t i32;
  int64_t i64;
  uint16_t ui16;
  uint32_t ui32;
  uint64_t ui64 = 0;
  float fl;
  double dbl;
  uint16_t mb32u[2];
  uint16_t mb64u[4];
};

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

struct typeprop_t {
  int rtype;
  int rsize;
};


// eof
