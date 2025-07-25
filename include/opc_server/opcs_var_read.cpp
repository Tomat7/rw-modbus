/*
  #include <open62541/plugin/log_stdout.h>
  #include <open62541/server.h>
  #include <open62541/server_config_default.h>
*/
//#include "include/open62541/open62541.h"

#include "include/logger.h"
#include "opcs_class.h"

#define DEBUG(a) if(isDebug){a}


void* OpcServer_c::get_VariantDataPtr(string s)
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

bool OpcServer_c::refresh_RawValue(string s)
{
  bool is_fresh = false;
  void* vardata_ = get_VariantDataPtr(s);

  if (vardata_ != nullptr) {
    vars[s].raw_value = *static_cast<numeric_u*>(vardata_);
    vars[s].ptr_value = &vars[s].raw_value;
    is_fresh = true;
  }

  return is_fresh;
}

numeric_u OpcServer_c::ReadRawUnion(string s)
{
  numeric_u raw_vu;

  if (isVariable(s))
    raw_vu = vars[s].raw_value;  // set old (last good) value
  else
    LOGW("%s: Ignore non-existing variable: %s", __func__, s.c_str());

  return raw_vu;
}

// eof
