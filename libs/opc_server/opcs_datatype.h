#pragma once
// plc_datatype.h ----------------------------------
// Copyright 2025 Tomat7 (star0413@gmail.com)

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
//#include "include/open62541/open62541.h"

#include <string>
#include <vector>
//#include <variant>

#include "include/numeric.h"

using std::string;
//using std::mutex;

struct nodeid_t {
  UA_NodeId var;
  UA_NodeId parent;
  UA_NodeId reference;
};

struct ua_var_t {
  string key_name;            // /PLC/Kub/Kub.Temp1
  char* ua_keyname = nullptr; // key_name.c_str()
  string var_name;            // Kub.Temp1
  char* ua_varname = nullptr; // var_name.c_str()
  string var_path;            // /PLC/Kub/
  char* ua_varpath = nullptr; // var_path.c_str()
  nodeid_t node_id;           // UA_NodeId (var, parent, reference)
  numeric_u raw_value;          // value_u
  void* ptr_value;            // ptr to "correct" value_u
  int acl_mode;               // 1 - mean RW
  int ua_type;                // UA_DataTypes
  bool is_var = false;        // for Variable = true, folder = false
  UA_StatusCode ua_status;    // set by Scada or form Modbus status
  UA_DateTime ua_timestamp;   // save timestamp of last write/update
};

// eof
