#pragma once

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>

#include <string.h>

#include <iostream>
#include <map>
#include <string>
#include <mutex>

void addVariable(UA_Server* server, void* var);
void setVariable(UA_Server* server, void* var);
void getVariable(UA_Server* server, void* var, bool isDebug = false);

void addCurrentTimeDataSourceVariable(UA_Server* server);
void addCurrentTimeExternalDataSource(UA_Server* server);

using namespace std;

struct nodeid_t {
  UA_NodeId var;
  UA_NodeId parent;
  UA_NodeId reference;
};

struct var_t {
  char* name;
  string fullname;
  char* folder;
  string foldername;
  nodeid_t node_id;
  void* ptr_value;    // ptr to "correct" value_u
  int rmode;          // 1 - mean RW
  int type;
  // UA_TYPES_INT16, UA_TYPES_UINT16, UA_TYPES_INT32, UA_TYPES_UINT32,
  // UA_TYPES_INT64, UA_TYPES_UINT64, UA_TYPES_FLOAT, UA_TYPES_DATETIME
  union value_u {
    int16_t i16;
    int32_t i32;
    int64_t i64;
    uint16_t ui16;
    uint32_t ui32;
    uint64_t ui64;
    int64_t dt;
    float fl;
  } value;
};

class OpcServer_c
{
public:
  OpcServer_c(UA_UInt16 _port = 4840);
  ~OpcServer_c();

  void init(UA_UInt16 _port = 0);
  void run();
  void stop();
  void addVar(string s, int16_t i16, int mode, string f = "");
  void addVar(string s, int32_t i32, int mode, string f = "");
  void addVar(string s, int64_t i64, int mode, string f = "");
  void addVar(string s, uint16_t ui16, int mode, string f = "");
  void addVar(string s, uint32_t ui32, int mode, string f = "");
  void addVar(string s, uint64_t ui64, int mode, string f = "");
  void addVar(string s, float fl, int mode, string f = "");

  void setVar(string s, int16_t i16);

  void setVariable(var_t &var);
  void getVariable(var_t &var);


private:
  UA_UInt16 uaPort = 4840;
  UA_Server* uaServer = nullptr;
  mutex* uaRunning_mux = nullptr;
  volatile UA_Boolean uaRunning = true;

  void* getPtrToVariable(var_t &var);
  void addVar_Names(string s, int t, int m, string f);

  UA_NodeId addFolder(char* fname);
  void addVar_NodeId(var_t &v);
  void addVariable(var_t &var);

  map<string, var_t> vars;  // All regs here.
  // nodeid_t uaNodeId;


};


