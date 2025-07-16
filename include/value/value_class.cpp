/*
  #include <open62541/plugin/log_stdout.h>
  #include <open62541/server.h>
  #include <open62541/server_config_default.h>
*/
//#include "include/open62541/open62541.h"

#include <map>
#include <mutex>
#include <set>
#include <thread>
#include <typeindex>
#include <typeinfo>

#include "include/console.h"
#include "include/logger.h"
#include "value_class.h"

#define _TYPE_INDEX(_XNUM) type_index(typeid(_XNUM))
#define _UA_TYPE(_XNUM) ua_types[_TYPE_INDEX(_XNUM)]

#define DEBUG(a) if(isDebug){a}

using std::set;
using std::to_string;
/*
  bool operator<(const Value_c &v1, const Value_c &v2) { return v1.ui64 < v2.ui64; }
  bool operator>(const Value_c &v1, const Value_c &v2) { return v1.ui64 > v2.ui64; }
  bool operator!=(const Value_c &v1, const Value_c &v2) { return v1.ui64 != v2.ui64; }
*/


/*
  int OpcServer_c::getType(string s)
  {
  if (vars.count(s))
    return vars[s].type;
  else
    LOGW("Type: Ignore non-existing variable: %s", s.c_str());
  return -1;
  }
*/
/*
  int OpcServer_c::getStatus(string s)
  {
  if (vars.count(s))
    return vars[s].ua_status;  // 0 - is OK, any other (1 or -1) is BAD
  else
    LOGW("Status: Ignore non-existing variable: %s", s.c_str());
  return -1;
  }
*/


// eof