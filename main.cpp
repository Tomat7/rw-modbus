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

std::map<string, rmap_t> REGmap;
std::vector<PLC> PLCset;
Timer t;
int rc;

static void close_sigint(int dummy) { exit(dummy); }

// int main(int argc, char **argv) {

int main() {

  signal(SIGINT, close_sigint);
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
  plc_show();
  //  t.stop();
  t.spent_auto("============ PLC show finished.");
  //  t.spent();

  t.start();
  regs_init();
  //  t.stop();
  t.spent_auto("============ REG init finished.");
  //  t.spent();

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
    // t.stop();
    //    mb_write();
    t.spent_auto("============ REG print finished in: ");
    //    t.spent_auto("Printing: ");

    const char *x = nullptr;
    t.start(x);
    mb_update();
    //    t.stop();
    //    cout << "============ MB update finished." << endl;
    t.spent_auto("============ MB update: spent on ALL PLCs by TCP: ");

    t.start(x);
    t.sleep_sec(1);
    //    t.stop();
    t.spent();
  }

  //   getr();

  return (EXIT_SUCCESS);
}

// eof
