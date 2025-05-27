/*
  #include <open62541/plugin/log_stdout.h>
  #include <open62541/server.h>
  #include <open62541/server_config_default.h>
*/
//#include "include/open62541/open62541.h"

#include "include/logger.h"
#include "opcs_class.h"

#define DEBUG(a) if(isDebug){a}


bool OpcServer_c::WriteRawUnion(string s, value_u raw_vu, bool isOK)
{
  bool ret = false;
  uaDataMux->lock();

  if (vars.count(s)) {
    vars[s].raw_value = raw_vu;
    vars[s].ptr_value = &raw_vu;
    ret = write_Variable(vars[s], isOK);
  } else
    LOGA("Set: Ignore non-existing variable: %s", s.c_str());

  uaDataMux->unlock();

  return ret;
}

bool OpcServer_c::write_Variable(var_t &v, bool isOk)
{
  if (isOk) {
    v.ua_status = UA_STATUSCODE_GOOD;
    v.ua_timestamp = UA_DateTime_now();
  } else
    v.ua_status = UA_STATUSCODE_BADNOTCONNECTED;

  /* Write a different value */
  UA_VariableAttributes Attr;
  UA_Variant_init(&Attr.value);
  UA_Variant_setScalar(&Attr.value, v.ptr_value, &UA_TYPES[v.ua_type]);

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
  bool wr_status = UA_Server_writeDataValue(uaServer, v.node_id.var, wv.value);

  if (v.ua_status == UA_STATUSCODE_GOOD)
    v.ua_status = wr_status;

  return (v.ua_status == UA_STATUSCODE_GOOD);
}

// eof
