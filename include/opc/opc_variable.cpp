
#include "opc_class.h"

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include "include/logger.h"

#define DEBUG(a) if (isDebug) {a}



void OpcServer_c::addVariable(var_t &v)
{
  if (v.ptr_value == nullptr) {
    LOGA("Wrong ptr: %s", v.name);
    return;
  }

  getPtrToVariable(v);

  UA_Byte acl = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

  UA_VariableAttributes attr = UA_VariableAttributes_default;
  UA_Variant_setScalar(&attr.value, v.ptr_value, &UA_TYPES[v.type]);

  attr.description = UA_LOCALIZEDTEXT_ALLOC("en-US", v.name);
  attr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", v.name);
  attr.dataType = UA_TYPES[v.type].typeId;
  attr.accessLevel = acl;

  // Add the variable node to the information model
  //UA_NodeId varNodeId = UA_NODEID_STRING(1, v.name);
  UA_QualifiedName varQName = UA_QUALIFIEDNAME(1, v.ua_name);

  //UA_NodeId parentNodeId = UA_NS0ID(OBJECTSFOLDER);
  //UA_NodeId parentReferenceNodeId = UA_NS0ID(ORGANIZES);

  UA_Server_addVariableNode(uaServer, v.node_id.var, v.node_id.parent,
                            v.node_id.reference, varQName,
                            UA_NS0ID(BASEDATAVARIABLETYPE), attr, NULL, NULL);

  DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                    "New variable: %s path: %s - %s", v.name, v.ua_name, UA_StatusCode_name(rc));)
}

void OpcServer_c::setVariable(var_t &v)
{
  if (v.ptr_value == nullptr) {
    LOGA("Wrong ptr: %s", v.name);
    return;
  }

  //UA_NodeId varNodeId = UA_NODEID_STRING(1, v.qf_name);

  UA_Variant myVariant;
  UA_Variant_init(&myVariant);

  UA_Variant_setScalar(&myVariant, v.ptr_value, &UA_TYPES[v.type]);
  UA_Server_writeValue(uaServer, v.node_id.var, myVariant);

  /* Set the status code of the value to an error code. The function
     UA_Server_write provides access to the raw service. The above
     UA_Server_writeValue is syntactic sugar for writing a specific node
     attribute with the write service. */
  UA_WriteValue wv;
  UA_WriteValue_init(&wv);
  wv.nodeId = v.node_id.var;
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


void OpcServer_c::getVariable(var_t &v, UA_Variant* vrnt_)
{

  if (v.ptr_value == nullptr) {
    LOGA("Wrong ptr: %s", v.name);
    return;
  }

  //UA_NodeId varNodeId = UA_NODEID_STRING(1, v.name);
  UA_Variant myVariant;
  UA_Variant_init(&myVariant);
  UA_Server_readValue(uaServer, v.node_id.var, &myVariant);


  if (v.type == UA_TYPES_INT16) {
    v.value.i16 = *(static_cast<UA_Int16*>(myVariant.data));
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (ui16) = %i", v.name, *(static_cast<int16_t*>(v.ptr_value)));)
  } else if (v.type == UA_TYPES_UINT16) {
    v.value.ui16 = *(static_cast<UA_UInt16*>(myVariant.data));
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (ui16) = %i", v.name, *(static_cast<uint16_t*>(v.ptr_value)));)
  } else if (v.type == UA_TYPES_INT32) {
    v.value.i32 = *(static_cast<int32_t*>(myVariant.data));
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (i32) = %i", v.name, *(static_cast<int32_t*>(v.ptr_value)));)
  } else if (v.type == UA_TYPES_FLOAT) {
    v.value.fl = *(static_cast<UA_Float*>(myVariant.data));
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (float) = %.3f", v.name, *(static_cast<float*>(v.ptr_value)));)
  } else if (v.type == UA_TYPES_DATETIME) {
    v.value.dt = *(static_cast<UA_DateTime*>(myVariant.data));
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (DateTime) = %i", v.name, *(static_cast<int64_t*>(v.ptr_value)));)
  } else
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (WRONG TYPE)", v.name);)


  }


void* OpcServer_c::getPtrToVariable(var_t &v)
{

  if (v.type == UA_TYPES_INT16) {
    //v.ptr_value = (UA_Int16*)&(v.value.i16);
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (i16) = %i", v.name, *(static_cast<int16_t*>(v.ptr_value)));)
  } else if (v.type == UA_TYPES_UINT16) {
    //v.ptr_value = (UA_UInt16*)&(v.value.ui16);
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (ui16) = %i", v.name, *(static_cast<uint16_t*>(v.ptr_value)));)
  } else if (v.type == UA_TYPES_INT32) {
    //v.ptr_value = (UA_Int32*)&(v.value.i32);
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (i32) = %i", v.name, *(static_cast<int32_t*>(v.ptr_value)));)
  } else if (v.type == UA_TYPES_FLOAT) {
    //v.ptr_value = (UA_Float*)&(v.value.fl);
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (float) = %.3f", v.name, *(static_cast<float*>(v.ptr_value)));)
  } else if (v.type == UA_TYPES_DATETIME) {
    //v.ptr_value = (UA_DateTime*)&(v.value.dt);
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (DateTime) = %i", v.name, *(static_cast<int64_t*>(v.ptr_value)));)
  } else {
    DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "%s (WRONG TYPE)", v.name);)
  }

  return v.ptr_value;
}

