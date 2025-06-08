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

  template <typename T> bool ReadNumber(string varname, T &x);
  template <typename T> bool WriteNumber(string varname, T &x);

private:
  bool connect();

  std::map<type_index, int> types;  // UA types coding (index is CPP type_index)
  mutex* muxClient = nullptr;
  UA_Client* uaClient = nullptr;
  UA_StatusCode scConnect = UA_STATUSCODE_BAD;
  UA_StatusCode scRead = UA_STATUSCODE_BAD;
  UA_StatusCode scWrite = UA_STATUSCODE_BAD;
  const char* uaUrl = nullptr;
//  int rc = 0;

};

} // namespace OPC

// ======== Definition of TEMPLATEs =========

#include "opc_templates.h"

// eof
