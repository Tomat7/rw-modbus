// ----------------------------------------------------------------------------

#include "config.h"
#include "libs.h"
#include "plc_class.h"
#include "timer.h" // Timer t; - already initialised here!

using namespace std;
using namespace libconfig;

map<string, uint16_t *> MBreg;
// vector<plc_t> PLCset;

vector<PLC> PLCset;

int rc;

// int main(int argc, char **argv) {

int main() {
  Timer t2;
  //  PLC plc;

  //  devset.push_back(plc);

  t.start();
  uint64_t s = 0;

  s = sizeof(PLCset);
  cout << "Size of PLC structure is: " << s << endl;

  cfg_read_mbset(CFG_FILE);
  mb_show();

  s = 10;
  t.stop();
  t.spent_auto();

  s = sizeof(PLCset);
  cout << "Size of PLC structure is: " << s << endl;

  t.start();
  mb_get();
  t.stop();
  t.spent();

  t.start();
  t.sleep_ms(987);
  t.stop();
  t.spent();

  //   getr();

  return (EXIT_SUCCESS);
}

// eof
