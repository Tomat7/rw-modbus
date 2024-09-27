#pragma once

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>

#include <string.h>

#include <iostream>
#include <map>
#include <string>

void addVariable(UA_Server* server, void* var);
void setVariable(UA_Server* server, void* var);
void getVariable(UA_Server* server, void* var, bool isDebug = false);

void addCurrentTimeDataSourceVariable(UA_Server* server);
void addCurrentTimeExternalDataSource(UA_Server* server);

using namespace std;

struct var_t {
  char* name;
  string str_name;
  int rmode;          // 1 - mean RW
  int type;           // UA_TYPES_INT16, UA_TYPES_UINT16, UA_TYPES_INT32, UA_TYPES_UINT32,
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
  OpcServer_c();
  ~OpcServer_c();

  void init(UA_UInt16 portNumber = 4840);
  void run();
  void addVar(string s, int16_t i16);
  void addVar(string s, int32_t i32);
  void addVar(string s, int64_t i64);
  void addVar(string s, uint16_t ui16);
  void addVar(string s, uint32_t ui32);
  void addVar(string s, uint64_t ui64);
  void addVar(string s, float fl);

  void stop();
  void addVariable(void* var);
  void setVariable(void* var);
  void getVariable(void* var, bool isDebug = false);
  map<string, var_t> vars;  // All regs here.

private:
  UA_Server* uaServer = nullptr;
  volatile UA_Boolean uaRunning = true;
  void* getPtrToVariable(void* var, char* &vName, int &vType, bool isDebug = false);
  void addVar_name(string s, var_t &v);

};
