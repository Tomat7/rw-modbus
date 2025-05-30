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

#include "include/console.h"
#include "include/logger.h"
#include "opc_client.h"

#define DEBUG(a) if(isDebug){a}

/*
  bool operator<(const value_u &v1, const value_u &v2) { return v1.ui64 < v2.ui64; }
  bool operator>(const value_u &v1, const value_u &v2) { return v1.ui64 > v2.ui64; }
  bool operator!=(const value_u &v1, const value_u &v2) { return v1.ui64 != v2.ui64; }
*/

OpcClient_c::OpcClient_c(const char* url_)
{
  uaUrl = url_;
  types[type_index(typeid(int16_t))] = UA_TYPES_INT16;
  types[type_index(typeid(uint16_t))] = UA_TYPES_UINT16;
  types[type_index(typeid(int32_t))] = UA_TYPES_INT32;
  types[type_index(typeid(uint32_t))] = UA_TYPES_UINT32;
  types[type_index(typeid(int64_t))] = UA_TYPES_INT64;
  types[type_index(typeid(uint64_t))] = UA_TYPES_UINT64;
  types[type_index(typeid(float))] = UA_TYPES_FLOAT;
  types[type_index(typeid(double))] = UA_TYPES_DOUBLE;
}


OpcClient_c::~OpcClient_c()
{
  if (uaClient != nullptr)
    stop();
  LOGN("uaClient: finished.");
}

void OpcClient_c::init(const char* url_)
{
  uaUrl = url_;
  init();
}

void OpcClient_c::init()
{
  if ( uaUrl == nullptr) {
    LOGA("UA_Client URL not configured!");
    return;
  }

  muxClient = new mutex;
  uaClient = UA_Client_new();
  UA_ClientConfig_setDefault(UA_Client_getConfig(uaClient));
  LOGI("INIT done");

}

bool OpcClient_c::connect()
{
  bool ret = true;
  if (scConnect != UA_STATUSCODE_GOOD) {

    if (uaClient != nullptr) {
      UA_Client_disconnect(uaClient);
      UA_Client_delete(uaClient);
      uaClient = nullptr;
      LOGA("Connect: uaClient deleted.");
    }

    uaClient = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(uaClient));
    scConnect = UA_Client_connect(uaClient, uaUrl);

    if (scConnect != UA_STATUSCODE_GOOD) {
      UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                  "The connection failed: %s", UA_StatusCode_name(scConnect));
      UA_Client_delete(uaClient);
      uaClient = nullptr;
      ret = false;
    }
  }
  return ret;
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

// eof