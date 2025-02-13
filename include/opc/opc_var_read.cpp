
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include "include/logger.h"
#include "opc_class.h"

#define DEBUG(a) if (isDebug) { a }

// void* OpcServer_c::getVariantData(string s);
// var_union OpcServer_c::readRawValue(string s);

void* OpcServer_c::getVariantData(string s)
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

value_u OpcServer_c::readRawValue(string s)
{
  value_u vu;
  vu.i64 = 0x8000000000000000;
  vu.fl = -999.32f;
  vu.dbl = -9999.987;

  if (isVariable(s))
    vu = vars[s].value;  // set old (last good) value

  return vu;
}

// eof
