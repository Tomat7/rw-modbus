// main.cpp ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <map>
#include <string>
#include <vector>

#include "./config.h"
#include "./libs.h"
//#include "./timer.h" // Timer t; - already initialised here!

// using namespace std;
// using namespace libconfig;

void init_all() {

  Timer t;
  int ret = 0;

  t.start();
  ret = cfg_master(CFG_DIR, CFG_FILE);
  t.spent_auto("============ Cfg finished in: ");
  if (ret == EXIT_FAILURE)
    exit(EXIT_FAILURE);
  wait_console(TIMEOUT_SEC);

  //  /*
  plc_show2();
  printf("===222\n");
  wait_console(TIMEOUT_SEC);
  //  t.sleep_ms(TMOUT);
  //  */

  t.start();
  plc_show1();
  t.spent_auto("============ PLC show finished in: ");
  wait_console(TIMEOUT_SEC);
  //  t.sleep_ms(TMOUT);

  t.start();
  regs_init();
  t.spent_auto("============ REG init finished in: ");
  wait_console(TIMEOUT_SEC);
  //  t.sleep_ms(TMOUT);

  return;
}

void reinit() {
  REGmap.clear();
  PLCset.clear();
  init_all();
}

// eof
