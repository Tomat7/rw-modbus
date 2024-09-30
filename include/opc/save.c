
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include "opc_class.h"

#define DEBUG(a) if (isDebug) {a}

OpcServer_c::OpcServer_c() { uaServer = UA_Server_new(); }

OpcServer_c::~OpcServer_c() { UA_Server_delete(uaServer); }


void OpcServer_c::init(UA_UInt16 portNumber)
{
  // uaServer = UA_Server_new();
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

void OpcServer_c::initVar(string s, void* value, int uatype, int uamode)
{
  var_t v;
  v.rmode = uamode;
  v.fullname = s;
  v.name = const_cast<char*>(v.fullname.c_str());
  vars[v.fullname] = v;
  addVariable(vars[v.fullname]);
}

void OpcServer_c::addVar(string s, int16_t i16, int rmode)
{
  initVar(s, &i16, UA_TYPES_INT16, rmode);
}

/*
void OpcServer_c::addVar(string s, int32_t i32, int rmode)
{
  initVar(s, UA_TYPES_INT32, i32, rmode);
}
*/

void OpcServer_c::addVar(string s, int64_t i64, int rmode)
{
  initVar(s, &i64, UA_TYPES_INT64,rmode);
}


void OpcServer_c::addVar(string s, uint16_t ui16, int rmode)
{
  initVar(s, &ui16, UA_TYPES_UINT16,rmode);
}

/*
void OpcServer_c::addVar(string s, uint32_t ui32, int _rmode)
{
  var_t v;
  v.type = UA_TYPES_UINT32;
  v.value.ui32 = ui32;
  v.ptr_value = &v.value.ui32;
  initVar(s, v, _rmode);
}
*/

/*
void OpcServer_c::addVar(string s, uint64_t ui64, int _rmode)
{
  initVar(s, UA_TYPES_INT32, rmode);
}
*/

void OpcServer_c::addVar(string s, float fl, int rmode)
{
  initVar(s, &fl, UA_TYPES_FLOAT, rmode);
}
