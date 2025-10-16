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

namespace OPC
{

static const uint16_t ua_types_arr[] = {
  UA_TYPES_BOOLEAN,
  UA_TYPES_SBYTE,
  UA_TYPES_BYTE,
  UA_TYPES_INT16,
  UA_TYPES_UINT16,
  UA_TYPES_INT32,
  UA_TYPES_UINT32,
  UA_TYPES_INT64,
  UA_TYPES_UINT64,
  UA_TYPES_FLOAT,
  UA_TYPES_DOUBLE,
  UA_TYPES_STRING
};

static size_t nb_types = sizeof(ua_types_arr) / sizeof(*ua_types_arr);

bool OpcClient_c::_connect()
{
  bool ret = true;
  if (scConnect != UA_STATUSCODE_GOOD) {

    if (uaClient != nullptr) {
      UA_Client_disconnect(uaClient);
      UA_Client_delete(uaClient);
      uaClient = nullptr;
      LOGI("OPC_cli::Connect: uaClient deleted.");
    }

    uaClient = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(uaClient));
    scConnect = UA_Client_connect(uaClient, uaUrl);

    if (scConnect != UA_STATUSCODE_GOOD) {
      LOGA("OPC_cli::Connect: failed: %s", UA_StatusCode_name(scConnect));
      UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                  "The connection failed: %s", UA_StatusCode_name(scConnect));
      UA_Client_delete(uaClient);
      uaClient = nullptr;
      ret = false;
    }
  }
  return ret;
}

void OpcClient_c::_variant_init()
{
  uaVariant = UA_Variant_new();
  UA_Variant_clear(uaVariant);
  UA_Variant_init(uaVariant);
}

void OpcClient_c::_variant_clean()
{
  UA_Variant_clear(uaVariant);
  UA_Variant_delete(uaVariant);
}

int OpcClient_c::_variant_get_uatype(UA_Variant* v)
{
  int rc = -1;
  for (size_t i = 0; i < nb_types; i++)
    if (v->type == &UA_TYPES[ua_types_arr[i]]) {
      rc = ua_types_arr[i];
      break;
    }
  return rc;
}

/*
  void OpcClient_c::_nodeid_init(string s)
  {
  uaNodeId = UA_NODEID_STRING(1, const_cast<char*>(s.c_str()));
  }

*/
} // namespace OPC

// eof