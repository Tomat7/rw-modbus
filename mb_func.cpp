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
    D.read();
    tt.sleep_ms(10);
  }
  return 0;
}

int mb_write() {
  cout << endl << "===== mb_write =====" << endl;

  for (auto &D : PLCset) {
    D.write();
    tt.sleep_ms(10);
  }

  return 0;
}

// eof
