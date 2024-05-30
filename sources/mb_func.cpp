// mb_func.cpp ------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
// https://www.bogotobogo.com/cplusplus/C11/1_C11_creating_thread.php
// https://stackoverflow.com/questions/266168/simple-example-of-threading-in-c

#include <thread>
#include <vector>

#include "./config.h"
#include "./libs.h"
// using namespace std;
// using namespace libconfig;

static vector<int> res;
static vector<uint64_t> prev_ts;

void mb_update_master(int x)
{
  PLC_c &D = PLCset[x];
  prev_ts[x] = D.mb.timestamp_try_ms;
  res[x] = D.update_master();
  std::this_thread::yield();
  return;
}

void mb_print_summary(int x)
{
  PLC_c &D = PLCset[x];
  printf("%-7s_dT: %4ld ret: %2d err: %d cn: %d rd: %d wr: %d rc: %2d\n",
         D.dev_name, D.mb.timestamp_try_ms - prev_ts[x], res[x], D.mb.errors,
         D.mb.errors_cn, D.mb.errors_rd, D.mb.errors_wr, D.get_rc());
}

int mb_update()
{
  //  cout << "===== mb_update =====" << endl;
  uint64_t i = 0;
  uint64_t nb_plcs = PLCset.size();
  vector<thread> thr(nb_plcs);
  res.resize((int)nb_plcs);
  prev_ts.resize(nb_plcs);

  for (i = 0; i < nb_plcs; i++)
    thr[i] = thread(mb_update_master, i);

  for (auto &th : thr)
    th.join();

  printf("\n");
  for (i = 0; i < nb_plcs; i++)
    mb_print_summary((int)i);

  res.clear();
  prev_ts.clear();

  return 0;
}

int mb_read()
{
  cout << endl << "===== mb_read =====" << endl;
  int ret = 0;

  for (auto &D : PLCset) {
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
  cout << endl << "===== mb_write =====" << endl;

  for (auto &D : PLCset) {
    D.write_master();
    //    tt.sleep_ms(10);
  }

  return 0;
}

// eof
