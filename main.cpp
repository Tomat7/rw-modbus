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

// using namespace std;
// using namespace libconfig;

cfg_t Cfg;
map<string, Reg_c> REGmap;
vector<PLC_c> PLCvec;
// PLC_c Slave(MB_SLAVE_PORT);
OpcClient_c OPCclient;
OpcServer_c OPCs;
Schedule_c Task /* (TASKS_NB_MAX) */;
// string PLC_folder = PLC_FOLDER;
// string SCADA_folder = OPC_SCADA_FOLDER;

// INotify IN(CFG_DIR);

const char* mode = "master";
// int timeout_sec = TIMEOUT_SEC;
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

  set<string> Mode{MODBUS_MODES};
  signal(SIGINT, close_sigint);
  openlog("Modbus", LOG_NDELAY, LOG_LOCAL1);
  // log_level = 3;

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

  init_all();

  OPCclient.init("opc.tcp://localhost:4840");
  uint16_t cnt = 0;
  uint16_t ccc = 0;
  // logger_set_queue(true);

  for (;;) {
    logger_set_queue(true);
    printf("%s", ESC_CLS);
    printf("%s", ESC_HOME);
    fflush(stdout);

    cnt++;
    string s = OPCs.GetVarFullName("Millis");

    t.start();
    OPCclient.ReadNumber(s, ccc);
    t.spent_auto("OPC Client read ONE reg in: ");

    if (OPCclient.WriteNumber(s, cnt)) {
      OPCclient.ReadNumber(s, ccc);
      OPCs.RefreshAllValues();
      printf("%s: %d %d %d\n", s.c_str(), ccc, cnt, OPCs.ReadRawUnion(s).ui16);
    } else
      printf("%s: %d %d %d error!\n", s.c_str(), ccc, cnt, OPCs.ReadRawUnion(s).ui16);

    /*
        i++;
        string s;
        s = "/PLC/Kub/Kub.millis";
        OPCs.setVar(s, i);
        s = "/PLC/Kub/Kub.Temp1";
        OPCs.setVar(s, (float)i);
    */

    s = "/PLC/Kub/Kub.millis";
    printf("Kub.millis: %d, ", OPCs.ReadRawUnion(s).ui16);

    s = "/PLC/Kub/Kub.Temp1";
    printf("T1: %5.2f, ", OPCs.ReadRawUnion(s).fl);

    s = "/PLC/Kub/Kub.Temp2";
    float fl;
    OPCs.ReadNumber(s, fl);
    printf("T2: %5.2f", fl);

    s = "Kub.Temp3";
    float myfl = ReadValue(s);
    const char* C = getColor(OPCs.isVariable(s));
    const char* B = getBlynk(OPCs.isGood(s));
    printf("%sT3a: %s%5.2f%s, ", C, B, myfl, NRM);

    s = "Kub.Temp3";
    myfl = ReadValue(s);
    C = getColor(OPCs.isVariable(s));
    B = getBlynk(OPCs.isGood(s));
    printf("%sT3b: %s%5.2f%s, ", C, B, myfl, NRM);
    // printf("T4: %5.3f, ", myfl /*(float)ReadValue(s)*/);
    int16_t t16 = (int16_t)round(myfl * 100);
    myfl = t16 / 100;

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
