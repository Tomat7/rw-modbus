
#include "opc_class.h"

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <thread>
#include <mutex>
#include <string>

#include "include/logger.h"

#define DEBUG(a) if (isDebug) {a}


int OpcServer_c::addVar(string s, int16_t i16, int rmode)
{
  rc = addVar_Names(s, UA_TYPES_INT16, rmode);
  if (rc == 0)
    return 0;

  addVar_NodeId(vars[s]);
  vars[s].ptr_value = &i16;
  addVariable(vars[s]);
  return 1;
}

int OpcServer_c::addVar(string s, uint16_t ui16, int rmode)
{
  rc = addVar_Names(s, UA_TYPES_UINT16, rmode);
  if (rc == 0)
    return 0;

  addVar_NodeId(vars[s]);
  //vars[s].value.ui16 = ui16;
  //vars[s].ptr_value = &vars[s].value.ui16;
  vars[s].ptr_value = &ui16;
  addVariable(vars[s]);
  return 1;
}

int OpcServer_c::addVar(string s, int64_t i64, int rmode)
{
  addVar_Names(s, UA_TYPES_INT64, rmode);
  if (rc == 0)
    return 0;

  addVar_NodeId(vars[s]);
  //vars[s].value.i64 = i64;
  vars[s].ptr_value = &i64;
  addVariable(vars[s]);
  return 1;
}


int OpcServer_c::addVar(string s, float fl, int rmode)
{
  rc = addVar_Names(s, UA_TYPES_FLOAT, rmode);
  if (rc == 0)
    return 0;

  addVar_NodeId(vars[s]);
  //vars[s].value.fl = fl;
  vars[s].ptr_value = &fl;
  addVariable(vars[s]);
  return 1;
}
