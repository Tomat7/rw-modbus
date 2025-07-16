// main.cpp ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <cmath>
#include <map>
#include <set>
#include <string>
#include <variant>
#include <vector>

#include "./config.h"
#include "./libs.h"

cfg_t Cfg;
map<string, Reg_c> REGmap;
vector<PLC_c> PLCvec;
OPC::OpcClient_c OPCclient;
OpcServer_c OPCs;
Schedule_c Task /* (TASKS_NB_MAX) */;
// PLC_c Slave(MB_SLAVE_PORT);
// string PLC_folder = PLC_FOLDER;
// string SCADA_folder = OPC_SCADA_FOLDER;
// INotify IN(CFG_DIR);
// int timeout_sec = TIMEOUT_SEC;

const char* mode = "master";
int rc;

static void close_sigint(int dummy)
{
  LOGC("Exit by Ctrl-C. Bye.\n");
  deinit_all();
  restore_console();
  closelog();
  exit(dummy);
}

//  std::cout << "dbl_fmt: " << std::format("{}", 2.235435435) << "\n";
//  The "std::format" and "std::print" is real BULLSHIT! Never use it!

int main(int argc, char** argv)
{
  Timer t;

  std::set<string> Mode{MODBUS_MODES};
  log_level = LOG_LEVEL_DEFAULT;  // Need "work" logging // log_level = 3;

  openlog("Modbus", LOG_NDELAY, LOG_LOCAL1);
  signal(SIGINT, close_sigint);

  if (argc > 1) {
    if (Mode.count(string(argv[1])))
      mode = argv[1];
    else
      LOGA("Argument '%s' ignored.", argv[1]);
  }

  if (argc > 2) {
    char ch = *argv[2];
    if ((strlen(argv[2]) == 1) && (isdigit((char)ch))) {
      log_level = int((char)ch - '0');
      LOGA("LOG_LEVEL set to: %d", log_level);
    } else
      LOGA("Argument '%s' ignored.", argv[2]);
  }

  uint16_t Ui16 = 12345;
  float F = 3.1415f;
  Value_c Ux = Ui16;
  Value_c Fx = F;
  printf("\n\n%i\n", (uint16_t)Ux);
  printf("%f\n", (float)Fx);

  wait_console(Cfg.timeout_sec * 3);

  init_all();

  OPCclient.init("opc.tcp://localhost:4840");
//  logger_set_queue(true);

  for (;;) {
    logger_set_queue(true);
    printf("%s", ESC_CLS);
    printf("%s", ESC_HOME);
    fflush(stdout);

    opc_client_();
    opc_server_();

    printf("\n");

    t.start();
    regs_update();
    t.spent_auto("============ REG print finished in: ");

    const char* x = nullptr;
    t.start(x);
    mb_print_summary();
    // mb_update();
    // LOGD("regdata_t size: %d", sizeof(regdata_t));
    // LOGD("P array size: %d", sizeof(P));
    t.spent_auto("============ MB update: spent on ALL PLCs by TCP: ");

    logger_set_queue(false);
    logger_flush();

    // Slave.handle_slave(timeout_sec * 1000000);

    int ch = read_console(0, Cfg.timeout_sec * 1000000);
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

  return (EXIT_SUCCESS);
}

// eof
