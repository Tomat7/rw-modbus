
#include "opc_class.h"

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <thread>
#include <mutex>

#include "include/logger.h"

#define DEBUG(a) if (isDebug) {a}

/* int16_t OpcServer_c::getVar(string s, int16_t i16)
  {
  if (!vars.count(s)) {
    LOGA("Ignore non-existing variable: %s", s.c_str());
    return 0;
  } else
    LOGA("Variable exist: %s", s.c_str());

  UA_Variant Vrnt;
  getVariable(vars[s], &Vrnt);
  i16 = *(static_cast<UA_Int16*>(Vrnt.data));
  return i16;
  }

  uint16_t OpcServer_c::getVar(string s, uint16_t &ui16)
  {
  UA_Variant Vrnt;
  getVariable(vars[s], &Vrnt);
  ui16 = *(static_cast<UA_UInt16*>(Vrnt.data));
  return ui16;
  }

  int64_t OpcServer_c::getVar(string s, int64_t &i64)
  {
  UA_Variant Vrnt;
  getVariable(vars[s], &Vrnt);
  i64 = *(static_cast<UA_Int64*>(Vrnt.data));
  return i64;
  }


  float OpcServer_c::getVar(string s, float &fl)
  {
  UA_Variant Vrnt;
  getVariable(vars[s], &Vrnt);
  fl = *(static_cast<UA_Float*>(Vrnt.data));
  return fl;
  }
*/
// eof