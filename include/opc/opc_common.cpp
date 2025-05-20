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
#include "opc_class.h"

#define DEBUG(a) if(isDebug){a}

bool operator<(const value_u &v1, const value_u &v2) { return v1.ui64 < v2.ui64; }
bool operator>(const value_u &v1, const value_u &v2) { return v1.ui64 > v2.ui64; }
bool operator!=(const value_u &v1, const value_u &v2) { return v1.ui64 != v2.ui64; }

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
  types[type_index(typeid(double))] = UA_TYPES_DOUBLE;
}

OpcServer_c::~OpcServer_c()
{
  if (uaSrvMux != nullptr)
    stop();
  LOGN("uaDestructor: done.");
}

void OpcServer_c::init(UA_UInt16 _port)
{
  if (_port != 0)
    uaPort = _port;

  uaSrvMux = new mutex;
  uaDataMux = new mutex;
  // uaGetMux = new mutex;
  //uaServer = UA_Server_new();
  UA_ServerConfig uaServerConfig;
  //  = UA_Server_getConfig(uaServer);
  memset(&uaServerConfig, 0, sizeof(UA_ServerConfig));
  //UA_ServerConfig_setDefault(uaServerConfig);
  LOGI("Memset done");
  UA_ServerConfig_setMinimal(&uaServerConfig, uaPort, NULL);
  //UA_ServerConfig_setBasics_withPort(uaServerConfig, uaPort);
  LOGI("SetMinimal done");
  wait_console(3);
  init_config(&uaServerConfig);
  LOGI("INIT done");

  uaServer = UA_Server_newWithConfig(&uaServerConfig);
  uaVariant = UA_Variant_new();

  LOGN("Init: server ready to start on port:%d", uaPort);
}

void OpcServer_c::init_config(UA_ServerConfig* conf)
{
  auto &Info = conf->buildInfo;
  Info.productUri = UA_STRING_ALLOC(PRODUCT_URI);
  Info.manufacturerName = UA_STRING_ALLOC(MANUFACTURER_NAME);
  Info.productName = UA_STRING_ALLOC(PRODUCT_NAME);

  auto &Desc = conf->applicationDescription;
  Desc.applicationUri = UA_STRING_ALLOC(APPLICATION_URI_SERVER);
  Desc.productUri = UA_STRING_ALLOC(PRODUCT_URI);
  Desc.applicationName = UA_LOCALIZEDTEXT_ALLOC("en", APPLICATION_NAME);
}

void OpcServer_c::run()
{
  LOGN("Run: try to lock mutex.");
  uaSrvMux->lock();
  uaRunning = true;
  UA_Server_run(uaServer, &uaRunning);
  LOGN("Run: got &uaRunning = false and stopped now.");
  uaSrvMux->unlock();
}

void OpcServer_c::stop()
{
  LOGN("Stop: set &uaRunning = false.");
  uaRunning = false;
  /*   if (uaGetMux != nullptr)
      uaGetMux->lock(); */
  if (uaDataMux != nullptr)
    uaDataMux->lock();
  if (uaSrvMux != nullptr)
    uaSrvMux->lock();

  set<string> vs;
  for (auto &[s, v] : vars)
    vs.insert(s);
  for (auto &s : vs)
    DeleteVar(s);

  vars.clear();
  LOGN("Stop: map cleared.");

  if (uaVariant != nullptr) {
    UA_Variant_clear(uaVariant);
    UA_Variant_delete(uaVariant);
  }

  if (uaServer != nullptr)
    UA_Server_delete(uaServer);

  uaServer = nullptr;
  uaVariant = nullptr;
  LOGN("Stop: server deleted.");

  /* if (uaGetMux != nullptr)
    uaGetMux->unlock(); */
  if (uaDataMux != nullptr)
    uaDataMux->unlock();
  if (uaSrvMux != nullptr)
    uaSrvMux->unlock();

  // delete uaGetMux;
  delete uaDataMux;
  delete uaSrvMux;
  // uaGetMux = nullptr;
  uaDataMux = nullptr;
  uaSrvMux = nullptr;
  LOGN("Stop: MUX free.");
}

void OpcServer_c::DeleteVar(string s)
{
  UA_Server_deleteNode(uaServer, vars[s].node_id.var, true);
  vars.erase(s);
  LOGI("Deleted Var/Node: %s", s.c_str());
}

/*
  int OpcServer_c::getType(string s)
  {
  if (vars.count(s))
    return vars[s].type;
  else
    LOGW("Type: Ignore non-existing variable: %s", s.c_str());
  return -1;
  }
*/
/*
  int OpcServer_c::getStatus(string s)
  {
  if (vars.count(s))
    return vars[s].ua_status;  // 0 - is OK, any other (1 or -1) is BAD
  else
    LOGW("Status: Ignore non-existing variable: %s", s.c_str());
  return -1;
  }
*/

bool OpcServer_c::isGood(string s)
{
  bool is_good = false;
  if (isVariable(s))
    is_good = (vars[s].ua_status == UA_STATUSCODE_GOOD);
  return is_good;
}

bool OpcServer_c::isVariable(string s)
{
  if (vars.count(s))
    return vars[s].is_var;
  else
    return false;
}

bool OpcServer_c::isFolder(string s)
{
  if (vars.count(s))
    return !vars[s].is_var;
  else
    return false;
}

string OpcServer_c::GetVarFullName(string s)
{
  for (auto [ss, v] : vars) {
    if (ss == s)
      break;
    if (ss.find(s) != std::string::npos) {
      auto last_slash = ss.rfind("/");  // Find last "/"
      if (last_slash != std::string::npos) {
        string _s = ss.substr(last_slash + 1);  // Get str after "/"
        if (_s == s)                            //  - BUF.Press"
          return ss;
      }
    }
  }
  return s;
}

int OpcServer_c::RefreshAllValues()
{
  LOGX("%s: onStart.", __func__);
  int x = 0;
  uaDataMux->lock();
  for (auto [_s, v] : vars) {
    refresh_RawValue(_s);
    x++;
  }
  uaDataMux->unlock();
  LOGX("%s: onFinish.", __func__);
  return x;
}

string OpcServer_c::get_StrVarDetails(var_t &v)
{
  string ret;
  if (v.type == UA_TYPES_INT16)
    ret = "(i16) = " + to_string(*(static_cast<int16_t*>(v.ptr_value)));
  else if (v.type == UA_TYPES_INT32)
    ret = "(i32) = " + to_string(*(static_cast<int32_t*>(v.ptr_value)));
  else if (v.type == UA_TYPES_INT64)
    ret = "(i64) = " + to_string(*(static_cast<int64_t*>(v.ptr_value)));
  else if (v.type == UA_TYPES_UINT16)
    ret = "(ui16) = " + to_string(*(static_cast<uint16_t*>(v.ptr_value)));
  else if (v.type == UA_TYPES_UINT32)
    ret = "(ui32) = " + to_string(*(static_cast<uint32_t*>(v.ptr_value)));
  else if (v.type == UA_TYPES_UINT64)
    ret = "(ui64) = " + to_string(*(static_cast<uint64_t*>(v.ptr_value)));
  else if (v.type == UA_TYPES_FLOAT)
    ret = "(float) = " + to_string(*(static_cast<float*>(v.ptr_value)));
  else if (v.type == UA_TYPES_DOUBLE)
    ret = "(double) = " + to_string(*(static_cast<double*>(v.ptr_value)));
  else if (v.type == UA_TYPES_DATETIME)
    ret = "(DateTime)";

  return ret;
}

// eof