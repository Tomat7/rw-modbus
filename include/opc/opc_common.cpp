
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <map>
#include <mutex>
#include <set>
#include <thread>
#include <typeindex>
#include <typeinfo>

#include "include/logger.h"
#include "opc_class.h"

#define DEBUG(a) \
  if (isDebug) { \
    a            \
  }

OpcServer_c::OpcServer_c(UA_UInt16 _port)
{
  uaPort = _port;
  types[type_index(typeid(int16_t))] = UA_TYPES_INT16;
  types[type_index(typeid(uint16_t))] = UA_TYPES_UINT16;
  types[type_index(typeid(int32_t))] = UA_TYPES_INT32;
  types[type_index(typeid(uint32_t))] = UA_TYPES_UINT32;
  types[type_index(typeid(int64_t))] = UA_TYPES_INT64;
  types[type_index(typeid(uint64_t))] = UA_TYPES_UINT64;
  types[type_index(typeid(float))] = UA_TYPES_FLOAT;
}

OpcServer_c::~OpcServer_c()
{
  if (uaSrvMux != nullptr)
    stop();
  LOGW("uaDestructor: done.");
}

void OpcServer_c::init(UA_UInt16 _port)
{
  if (_port != 0)
    uaPort = _port;

  uaSrvMux = new mutex;
  uaGetMux = new mutex;
  uaDataMux = new mutex;
  uaServer = UA_Server_new();
  uaVariant = UA_Variant_new();

  UA_ServerConfig* uaServerConfig = UA_Server_getConfig(uaServer);
  UA_ServerConfig_setDefault(uaServerConfig);
  UA_ServerConfig_setBasics_withPort(uaServerConfig, uaPort);
  LOGW("Init: server ready to start on port:%d", uaPort);
}

void OpcServer_c::run()
{
  LOGW("Run: try to lock mutex.");
  uaSrvMux->lock();
  uaRunning = true;
  UA_Server_run(uaServer, &uaRunning);
  LOGW("Run: got &uaRunning = false and stopped now.");
  uaSrvMux->unlock();
}

void OpcServer_c::stop()
{
  LOGW("Stop: set &uaRunning = false.");
  uaRunning = false;
  uaDataMux->lock();
  uaSrvMux->lock();

  set<string> vs;
  for (auto &[s, v] : vars)
    vs.insert(s);
  for (auto &s : vs)
    delVar(s);
  LOGW("Stop: map cleared.");

  UA_Variant_clear(uaVariant);
  UA_Variant_delete(uaVariant);
  UA_Server_delete(uaServer);
  uaServer = nullptr;
  LOGW("Stop: server deleted.");

  uaSrvMux->unlock();
  uaDataMux->unlock();
  delete uaSrvMux;
  delete uaGetMux;
  delete uaDataMux;
  uaSrvMux = nullptr;
  uaGetMux = nullptr;
  uaDataMux = nullptr;
  LOGW("Stop: MUX free.");
}

void OpcServer_c::delVar(string s)
{
  UA_Server_deleteNode(uaServer, vars[s].node_id.var, true);
  vars.erase(s);
  LOGI("Deleted Var/Node: %s", s.c_str());
}

int OpcServer_c::getType(string s)
{
  if (vars.count(s))
    return vars[s].type;
  else
    LOGA("Type: Ignore non-existing variable: %s", s.c_str());
  return -1;
}

int OpcServer_c::getStatus(string s)
{
  if (vars.count(s))
    return vars[s].ua_status;  // 0 - is OK, any other (1 or -1) is BAD
  else
    LOGA("Status: Ignore non-existing variable: %s", s.c_str());
  return -1;
}

/* var_union OpcServer_c::getValue(string s)
  {
  if (vars.count(s))
    return vars[s].value;
  else
    LOGA("Value: Ignore non-existing variable: %s", s.c_str());
  var_union v;
  return v;
  }
*/

bool OpcServer_c::isGood(string s)
{
  bool ret = false;
  if (vars.count(s))
    if (vars[s].ua_status == UA_STATUSCODE_GOOD)
      ret = true;
  return ret;
}

bool OpcServer_c::isVariable(string s)
{
  return vars.count(s);
}

string OpcServer_c::lookupVar(string s)
{
  for (auto [_s, v] : vars)
    if (_s.find(s) != std::string::npos)
      return _s;
  return s;
}

int OpcServer_c::refreshValues()
{
  LOGC("%s: onStart.\n", __func__);
  int i = 0;
  for (auto [_s, v] : vars) {
    getVariantData(_s);
    i++;
  }
  LOGC("%s: onFinish.\n", __func__);
  return i;
}

// eof