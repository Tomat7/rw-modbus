#pragma once
// plc_datatype.h ----------------------------------
// Copyright 2025 Tomat7 (star0413@gmail.com)

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>

//#include "include/open62541/open62541.h"

#include <string>
#include <variant>
#include <vector>

using namespace std;

/*
  using fl2 = uint16_t[2];
  using dbl4 = uint16_t[4];
  using byte8 = uint8_t[8];

  using value_u = variant<int16_t, int32_t, int64_t, uint16_t, uint32_t,
      uint64_t, int64_t, float, double,
      fl2, dbl4, byte8>;
*/

union value_u {
  int16_t i16;
  int32_t i32;
  int64_t i64;
  uint16_t ui16;
  uint32_t ui32;
  uint64_t ui64 = 0;
  int64_t dt;
  float fl;
  double dbl;
  uint16_t fl2u[2];
  uint16_t dbl2u[4];
  uint8_t byte2u[8];
};

struct badvalue_t {
  int16_t i16 = -9999;
  int32_t i32 = -99999;
  int64_t i64 = -99999999;
  uint16_t ui16 = 65333;
  uint32_t ui32 = 99999;
  uint64_t ui64 = 9999999;
  float fl = -99.54f;
  double dbl = -999.8765;
};

struct nodeid_t {
  UA_NodeId var;
  UA_NodeId parent;
  UA_NodeId reference;
};

struct var_t {
  string key_name;            // /PLC/Kub/Kub.Temp1
  char* ua_name = nullptr;    // key_name.c_str()
  string str_name;            // Kub.Temp1
  char* name = nullptr;       // str_name.c_str()
  string str_path;            // /PLC/Kub/
  char* path = nullptr;       // str_path.c_str()
  string str_pathname;        // str_path + str_name = /PLC/Kub/Kub.Temp1
  char* path_name = nullptr;  // str_full.c_str()
  nodeid_t node_id;           // UA_NodeId (var, parent, reference)
  void* ptr_value;            // ptr to "correct" value_u
  int rmode;                  // 1 - mean RW
  int type;                   // UA_DataTypes
  UA_StatusCode ua_status;
  UA_DateTime ua_timestamp;
  value_u value;
  //  variant_t value;
  /*   var_union value_wrong;
    var_union value_min;
    var_union value_max; */
};

// eof
