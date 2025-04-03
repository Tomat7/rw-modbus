/*
  #include <open62541/plugin/log_stdout.h>
  #include <open62541/server.h>
  #include <open62541/server_config_default.h>
*/
//#include "include/open62541/open62541.h"

#include "include/logger.h"
#include "opc_class.h"

#define DEBUG(a) if(isDebug){a}


void* OpcServer_c::getVariantDataPtr(string s)
{
  if (vars.count(s)) {
    UA_Variant_clear(uaVariant);
    UA_Variant_init(uaVariant);
    UA_Server_readValue(uaServer, vars[s].node_id.var, uaVariant);
  } else {
    LOGW("%s: Ignore non-existing variable: %s", __func__, s.c_str());
    return nullptr;
  }

  return (uaVariant->data);
}

bool OpcServer_c::refreshRawValue(string s)
{
  bool ret = false;
  void* v_ = getVariantDataPtr(s);

  if (v_ != nullptr) {
    vars[s].raw_value = *static_cast<value_u*>(v_);
    vars[s].ptr_value = &vars[s].raw_value;
    ret = true;
  }

  return ret;
}

value_u OpcServer_c::ReadRawValue(string s)
{
  value_u raw_vu;

  if (isVariable(s))
    raw_vu = vars[s].raw_value;  // set old (last good) value
  else
    LOGW("%s: Ignore non-existing variable: %s", __func__, s.c_str());

  return raw_vu;
}

// eof
