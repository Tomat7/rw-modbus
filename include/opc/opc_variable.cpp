
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

  UA_Byte acl = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  UA_Variant_setScalar(&attr.value, v.ptr_value, &UA_TYPES[v.type]);

  attr.description = UA_LOCALIZEDTEXT_ALLOC("en-US", v.name);
  attr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", v.name);
  attr.dataType = UA_TYPES[v.type].typeId;
  attr.accessLevel = acl;

  UA_QualifiedName varQName = UA_QUALIFIEDNAME(1, v.ua_name);

  UA_Server_addVariableNode(uaServer, v.node_id.var, v.node_id.parent,
                            v.node_id.reference, varQName,
                            UA_NS0ID(BASEDATAVARIABLETYPE), attr, NULL, NULL);
  string d = strVarDetails(v);
  DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                    "NewVar: %s %s, path: %s - %s", v.name, d.c_str(), v.ua_name,
                    UA_StatusCode_name(rc));)
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
  wv.value.status = UA_STATUSCODE_GOOD; //UA_STATUSCODE_BADNOTCONNECTED;
  //wv.value.status = UA_STATUSCODE_BADCOMMUNICATIONERROR;
  wv.value.hasStatus = true;
  wv.value.value = myVariant;
  wv.value.hasValue = true;
  //wv.value.serverTimestamp = true;
  UA_Server_write(uaServer, &wv);

}


void OpcServer_c::writeVariable(var_t &v)
{
  //UA_NodeId myIntegerNodeId = UA_NODEID_STRING(1, "fake.source_timestamp");

  /* Write a different integer value */
  UA_VariableAttributes Attr;
  UA_Variant_init(&Attr.value);
  UA_Variant_setScalar(&Attr.value, v.ptr_value, &UA_TYPES[v.type]);

  // Use a more detailed write function than UA_Server_writeValue
  UA_WriteValue wv;
  UA_WriteValue_init(&wv);
  wv.nodeId = v.node_id.var;
  wv.attributeId = UA_ATTRIBUTEID_VALUE;
  wv.value.status = UA_STATUSCODE_GOOD;
  //wv.value.status = UA_STATUSCODE_BADNODEIDUNKNOWN; //UA_STATUSCODE_BAD;
  wv.value.hasStatus = true;
  wv.value.value = Attr.value;
  wv.value.hasValue = true;

  UA_DateTime currentTime = UA_DateTime_now();
  wv.value.hasServerTimestamp = true;
  wv.value.serverTimestamp = currentTime;
  wv.value.hasSourceTimestamp = true;
  wv.value.sourceTimestamp = currentTime - 1800 * UA_DATETIME_SEC;

  UA_Server_writeDataValue(uaServer, v.node_id.var, wv.value);

  /* Write a string */
  UA_String myString = UA_STRING("test");
  UA_Variant myVar;
  UA_Variant_init(&myVar);
  UA_Variant_setScalar(&myVar, &myString, &UA_TYPES[UA_TYPES_STRING]);
  UA_StatusCode retval = UA_Server_writeValue(uaServer, v.node_id.var, myVar);
  printf("Writing a string returned statuscode %s\n", UA_StatusCode_name(retval));

}


void OpcServer_c::getVariable(var_t &v, UA_Variant* vrnt_)
{
  UA_Variant_init(vrnt_);
  UA_Server_readValue(uaServer, v.node_id.var, vrnt_);
}


string OpcServer_c::strVarDetails(var_t &v)
{
  string ret;
  if (v.type == UA_TYPES_INT16)
    ret = "(i16) = " + to_string(*(static_cast<int16_t*>(v.ptr_value)));
  else if (v.type == UA_TYPES_INT32)
    ret = "(i32) = " + to_string(*(static_cast<int32_t*>(v.ptr_value)));
  else if (v.type == UA_TYPES_INT64)
    ret = "(i64) = " + to_string(*(static_cast<int64_t*>(v.ptr_value)));
  else if (v.type == UA_TYPES_UINT16)
    ret = "(ui16) = " + to_string(*(static_cast<uint16_t*>(v.ptr_value)));
  else if (v.type == UA_TYPES_UINT32)
    ret = "(ui32) = " + to_string(*(static_cast<uint32_t*>(v.ptr_value)));
  else if (v.type == UA_TYPES_UINT64)
    ret = "(ui64) = " + to_string(*(static_cast<uint64_t*>(v.ptr_value)));
  else if (v.type == UA_TYPES_FLOAT)
    ret = "(float) = " + to_string(*(static_cast<float*>(v.ptr_value)));
  else if (v.type == UA_TYPES_DATETIME)
    ret = "(DateTime)";

  return ret;
}

