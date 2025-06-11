#pragma once
// plc_datatype.h ----------------------------------
// Copyright 2025 Tomat7 (star0413@gmail.com)

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>

//#include "include/open62541/open62541.h"

#include <string>
#include <variant>
#include <vector>

using std::string;
using std::mutex;

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
  uint16_t mb32u[2];
  uint16_t mb64u[4];
  /*   uint16_t fl2u[2];
    uint16_t dbl2u[4];
    uint8_t byte2u[8]; */
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
  char* ua_keyname = nullptr; // key_name.c_str()
  string var_name;            // Kub.Temp1
  char* ua_varname = nullptr; // var_name.c_str()
  string var_path;            // /PLC/Kub/
  char* ua_varpath = nullptr; // var_path.c_str()
  nodeid_t node_id;           // UA_NodeId (var, parent, reference)
  value_u raw_value;          // value_u
  void* ptr_value;            // ptr to "correct" value_u
  int rmode;                  // 1 - mean RW
  int ua_type;                // UA_DataTypes
  bool is_var = false;        // for Variable = true, folder = false
  UA_StatusCode ua_status;    // set by Scada or form Modbus status
  UA_DateTime ua_timestamp;   // save timestamp of last write/update
};

//  string str_pathname;        // str_path + str_name = /PLC/Kub/Kub.Temp1
//  char* path_name = nullptr;  // str_full.c_str()
//  variant_t value;
/*   var_union value_wrong;
  var_union value_min;
  var_union value_max; */

// eof
