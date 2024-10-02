
#include "opc_class.h"

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <thread>
#include <mutex>

#include "include/logger.h"

#define DEBUG(a) if (isDebug) {a}

string OpcServer_c::getFolder_Name(string &name)
{
  string folder = "/";

// "KUB.Temp1" - only name => Do not modufy name, return "/" as folder.
  if (name.find("/") == std::string::npos)
    return folder;

// "PLC/BUF.Press" - no "/" at begin => add "/" => ("/PLC/BUF.Press")
  if (name.front() != '/')
    name.insert(0, folder);

  auto last_slash = name.rfind("/");
  folder = name.substr(0, last_slash);  // folder =  "/PLC" or "/SCADA/PLC"
  name.erase(0, last_slash + 1);        // "BUF.Press"

  return folder;
}


int OpcServer_c::addVar_Names(string str_name, int t, int m)
{
  if (str_name.back() == '/') {
    LOGA("Ignore wrong register name: %s", str_name.c_str());
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

  if (s_folder == "/")
    v.fullname = s_folder + str_name;
  else {
    v.fullname = s_folder + "/" + str_name;
    s_folder.erase(0, 1);
  }

  v.str_folder = s_folder;

  if (vars.count(v.fullname)) {
    LOGA("Ignore existing variable: %s", v.fullname.c_str());
    return 0;
  }

  vars[v.fullname] = v;
  var_t &v_ = vars[v.fullname];
  v_.name = const_cast<char*>(v_.str_name.c_str());
  v_.qf_name = const_cast<char*>(v_.fullname.c_str());

  v_.folder = nullptr;
  if (s_folder != "/") {
    if (!v_.str_folder.empty())
      v_.folder = const_cast<char*>(v_.str_folder.c_str());
  }

  LOGD("Fullname: %s, name: %s, folder: %s", vars[v.fullname].fullname.c_str(),
       vars[v.fullname].name, vars[v.fullname].folder);

  return 1;
}

