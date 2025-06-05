// tasks.cpp ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <string>

#include "config.h"
#include "libs.h"

void tasks_init()
{
  Task.init(Cfg.tasks_nb);
  Task.add_task(task_millis_, Cfg.millis_heartbeat_ms, "Millis_");
  Task.add_task(task_opc_refresh_, Cfg.opc.polling_ms, "OPC:Refresh_");
  Task.add_task(task_regs_refresh_, Cfg.regs_refresh_ms, "REGS:Refresh_");
  // Task.add_task(task_begin_, 1300, "Begin_", &timeout_sec);
  mb_add_refresh_tasks();
}

void tasks_start() { Task.start(); }

int task_millis_(void* params)
{
  UNUSED(params);
  int x = 0;
  for (auto& [n, rm] : REGmap) {
    if (n.find("millis") != std::string::npos) {
      LOGD("%s", n.c_str());
      OPC_server::WriteValue(n, 0);
      x++;
    }
  }
  LOGI("%s done: %d", __func__, x);
  return x;
}

int task_opc_refresh_(void* params)
{
  UNUSED(params);
  int y = OPCs.RefreshAllValues();
  LOGI("%s done: %d", __func__, y);
  return y;
}

/*
  int task_begin_(void* params)
  {
  // OPCs.refreshValues();
  LOGI("%s: %d Done.", __func__, *(int*)params);
  return 1;
  }
*/

/* void task_mb_update_(void *idx)
  {
  int x = *(int *)idx;
  PLC_c &D = PLCvec[x];
  prev_ts[x] = D.mb.timestamp_try_ms;
  res[x] = D.update_master();
  std::this_thread::yield();
  return;
  }
*/
// eof
