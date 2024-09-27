
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include "opc_class.h"

#define DEBUG(a) if (isDebug) {a}

OpcServer_c::OpcServer_c() {}

OpcServer_c::~OpcServer_c() { UA_Server_delete(uaServer); }


void OpcServer_c::init(UA_UInt16 portNumber)
{
  uaServer = UA_Server_new();

  UA_ServerConfig* uaServerConfig = UA_Server_getConfig(uaServer);
  UA_ServerConfig_setDefault(uaServerConfig);
  UA_ServerConfig_setBasics_withPort(uaServerConfig, portNumber);
}

void OpcServer_c::run()
{
  uaRunning = true;
  // UA_StatusCode retval =
  UA_Server_run(uaServer, &uaRunning);
  UA_Server_delete(uaServer);
}

void OpcServer_c::stop()
{
  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Stopping by external signal.");
  uaRunning = false;
}

void OpcServer_c::addVariable(void* var)
{
  char* vName = nullptr;
  int vType = 0;

  void* val = getPtrToVariable(var, vName, vType, true);

  UA_VariableAttributes attr = UA_VariableAttributes_default;

  UA_Variant_setScalar(&attr.value, val, &UA_TYPES[vType]);

  attr.description = UA_LOCALIZEDTEXT_ALLOC("en-US", vName);
  attr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", vName);
  attr.dataType = UA_TYPES[vType].typeId;
  attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

  // Add the variable node to the information model
  UA_NodeId varNodeId = UA_NODEID_STRING(1, vName);
  UA_QualifiedName varName = UA_QUALIFIEDNAME(1, vName);

  UA_NodeId parentNodeId = UA_NS0ID(OBJECTSFOLDER);
  UA_NodeId parentReferenceNodeId = UA_NS0ID(ORGANIZES);
  UA_Server_addVariableNode(uaServer, varNodeId, parentNodeId,
                            parentReferenceNodeId, varName,
                            UA_NS0ID(BASEDATAVARIABLETYPE), attr, NULL, NULL);

  // allocations on the heap need to be freed
  UA_VariableAttributes_clear(&attr);
  UA_NodeId_clear(&varNodeId);
  UA_QualifiedName_clear(&varName);
}

void OpcServer_c::setVariable(void* var)
{
  char* vName = nullptr;
  int vType = 0;

  void* val = getPtrToVariable(var, vName, vType);

  UA_NodeId varNodeId = UA_NODEID_STRING(1, vName);

  UA_Variant myVariant;
  UA_Variant_init(&myVariant);

  UA_Variant_setScalar(&myVariant, val, &UA_TYPES[vType]);
  UA_Server_writeValue(uaServer, varNodeId, myVariant);

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
  UA_Server_write(uaServer, &wv);

  /* Reset the variable to a good statuscode with a value */
  wv.value.hasStatus = false;
  wv.value.value = myVariant;
  wv.value.hasValue = true;
  UA_Server_write(uaServer, &wv);

}


void OpcServer_c::getVariable(void* var, bool isDebug)
{
  var_t* v = (var_t*)var;
  char* vName = nullptr;
  int vType = 0;

  void* val = getPtrToVariable(var, vName, vType);

  UA_NodeId varNodeId = UA_NODEID_STRING(1, vName);
  UA_Variant myVariant;
  UA_Variant_init(&myVariant);
  UA_Server_readValue(uaServer, varNodeId, &myVariant);

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


void* OpcServer_c::getPtrToVariable(void* var, char* &vName, int &vType, bool isDebug)
{
  void* val = nullptr;
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



