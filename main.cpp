// ----------------------------------------------------------------------------

#include "config.h"
#include "libs.h"
#include "plc_class.h"
#include "timer.h" // Timer t; - already initialised here!

using namespace std;
using namespace libconfig;

map<string, reg_t *> REGmap;
vector<PLC> PLCset;
int rc;

// int main(int argc, char **argv) {

int main() {

  t.start();
  cfg_read(CFG_FILE);
  t.stop();
  cout << "============ Cfg finished." << endl;
  t.spent();

  t.start();
  plc_show();
  t.stop();
  cout << "============ PLC show finished." << endl;
  t.spent();

  t.start();
  reg_init();
  t.stop();
  cout << "============ REG init finished." << endl;
  t.spent();

  for (;;) {
    t.start();
    mb_read();
    t.stop();
    cout << "============ MB read finished." << endl;
    t.spent_auto("MB: spent on 3xPLC by TCP: ");

    t.start();
    reg_print_name();
    t.stop();
    cout << "============ REG print finished." << endl;
    t.spent();

    t.start();
    t.sleep_ms(987);
    t.stop();
    t.spent();
  }
  //   getr();

  return (EXIT_SUCCESS);
}

// eof
