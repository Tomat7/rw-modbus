#pragma once

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>

#include <string.h>

#include <iostream>
#include <map>
#include <string>
#include <mutex>
#include <typeinfo>
#include <typeindex>

#include "include/logger.h"

using namespace std;

struct nodeid_t {
  UA_NodeId var;
  UA_NodeId parent;
  UA_NodeId reference;
};

struct var_t {
  string raw_name;
  char* ua_name = nullptr;
  string str_name;
  char* name = nullptr;
  string str_path;
  char* path  = nullptr;
  string str_full;
  char* path_name = nullptr;
  nodeid_t node_id;
  void* ptr_value;    // ptr to "correct" value_u
  int rmode;          // 1 - mean RW
  int type_id;
  int type;
  UA_StatusCode ua_status;
};

class OpcServer_c
{
public:
  OpcServer_c(UA_UInt16 _port = 4840);
  ~OpcServer_c();

  void init(UA_UInt16 _port = 0); // Necessary init() before run()
  void run();
  void stop();

  void delVar(string s);
  template<typename T> int addVar(string s, T Value, int rmode);
  template<typename T> void setVar(string s, T Value, bool isOK);
  template<typename T> T getVar(string s, T &Value);
// Definition at the bottom of THIS file

private:
  bool isDebug = true;
  UA_UInt16 uaPort = 4840;
  UA_Server* uaServer = nullptr;
  mutex* uaMutex = nullptr;
  volatile UA_Boolean uaRunning = true;
  int rc = 0;

  UA_NodeId getFolder_NodeId(string str_path);
  UA_NodeId addFolders(string full_name, UA_NodeId parentNodeId);
  string getPath_Name(string &n);

  int addVar_Names(string raw_name, int t, int m);
  void addVar_NodeId(var_t &v);

  void addVariable(var_t &var);
  void setVariable(var_t &var);
  void writeVariable(var_t &var, bool isOK);
  void getVariable(var_t &var, UA_Variant* vrnt);


  int countSlash(string Path);
  string strVarDetails(var_t &var);
  string getPathByLevel(string Path, int level);

  map<string, var_t> vars;  // All regs here.
  map<type_index, int> types; // Types coding is in constructor

};

// Definition of TEMPLATEs

template<typename T>
int OpcServer_c::addVar(std::string s, T Value, int rmode)
{
  rc = addVar_Names(s, types[type_index(typeid(Value))], rmode);
  if (rc == 0)
    return 0;

  addVar_NodeId(vars[s]);
  vars[s].ptr_value = &Value;
  addVariable(vars[s]);
  return 1;
}

template<typename T>
void OpcServer_c::setVar(std::string s, T Value, bool isOK)
{
  if (vars.count(s)) {
    vars[s].ptr_value = &Value;
    //setVariable(vars[s]);
    writeVariable(vars[s], isOK);
  } else
    LOGA("Ignore non-existing variable: %s", s.c_str());
}

template<typename T>
T OpcServer_c::getVar(std::string s, T &Value)
{
  if (vars.count(s)) {
    UA_Variant Vrnt;
    getVariable(vars[s], &Vrnt);
    if (Vrnt.data != nullptr)
      Value = *(static_cast<T*>(Vrnt.data));
  } else
    LOGA("Ignore non-existing variable: %s", s.c_str());
  return Value;
}

// eof
