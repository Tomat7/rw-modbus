
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include "include/logger.h"
#include "opc_class.h"

#define DEBUG(a) if (isDebug) { a }

// void* OpcServer_c::getVariantData(string s);
// var_union OpcServer_c::readRawValue(string s);

void* OpcServer_c::getVariantDataPtr(string s)
{
  void* VarData = nullptr;

  if (vars.count(s)) {
    UA_Variant_clear(uaVariant);
    UA_Variant_init(uaVariant);
    UA_Server_readValue(uaServer, vars[s].node_id.var, uaVariant);
    if (uaVariant->data != nullptr) {
      vars[s].value = *static_cast<value_u*>(uaVariant->data);
      VarData = &vars[s].value;
    } else
      VarData = nullptr;
  } else
    LOGW("%s: Ignore non-existing variable: %s", __func__, s.c_str());

  return VarData;
}

value_u* OpcServer_c::getRawValue(string s)
{
  return static_cast<value_u*>(getVariantDataPtr(s));
}


value_u OpcServer_c::ReadRawValue(string s)
{
  value_u raw_vu;

  if (isVariable(s))
    raw_vu = vars[s].value;  // set old (last good) value

  return raw_vu;
}

bool OpcServer_c::WriteRawValue(string s, value_u raw_vu, bool isOK)
{
  bool ret = false;
  uaDataMux->lock();

  if (vars.count(s)) {
    vars[s].value = raw_vu;
    vars[s].ptr_value = &raw_vu;
    writeVariable(vars[s], isOK);
    ret = true;
  } else
    LOGA("Set: Ignore non-existing variable: %s", s.c_str());

  uaDataMux->unlock();

  return ret;
}

// eof
