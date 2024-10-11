#pragma once

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>

#include <string.h>

#include <iostream>
#include <map>
#include <string>
#include <mutex>
#include <typeinfo>
#include <typeindex>

#include "include/logger.h"

using namespace std;

struct nodeid_t {
  UA_NodeId var;
  UA_NodeId parent;
  UA_NodeId reference;
};

struct var_t {
  string raw_name;
  char* ua_name = nullptr;
  string str_name;
  char* name = nullptr;
  string str_path;
  char* path  = nullptr;
  string str_full;
  char* path_name = nullptr;
  nodeid_t node_id;
  void* ptr_value;    // ptr to "correct" value_u
  int rmode;          // 1 - mean RW
  int type_id;
  int type;
  // UA_TYPES_INT16, UA_TYPES_UINT16, UA_TYPES_INT32, UA_TYPES_UINT32,
  // UA_TYPES_INT64, UA_TYPES_UINT64, UA_TYPES_FLOAT, UA_TYPES_DATETIME
  /* union value_u {
    int16_t i16;
    int32_t i32;
    int64_t i64;
    uint16_t ui16;
    uint32_t ui32;
    uint64_t ui64;
    int64_t dt;
    float fl;
    } value; */
};

class OpcServer_c
{
public:
  OpcServer_c(UA_UInt16 _port = 4840);
  ~OpcServer_c();

  void init(UA_UInt16 _port = 0);
  void run();
  void stop();

  template<typename T> int addVar(string s, T Value, int rmode)
  {
    rc = addVar_Names(s, types[type_index(typeid(Value))], rmode);
    if (rc == 0)
      return 0;

    addVar_NodeId(vars[s]);
    vars[s].ptr_value = &Value;
    addVariable(vars[s]);
    return 1;
  }

  template<typename T> void setVar(string s, T Value)
  {
    if (vars.count(s)) {
      vars[s].ptr_value = &Value;
      //setVariable(vars[s]);
      writeVariable(vars[s]);
    } else
      LOGA("Ignore non-existing variable: %s", s.c_str());
  }

  template<typename T> T getVar(string s, T &Value)
  {
    if (vars.count(s)) {
      UA_Variant Vrnt;
      getVariable(vars[s], &Vrnt);
      printf("\n1-%s: %s\n", __func__, vars[s].name);
      if (Vrnt.data != nullptr)
        Value = *(static_cast<T*>(Vrnt.data));
      printf("\n2-%s: %s\n", __func__, vars[s].name);
    } else
      LOGA("Ignore non-existing variable: %s", s.c_str());
    return Value;
  }

private:
  bool isDebug = true;
  UA_UInt16 uaPort = 4840;
  UA_Server* uaServer = nullptr;
  mutex* uaRunning_mux = nullptr;
  volatile UA_Boolean uaRunning = true;
  int rc = 0;

  UA_NodeId getFolder_NodeId(string str_path);
  UA_NodeId addFolders(string full_name, UA_NodeId parentNodeId);
  string getPath_Name(string &n);

  int addVar_Names(string raw_name, int t, int m);
  void addVar_NodeId(var_t &v);

  void addVariable(var_t &var);
  void setVariable(var_t &var);
  void writeVariable(var_t &var);
  void getVariable(var_t &var, UA_Variant* vrnt);


  int countSlash(string Path);
  string strVarDetails(var_t &var);
  string getPathByLevel(string Path, int level);

  map<string, var_t> vars;  // All regs here.
  map<type_index, int> types;

};


