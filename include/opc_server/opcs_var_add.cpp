/*
  #include <open62541/plugin/log_stdout.h>
  #include <open62541/server.h>
  #include <open62541/server_config_default.h>
*/
//#include "include/open62541/open62541.h"

#include <mutex>
#include <string>
#include <thread>
#include <typeinfo>

#include "include/logger.h"
#include "opcs_class.h"

#define DEBUG(a) if(isDebug){a}

#define CHAR_PTR const_cast<char *>
#define CHAR_P const_cast<char *>


int OpcServer_c::add_VarName(string raw_name, int t, int m)
{
  if (vars.count(raw_name)) {
    LOGA("Add: Ignore existing variable: %s", raw_name.c_str());
    return 0;
  }

  if (raw_name.find("//") != std::string::npos) {
    LOGA("Add: Ignore wrong name/path: %s", raw_name.c_str());
    return 0;
  }

  string str_name = raw_name;                // /PLC/Kub/Kub.Temp1
  string str_path = get_PathName(str_name);  // /PLC/Kub/, str_name=Kub.Temp1

  var_t v;
  v.is_var = true;
  v.ua_type = t;
  v.rmode = m;
  v.key_name = raw_name;  // KEY for map and OPC FQName - /PLC/Kub/Kub.Temp1
  v.var_name = str_name;  // Kub.Temp1
  v.var_path = str_path;  // /PLC/Kub/
//  v.str_pathname = str_path + str_name;  // /PLC/Kub/Kub.Temp1

  vars[v.key_name] = v;
  vars[v.key_name].ua_keyname = CHAR_PTR(vars[v.key_name].key_name.c_str());
  vars[v.key_name].ua_varname = CHAR_PTR(vars[v.key_name].var_name.c_str());
  vars[v.key_name].ua_varpath = CHAR_PTR(vars[v.key_name].var_path.c_str());
//  vars[v.key_name].path_name = CHAR_PTR(vars[v.key_name].str_pathname.c_str());
  /*
    LOGD("STR %s, %s, %s", vars[v.raw_name].str_full.c_str(),
         vars[v.raw_name].str_path.c_str(), vars[v.raw_name].str_name.c_str());
    LOGD("CH* %s, %s, %s", vars[v.raw_name].path_name,
         vars[v.raw_name].path, vars[v.raw_name].name);
  */
  return 1;
}

string OpcServer_c::get_PathName(string &name)
{
  string path = name;

  // "KUB.Temp1" - only name => Do not modufy name, return "/" as folder.
  if (name.find("/") == std::string::npos)
    return "/";

  // "PLC/BUF.Press" - if no "/" at begin => add "/" => ("/PLC/BUF.Press")
  if (path.front() != '/')
    path.insert(0, "/");

  // Is this a FOLDER?
  if (name.back() == '/') {
    name = "";
    return path;
  }

  auto last_slash = path.rfind("/");
  name = path.substr(last_slash + 1);  // name = "BUF.Press"
  path.erase(last_slash + 1);          // path = "/PLC" or "/SCADA/PLC/"

  return path;
}

void OpcServer_c::add_VarNodeId(var_t &v)
{
  v.node_id.var = UA_NODEID_STRING(1, v.ua_keyname);

  if (v.var_path == "/") {
    // v.node_id.parent = UA_NS0ID(OBJECTSFOLDER);
    // v.node_id.reference = UA_NS0ID(ORGANIZES);
    v.node_id.parent = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    v.node_id.reference = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
  } else {
    // v.node_id.reference = UA_NS0ID(HASCOMPONENT);
    v.node_id.parent = get_FolderNodeId(v.var_path);
    v.node_id.reference = UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT);
  }
}

