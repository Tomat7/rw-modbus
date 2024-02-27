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
// map<string, rshm_t> SHMmap;
vector<PLC> PLCset;
int rc;

// int main(int argc, char **argv) {

int main() {


  t.start();
  cfg_read(CFG_FILE);
  t.stop();
  cout << "============ Cfg finished." << endl;
  t.spent();
  t.sleep_ms(987);

  t.start();
  plc_show();
  t.stop();
  cout << "============ PLC show finished." << endl;
  t.spent();

  t.start();
  regs_init();
  t.stop();
  cout << "============ REG init finished." << endl;
  t.spent();

  static uint16_t w = 0;

  for (;;) {
    printf("%s", CLS);
    printf("%s", HOME);
    fflush(stdout);
/*
    t.start();
    mb_read();
    t.stop();
    cout << "============ MB read finished." << endl;
    t.spent_auto("MB: spent on 3xPLC by TCP: ");
*/
    t.start();
    regs_update();
    w++;
    reg_write("Kub.Pset", w);
//    mb_write();
    t.stop();
    cout << "============ REG print finished." << endl;
    t.spent_auto("Printing: ");

    t.start();
    t.sleep_sec(1);
    t.stop();
    t.spent();
  }

  //   getr();

  return (EXIT_SUCCESS);
}

// eof
