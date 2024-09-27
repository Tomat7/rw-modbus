
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include "opc_class.h"

#define DEBUG(a) if (isDebug) {a}

OpcServer_c::OpcServer_c() {}

OpcServer_c::~OpcServer_c() { UA_Server_delete(uaServer); }


void OpcServer_c::init(UA_UInt16 portNumber)
{
  uaServer = UA_Server_new();

  UA_ServerConfig* uaServerConfig = UA_Server_getConfig(uaServer);
  UA_ServerConfig_setDefault(uaServerConfig);
  UA_ServerConfig_setBasics_withPort(uaServerConfig, portNumber);
}

void OpcServer_c::run()
{
  uaRunning = true;
  // UA_StatusCode retval =
  UA_Server_run(uaServer, &uaRunning);
  UA_Server_delete(uaServer);
}

void OpcServer_c::stop()
{
  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Stopping by external signal.");
  uaRunning = false;
}

void OpcServer_c::addVar_name(string s, var_t &v)
{
  v.str_name = s;
  v.name = const_cast<char*>(v.str_name.c_str());
  vars[v.str_name] = v;
}

void OpcServer_c::addVar(string s, int16_t i16)
{
  var_t v;
  v.value.i16 = i16;
  v.type = UA_TYPES_INT16;
  addVar_name(s, v);
}

void OpcServer_c::addVar(string s, int32_t i32)
{
  var_t v;
  v.value.i32 = i32;
  v.type = UA_TYPES_INT32;
  addVar_name(s, v);
}

void OpcServer_c::addVar(string s, int64_t i64)
{
  var_t v;
  v.value.i64 = i64;
  v.type = UA_TYPES_INT64;
  addVar_name(s, v);
}

void OpcServer_c::addVar(string s, uint16_t ui16)
{
  var_t v;
  v.value.ui16 = ui16;
  v.type = UA_TYPES_UINT16;
  addVar_name(s, v);
}

void OpcServer_c::addVar(string s, uint32_t ui32)
{
  var_t v;
  v.value.ui32 = ui32;
  v.type = UA_TYPES_UINT32;
  addVar_name(s, v);
}

void OpcServer_c::addVar(string s, uint64_t ui64)
{
  var_t v;
  v.value.ui64 = ui64;
  v.type = UA_TYPES_UINT64;
  addVar_name(s, v);
}

void OpcServer_c::addVar(string s, float fl)
{
  var_t v;
  v.value.fl = fl;
  v.type = UA_TYPES_FLOAT;
  addVar_name(s, v);
}
