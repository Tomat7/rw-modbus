
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include "opc_class.h"

#define DEBUG(a) if (isDebug) {a}


void OpcServer_c::addVariable(var_t &v)
{
  char* vName = v.name;
  int vType = v.type;
  void* val = v.ptr_value;
  UA_Byte acl = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
              "%x (val1), %s %i", val, vName, *(static_cast<int16_t*>(val)));

  /* void* val = */
  //val = getPtrToVariable(v, vName, vType, true);
  if (val == nullptr)
    return;

  UA_VariableAttributes attr = UA_VariableAttributes_default;

  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
              "%x (val2), %s %i", val, vName, *(static_cast<int16_t*>(val)));

  UA_Variant_setScalar(&attr.value, val, &UA_TYPES[vType]);

  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
              "%x (val3), %s %i", val, v.name, *(static_cast<int16_t*>(val)));

  attr.description = UA_LOCALIZEDTEXT_ALLOC("en-US", vName);
  attr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", vName);
  attr.dataType = UA_TYPES[vType].typeId;
  attr.accessLevel = acl;

  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
              "%x (val4)", val);

  // Add the variable node to the information model
  UA_NodeId varNodeId = UA_NODEID_STRING(1, vName);
  UA_QualifiedName varName = UA_QUALIFIEDNAME(1, vName);

  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
              "%x (val5)", val);

  UA_NodeId parentNodeId = UA_NS0ID(OBJECTSFOLDER);
  UA_NodeId parentReferenceNodeId = UA_NS0ID(ORGANIZES);

  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
              "%x (val6)", val);

  UA_Server_addVariableNode(uaServer, varNodeId, parentNodeId,
                            parentReferenceNodeId, varName,
                            UA_NS0ID(BASEDATAVARIABLETYPE), attr, NULL, NULL);

  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
              "%x (val7)", val);

  // allocations on the heap need to be freed
  //UA_VariableAttributes_clear(&attr);
  //UA_NodeId_clear(&varNodeId);
  //UA_QualifiedName_clear(&varName);
}

void OpcServer_c::setVariable(void* var)
{
  char* vName = nullptr;
  int vType = 0;

  void* val = nullptr;
  //(var, vName, vType);

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

  void* val = nullptr;
  //(var, vName, vType);
  if (val == nullptr)
    return;

  UA_NodeId varNodeId = UA_NODEID_STRING(1, vName);
  UA_Variant myVariant;
  UA_Variant_init(&myVariant);
  UA_Server_readValue(uaServer, varNodeId, &myVariant);

  if (vType == UA_TYPES_INT16) {
    v->value.i16 = *(static_cast<UA_Int16*>(myVariant.data));
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (ui16) = %i", vName, *(static_cast<int16_t*>(val)));)
  } else if (vType == UA_TYPES_UINT16) {
    v->value.ui16 = *(static_cast<UA_UInt16*>(myVariant.data));
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (ui16) = %i", vName, *(static_cast<uint16_t*>(val)));)
  } else if (vType == UA_TYPES_INT32) {
    v->value.i32 = *(static_cast<int32_t*>(myVariant.data));
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (i32) = %i", vName, *(static_cast<int32_t*>(val)));)
  } else if (vType == UA_TYPES_FLOAT) {
    v->value.fl = *(static_cast<UA_Float*>(myVariant.data));
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (float) = %.3f", vName, *(static_cast<float*>(val)));)
  } else if (vType == UA_TYPES_DATETIME) {
    v->value.dt = *(static_cast<UA_DateTime*>(myVariant.data));
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (DateTime) = %i", vName, *(static_cast<int64_t*>(val)));)
  } else
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (WRONG TYPE)", vName);)


  }


void* OpcServer_c::getPtrToVariable(var_t &var, char* &vName, int &vType, bool isDebug)
{
  void* val = nullptr;
  var_t* v = &var;

  vName = v->name;
  vType = v->type;

  if (vType == UA_TYPES_INT16) {
    val = (UA_Int16*)&(v->value.i16);
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (i16) = %i", vName, *(static_cast<int16_t*>(val)));)
  } else if (vType == UA_TYPES_UINT16) {
    val = (UA_UInt16*)&(v->value.ui16);
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (ui16) = %i", vName, *(static_cast<uint16_t*>(val)));)
  } else if (vType == UA_TYPES_INT32) {
    val = (UA_Int32*)&(v->value.i32);
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (i32) = %i", vName, *(static_cast<int32_t*>(val)));)
  } else if (vType == UA_TYPES_FLOAT) {
    val = (UA_Float*)&(v->value.fl);
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (float) = %.3f", vName, *(static_cast<float*>(val)));)
  } else if (vType == UA_TYPES_DATETIME) {
    val = (UA_DateTime*)&(v->value.dt);
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (DateTime) = %i", vName, *(static_cast<int64_t*>(val)));)
  } else {
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (WRONG TYPE)", vName);)
  }

  return val;
}


