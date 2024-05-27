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
std::map<string, uint64_t> PLCmap;
std::vector<PLC_c> PLCset;
std::vector<string> SHset {"SF45", "SF47"} ;

// INotify IN(CFG_DIR);

regdata_t* P;
int rc;
bool isNight = true;

static void close_sigint(int dummy)
{
  regs_deinit();
  restore_console();
  LOGN("Exit by Ctrl-C. Bye.\n");
  closelog();
  exit(dummy);
}

// int main(int argc, char **argv) {

int main()
{
//  Timer t;
  int ret = 0;
  uint64_t i = 0;

  signal(SIGINT, close_sigint);
  openlog("Modbus", LOG_NDELAY, LOG_LOCAL1);

  ret = cfg_master(CFG_DIR, CFG_FILE);
  if (ret == EXIT_FAILURE)
    exit(EXIT_FAILURE);
//  wait_console(TIMEOUT_SEC);
//  init_all();

//  for (;;) {
//  printf("%s", CLS);
//  printf("%s", HOME);
  fflush(stdout);

  uint64_t nb_plcs = PLCset.size();

  for (i = 0; i < nb_plcs; i++)
    PLCmap[PLCset[i].str_dev_name] = i;

//    t.start();
//    regs_update();
//    t.spent_a
//  uto("============ REG print finished in: ");

//    const char* x = nullptr;
//    t.start(x);
  mb_update();
//    LOGD("regdata_t size: %d", sizeof(regdata_t));
//    LOGD("P array size: %d", sizeof(P));
//    t.spent_auto("============ MB update: spent on ALL PLCs by TCP: ");

  for (auto &S : SHset) {
    PLC_c &D = PLCset[PLCmap[S]];
//    regdata_t &rd = D.regs[0].data;
    if ((isNight) && (D.get_reg("open2") < 65500)) {
      LOGD("%s %s %d", D.ip_addr, D.dev_name, D.get_reg("millis"));
      D.set_reg("millis", 0);
      D.set_reg("open2", 65005);
      D.set_reg("close2", 1);
    }
  }


  mb_write();

  /*
      int ch = read_console(TIMEOUT_SEC);
      if (ch != -1)
        parse_char(ch);
      else
        printf("!\n");
  */
  fflush(stdout);

  //    wait_console(TIMEOUT_SEC);

  //    t.start(x);
  //    t.sleep_ms(TMOUT);
  //    t.spent();
//  }

  //   getr();

  return (EXIT_SUCCESS);
}

// eof
