
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <thread>
#include <mutex>

#include "opc_class.h"

#define DEBUG(a) if (isDebug) {a}
static mutex uaRunning_mux;

OpcServer_c::OpcServer_c()
{
  uaServer = UA_Server_new();
  uaRunning = new mutex;
}

OpcServer_c::~OpcServer_c()
{

  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Destructor: set uaRunning = false.");
  uaRunning = false;
  uaRunning_mux.lock();
  //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Destructor: waiting 1 sec...");
  //std::this_thread::sleep_for(std::chrono::seconds(1));
  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Destructor: trying to delete...");
  UA_Server_delete(uaServer);
  uaRunning_mux.unlock();
  //delete uaRunning_mux;
  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Destructor: deleted.");
}


void OpcServer_c::init(UA_UInt16 portNumber)
{
  // uaServer = UA_Server_new();
  UA_ServerConfig* uaServerConfig = UA_Server_getConfig(uaServer);
  UA_ServerConfig_setDefault(uaServerConfig);
  UA_ServerConfig_setBasics_withPort(uaServerConfig, portNumber);
}

void OpcServer_c::run()
{
  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Run: try to lock mutex.");
  uaRunning_mux.lock();
  uaRunning = true;
  // UA_StatusCode retval =
  UA_Server_run(uaServer, &uaRunning);
  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Run: got &uaRunning = false");
  uaRunning_mux.unlock();
  //UA_Server_delete(uaServer);
  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Run: deleted.");
}

void OpcServer_c::stop()
{
  uaRunning = false;
  UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Stopping by external signal. 5");
}

void OpcServer_c::initVar(string s, int t, int m)
{
  var_t v;
  v.type = t;
  v.rmode = m;
  v.fullname = s;
  v.name = const_cast<char*>(s.c_str());
  vars[s] = v;
}

void OpcServer_c::addVar(string s, int16_t i16, int rmode)
{
  initVar(s, UA_TYPES_INT16, rmode);
  vars[s].value.i16 = i16;
  vars[s].ptr_value = &vars[s].value.i16;
  addVariable(vars[s]);
}

/*
  void OpcServer_c::addVar(string s, int32_t i32, int rmode)
  {
  initVar(s, UA_TYPES_INT32, i32, rmode);
  }
*/

void OpcServer_c::addVar(string s, int64_t i64, int rmode)
{
  initVar(s, UA_TYPES_INT64, rmode);
  vars[s].value.i64 = i64;
  vars[s].ptr_value = &vars[s].value.i64;
  addVariable(vars[s]);
}


void OpcServer_c::addVar(string s, uint16_t ui16, int rmode)
{
  initVar(s, UA_TYPES_UINT16, rmode);
  vars[s].value.ui16 = ui16;
  vars[s].ptr_value = &vars[s].value.ui16;
  addVariable(vars[s]);
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
  initVar(s, UA_TYPES_FLOAT, rmode);
  vars[s].value.fl = fl;
  vars[s].ptr_value = &vars[s].value.fl;
  addVariable(vars[s]);
}
