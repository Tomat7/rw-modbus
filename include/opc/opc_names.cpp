
#include "opc_class.h"

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <thread>
#include <mutex>
#include <string>

#include "include/logger.h"

#define DEBUG(a) if (isDebug) {a}

string OpcServer_c::getFolder_Name(string &name)
{
  string folder = "/";
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
  name = path.substr(last_slash + 1);   // "BUF.Press"
  path.erase(last_slash + 1);           // folder =  "/PLC" or "/SCADA/PLC/"

  return path;
}


int OpcServer_c::addVar_Names(string str_name, int t, int m)
{

  if (str_name.find("//") != std::string::npos) {
    LOGA("Ignore wrong name/path: %s", str_name.c_str());
    return 0;
  }

  string s_folder = getFolder_Name(str_name);

  if (s_folder.find("//") != std::string::npos) {
    LOGA("Ignore wrong folder name: %s", s_folder.c_str());
    return 0;
  }

  var_t v;
  v.type = t;
  v.rmode = m;
  v.str_name = str_name;
  v.str_folder = s_folder;
  v.fullname = s_folder + str_name;

  if (vars.count(v.fullname)) {
    LOGA("Ignore existing variable: %s", v.fullname.c_str());
    return 0;
  }

  vars[v.fullname] = v;
  var_t &v_ = vars[v.fullname];
  v_.name = const_cast<char*>(v_.str_name.c_str());
  v_.folder = const_cast<char*>(v_.str_folder.c_str());
  v_.ua_name = const_cast<char*>(v_.fullname.c_str());

  LOGD("Fullname: %s, name: %s, folder: %s", vars[v.fullname].fullname.c_str(),
       vars[v.fullname].name, vars[v.fullname].folder);

  return 1;
}

