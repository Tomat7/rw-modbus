
#include "opc_class.h"

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <thread>
#include <mutex>

#include "include/logger.h"

#define DEBUG(a) if (isDebug) {a}

int16_t OpcServer_c::getVar(string s, int16_t &i16)
{
  UA_Variant Vrnt;
  printf("\ngetVar1: %s\n", s.c_str());
  getVariable(vars[s], &Vrnt);
  vars[s].value.i16 = *(static_cast<UA_Int16*>(Vrnt.data));
  printf("\ngetVar2: %s\n", s.c_str());

  int16_t ii16 = *(static_cast<UA_Int16*>(Vrnt.data));
  printf("\ngetVar3: %s\n", s.c_str());

  i16 = *(static_cast<UA_Int16*>(Vrnt.data));
  printf("\ngetVar4: %s\n", s.c_str());

  return i16;
}

uint16_t OpcServer_c::getVar(string s, uint16_t &ui16)
{
  UA_Variant Vrnt;
  getVariable(vars[s], &Vrnt);
  //vars[s].value.ui16 = *(static_cast<UA_UInt16*>(Vrnt.data));
  ui16 = *(static_cast<UA_UInt16*>(Vrnt.data));
  return ui16;
}

int64_t OpcServer_c::getVar(string s, int64_t &i64)
{
  UA_Variant Vrnt;
  getVariable(vars[s], &Vrnt);
  //vars[s].value.i64 = *(static_cast<UA_Int64*>(Vrnt.data));
  i64 = *(static_cast<UA_Int64*>(Vrnt.data));
  return i64;
}


float OpcServer_c::getVar(string s, float &fl)
{
  UA_Variant Vrnt;
  getVariable(vars[s], &Vrnt);
  //vars[s].value.fl = *(static_cast<UA_Float*>(Vrnt.data));
  fl = *(static_cast<UA_Float*>(Vrnt.data));
  return fl;
}