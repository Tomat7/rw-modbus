#pragma once
// opc_class.h ----------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

/*
  #include <open62541/plugin/log_stdout.h>
  #include <open62541/server.h>
  #include <open62541/server_config_default.h>
*/

#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/plugin/log_stdout.h>
//#include "include/open62541/open62541.h"

#include <string.h>

//#include <iostream>
#include <mutex>
#include <map>
#include <string>
#include <typeindex>
#include <typeinfo>

#include "include/logger.h"
#include "libs/number/number_class.h"

#ifndef MANUFACTURER_NAME
#define MANUFACTURER_NAME "tomat7@vm32.ru"
#define PRODUCT_NAME "Modbus-OPC UA gate"
#define PRODUCT_URI "https://github.com/Tomat7/rw-modbus"
#define APPLICATION_NAME "open62541-based Modbus-OPC UA gate"
//#define APPLICATION_URI "urn:unconfigured:application"
#define APPLICATION_URI_SERVER "opc.tcp://mb-opc.vm32.ru"
//#define APPLICATION_URI_SERVER "urn:open62541.server.application"
#endif

using std::string;
using std::mutex;
using std::type_index;

namespace OPC
{

class OpcClient_c
{
public:
  OpcClient_c(const char* url = nullptr);
  ~OpcClient_c();

  void init(const char* url);  // Necessary init() before run()
  void init();
  void stop();

  int get_uatype(string varname);

  template <typename T> bool Read(string varname, T &x);
  template <typename T> bool Write(string varname, T &x);

  bool ReadNumber(string varname, Number_c &Numx);
  Number_c ReadNumber(string varname);
//  bool WriteNumber(string varname, Number_c &x);

private:
  bool _connect();
  void _variant_init();
  void _variant_clean();
  //void _nodeid_init(string s);
  int _variant_get_uatype(UA_Variant* v);

  std::map<type_index, int> ua_types_map;  // UA types coding (index is CPP type_index)
  mutex* muxClient = nullptr;
  UA_Client* uaClient = nullptr;
  UA_Variant* uaVariant = nullptr;
  UA_NodeId uaNodeId;

  UA_StatusCode scConnect = UA_STATUSCODE_BAD;
  UA_StatusCode scWrite = UA_STATUSCODE_BAD;
  UA_StatusCode scRead = UA_STATUSCODE_BAD;
  const char* uaUrl = nullptr;
  string uaUrlString;
//  int rc = 0;
};

} // namespace OPC

// ======== Definition of TEMPLATEs =========

#include "opc_templates.h"

// eof
