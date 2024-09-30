// main.cpp ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <map>
#include <set>
#include <string>
#include <vector>

#include "./config.h"
#include "./libs.h"

// using namespace std;
// using namespace libconfig;

std::map<string, RegMap_c> REGmap;
std::vector<PLC_c> PLCvec;
PLC_c Slave(1502);
OpcServer_c OPCs;

// INotify IN(CFG_DIR);

const char* mode = "master";
int timeout_sec = TIMEOUT_SEC;
int rc;

static void close_sigint(int dummy)
{
  LOGC("Exit by Ctrl-C. Bye.\n");
  deinit_all();
  restore_console();
  closelog();
  exit(dummy);
}

// int main(int argc, char **argv) {

int main(int argc, char** argv)
{
  Timer t;
  std::set<string> Mode{MODBUS_MODES};
  signal(SIGINT, close_sigint);
  openlog("Modbus", LOG_NDELAY, LOG_LOCAL1);
  log_level = 7;

  if (argc > 1) {
    if (Mode.count(string(argv[1])))
      mode = argv[1];
    else
      LOGC("Argument '%s' ignored.", argv[1]);
  }

  if (argc > 2) {
    char ch = *argv[2];
    if ((strlen(argv[2]) == 1) && (isdigit((char)ch))) {
      log_level = int((char)ch - '0');
      LOGC("LOG_LEVEL set to: %d", log_level);
    } else
      LOGC("Argument '%s' ignored.", argv[2]);
  }

  init_all();

  opc_regs_init();
  std::thread opc_thr(opc_init);
  opc_thr.detach();

  for (;;) {
    printf("%s", CLS);
    printf("%s", HOME);
    fflush(stdout);

    t.start();
    regs_update();
    t.spent_auto("============ REG print finished in: ");

    const char* x = nullptr;
    t.start(x);
    mb_update();
    LOGD("regdata_t size: %d", sizeof(regdata_t));
    //    LOGD("P array size: %d", sizeof(P));
    t.spent_auto("============ MB update: spent on ALL PLCs by TCP: ");

    Slave.handle_slave(timeout_sec * 1000000);

    int ch = read_console(0, 1000000);
    if (ch != -1)
      parse_char(ch);
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
