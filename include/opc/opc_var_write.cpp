/*
  #include <open62541/plugin/log_stdout.h>
  #include <open62541/server.h>
  #include <open62541/server_config_default.h>
*/
//#include "include/open62541/open62541.h"

#include "include/logger.h"
#include "opc_class.h"

#define DEBUG(a) if(isDebug){a}


bool OpcServer_c::WriteRawUnion(string s, value_u raw_vu, bool isOK)
{
  bool ret = false;
  uaDataMux->lock();

  if (vars.count(s)) {
    vars[s].raw_value = raw_vu;
    vars[s].ptr_value = &raw_vu;
    writeVariable(vars[s], isOK);
    ret = true;
  } else
    LOGA("Set: Ignore non-existing variable: %s", s.c_str());

  uaDataMux->unlock();

  return ret;
}

void OpcServer_c::writeVariable(var_t &v, bool isOk)
{
  if (isOk) {
    v.ua_status = UA_STATUSCODE_GOOD;
    v.ua_timestamp = UA_DateTime_now();
  } else
    v.ua_status = UA_STATUSCODE_BADNOTCONNECTED;

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

  return;
}

// eof
