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

map<string, RegMap_c> REGmap;
//map<string, rshm_t> SHMmap;
vector<PLC_c> PLCvec;
int rc;

// int main(int argc, char **argv) {

int main() {

  t.start();
  cfg_read(CFG_FILE);
  t.stop();
  cout << "============ Cfg finished." << endl;
  t.spent();
  t.sleep_ms(2987);

  t.start();
  plc_show1();
  t.stop();
  cout << "============ PLC show finished." << endl;
  t.spent();

  t.start();
  regs_create_from_masters();
  t.stop();
  cout << "============ REG init finished." << endl;
  t.spent();

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
    reg_print_name();
    t.stop();
    cout << "============ REG print finished." << endl;
    t.spent_auto("Printing: ");

    t.start();
    t.sleep_sec(3);
    t.stop();
    t.spent();
  }

  //   getr();

  return (EXIT_SUCCESS);
}

// eof
