// tasks.cpp ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

/* #include <map>
  #include <set>

  #include <vector>
  #include <variant>
*/

#include <string>
#include "config.h"
#include "libs.h"

void tasks_init()
{
  Task.init(TASKS_NB_MAX);
  Task.add_task(task_begin_, 1300, "Begin_", &timeout_sec);
  Task.add_task(task_opc_refresh_, 700, "Refresh_");
  Task.add_task(task_millis_, 2500, "Test_");
  mb_add_tasks();
  Task.run();
}

int task_millis_(void* params)
{
  UNUSED(params);
  int i = 0;
  for (auto &[n, rm] : REGmap) {
    if (n.find("millis") != std::string::npos) {
      WriteValue(n, 0);
      i++;
    }
  }
  LOGI("%s done: %d\n", __func__, i);
  return i;
}

int task_opc_refresh_(void* params)
{
  UNUSED(params);
  OPCs.refreshValues();
  LOGI("%s: Done.\n", __func__);
  return 1;
}

int task_begin_(void* params)
{
  // OPCs.refreshValues();
  LOGI("%s: %d Done.\n", __func__, *(int*)params);
  return 1;
}

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
