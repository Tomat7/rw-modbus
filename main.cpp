// main.cpp ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <cmath>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <deque>

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

void var_test0();

static void close_sigint(int dummy)
{
  logger_set_queue(false);
  LOGC("Exit by Ctrl-C. Bye.\n");
  deinit_all();
  closelog();
  exit(dummy);
}

//  std::cout << "dbl_fmt: " << std::format("{}", 2.235435435) << "\n";
//  The "std::format" and "std::print" is real BULLSHIT! Never use it!

int main(int argc, char** argv)
{
  Console::save();
  log_level = LOG_LEVEL_DEFAULT;  // Need "work" logging
  LOGA("Log 'Modbus' started.");
  signal(SIGINT, close_sigint);

//======================================================
  var_test0();
  int R, C;
  Console::get_size(&R, &C);
  LOGA("TTY window size: %d rows, %d columns.", R, C);
  console_wait_sec(Cfg.timeout_sec * 3);

  std::deque<int> numbers { 1, 2, 3, 4, 5 };
  int first = numbers.front();    // 1
  int last = numbers.back();      // 5
  int second = numbers[1];        // 2
  int third = numbers.at(2);      // 3
  std::cout << first << second << third << last << std::endl; // 1235
// =====================================================

  Timer t;
  std::set<string> Mode{MODBUS_MODES};

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

// =======================================================

  init_all();

  OPCclient.init("opc.tcp://localhost:4840");
//  logger_set_queue(true);
//  float f = 1.2345;

// ==================================================================
// ==================================================================
// ==================================================================

  for (;;) {
    logger_set_queue(true);
    if (Console::size_changed())
      Console::clear();
    Console::home();
    fflush(stdout);

    opc_client_();
    opc_server_();

    PRINTF("\n");
    regs_update();
    mb_print_summary();
    mb_print_help();
    fflush(stdout);
    //Console::read_ms(3000);

    Console::scrolling_start();
    int nb_cycles = Cfg.timeout_sec * 1000 / CONSOLE_WAIT_MS;
    string logged_;

    for (int i = 0; i < nb_cycles; i++) {
      int ch = Console::read_ms(CONSOLE_WAIT_MS);
      flush_logger();
      if (ch != -1) {
        logger_set_queue(false);
        parse_char(ch);
        Console::refresh();
        break;
      }
    }

    PRINTF("\n");

  }

  return (EXIT_SUCCESS);
}

// eof
