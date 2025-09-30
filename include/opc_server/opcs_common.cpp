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
#include "opcs_class.h"

#define DEBUG(a) if(isDebug){a}

using std::set;
using std::to_string;

bool operator<(const numeric_u &v1, const numeric_u &v2) { return v1.ui64 < v2.ui64; }
bool operator>(const numeric_u &v1, const numeric_u &v2) { return v1.ui64 > v2.ui64; }
bool operator!=(const numeric_u &v1, const numeric_u &v2) { return v1.ui64 != v2.ui64; }

OpcServer_c::OpcServer_c(UA_UInt16 _port)
{
  uaPort = _port;
  // Filling the type_map
  ua_types_map[type_index(typeid(int16_t))] = UA_TYPES_INT16;
  ua_types_map[type_index(typeid(uint16_t))] = UA_TYPES_UINT16;
  ua_types_map[type_index(typeid(int32_t))] = UA_TYPES_INT32;
  ua_types_map[type_index(typeid(uint32_t))] = UA_TYPES_UINT32;
  ua_types_map[type_index(typeid(int64_t))] = UA_TYPES_INT64;
  ua_types_map[type_index(typeid(uint64_t))] = UA_TYPES_UINT64;
  ua_types_map[type_index(typeid(float))] = UA_TYPES_FLOAT;
  ua_types_map[type_index(typeid(double))] = UA_TYPES_DOUBLE;
}

OpcServer_c::~OpcServer_c()
{
  if (uaSrvMux != nullptr)
    stop();
  LOGN("uaDestructor: done.");
}

void OpcServer_c::init(UA_UInt16 _port, UA_LogLevel _ll)
{
  uaPort = _port;
  uaLoglevel = _ll;

  uaSrvMux = new mutex;
  uaDataMux = new mutex;

  uaServerConfig = new UA_ServerConfig;
  memset(uaServerConfig, 0, sizeof(UA_ServerConfig)); // LOGI("Memset done");
  UA_ServerConfig_setMinimal(uaServerConfig, uaPort, NULL); // LOGI("SetMin");
  // wait_console(3);

  uaLogger = UA_Log_Stdout_new(uaLoglevel);
  LOGD("uaLogger done new");
  uaLogger->clear = uaServerConfig->logging->clear;
  LOGD("uaLogger->clear done");
  *uaServerConfig->logging = *uaLogger;
  LOGD("uaLogger->logging done");

  init_config(uaServerConfig);
  LOGD("INIT done");

  uaServer = UA_Server_newWithConfig(uaServerConfig);
  uaVariant = UA_Variant_new();
  LOGN("Init: server ready to start on port:%d", uaPort);
}

void OpcServer_c::init_logger()
{
  /* Exchange the logger */
  //uaLogger = UA_Log_Stdout_withLevel(uaLoglevel);
  //  LOGD("uaLogger done");
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
  LOG_BLU("%s: onStart.", __func__);
  int x = 0;
  uaDataMux->lock();
  for (auto [_s, v] : vars) {
    refresh_RawValue(_s);
    x++;
  }
  uaDataMux->unlock();
  LOG_BLU("%s: onFinish.", __func__);
  return x;
}

string OpcServer_c::get_StrVarDetails(ua_var_t &v)
{
  string ret = "= ";

  if (v.ua_type == UA_TYPES_INT16)
    ret += to_string(*(static_cast<int16_t*>(v.ptr_value))) + " (i16";
  else if (v.ua_type == UA_TYPES_INT32)
    ret += to_string(*(static_cast<int32_t*>(v.ptr_value))) + " (i32";
  else if (v.ua_type == UA_TYPES_INT64)
    ret += to_string(*(static_cast<int64_t*>(v.ptr_value))) + " (i64";
  else if (v.ua_type == UA_TYPES_UINT16)
    ret += to_string(*(static_cast<uint16_t*>(v.ptr_value))) + " (ui16";
  else if (v.ua_type == UA_TYPES_UINT32)
    ret += to_string(*(static_cast<uint32_t*>(v.ptr_value))) + " (ui32";
  else if (v.ua_type == UA_TYPES_UINT64)
    ret += to_string(*(static_cast<uint64_t*>(v.ptr_value))) + " (ui64";
  else if (v.ua_type == UA_TYPES_FLOAT)
    ret += to_string(*(static_cast<float*>(v.ptr_value))) + " (float";
  else if (v.ua_type == UA_TYPES_DOUBLE)
    ret += to_string(*(static_cast<double*>(v.ptr_value))) + " (double";
  else if (v.ua_type == UA_TYPES_DATETIME)
    ret = "(DateTime)";

  ret += "," + to_string(v.ua_type) + ")";

  return ret;
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


// eof