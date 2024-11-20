
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include "include/logger.h"
#include "opc_class.h"

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
                    "NewVar: %s %s, path: %s - %s", v.name, d.c_str(),
                    v.ua_name, UA_StatusCode_name(rc));)
}

void OpcServer_c::writeVariable(var_t &v)
{
  /* Write a different value */
  UA_VariableAttributes Attr;
  UA_Variant_init(&Attr.value);
  UA_Variant_setScalar(&Attr.value, v.ptr_value, &UA_TYPES[v.type]);

  // Use a more detailed write function than UA_Server_writeValue
  UA_WriteValue wv;
  UA_WriteValue_init(&wv);

  wv.value.status = v.ua_status;  // OK or BAD
  wv.value.hasStatus = true;
  wv.nodeId = v.node_id.var;
  wv.attributeId = UA_ATTRIBUTEID_VALUE;
  wv.value.value = Attr.value;
  wv.value.hasValue = true;

  // UA_DateTime currentTime = UA_DateTime_now();
  wv.value.hasServerTimestamp = true;
  wv.value.serverTimestamp = UA_DateTime_now();
  wv.value.hasSourceTimestamp = true;
  wv.value.sourceTimestamp = v.ua_timestamp;
  UA_Server_writeDataValue(uaServer, v.node_id.var, wv.value);

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

// get pointer to UA_Variant.Data
var_union OpcServer_c::getVarUnion(string s)
{
  var_union vu;
  vu.i64 = 0x8000000000000000;
  vu.fl = -999.99f;
  vu.dbl = -9999.999;

  void* VarData = getVarData(s);
  if (VarData != nullptr) {
    vu = *static_cast<var_union*>(VarData);
    vars[s].value = vu;
  }

  return vu;
}

void* OpcServer_c::getVarData(string s)
{
  void* VarData = nullptr;

  if (vars.count(s)) {
    UA_Variant Vrnt;
    UA_Variant_init(&Vrnt);
    UA_Server_readValue(uaServer, vars[s].node_id.var, &Vrnt);
    VarData = Vrnt.data;
    UA_Variant_clear(&Vrnt);
  } else
    LOGA("%s: Ignore non-existing variable: %s", __func__, s.c_str());

  return VarData;
}



// eof
