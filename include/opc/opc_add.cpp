
#include "opc_class.h"

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <thread>
#include <mutex>

#include "include/logger.h"

#define DEBUG(a) if (isDebug) {a}

void OpcServer_c::addVar_NodeId(var_t &v)
{
  v.node_id.var = UA_NODEID_STRING(1, v.name);

  if (v.folder != nullptr) {
    v.node_id.parent = addFolder(v.folder);
    v.node_id.reference = UA_NS0ID(HASCOMPONENT);
  } else {
    v.node_id.parent = UA_NS0ID(OBJECTSFOLDER);
    v.node_id.reference = UA_NS0ID(ORGANIZES);
  }
}


void OpcServer_c::addVar_Names(string str_name, int t, int m, string str_folder)
{
  var_t v;
  v.type = t;
  v.rmode = m;

  v.fullname = str_name;
  v.foldername = str_folder;

  vars[str_name] = v;
  vars[str_name].name = const_cast<char*>(vars[str_name].fullname.c_str());

  if (str_folder != "")
    vars[str_name].folder = const_cast<char*>(vars[str_name].foldername.c_str());
  else
    vars[str_name].folder = nullptr;
}

void OpcServer_c::addVar(string s, int16_t i16, int rmode, string folder)
{
  addVar_Names(s, UA_TYPES_INT16, rmode, folder);
  addVar_NodeId(vars[s]);
  vars[s].value.i16 = i16;
  vars[s].ptr_value = &vars[s].value.i16;
  addVariable(vars[s]);
}

/*
  void OpcServer_c::addVar(string s, int32_t i32, int rmode)
  {
  addVar_Names(s, UA_TYPES_INT32, i32, rmode);
  }
*/

void OpcServer_c::addVar(string s, int64_t i64, int rmode, string folder)
{
  addVar_Names(s, UA_TYPES_INT64, rmode, folder);
  addVar_NodeId(vars[s]);
  vars[s].value.i64 = i64;
  vars[s].ptr_value = &vars[s].value.i64;
  addVariable(vars[s]);
}


void OpcServer_c::addVar(string s, uint16_t ui16, int rmode, string folder)
{
  addVar_Names(s, UA_TYPES_UINT16, rmode, folder);
  addVar_NodeId(vars[s]);
  vars[s].value.ui16 = ui16;
  vars[s].ptr_value = &vars[s].value.ui16;
  addVariable(vars[s]);
}

/*
  void OpcServer_c::addVar(string s, uint32_t ui32, int _rmode)
  {
  var_t v;
  v.type = UA_TYPES_UINT32;
  v.value.ui32 = ui32;
  v.ptr_value = &v.value.ui32;
  addVar_Names(s, v, _rmode);
  }
*/

/*
  void OpcServer_c::addVar(string s, uint64_t ui64, int _rmode)
  {
  addVar_Names(s, UA_TYPES_INT32, rmode);
  }
*/

void OpcServer_c::addVar(string s, float fl, int rmode, string folder)
{
  addVar_Names(s, UA_TYPES_FLOAT, rmode, folder);
  addVar_NodeId(vars[s]);
  vars[s].value.fl = fl;
  vars[s].ptr_value = &vars[s].value.fl;
  addVariable(vars[s]);
}
