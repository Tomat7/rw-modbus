
#include "opc_class.h"

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <thread>
#include <mutex>
#include <string>
#include <typeinfo>

#include "include/logger.h"

#define DEBUG(a) if (isDebug) {a}


void OpcServer_c::addVar_NodeId(var_t &v)
{
  printf("\naddVarNodeId0: %s, %s... ", v.str_path.c_str(), v.ua_name);

  v.node_id.var = UA_NODEID_STRING(1, v.ua_name);
//  printf("\naddVarNodeId1: %s\n", v.str_path.c_str());

  if (v.str_path == "/") {
    printf("2: %s\n", v.str_path.c_str());
    v.node_id.parent = UA_NS0ID(OBJECTSFOLDER);
    v.node_id.reference = UA_NS0ID(ORGANIZES);
  } else {
    printf("3: %s\n", v.str_path.c_str());
    v.node_id.parent = getFolder_NodeId(v.str_path);
    v.node_id.reference = UA_NS0ID(HASCOMPONENT);
  }
}


UA_NodeId OpcServer_c::getFolder_NodeId(string str_path)
{
  //string str_path = string(path);
  if (vars.count(str_path)) {
    LOGD("Folder: %s already exist. / = %u",
         str_path.c_str(), countSlash(str_path));
    return vars[str_path].node_id.var;
  }

  int folder_levels = countSlash(str_path) - 1;
  string Path = str_path;

  for (int i = 0; i < folder_levels; i++) {
    if (vars.count(Path)) {
      LOGD("Folder: %s already exist. / = %u",
           Path.c_str(), countSlash(Path));
      return vars[Path].node_id.var;
    }

    Path.erase(Path.length() - 1);      // remove last "/"
    auto last_slash = Path.rfind("/");
    Path.erase(last_slash + 1);

    LOGD("Folder: %s ready to create.", Path.c_str());

  }

  return addFolders(str_path, UA_NS0ID(OBJECTSFOLDER) /* top-level Folder ID */);
}


UA_NodeId OpcServer_c::addFolders(string str_path, UA_NodeId parentNodeId)
{

  char* folder_path = const_cast<char*>(str_path.c_str());
  string str_displayName = str_path;
  str_displayName.erase(str_displayName.length() - 1);      // remove last "/"
  auto last_slash = str_displayName.rfind("/");             // find name of
  str_displayName = str_displayName.substr(last_slash + 1); // "destination"
  char* display_name = const_cast<char*>(str_displayName.c_str());
  LOGA("Folder display name: %s", display_name);

  UA_NodeId folderId = UA_NODEID_STRING(1, folder_path); /* get the nodeid assigned by the server */
  UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
  oAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", display_name);
  UA_StatusCode sc = UA_Server_addObjectNode(uaServer, folderId, parentNodeId,
                     UA_NS0ID(ORGANIZES), UA_QUALIFIEDNAME(1, /* display_name */ folder_path),
                     UA_NS0ID(BASEOBJECTTYPE),
                     oAttr, NULL, &folderId);

  var_t v;
  v.str_full = str_path;
  //v.str_name = folderId.identifier.string;
  v.node_id.var = folderId;
  vars[str_path] = v;

  DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                    "Created folder: %s, name: %s - %s", folder_path, display_name, UA_StatusCode_name(sc));)

  return folderId;
}


int OpcServer_c::countSlash(string Path)
{
  size_t index = 0;
  int count = 0;
  string Slash = "/";

  while ((index = Path.find(Slash, index)) != std::string::npos) {
    ++count;
    index += Slash.length(); // перемещаем индекс на позицию после завершения слова в тексте
  }
  return count;
}

// eof