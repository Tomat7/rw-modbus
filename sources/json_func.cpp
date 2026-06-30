// reg_func.cpp -----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
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

/*
  void replace_str(string& str, const string& from, const string& to)
  {
  size_t start_pos = 0;
  // Цикл ищет подстроку 'from' до тех пор, пока она не перестанет встречаться
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length(); // Продвигаем позицию, чтобы избежать бесконечного цикла
  }
  }
*/


void json_update(Reg_c &rm)
{
  JSON_reg[rm.str_opcname] = rm.c_str();
  //  NetSvc.set_answer(JSON_reg.dump());

  //  using OPC_client::ReadValue;
  //  using OPC_client::WriteValue;

  if (rm.str_opcname.ends_with(".millis")) {
    string s = rm.str_opcname;
    //replace_str(s, ".millis", ".status");
    s.replace(s.length() - string(".millis").length(), string(".status").length(), ".status");
    JSON_reg[s] = (int16_t)OPC_server::ReadValue(s);
  }
}


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


string json_get_answer()
{
  return JSON_reg.dump(4);
  // string json_str = JSON_reg.dump(4);

  // string http_header = "HTTP/1.1 200 OK\r\n";
  // http_header += "Content-Type: application/json\r\n";
  // http_header += "Access-Control-Allow-Origin: *\r\n";
  // //  http_header += "Content-Length: [";
  // //  http_header += std::to_string(json_str.length()) + "]\r\n\r\n" ;

  // http_header += "\r\n" ;
  // //  NetSvc.set_answer(http_header + json_str + "\r\n");
  // return (http_header + json_str + "\r\n");
}

// eof
