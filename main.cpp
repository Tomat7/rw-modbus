// main.cpp ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <map>
#include <set>
#include <string>
#include <vector>
#include <variant>

#include "./config.h"
#include "./libs.h"

// using namespace std;
// using namespace libconfig;

map<string, Reg_c> REGmap;
vector<PLC_c> PLCvec;
PLC_c Slave(1502);
OpcServer_c OPCs(4840);
Schedule_c Task;

// INotify IN(CFG_DIR);

const char* mode = "master";
int timeout_sec = TIMEOUT_SEC;
int rc;

template <typename T>
T ReadOpcChannel(string s);
template <>
uint16_t ReadOpcChannel(string s) { return OPCs.getValue(s).ui16; }
template <>
int16_t ReadOpcChannel(string s) { return OPCs.getValue(s).i16; }
template <>
uint32_t ReadOpcChannel(string s) { return OPCs.getValue(s).ui32; }
template <>
int32_t ReadOpcChannel(string s) { return OPCs.getValue(s).i32; }
template <>
uint64_t ReadOpcChannel(string s) { return OPCs.getValue(s).ui64; }
template <>
int64_t ReadOpcChannel(string s) { return OPCs.getValue(s).i64; }
template <>
float ReadOpcChannel(string s) { return OPCs.getValue(s).fl; }

struct ReadValue {
  string _s;
  ReadValue(string ss) : _s(ss) {}
  template <typename T>
  operator T() { return ReadOpcChannel<T>(_s); }
};


int test_()
{
  LOGC("Test task. Done.\n");
  return 1;
}

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

  Task.add_task(test_, 500);
  Task.run();

  std::thread opc_thr(opc_run);
  opc_thr.detach();
  wait_console(timeout_sec);


  for (;;) {
    printf("%s", CLS);
    printf("%s", HOME);
    fflush(stdout);
    /*
        i++;
        string s;
        s = "/PLC/Kub/Kub.millis";
        OPCs.setVar(s, i);
        s = "/PLC/Kub/Kub.Temp1";
        OPCs.setVar(s, (float)i);
    */
    string s;
    s = "/PLC/Kub/Kub.millis";
    printf("Millis: %d, ", OPCs.getValue(s).ui16);

    s = "/PLC/Kub/Kub.Temp1";
    printf("T1: %5.2f, ", OPCs.getValue(s).fl);

    s = "/PLC/Kub/Kub.Temp2";
    printf("T2: %5.2f, ", ReadOpcChannel<float>(s));

    s = "/PLC/Kub/Kub.Temp3";
    float myfl = ReadValue(s);
    printf("T3: %5.2f, ", myfl);

    s = "/PLC/Buf/Buf.Temp3";
    printf("T4: %d, ", (int16_t)ReadValue(s));

    printf("\n");

    t.start();
    regs_update();
    t.spent_auto("============ REG print finished in: ");

    const char* x = nullptr;
    t.start(x);
    mb_update();
    // LOGD("regdata_t size: %d", sizeof(regdata_t));
    // LOGD("P array size: %d", sizeof(P));
    t.spent_auto("============ MB update: spent on ALL PLCs by TCP: ");

    // Slave.handle_slave(timeout_sec * 1000000);

    int ch = read_console(0, timeout_sec * 1000000);
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
