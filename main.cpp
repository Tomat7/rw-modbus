// ----------------------------------------------------------------------------

#include "config.h"
#include "libs.h"
#include "timer.h"  // Timer t; - already initialised here!

using namespace std;
using namespace libconfig;

vector<plc_t> plcs;
int rc;

// int main(int argc, char **argv) {

int main() {
//Timer t;
  t.start();
  int s = 8;

  cfg_read_mbset(CFG_FILE, plcs);
  mb_show(plcs);

  s = 10;
  t.stop();
  t.spent_msec();

  return (EXIT_SUCCESS);
}

// eof
