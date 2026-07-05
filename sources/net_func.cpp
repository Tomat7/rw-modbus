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
#include <httplib.h>

//#include "libs/net_service/net_service.h"
#include "config.h"
#include "libs.h"

//NetService_c NetSvc;
httplib::Server svr;
string json_dump;

void netsvc_deinit() { svr.stop(); }

void netsvc_init()
{
  // GET endpoint: Returns a JSON object
  svr.Get("/api/opc", [](const httplib::Request &, httplib::Response& res) {
    res.set_content(json_dump, "application/json");
  });

  // Статика — в конце
  svr.set_mount_point("/", "./www");

}

void netsvc_start()
{
  LOGN("Running REST API on port: %i, thread: %s", NET_SERVER_PORT, NET_SERVER_THREAD_NAME);
  svr.new_task_queue = [] {
    return new httplib::ThreadPool(NET_SERVER_THREADS_BASE, NET_SERVER_THREADS_MAX);
  };
  std::thread netsvc_thread(netsvc_run_thread);
  netsvc_thread.detach();
  console_wait_sec(Cfg.timeout_sec);
}

void netsvc_run_thread()
{
  prctl(PR_SET_NAME, NET_SERVER_THREAD_NAME);
  svr.listen("0.0.0.0", NET_SERVER_PORT);
}

void netsvc_update_json(string _str)
{
  json_dump = _str;
}




// eof
