// ----------------------------------------------------------------------------

#include "config.h"
#include "libs.h"
#include "timer.h" // Timer t; - already initialised here!

using namespace std;
using namespace libconfig;

map<string, uint16_t*> MBreg;
vector<plc_t> PLCset;
int rc;

// int main(int argc, char **argv) {

int main() {
  // Timer t;

  std::map<std::string, unsigned> products;
  map<string, unsigned> product2;
  map<string, reg_t> product3;

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
  t.sleep_ms(1987);
  t.stop();
  t.spent();

//   getr();

  return (EXIT_SUCCESS);
}

// eof
