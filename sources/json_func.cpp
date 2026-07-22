// json_func.cpp -----------------------------
// Copyright 2026 Tomat7 (star0413@gmail.com)
// http://jsonplaceholder.typicode.com/users
// http://localhost:8080/timerfetch.html
//

#include <string.h>

#include <map>
#include <string>
#include <vector>
#include <sys/prctl.h>
#include <nlohmann/json.hpp>

//#include "libs/net_service/net_service.h"
#include "config.h"
#include "libs.h"

nlohmann::json JSON_reg;

void str_replace(string& s, const string& _orig, const string& _new)
{
  s.replace(s.length() - string(_orig).length(), string(_new).length(), _new);
}

void json_set_number(Reg_c &rm)
{
  int _type_ua = rm.Number.get_type();
  switch (_type_ua) {
  case UA_TYPES_INT16:
    JSON_reg[rm.str_opcname] = rm.Number.i16;
    break;
  case UA_TYPES_INT32:
    JSON_reg[rm.str_opcname] = rm.Number.i32;
    break;
  case UA_TYPES_INT64:
    JSON_reg[rm.str_opcname] = rm.Number.i64;
    break;
  case UA_TYPES_UINT16:
    JSON_reg[rm.str_opcname] = rm.Number.ui16;
    break;
  case UA_TYPES_UINT32:
    JSON_reg[rm.str_opcname] = rm.Number.ui32;
    break;
  case UA_TYPES_UINT64:
    JSON_reg[rm.str_opcname] = rm.Number.ui64;
    break;
  case UA_TYPES_FLOAT:
    JSON_reg[rm.str_opcname] = rm.Number.fl;
    break;
  case UA_TYPES_DOUBLE:
    JSON_reg[rm.str_opcname] = rm.Number.dbl;
    break;
  case NOTUA_TYPES_F10:
    JSON_reg[rm.str_opcname] = rm.Number.fl;
    break;
  case NOTUA_TYPES_F100:
    JSON_reg[rm.str_opcname] = rm.Number.fl;
    break;
  default:
    JSON_reg[rm.str_opcname] = false;
    LOGE("Type: %i not supported", _type_ua);
  }
}


void json_update(Reg_c &rm)
{
  //JSON_reg[rm.str_opcname] = rm.c_str();
  json_set_number(rm);

  if (rm.str_opcname.ends_with(".millis")) {
    string s = rm.str_opcname;
    str_replace(s, ".millis", ".status");
    JSON_reg[s] = (int16_t)OPC_server::ReadValue(s);
  }
}


string json_get_answer() { return JSON_reg.dump(4); }

/*
  void json_update(string _key, const char* _value, int16_t _status)
  {
  JSON_reg[_key] = _value;

  if (_key.ends_with(".millis")) {
    string s = _key;
    //replace_str(s, ".millis", ".status");
    s.replace(s.length() - string(".millis").length(), string(".status").length(), ".status");
    JSON_reg[s] = _status;
  }
  }
*/


/*
  void replace_str(string& str, const string& from, const string& to)
  {
  size_t start_pos = 0;
  // Цикл ищет подстроку 'from' до тех пор, пока она не перестанет встречаться
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length(); // Продвигаем позицию, чтобы избежать бесконечного цикла
  }
  // s.replace(s.length() - string(".millis").length(), string(".status").length(), ".status");
  }
*/


// string json_str = JSON_reg.dump(4);

// string http_header = "HTTP/1.1 200 OK\r\n";
// http_header += "Content-Type: application/json\r\n";
// http_header += "Access-Control-Allow-Origin: *\r\n";
// //  http_header += "Content-Length: [";
// //  http_header += std::to_string(json_str.length()) + "]\r\n\r\n" ;

// http_header += "\r\n" ;
// //  NetSvc.set_answer(http_header + json_str + "\r\n");
// return (http_header + json_str + "\r\n");

// eof
