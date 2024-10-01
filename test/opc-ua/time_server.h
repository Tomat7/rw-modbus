#pragma once

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>

#include <iostream>

void addVariable(UA_Server* server, void* var);
void setVariable(UA_Server* server, void* var);
void getVariable(UA_Server* server, void* var, bool isDebug = false);

void addCurrentTimeDataSourceVariable(UA_Server* server);
void addCurrentTimeExternalDataSource(UA_Server* server);

struct var_t {
  char* name;
  int type;           // UA_TYPES_INT16, UA_TYPES_UINT16, UA_TYPES_INT32, UA_TYPES_UINT32, 
                      // UA_TYPES_INT64, UA_TYPES_UINT64, UA_TYPES_FLOAT, UA_TYPES_DATETIME
  union value_u {
    uint16_t ui16;
    int32_t i32;
    int64_t dt;
    float fl;
  } value;
};

extern volatile int32_t myInt;
extern volatile float myFl;

