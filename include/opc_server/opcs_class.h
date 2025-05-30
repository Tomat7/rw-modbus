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
#include "opcs_datatype.h"

#ifndef MANUFACTURER_NAME
#define MANUFACTURER_NAME "tomat7@vm32.ru"
#define PRODUCT_NAME "Modbus-OPC UA gate"
#define PRODUCT_URI "https://github.com/Tomat7/rw-modbus"
#define APPLICATION_NAME "open62541-based Modbus-OPC UA gate"
//#define APPLICATION_URI "urn:unconfigured:application"
#define APPLICATION_URI_SERVER "opc.tcp://mb-opc.vm32.ru"
//#define APPLICATION_URI_SERVER "urn:open62541.server.application"
#endif

//using namespace std;
using std::string;
using std::mutex;
using std::map;
using std::type_index;

class OpcServer_c
{
public:
  OpcServer_c(UA_UInt16 _port = 4840);
  ~OpcServer_c();

  void init(UA_UInt16 _port = 0);  // Necessary init() before run()
  void run();
  void stop();

  bool isVariable(string s);  // Variable exist
  bool isFolder(string s);    // Folder (container) exist
  bool isGood(string s);      // OPC var has no errors

  string GetVarFullName(string s);
  void DeleteVar(string s);

  value_u ReadRawUnion(string s);  // returns saved value_union
  bool WriteRawUnion(string s, value_u raw_val, bool isOK);  // write value_u

  int RefreshAllValues();  // getVar for ALL variables, returns - qty of vars

  // for init
  template <typename T>
  int AddVar(string s, T Number, int rmode);

  // return T value and set Numeric_set
  template <typename T>
  T UpdateVar(string s, T Numeric_set, bool &isOK);

  template <typename T>
  bool ReadNumber(string s, T &x);

  template <typename T>
  bool WriteNumber(string s, T &x, bool isOK);

  /*
    int getType(string s);
    int getStatus(string s);    // 0 - is OK, any other (1 or -1) is BAD
  */
  /*
    // ask OPC server for current value (refresh)
    template <typename T>
    T GetNumber(string s);
  */
  /*
    // read value saved on previous/last correct getValue
    template <typename T>
    T ReadValue(string s);
  */
  /*
    template <typename T>
    bool ReadValue(string s, T &x);
    //  Definition at the bottom of THIS file
  */

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

  UA_NodeId get_FolderNodeId(string str_path);
  UA_NodeId add_FolderToParent(string full_name, UA_NodeId parentNodeId);
  int count_Slash(string Path);
  string get_PathName(string &n);
  string get_StrVarDetails(var_t &var);
  string get_PathByLevel(string Path, int level);

  int add_VarName(string raw_name, int rtype, int rmode);
  void add_VarNodeId(var_t &v);
  void add_Variable(var_t &var);

  bool write_Variable(var_t &var, bool isOk);
  void* get_VariantDataPtr(string s);  // get pointer to UA_Variant.Data
  bool refresh_RawValue(string s);     // Reread value_u form server

  template <typename T>
  bool get_NumericValue(string s, T &Value);

  template <typename T>
  bool set_NumericValue(string s, T Numeric_set, bool isOK);

  badvalue_t bad_value;
  map<string, var_t> vars;     // All regs here.
  map<type_index, int> types;  // UA types coding (index is CPP type_index)
};

// ======== Definition of TEMPLATEs =========

#include "opcs_templates.h"

// eof
