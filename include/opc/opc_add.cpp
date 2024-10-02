
#include "opc_class.h"

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <thread>
#include <mutex>

#include "include/logger.h"

#define DEBUG(a) if (isDebug) {a}

void OpcServer_c::addVar_NodeId(var_t &v)
{
  v.node_id.var = UA_NODEID_STRING(1, v.name);

  if (v.folder != nullptr) {
    v.node_id.parent = addFolder(v.folder);
    v.node_id.reference = UA_NS0ID(HASCOMPONENT);
  } else {
    v.node_id.parent = UA_NS0ID(OBJECTSFOLDER);
    v.node_id.reference = UA_NS0ID(ORGANIZES);
  }
}




int OpcServer_c::addVar(string s, int16_t i16, int rmode)
{
  rc = addVar_Names(s, UA_TYPES_INT16, rmode);
  if (rc == 0)
    return 0;

  addVar_NodeId(vars[s]);
  vars[s].value.i16 = i16;
  vars[s].ptr_value = &vars[s].value.i16;
  addVariable(vars[s]);
  return 1;
}

int OpcServer_c::addVar(string s, uint16_t ui16, int rmode)
{
  rc = addVar_Names(s, UA_TYPES_UINT16, rmode);
  if (rc == 0)
    return 0;

  addVar_NodeId(vars[s]);
  vars[s].value.ui16 = ui16;
  vars[s].ptr_value = &vars[s].value.ui16;
  addVariable(vars[s]);
  return 1;
}

int OpcServer_c::addVar(string s, int64_t i64, int rmode)
{
  addVar_Names(s, UA_TYPES_INT64, rmode);
  if (rc == 0)
    return 0;

  addVar_NodeId(vars[s]);
  vars[s].value.i64 = i64;
  vars[s].ptr_value = &vars[s].value.i64;
  addVariable(vars[s]);
  return 1;
}


int OpcServer_c::addVar(string s, float fl, int rmode)
{
  rc = addVar_Names(s, UA_TYPES_FLOAT, rmode);
  if (rc == 0)
    return 0;

  addVar_NodeId(vars[s]);
  vars[s].value.fl = fl;
  vars[s].ptr_value = &vars[s].value.fl;
  addVariable(vars[s]);
  return 1;
}
