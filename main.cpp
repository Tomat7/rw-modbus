// main.cpp ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <map>
#include <string>
#include <vector>

#include "./config.h"
#include "./libs.h"
// #include "./timer.h" // Timer t; - already initialised here!

// using namespace std;
// using namespace libconfig;

std::map<string, rmap_t> REGmap;
std::vector<PLC> PLCset;
// INotify IN(CFG_DIR);

int rc;

static void close_sigint(int dummy) {
  LOGERR("Exit by Ctrl-C. Bye.\n");
  closelog();
  exit(dummy);
}

// int main(int argc, char **argv) {

int main() {
  Timer t;
  signal(SIGINT, close_sigint);
  openlog("Modbus", LOG_NDELAY, LOG_LOCAL1);

  init_all();

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

    int ch = read_console(TIMEOUT_SEC);
    if (ch != -1)
      if (((char)ch == 'e') || ((char)ch == 'q')) {
        LOGERR("Char 'e' or 'q' pressed. Correct shutdown. Bye.\n");
        wait_console(TIMEOUT_SEC);
        // t.sleep_sec(3);
        return (EXIT_SUCCESS);
      } else if ((char)ch == 'r') {
        LOGERR("Char 'r' pressed. Full reconfiguration.\n");
        wait_console(TIMEOUT_SEC);
        reinit();
        // t.sleep_sec(3);
      } else if ((char)ch == ' ')
        printf("%s %s %s \n", KGRN, "=============================", KNRM);
      else {
        printf("%s %c %s \n", KBLU, (char)ch, KNRM);
        wait_console(TIMEOUT_SEC);
      }
    else
      printf("!\n");
    fflush(stdout);

    //    wait_console(TIMEOUT_SEC);

    //    t.start(x);
    //    t.sleep_ms(TMOUT);
    //    t.spent();
  }

  //   getr();

  return (EXIT_SUCCESS);
}

// eof
