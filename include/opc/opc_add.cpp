
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
  v.node_id.var = UA_NODEID_STRING(1, v.ua_name);

  if (v.folder != nullptr) {
    v.node_id.parent = getFolder_NodeId(v.folder);
    v.node_id.reference = UA_NS0ID(HASCOMPONENT);
  } else {
    v.node_id.parent = UA_NS0ID(OBJECTSFOLDER);
    v.node_id.reference = UA_NS0ID(ORGANIZES);
  }
}

UA_NodeId OpcServer_c::getFolder_NodeId(char* fname)
{
  string full_name = string(fname);
  if (vars.count(full_name)) {
    LOGD("Folder: %s already exist.", fname);
    return vars[full_name].node_id.var;
  }

  UA_NodeId folderId = UA_NODEID_STRING(1, fname); /* get the nodeid assigned by the server */
  UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
  oAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", fname);
  UA_Server_addObjectNode(uaServer, folderId, UA_NS0ID(OBJECTSFOLDER),
                          UA_NS0ID(ORGANIZES), UA_QUALIFIEDNAME(1, fname),
                          UA_NS0ID(BASEOBJECTTYPE),
                          oAttr, NULL, NULL);

  var_t v;
  v.fullname = full_name;
  v.node_id.var = folderId;
  vars[full_name] = v;

  DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                    "Created folder: %s ", fname);)

  return folderId;
}



int OpcServer_c::addVar(string s, int16_t i16, int rmode)
{
  rc = addVar_Names(s, UA_TYPES_INT16, rmode);
  if (rc == 0)
    return 0;

  addVar_NodeId(vars[s]);
  //vars[s].value.i16 = i16;
  //vars[s].ptr_value = &vars[s].value.i16;
  vars[s].ptr_value = &i16;
  addVariable(vars[s]);
  return 1;
}

int OpcServer_c::addVar(string s, uint16_t ui16, int rmode)
{
  rc = addVar_Names(s, UA_TYPES_UINT16, rmode);
  if (rc == 0)
    return 0;

  addVar_NodeId(vars[s]);
  //vars[s].value.ui16 = ui16;
  //vars[s].ptr_value = &vars[s].value.ui16;
  vars[s].ptr_value = &ui16;
  addVariable(vars[s]);
  return 1;
}

int OpcServer_c::addVar(string s, int64_t i64, int rmode)
{
  addVar_Names(s, UA_TYPES_INT64, rmode);
  if (rc == 0)
    return 0;

  addVar_NodeId(vars[s]);
  //vars[s].value.i64 = i64;
  vars[s].ptr_value = &i64;
  addVariable(vars[s]);
  return 1;
}


int OpcServer_c::addVar(string s, float fl, int rmode)
{
  rc = addVar_Names(s, UA_TYPES_FLOAT, rmode);
  if (rc == 0)
    return 0;

  addVar_NodeId(vars[s]);
  //vars[s].value.fl = fl;
  vars[s].ptr_value = &fl;
  addVariable(vars[s]);
  return 1;
}
