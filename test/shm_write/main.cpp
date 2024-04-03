// main.cpp ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <map>
#include <string>
#include <vector>

#include "./config.h"
#include "./libs.h"
//#include "./plc_class.h"
//#include "./timer.h" // Timer t; - already initialised here!

// using namespace std;
// using namespace libconfig;

map<string, RegMap_c> REGmap;
vector<PLC_c> PLCset;
int rc;
Timer t;

// int main(int argc, char **argv) {

int main()
{

  t.start();
  cfg_master(CFG_DIR, CFG_FILE);
  t.stop();
  cout << "============ Cfg finished." << endl;
  t.spent();
  t.sleep_ms(987);

  /*
    t.start();
    plc_show();
    t.stop();
    cout << "============ PLC show finished." << endl;
    t.spent();
  */

  t.start();
  regs_init();
  t.stop();
  cout << "============ REG init finished." << endl;
  t.spent();
  /*
    mb_slave_init();
    printf("============ SLAVE init finished.\n");
    fflush(stdout);
    t.sleep_sec(3);
  */
  static uint16_t w = 500;
  uint64_t mm = t.millis();

  for (;;) {
    /*
        t.start();
        mb_read();
        t.stop();
        cout << "============ MB read finished." << endl;
        t.spent_auto("MB: spent on 3xPLC by TCP: ");
    */

    if ((t.millis() - mm) > 1000) {
      t.start();
      printf("%s", CLS);
      printf("%s", HOME);
      fflush(stdout);
      regs_update_shm();

      w++;
      if (w > 599)
        w = 500;

      write_shm("Kub.Pset", w);
      write_rm("Kub.millis", 0);
      write_rm("Buf.millis", 0);
      write_rm("Def.millis", 0);

      write_rm("TH41.millis", 0);
      write_rm("TH42.millis", 0);
      write_rm("SF45.millis", 0);
      write_rm("SF47.millis", 0);

      write_shm("GATE49.close2", 65535);
      write_shm("GATE49.open2", 0);
      write_shm("GATE49.millis", 0);

      mm = t.millis();

      // mb_write();
      t.stop();
      cout << "============ REG print finished." << endl;
      t.spent_auto("Printing: ");
    }

    //    mb_slave();

    /*
        t.start();
        t.sleep_sec(3);
        t.stop();
        t.spent();
    */
  }

  //   getr();

  return (EXIT_SUCCESS);
}

// eof
