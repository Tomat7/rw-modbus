#pragma once

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <string.h>

#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <typeindex>
#include <typeinfo>

#include "include/logger.h"

using namespace std;

union var_union {
  int16_t i16;
  int32_t i32;
  int64_t i64;
  uint16_t ui16;
  uint32_t ui32;
  uint64_t ui64;
  int64_t dt;
  float fl;
  double dbl;
};

struct badvalue_t {
  int16_t i16 = -32000;
  int32_t i32 = -65000;
  int64_t i64 = -128000;
  uint16_t ui16 = 65535;
  uint32_t ui32 = 128765;
  uint64_t ui64 = 2565432;
  float fl = -99.54f;
  double dbl = -999.987;
};

struct nodeid_t {
  UA_NodeId var;
  UA_NodeId parent;
  UA_NodeId reference;
};

struct var_t {
  string key_name;
  char* ua_name = nullptr;
  string str_name;
  char* name = nullptr;
  string str_path;
  char* path = nullptr;
  string str_full;
  char* path_name = nullptr;
  nodeid_t node_id;
  void* ptr_value; // ptr to "correct" value_u
  int rmode;       // 1 - mean RW
  int type;
  UA_StatusCode ua_status;
  UA_DateTime ua_timestamp;
  var_union value;
  /*   var_union value_wrong;
    var_union value_min;
    var_union value_max; */
};

class OpcServer_c
{
public:
  OpcServer_c(UA_UInt16 _port = 4840);
  ~OpcServer_c();

  void init(UA_UInt16 _port = 0); // Necessary init() before run()
  void run();
  void stop();

  bool isVariable(string s);
  bool isGood(string s);
  string lookupVar(string s);
  void delVar(string s);
  int getType(string s);
  int getStatus(string s); // 0 - is OK, any other (1 or -1) is BAD
  // var_union getValue(string s); // returns union
  var_union readRawValue(string s); // returns value_union
  int refreshValues();                // getVar for ALL variables, returns - qty of vars

  template <typename T> int addVar(string s, T Value, int rmode); // for init
  template <typename T> T updateVar(string s, T Value_set, bool isOK);
  template <typename T> T getValue(string s); // ask OPC server for current value
  template <typename T> T readValue(string s); // read value saved on previous getValue
  // template <typename T> void setDefaultValue(string s, T Value_set);
  //  Definition at the bottom of THIS file

private:
  bool isDebug = true;
  UA_UInt16 uaPort = 4840;
  UA_Server* uaServer = nullptr;
  UA_Variant* uaVariant = nullptr;
  mutex* uaSrvMux = nullptr;
  mutex* uaGetMux = nullptr;
  mutex* uaDataMux = nullptr;
  volatile UA_Boolean uaRunning = true;
  int rc = 0;

  UA_NodeId getFolder_NodeId(string str_path);
  UA_NodeId addFolders(string full_name, UA_NodeId parentNodeId);
  string getPath_Name(string &n);

  int addVar_Names(string raw_name, int t, int m);
  void addVar_NodeId(var_t &v);

  template <typename T>
  bool getVariableValue(string s, T &Value);
  template <typename T>
  void setVariableValue(string s, T Value_set, bool isOK = true);

  void addVariable(var_t &var);
  void writeVariable(var_t &var);
  void* getVariantData(string s); // get pointer to UA_Variant.Data
  //  void readVariable(var_t &var);

  int countSlash(string Path);
  string strVarDetails(var_t &var);
  string getPathByLevel(string Path, int level);

  badvalue_t bad_value;
  map<string, var_t> vars;    // All regs here.
  map<type_index, int> types; // Types coding is in constructor
};


// ======== Definition of TEMPLATEs =========

template <typename T>
int OpcServer_c::addVar(std::string s, T Value, int rmode)
{
  rc = addVar_Names(s, types[type_index(typeid(Value))], rmode);
  if (rc == 0)
    return 0;

  addVar_NodeId(vars[s]);
  vars[s].ptr_value = &Value;
  addVariable(vars[s]);
  return 1;
}

template <typename T>
void OpcServer_c::setVariableValue(std::string s, T Value_set, bool isOK)
{
  if (vars.count(s)) {
    vars[s].ptr_value = static_cast<T*>(&Value_set);
    vars[s].value = *static_cast<var_union*>(vars[s].ptr_value);

    if (isOK) {
      vars[s].ua_status = UA_STATUSCODE_GOOD;
      vars[s].ua_timestamp = UA_DateTime_now();
    } else
      vars[s].ua_status = UA_STATUSCODE_BADNOTCONNECTED;

    writeVariable(vars[s]);
  } else
    LOGA("Set: Ignore non-existing variable: %s", s.c_str());
}

template <typename T>
bool OpcServer_c::getVariableValue(std::string s, T &Value_get)
{
  uaGetMux->lock();
  bool ret = false;
  void* VarData = getVariantData(s);
  if (VarData != nullptr) {
    Value_get = *(static_cast<T*>(VarData));
    vars[s].ptr_value = static_cast<T*>(&Value_get);
    vars[s].ptr_value = static_cast<T*>(VarData);
    //    vars[s].value = *static_cast<var_union*>(vars[s].ptr_value);
    ret = true;
  }
  uaGetMux->unlock();
  return ret;
}

template <typename T>
T OpcServer_c::updateVar(std::string s, T Value_set, bool isOK)
{
  T Value_get = Value_set;
  getVariableValue(s, Value_get);
  setVariableValue(s, Value_set, isOK);
  return Value_get;
}

/* template <typename T>
  void OpcServer_c::setDefaultValue(string s, T Value_set)
  {

  } */
// eof
