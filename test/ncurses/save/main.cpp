// main.cpp ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <map>
#include <string>
#include <vector>

#include "./config.h"
#include "./libs.h"

// using namespace std;
// using namespace libconfig;

std::map<string, RegMap_c> REGmap;
std::vector<PLC_c> PLCset;
// INotify IN(CFG_DIR);

regdata_t* P;
int rc;

static void close_sigint(int dummy)
{
  deinit_all();
  restore_console();
  LOGN("Exit by Ctrl-C. Bye.\n");
  closelog();
  endwin();
  exit(dummy);
}


// int main(int argc, char **argv) {

int main()
{
  Timer t;
  signal(SIGINT, close_sigint);
  openlog("Modbus", LOG_NDELAY, LOG_LOCAL1);
  log_level = 7;

  initscr();

  init_all();

  for (;;) {
//    PRINTF("%s", CLS);
//    PRINTF("%s", HOME);
//    fflush(stdout);
    sleep(5);
    t.start();
    regs_update();
    t.spent_auto("============ REG print finished in: ");
    refresh();

    const char* x = nullptr;
    t.start(x);
    mb_update();
    refresh();

    LOGD("regdata_t size: %d", sizeof(regdata_t));
    LOGD("P array size: %d", sizeof(P));
    t.spent_auto("============ MB update: spent on ALL PLCs by TCP: ");

    int ch = read_console(timeout_sec);
//    if (ch != -1)
//      parse_char(ch);
//    else
//      PRINTF("!\n");

//    fflush(stdout);
    refresh();

    //    wait_console(TIMEOUT_SEC);

    //    t.start(x);
    //    t.sleep_ms(TMOUT);
    //    t.spent();
  }

  //   getr();

  return (EXIT_SUCCESS);
}

// eof
