// mb_func.cpp ------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <vector>

#include "./config.h"
#include "./libs.h"
#include "./timer.h" // Timer t; - already initialised h
// using namespace std;
// using namespace libconfig;

Timer tt;

int mb_read() {
  cout << endl << "===== mb_read =====" << endl;

  for (auto &D : PLCset) {
    uint64_t old = D.mb.timestamp_try_ms;
    D.read_master();
    printf("%s _dT: %ld  err: %d cn: %d rd: %d wr: %d rc: %d\n", D.dev_name,
           D.mb.timestamp_try_ms - old, D.mb.errors, D.mb.errors_cn, D.mb.errors_rd,
           D.mb.errors_wr, D.get_rc());
    //    tt.sleep_ms(10);
  }
  return 0;
}

int mb_write() {
  cout << endl << "===== mb_write =====" << endl;

  for (auto &D : PLCset) {
    D.write_master();
    //    tt.sleep_ms(10);
  }

  return 0;
}

int mb_update() {
  cout << "===== mb_update =====" << endl;
  int ret = 0;

  for (auto &D : PLCset) {
    uint64_t old = D.mb.timestamp_try_ms;
    ret = D.update_master();
    printf("%s _dT: %4ld ret: %2d err: %d cn: %d rd: %d wr: %d rc: %2d\n",
           D.dev_name, D.mb.timestamp_try_ms - old, ret, D.mb.errors,
           D.mb.errors_cn, D.mb.errors_rd, D.mb.errors_wr, D.get_rc());
    //    tt.sleep_ms(10);
  }

  return ret;
}

// eof
