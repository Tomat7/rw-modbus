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

#include "libs/net_service/net_service.h"
#include "config.h"
#include "libs.h"

nlohmann::json JSON_reg;
NetService_c NetSvc;
string HTTP_header = "";

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

  using OPC_client::ReadValue;
  using OPC_client::WriteValue;

  if (rm.str_opcname.ends_with(".millis")) {
    string s = rm.str_opcname;
    //replace_str(s, ".millis", ".status");
    s.replace(s.length() - string(".millis").length(), string(".status").length(), ".status");
    JSON_reg[s] = (int16_t)OPC_server::ReadValue(s);
  }

}

void json_set_answer()
{
  //static float Y = 3.14f;
  // Y += 0.001f;
//  JSON_reg["y"] = "3.1415926"; //std::to_string(Y);
//  JSON_reg["x"] = "TheX";
  string json_str = JSON_reg.dump(4);

  string http_header = "HTTP/1.1 200 OK\r\n";
  http_header += "Content-Type: application/json\r\n";
  http_header += "Access-Control-Allow-Origin: *\r\n";
//  http_header += "Content-Length: [";
//  http_header += std::to_string(json_str.length()) + "]\r\n\r\n" ;

  http_header += "\r\n" ;
  NetSvc.set_answer(http_header + json_str + "\r\n");
}

void netsvc_deinit() { NetSvc.stop(); }

void netsvc_init()
{
  NetSvc.init(NET_SERVER_PORT, NET_SERVER_MAX_CLIENTS);
  //HTTP_header = "HTTP/1.1 200 OK\n";
  //HTTP_header += "Content-Type: application/json\n";
  //HTTP_header += "Access-Control-Allow-Origin: *\n";
}

void netsvc_start()
{
  std::thread netsvc_thread(netsvc_run_thread);
  netsvc_thread.detach();
  console_wait_sec(Cfg.timeout_sec);
}

void netsvc_run_thread()
{
  prctl(PR_SET_NAME, NET_SERVER_THREAD_NAME);
  NetSvc.run();
}


// eof
