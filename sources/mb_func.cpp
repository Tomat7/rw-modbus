// mb_func.cpp ------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
// https://www.bogotobogo.com/cplusplus/C11/1_C11_creating_thread.php
// https://stackoverflow.com/questions/266168/simple-example-of-threading-in-c

#include <thread>
#include <vector>

#include "config.h"
#include "libs.h"
// using namespace std;
// using namespace libconfig;

static mutex mbupdate_mux;
static vector<int> res;
static vector<uint64_t> prev_ts;

void mb_update_master(int x)
{
  PLC_c &D = PLCvec[x];
  prev_ts[x] = D.mb.timestamp_try_ms;
  res[x] = D.update_master();
  std::this_thread::yield();
  return;
}

void mb_print_summary(int x)
{
  PLC_c &D = PLCvec[x];
  printf("%-7s_dT: %5ld ret: %2d err: %4d conn: %4d rd: %4d wr: %4d rc: %2d\n",
         D.dev_name, D.mb.timestamp_try_ms - prev_ts[x], res[x], D.mb.errors,
         D.mb.errors_cn, D.mb.errors_rd, D.mb.errors_wr, D.get_rc());
}

int mb_update()
{
  //  "===== mb_update ====="
  uint64_t i = 0;
  uint64_t nb_plcs = PLCvec.size();
  vector<thread> thr(nb_plcs);
  res.resize((int)nb_plcs);
  prev_ts.resize(nb_plcs);

  for (i = 0; i < nb_plcs; i++)
    thr[i] = thread(mb_update_master, i);

  //std::this_thread::yield();
  std::this_thread::sleep_for(10ms);
  printf("mb_update threads STARTED and ready to JOIN.\n");
  std::this_thread::yield();
  //std::this_thread::sleep_for(10ms);

  for (auto &th : thr) {
    printf(".");
    th.join();
  }

  printf("\n");
  for (i = 0; i < nb_plcs; i++)
    mb_print_summary((int)i);

  res.clear();
  prev_ts.clear();

  return 0;
}

int mb_read()
{
  printf("\n===== mb_read =====\n");
  int ret = 0;

  for (auto &D : PLCvec) {
    uint64_t old = D.mb.timestamp_try_ms;
    ret = D.read_master();
    printf("%-7s_dT: %4ld ret: %2d err: %d cn: %d rd: %d wr: %d rc: %2d\n",
           D.dev_name, D.mb.timestamp_try_ms - old, ret, D.mb.errors,
           D.mb.errors_cn, D.mb.errors_rd, D.mb.errors_wr, D.get_rc());
  }
  return 0;
}

int mb_write()
{
  printf("\n===== mb_write =====\n");

  for (auto &D : PLCvec) {
    D.write_master();
    //    tt.sleep_ms(10);
  }

  return 0;
}

void mb_deinit()
{
//  printf("mb_deinit READY to clear\n");
  PLCvec.clear();
  Slave.mb_deinit();
//  printf("mb_deinit READY for UN-lock\n");
//  printf("mb_deinit UN-locked\n");
  return;
}

// eof
