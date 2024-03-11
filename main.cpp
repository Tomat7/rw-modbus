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
INotify IN(CFG_DIR);
int rc;

static void close_sigint(int dummy) { exit(dummy); }

// int main(int argc, char **argv) {

int main() {

  signal(SIGINT, close_sigint);
  int ret = 0;

  t.start();
  ret = cfg_read(CFG_DIR, CFG_FILE);
  t.spent_auto("============ Cfg finished.");
  if (ret == EXIT_FAILURE)
    return ret;
  t.sleep_ms(4987);

  for (auto &D : PLCset)
    for (auto &R : D.regs) {
      printf("%s: %s.%s [%d] %d   %s\n", D.ip_addr, D.dev_name, R.ch_name,
             R.raddr, R.rvalue, R.fullname);
    }
  printf("===222\n");
  t.sleep_ms(4987);

  t.start();
  plc_show();
  t.spent_auto("============ PLC show finished.");

  t.start();
  regs_init();
  t.spent_auto("============ REG init finished.");

  t.sleep_ms(4987);

  for (;;) {
    printf("%s", CLS);
    printf("%s", HOME);
    fflush(stdout);

    t.start();
    regs_update();
    t.spent_auto("============ REG print finished in: ");

    const char *x = nullptr;
    t.start(x);
    mb_update();
    t.spent_auto("============ MB update: spent on ALL PLCs by TCP: ");

    t.start(x);
    t.sleep_sec(3);
    t.spent();
  }

  //   getr();

  return (EXIT_SUCCESS);
}

// eof
