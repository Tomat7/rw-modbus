// main.cpp ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <map>
#include <string>
#include <vector>

#include "./config.h"
#include "./libs.h"
#include "./plc_class.h"
#include "./timer.h" // Timer t; - already initialised here!

// using namespace std;
// using namespace libconfig;

map<string, rmap_t> REGmap;
vector<PLC> PLCset;
Timer t;
int rc;

// int main(int argc, char **argv) {

int main() {

  int ret = 0;

  t.start();
  ret = cfg_read(CFG_FILE);
  t.stop();
  cout << "============ Cfg finished." << endl;
  t.spent();
  if (ret == EXIT_FAILURE)
    return ret;

  t.sleep_ms(987);

  t.start();
  plc_show1();
  t.stop();
  cout << "============ PLC show finished." << endl;
  t.spent();

  t.start();
  regs_init();
  t.stop();
  cout << "============ REG init finished." << endl;
  t.spent();

  for (;;) {
    printf("%s", CLS);
    printf("%s", HOME);
    fflush(stdout);

    //    t.start();
    //    mb_read();
    //    t.stop();
    //    cout << "============ MB read finished." << endl;
    //    t.spent_auto("MB: spent on 3xPLC by TCP: ");

    t.start();
    regs_update();
    t.stop();
    //    mb_write();
    cout << "============ REG print finished." << endl;
    t.spent_auto("Printing: ");

    t.start();
    mb_update();
    t.stop();
    cout << "============ MB update finished." << endl;
    t.spent_auto("MB: spent on ALL PLCs by TCP: ");

    t.start();
    t.sleep_sec(1);
    t.stop();
    t.spent();
  }

  //   getr();

  return (EXIT_SUCCESS);
}

// eof
