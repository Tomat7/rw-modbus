
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>

#include "time_server.h"

#define DEBUG(a) if (isDebug) {a}

void* getPtrToVariable(void* var, char* &vName, int& vType, bool isDebug = false);

volatile int32_t myInt = 0;
volatile float myFl = 0;


void addVariable(UA_Server* server, void* var)
{
  char* vName = nullptr;
  int vType = 0;

  void* val = getPtrToVariable(var, vName, vType, true);

  UA_VariableAttributes attr = UA_VariableAttributes_default;

  UA_Variant_setScalar(&attr.value, val, &UA_TYPES[vType]);

  attr.description = UA_LOCALIZEDTEXT("en-US", vName);
  attr.displayName = UA_LOCALIZEDTEXT("en-US", vName);
  attr.dataType = UA_TYPES[vType].typeId;
  attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

  // Add the variable node to the information model
  UA_NodeId varNodeId = UA_NODEID_STRING(1, vName);
  UA_QualifiedName varName = UA_QUALIFIEDNAME(1, vName);

  UA_NodeId parentNodeId = UA_NS0ID(OBJECTSFOLDER);
  UA_NodeId parentReferenceNodeId = UA_NS0ID(ORGANIZES);
  UA_Server_addVariableNode(server, varNodeId, parentNodeId,
                            parentReferenceNodeId, varName,
                            UA_NS0ID(BASEDATAVARIABLETYPE), attr, NULL, NULL);
}

void setVariable(UA_Server* server, void* var)
{
  char* vName = nullptr;
  int vType = 0;

  void* val = getPtrToVariable(var, vName, vType);

  UA_NodeId varNodeId = UA_NODEID_STRING(1, vName);

  UA_Variant myVariant;
  UA_Variant_init(&myVariant);

  UA_Variant_setScalar(&myVariant, val, &UA_TYPES[vType]);
  UA_Server_writeValue(server, varNodeId, myVariant);
  
  /* Set the status code of the value to an error code. The function
     UA_Server_write provides access to the raw service. The above
     UA_Server_writeValue is syntactic sugar for writing a specific node
     attribute with the write service. */
  UA_WriteValue wv;
  UA_WriteValue_init(&wv);
  wv.nodeId = varNodeId;
  wv.attributeId = UA_ATTRIBUTEID_VALUE;
  wv.value.status = UA_STATUSCODE_BADNOTCONNECTED;
  wv.value.hasStatus = true;
  UA_Server_write(server, &wv);

  /* Reset the variable to a good statuscode with a value */
  wv.value.hasStatus = false;
  wv.value.value = myVariant;
  wv.value.hasValue = true;
  UA_Server_write(server, &wv);

}


void getVariable(UA_Server* server, void* var, bool isDebug)
{
  var_t* v = (var_t*)var;
  char* vName = nullptr;
  int vType = 0;
  
  void* val = getPtrToVariable(var, vName, vType);

  UA_NodeId varNodeId = UA_NODEID_STRING(1, vName);
  UA_Variant myVariant;
  UA_Variant_init(&myVariant);
  UA_Server_readValue(server, varNodeId, &myVariant);

  if (vType == UA_TYPES_DATETIME) {
    v->value.dt = *(static_cast<UA_DateTime*>(myVariant.data));
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "%s (DateTime) = %i", vName, *(static_cast<int*>(val)));)
  } else if (vType == UA_TYPES_INT32) {
    v->value.i32 = *(static_cast<int32_t*>(myVariant.data));
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "%s (i32) = %i", vName, *(static_cast<int*>(val)));)
  } else if (vType == UA_TYPES_FLOAT) {
    v->value.fl = *(static_cast<UA_Float*>(myVariant.data));
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "%s (float) = %.3f", vName, *(static_cast<float*>(val)));)
  } else {
    v->value.ui16 = *(static_cast<UA_UInt16*>(myVariant.data));
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "%s (ui16) = %i", vName, *(static_cast<int*>(val)));)
  }

}


void* getPtrToVariable(void* var, char* &vName, int& vType, bool isDebug)
{
  void *val = nullptr;
  var_t* v = (var_t*)var;

  vName = v->name;
  vType = v->type;

  if (vType == UA_TYPES_DATETIME) {
    val = (UA_DateTime*)&(v->value.dt);
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "%s (DateTime) = %i", vName, *(static_cast<int*>(val)));)
  } else if (vType == UA_TYPES_INT32) {
    val = (UA_Int32*)&(v->value.i32);
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "%s (i32) = %i", vName, *(static_cast<int*>(val)));)
  } else if (vType == UA_TYPES_FLOAT) {
    val = (UA_Float*)&(v->value.fl);
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "%s (float) = %.3f", vName, *(static_cast<float*>(val)));)
  } else {
    val = (UA_UInt16*)&(v->value.ui16);
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "%s (ui16) = %i", vName, *(static_cast<int*>(val)));)
  }

  return val;
}



