
#include "opc_class.h"

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <thread>
#include <mutex>

#include "include/logger.h"

#define DEBUG(a) if (isDebug) {a}

OpcServer_c::OpcServer_c(UA_UInt16 _port)
{
  uaPort = _port;
  uaServer = UA_Server_new();
  uaRunning_mux = new mutex;
}

OpcServer_c::~OpcServer_c()
{

  LOGD("Destructor: set uaRunning = false.");
  uaRunning = false;
  uaRunning_mux->lock();
  LOGD("Destructor: trying to delete...");
  UA_Server_delete(uaServer);
  uaRunning_mux->unlock();
  delete uaRunning_mux;
  LOGD("Destructor: deleted.");
}


void OpcServer_c::init(UA_UInt16 _port)
{
  // uaServer = UA_Server_new();
  if (_port != 0)
    uaPort = _port;
  UA_ServerConfig* uaServerConfig = UA_Server_getConfig(uaServer);
  UA_ServerConfig_setDefault(uaServerConfig);
  UA_ServerConfig_setBasics_withPort(uaServerConfig, uaPort);
}

void OpcServer_c::run()
{
  LOGD("Run: try to lock mutex.");
  uaRunning_mux->lock();
  uaRunning = true;
  UA_Server_run(uaServer, &uaRunning);
  LOGD("Run: got &uaRunning = false");
  uaRunning_mux->unlock();
  LOGD("Run: deleted.");
}

void OpcServer_c::stop()
{
  uaRunning = false;
  LOGD("Stop: set &uaRunning = false.");
}

