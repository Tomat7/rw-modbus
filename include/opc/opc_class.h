#pragma once
// opc_class.h ----------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

//#include "include/open62541/open62541.h"
#include <string.h>

//#include <iostream>

#include <mutex>
#include <map>
#include <string>
#include <typeindex>
#include <typeinfo>

#include "include/logger.h"
#include "opc_datatype.h"

#ifndef MANUFACTURER_NAME
#define MANUFACTURER_NAME "tomat7@vm32.ru"
#define PRODUCT_NAME "Modbus-OPC UA gate"
#define PRODUCT_URI "https://github.com/Tomat7/rw-modbus"
#define APPLICATION_NAME "open62541-based Modbus-OPC UA gate"
//#define APPLICATION_URI "urn:unconfigured:application"
#define APPLICATION_URI_SERVER "opc.tcp://mb-opc.vm32.ru"
//#define APPLICATION_URI_SERVER "urn:open62541.server.application"
#endif

using namespace std;

class OpcServer_c
{
public:
  OpcServer_c(UA_UInt16 _port = 4840);
  ~OpcServer_c();

  void init(UA_UInt16 _port = 0);  // Necessary init() before run()
  void run();
  void stop();
  /*
    int getType(string s);
    int getStatus(string s);    // 0 - is OK, any other (1 or -1) is BAD
  */
  bool isVariable(string s);  // Variable exist
  bool isGood(string s);      // OPC var has no errors

  string getVarFullName(string s);
  void delVar(string s);

  value_u ReadRawValue(string s);  // returns saved value_union
  bool WriteRawUnion(string s, value_u raw_val, bool isOK);  // write value_u

  int RefreshAllValues();  // getVar for ALL variables, returns - qty of vars

  // for init
  template <typename T>
  int addVar(string s, T Number, int rmode);

  // return T value and set Numeric_set
  template <typename T>
  T updateVar(string s, T Numeric_set, bool isOK);

  // ask OPC server for current value (refresh)
  template <typename T>
  T getNumber(string s);

  template <typename T>
  bool ReadNumber(string s, T &x);

  // read value saved on previous/last correct getValue
  template <typename T>
  T readValue(string s);

  template <typename T>
  bool readValue(string s, T &x);
  //  Definition at the bottom of THIS file

private:
  void init_config(UA_ServerConfig* conf);
  bool isDebug = true;
  UA_UInt16 uaPort = 4840;
  UA_Server* uaServer = nullptr;
  UA_Variant* uaVariant = nullptr;
  mutex* uaSrvMux = nullptr;  // mutex* uaGetMux = nullptr;
  mutex* uaDataMux = nullptr;
  volatile UA_Boolean uaRunning = true;
  int rc = 0;

  UA_NodeId getFolder_NodeId(string str_path);
  UA_NodeId addFolders(string full_name, UA_NodeId parentNodeId);
  int countSlash(string Path);
  string getPath_Name(string &n);
  string strVarDetails(var_t &var);
  string getPathByLevel(string Path, int level);

  int addVar_Names(string raw_name, int t, int m);
  void addVar_NodeId(var_t &v);
  void addVariable(var_t &var);

  void writeVariable(var_t &var, bool isOk);
  void* getVariantDataPtr(string s);  // get pointer to UA_Variant.Data
  bool refreshRawValue(string s);     // Reread value_u form server

  template <typename T>
  bool getNumericValue(string s, T &Value);

  template <typename T>
  void WriteNumericValue(string s, T Numeric_set, bool isOK = true);

  badvalue_t bad_value;
  map<string, var_t> vars;     // All regs here.
  map<type_index, int> types;  // Types coding is in constructor
};

// ======== Definition of TEMPLATEs =========

#include "opc_templates.h"

// eof
