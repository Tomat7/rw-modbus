/*
  #include <open62541/plugin/log_stdout.h>
  #include <open62541/server.h>
  #include <open62541/server_config_default.h>
*/
//#include "include/open62541/open62541.h"

#include <map>
#include <mutex>
#include <set>
#include <thread>
#include <typeindex>
#include <typeinfo>

//#include "include/console.h"
#include "include/logger.h"
#include "opc_client.h"
//#include "opc_templates.h"

#define DEBUG(a) if(isDebug){a}

#define _NODEID(S) UA_NODEID_STRING(1, const_cast<char*>(S.c_str()))

namespace OPC
{


OpcClient_c::OpcClient_c(const char* url_)
{
//  uaUrl = url_;
  if (url_ != nullptr) {
    uaUrlString = string(url_);
    uaUrl = uaUrlString.c_str();
  }
  ua_types_map[type_index(typeid(int16_t))] = UA_TYPES_INT16;
  ua_types_map[type_index(typeid(uint16_t))] = UA_TYPES_UINT16;
  ua_types_map[type_index(typeid(int32_t))] = UA_TYPES_INT32;
  ua_types_map[type_index(typeid(uint32_t))] = UA_TYPES_UINT32;
  ua_types_map[type_index(typeid(int64_t))] = UA_TYPES_INT64;
  ua_types_map[type_index(typeid(uint64_t))] = UA_TYPES_UINT64;
  ua_types_map[type_index(typeid(float))] = UA_TYPES_FLOAT;
  ua_types_map[type_index(typeid(double))] = UA_TYPES_DOUBLE;
}


OpcClient_c::~OpcClient_c()
{
  if (uaClient != nullptr)
    stop();
  LOGN("uaClient: finished.");
}

void OpcClient_c::init(const char* url_)
{
//  uaUrl = url_;
  if (url_ != nullptr) {
    uaUrlString = string(url_);
    uaUrl = uaUrlString.c_str();
  }
  init();
}

void OpcClient_c::init()
{
  if ( uaUrl == nullptr) {
    LOGA("UA_Client: URL not configured!");
    return;
  }

  muxClient = new mutex;
  uaClient = UA_Client_new();
  UA_ClientConfig_setDefault(UA_Client_getConfig(uaClient));
  LOGI("UA_Client: INIT done");

}

void OpcClient_c::stop()
{
  if (muxClient != nullptr)
    muxClient->lock();

  if (uaClient != nullptr) {
    UA_Client_disconnect(uaClient);
    UA_Client_delete(uaClient);
    uaClient = nullptr;
  }
  LOGN("Stop: uaClient deleted.");

  if (muxClient != nullptr)
    muxClient->unlock();
  delete muxClient;
  muxClient = nullptr;
  LOGN("Stop: muxClient free.");
}


int OpcClient_c::get_uatype(string varname)
{
  muxClient->lock();
  int rc = -2;

  if (_connect()) {
    _variant_init();
    scRead = UA_Client_readValueAttribute(uaClient, _NODEID(varname), uaVariant);

    if (scRead == UA_STATUSCODE_GOOD)
      rc = _variant_get_uatype(uaVariant);

    _variant_clean();
  }

  muxClient->unlock();
  return rc;
}

} // namespace OPC

// eof