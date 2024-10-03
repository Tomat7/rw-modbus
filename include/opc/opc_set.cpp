
#include "opc_class.h"

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <thread>
#include <mutex>

#include "include/logger.h"

#define DEBUG(a) if (isDebug) {a}

void OpcServer_c::setVar(string s, int16_t i16)
{
  //vars[s].value.i16 = i16;
  vars[s].ptr_value = &i16;
  setVariable(vars[s]);
}

void OpcServer_c::setVar(string s, uint16_t ui16)
{
  //vars[s].value.ui16 = ui16;
  vars[s].ptr_value = &ui16;
  setVariable(vars[s]);
}

void OpcServer_c::setVar(string s, int64_t i64)
{
  vars[s].value.i64 = i64;
  setVariable(vars[s]);
}


void OpcServer_c::setVar(string s, float fl)
{
  vars[s].value.fl = fl;
  setVariable(vars[s]);
}