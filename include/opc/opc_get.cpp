
#include "opc_class.h"

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <thread>
#include <mutex>

#include "include/logger.h"

#define DEBUG(a) if (isDebug) {a}

int16_t OpcServer_c::getVar(string s)
{
  UA_Variant Vrnt;
  getVariable(vars[s], &Vrnt);
  vars[s].value.i16 = *(static_cast<UA_Int16*>(Vrnt.data));
}

uint16_t OpcServer_c::getVar(string s)
{
  UA_Variant Vrnt;
  getVariable(vars[s], &Vrnt);
  vars[s].value.ui16 = *(static_cast<UA_UInt16*>(Vrnt.data));
}

int64_t OpcServer_c::getVar(string s)
{
  UA_Variant Vrnt;
  getVariable(vars[s], &Vrnt);
  vars[s].value.i64 = *(static_cast<UA_Int64*>(Vrnt.data));
}


float OpcServer_c::getVar(string s)
{
  UA_Variant Vrnt;
  getVariable(vars[s], &Vrnt);
  vars[s].value.fl = *(static_cast<UA_Float*>(Vrnt.data));
}