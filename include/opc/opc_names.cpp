
#include "opc_class.h"

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <thread>
#include <mutex>

#include "include/logger.h"

#define DEBUG(a) if (isDebug) {a}

string OpcServer_c::getFolderName(string &name)
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

  string s_folder = getFolderName(str_name);

  if (s_folder.find("//") != std::string::npos) {
    LOGA("Ignore wrong folder name: %s", s_folder.c_str());
    return 0;
  }

  //LOGD("Try to add folder: %s, reg: %s", s_folder.c_str(), str_name.c_str());

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

/*
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
*/

