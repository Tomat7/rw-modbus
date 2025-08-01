// mb_func.cpp ------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
// https://www.bogotobogo.com/cplusplus/C11/1_C11_creating_thread.php
// https://stackoverflow.com/questions/266168/simple-example-of-threading-in-c

#include <thread>
#include <vector>

#include "config.h"
#include "libs.h"

using std::string;
//using std::mutex;
using std::map;
// using std::vector;

static mutex mbupdate_mux;
static std::vector<int> res;
static std::vector<uint64_t> idx;
static std::vector<uint64_t> prev_ts;

int task_plc_refresh_(void* params)
{
  uint64_t x = *(uint64_t*)params;
  PLC_c &D = PLCvec[x];
  prev_ts[x] = D.mb.timestamp_try_ms;
  res[x] = D.refresh_master();
  std::this_thread::yield();
  LOGI("%s: %d Done.", __func__, x);
  return (int)x;
}

int mb_add_refresh_tasks()
{
  //  "===== mb_update ====="
  uint64_t i = 0;
  uint64_t nb_plcs = PLCvec.size();
  res.resize(nb_plcs);
  idx.resize(nb_plcs);
  prev_ts.resize(nb_plcs);

  for (i = 0; i < nb_plcs; i++) {
    PLC_c &D = PLCvec[i];
    idx[i] = i;
    //string task_name = D.str_top_folder + ":" + D.str_dev_name + ":refresh_";
    string task_name = "Modbus:" + D.str_dev_name; // + ":refresh_";
    Task.add_task(task_plc_refresh_, D.mb.polling_ms, task_name, &idx[i]);
  }

  using namespace std::chrono_literals;
  std::this_thread::sleep_for(10ms);
  printf("mb_update threads STARTED.\n");
  std::this_thread::yield();

  return 0;
}

int mb_print_summary()
{
  uint64_t i = 0;
  uint64_t nb_plcs = PLCvec.size();

  for (i = 0; i < nb_plcs; i++) {
    PLC_c &D = PLCvec[i];
    printf(
      "%-7s_dT: %5ld ret: %2d err: %4d conn: %4d rd: %4d wr: %4d rc: %2d\n",
      D.dev_name, D.mb.timestamp_try_ms - prev_ts[i], res[i], D.mb.errors,
      D.mb.errors_cn, D.mb.errors_rd, D.mb.errors_wr, D.get_rc_read());
  }
  return (int)nb_plcs;
}

void mb_deinit()
{
  //  printf("mb_deinit READY to clear\n");
  res.clear();
  LOGD("res() - done");
  idx.clear();
  LOGD("idx() - done");
  prev_ts.clear();
  LOGD("prev_ts() - done");
  LOGD("PLCvec.size() - %d", PLCvec.size());
  PLCvec.clear();
  LOGD("PLCvec.clear() - done");
  // Slave.mb_deinit();
  //  printf("mb_deinit READY for UN-lock\n");
  //  printf("mb_deinit UN-locked\n");
  return;
}

// eof