UA_NodeId OpcServer_c::get_FolderNodeId(string str_path)
{
  // UA_NodeId parentNode = UA_NS0ID(OBJECTSFOLDER);
  UA_NodeId parentNode = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);

  if (vars.count(str_path))
    parentNode = vars[str_path].node_id.var;
  else {
    string Path;
    int folder_levels = count_Slash(str_path) - 1;
    for (int i = 1; i <= folder_levels; i++) {
      Path = get_PathByLevel(str_path, i);
      if (vars.count(Path))
        parentNode = vars[Path].node_id.var;
      else
        parentNode = add_FolderToParent(Path, parentNode);
    }
  }

  return parentNode;
}

UA_NodeId OpcServer_c::add_FolderToParent(string str_path, UA_NodeId parentNodeId)
{
  char* folder_path = const_cast<char*>(str_path.c_str());
  string str_displayName = str_path;
  str_displayName.erase(str_displayName.length() - 1);       // remove last "/"
  auto last_slash = str_displayName.rfind("/");              // find name of
  str_displayName = str_displayName.substr(last_slash + 1);  // "destination"
  char* display_name = const_cast<char*>(str_displayName.c_str());
  // LOGA("Folder display name: %s", display_name);

  /* get the nodeid assigned by the server */
  UA_NodeId folderId = UA_NODEID_STRING(1, folder_path);
  UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
  oAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", display_name);
  /* UA_StatusCode sc = UA_Server_addObjectNode(
                         uaServer, folderId, parentNodeId, UA_NS0ID(ORGANIZES),
                         UA_QUALIFIEDNAME(1, folder_path),
     UA_NS0ID(BASEOBJECTTYPE), oAttr, NULL, &folderId); */

  UA_StatusCode sc = UA_Server_addObjectNode(
                       uaServer, folderId, parentNodeId,
                       UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                       UA_QUALIFIEDNAME(1, folder_path),
                       UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                       oAttr, NULL, &folderId);

  var_t v;
//  v.str_pathname = str_path;
  v.is_var = false;
  v.node_id.var = folderId;
  vars[str_path] = v;
  LOGI("NewFolder: %s - %s", folder_path, UA_StatusCode_name(sc));

  /*   DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "NewFolder: %s, name: %s - %s", folder_path, display_name,
                      UA_StatusCode_name(sc));) */

  return folderId;
}

int OpcServer_c::count_Slash(string Path)
{
  size_t index = 0;
  int count = 0;
  string Slash = "/";

  while ((index = Path.find(Slash, index)) != std::string::npos) {
    count++;
    index += Slash.length();  // перемещаем индекс на позицию после завершения
    // слова в тексте
  }
  return count;
}

string OpcServer_c::get_PathByLevel(string Path, int level)
{
  size_t index = 0;
  Path.erase(0, 1);  // remove first "/"

  for (int i = 1; i <= level; i++)
    index = Path.find("/", index + 1);

  Path.erase(index);
  Path = "/" + Path + "/";

  return Path;
}

void OpcServer_c::add_Variable(var_t &v)
{
  if (v.ptr_value == nullptr) {
    LOGA("Wrong ptr: %s", v.ua_varname);
    return;
  }

  UA_Byte acl = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  UA_Variant_setScalar(&attr.value, v.ptr_value, &UA_TYPES[v.ua_type]);

  attr.description = UA_LOCALIZEDTEXT_ALLOC("en-US", v.ua_varname);
  attr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", v.ua_varname);
  attr.dataType = UA_TYPES[v.ua_type].typeId;
  attr.accessLevel = acl;

  UA_QualifiedName varQName = UA_QUALIFIEDNAME(1, v.ua_keyname);

  UA_Server_addVariableNode(
    uaServer, v.node_id.var, v.node_id.parent, v.node_id.reference, varQName,
    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), attr, NULL, NULL);

  string d = get_StrVarDetails(v);
  LOGI("NewVar: %s %s - %s", v.ua_keyname, d.c_str(), UA_StatusCode_name(rc));
  /*   DEBUG(UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                      "NewVar: %s %s, path: %s - %s, type: %i", v.ua_varname, d.c_str(),
                      v.ua_keyname, UA_StatusCode_name(rc), v.ua_type);) */
}

// eof