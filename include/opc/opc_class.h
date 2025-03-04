#pragma once

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <string.h>

#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <typeindex>
#include <typeinfo>

#include "include/logger.h"
#include "opc_datatype.h"

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
  bool isVariable(string s);
  bool isGood(string s);      // OPC var has no errors

  string LookupVar(string s);
  void delVar(string s);

  value_u ReadRawValue(string s);  // returns saved value_union
  bool WriteRawValue(string s, value_u raw_val, bool isOK);  // write value_union

  int RefreshAllValues();  // getVar for ALL variables, returns - qty of vars

  // for init
  template <typename T> int addVar(string s, T Value, int rmode);

  // get T value and set Value_set
  template <typename T> T updateVar(string s, T Value_set, bool isOK);

  // ask OPC server for current value
  template <typename T> T getValue(string s);
  template <typename T> bool getValue(string s, T &x);

  // read value saved on previous getValue
  template <typename T> T readValue(string s);
  template <typename T> bool readValue(string s, T &x);
  //  Definition at the bottom of THIS file

private:
  bool isDebug = true;
  UA_UInt16 uaPort = 4840;
  UA_Server* uaServer = nullptr;
  UA_Variant* uaVariant = nullptr;
  mutex* uaSrvMux = nullptr;
  // mutex* uaGetMux = nullptr;
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
  bool refreshRawValue(string s);  // Reread value_u form server

  template <typename T>
  bool getNumericValue(string s, T &Value);

  template <typename T>
  void setNumericValue(string s, T Value_set, bool isOK = true);

  badvalue_t bad_value;
  map<string, var_t> vars;     // All regs here.
  map<type_index, int> types;  // Types coding is in constructor
};

// ======== Definition of TEMPLATEs =========

#include "opc_templates.h"



// eof
