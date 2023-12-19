// ----------------------------------------------------------------------------

#include "config.h"
#include "libs.h"
#include "timer.h" // Timer t; - already initialised here!

using namespace std;
using namespace libconfig;

vector<plc_t> plcs;
int rc;

// int main(int argc, char **argv) {

int main() {
  // Timer t;
  t.start();
  uint64_t s = 0;

  s = sizeof(plcs);
  cout << "Size of PLC structure is: " << s << endl;

  cfg_read_mbset(CFG_FILE, plcs);
  mb_show(plcs);

  s = 10;
  t.stop();
  t.spent_auto();

  s = sizeof(plcs);
  cout << "Size of PLC structure is: " << s << endl;

  t.start();
  t.sleep_ms(900);
  t.stop();
  t.spent_auto();

//   getr();

  return (EXIT_SUCCESS);
}

// eof
