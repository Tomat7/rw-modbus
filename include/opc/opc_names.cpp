
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <mutex>
#include <string>
#include <thread>
#include <typeinfo>

#include "include/logger.h"
#include "opc_class.h"

#define DEBUG(a) \
  if (isDebug) { \
    a            \
  }

string OpcServer_c::getPath_Name(string &name)
{
  // string folder = "/";
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
  name = path.substr(last_slash + 1);  // "BUF.Press"
  path.erase(last_slash + 1);          // folder =  "/PLC" or "/SCADA/PLC/"

  return path;
}

int OpcServer_c::addVar_Names(string raw_name, int t, int m)
{
  if (vars.count(raw_name)) {
    LOGA("Add: Ignore existing variable: %s", raw_name.c_str());
    return 0;
  }

  if (raw_name.find("//") != std::string::npos) {
    LOGA("Add: Ignore wrong name/path: %s", raw_name.c_str());
    return 0;
  }

  string str_name = raw_name;
  string str_path = getPath_Name(str_name);

  var_t v;
  v.type = t;
  v.rmode = m;
  v.raw_name = raw_name;  // KEY for map and OPC FQName
  v.str_name = str_name;
  v.str_path = str_path;
  v.str_full = str_path + str_name;

  vars[v.raw_name] = v;
  vars[v.raw_name].ua_name =
    const_cast<char*>(vars[v.raw_name].raw_name.c_str());
  vars[v.raw_name].name = const_cast<char*>(vars[v.raw_name].str_name.c_str());
  vars[v.raw_name].path = const_cast<char*>(vars[v.raw_name].str_path.c_str());
  vars[v.raw_name].path_name =
    const_cast<char*>(vars[v.raw_name].str_full.c_str());
  /*
    LOGD("STR %s, %s, %s", vars[v.raw_name].str_full.c_str(),
         vars[v.raw_name].str_path.c_str(), vars[v.raw_name].str_name.c_str());
    LOGD("CH* %s, %s, %s", vars[v.raw_name].path_name,
         vars[v.raw_name].path, vars[v.raw_name].name);
  */
  return 1;
}

// eof
