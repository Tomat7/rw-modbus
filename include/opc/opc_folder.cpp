
#include "opc_class.h"

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <thread>
#include <mutex>
#include <string>

#include "include/logger.h"

#define DEBUG(a) if (isDebug) {a}


void OpcServer_c::addVar_NodeId(var_t &v)
{
  v.node_id.var = UA_NODEID_STRING(1, v.ua_name);

  if (v.str_folder == "/") {
    v.node_id.parent = UA_NS0ID(OBJECTSFOLDER);
    v.node_id.reference = UA_NS0ID(ORGANIZES);
  } else {
    v.node_id.parent = getFolder_NodeId(v.folder);
    v.node_id.reference = UA_NS0ID(HASCOMPONENT);
  }
}

UA_NodeId OpcServer_c::getFolder_NodeId(char* fname)
{
  string full_name = string(fname);
  if (vars.count(full_name)) {
    LOGD("Folder: %s already exist.", fname);
    return vars[full_name].node_id.var;
  }
  return addFolders(full_name, UA_NS0ID(OBJECTSFOLDER));
}

UA_NodeId OpcServer_c::addFolders(string full_name, UA_NodeId parentNodeId)
{

  char* ua_name = const_cast<char*>(full_name.c_str());
  string str_displayName = full_name;
  str_displayName.erase(str_displayName.length() - 1);
  auto last_slash = str_displayName.rfind("/");
  str_displayName = str_displayName.substr(last_slash + 1);
  char* display_name = const_cast<char*>(str_displayName.c_str());
  LOGA("folder display name: %s", display_name);

  UA_NodeId folderId = UA_NODEID_STRING(1, ua_name); /* get the nodeid assigned by the server */
  UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
  oAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", ua_name);
  UA_Server_addObjectNode(uaServer, folderId, parentNodeId,
                          UA_NS0ID(ORGANIZES), UA_QUALIFIEDNAME(1, ua_name),
                          UA_NS0ID(BASEOBJECTTYPE),
                          oAttr, NULL, NULL);

  var_t v;
  v.fullname = full_name;
  v.node_id.var = folderId;
  vars[full_name] = v;

  DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                    "Created folder: %s ", ua_name);)

  return folderId;
}

