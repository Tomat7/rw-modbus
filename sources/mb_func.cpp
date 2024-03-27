// mb_func.cpp ------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <vector>
#include <thread>

#include "./config.h"
#include "./libs.h"
// using namespace std;
// using namespace libconfig;


void mb_update_master(int x)
{
  PLC &D = PLCset[x];
  uint64_t old = D.mb.timestamp_ms;
  int ret = D.update_master();
  printf("%-7s_dT: %4ld ret: %2d err: %d cn: %d rd: %d wr: %d rc: %2d\n",
         D.dev_name, D.mb.timestamp_ms - old, ret, D.mb.errors,
         D.mb.errors_cn, D.mb.errors_rd, D.mb.errors_wr, D.get_rc());
  
  cout << "Th: " << x << " finished." << endl;
  
}

int mb_update()
{
  cout << "===== mb_update =====" << endl;
  int ret = 0;
  uint64_t i = 0;
  uint64_t nb_plcs = PLCset.size();
  vector<thread> thr(nb_plcs);

  for (i = 0; i < nb_plcs; i++) {
    cout << "thread " << i << " start." << endl;
    thr[i] = thread(mb_update_master, i);
  }

  for (auto &th : thr)
    th.join();

  return ret;
}


int mb_read()
{
  cout << endl << "===== mb_read =====" << endl;
  int ret = 0;

  for (auto &D : PLCset) {
    uint64_t old = D.mb.timestamp_ms;
    ret = D.read_master();
    printf("%-7s_dT: %4ld ret: %2d err: %d cn: %d rd: %d wr: %d rc: %2d\n",
           D.dev_name, D.mb.timestamp_ms - old, ret, D.mb.errors,
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
