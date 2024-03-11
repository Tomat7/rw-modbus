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

#define TMOUT 987

std::map<string, rmap_t> REGmap;
std::vector<PLC> PLCset;
Timer t;
INotify IN(CFG_DIR);
INotify GETCH(STDIN_FILENO);

int rc;

static void close_sigint(int dummy) { 
    LOGERR("Exit by Ctrl-C. Bye.\n");
    exit(dummy); }

// int main(int argc, char **argv) {


void init_all() {
  int ret = 0;
  t.start();
  ret = cfg_read(CFG_DIR, CFG_FILE);
  t.spent_auto("============ Cfg finished in:");
  if (ret == EXIT_FAILURE)
    exit(EXIT_FAILURE);
  t.sleep_ms(TMOUT);

  //  /*
  for (auto &D : PLCset)
    for (auto &R : D.regs) {
      printf("%s: %s.%s (%d) %d   [%s]\n", D.ip_addr, D.dev_name, R.ch_name,
             R.raddr, R.rvalue, R.fullname.c_str());
    }
  printf("===222\n");
  t.sleep_ms(TMOUT);
  //  */

  t.start();
  plc_show();
  t.spent_auto("============ PLC show finished in: ");
  t.sleep_ms(TMOUT);

  t.start();
  regs_init();
  t.spent_auto("============ REG init finished in: ");
  t.sleep_ms(TMOUT);

  return;
}

void reinit()
{
    REGmap.clear();
    PLCset.clear();
    init_all();
}

int main() {

  signal(SIGINT, close_sigint);

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

//    int ch = read_console();
    int ch = GETCH();
    if (ch != -1)
	if ((char)ch == 'e') {
	    LOGERR("Char 'e' pressed. Correct shutdown. Bye.\n");
	    t.sleep_sec(3);
	    return(EXIT_SUCCESS);
	} else if ((char)ch == 'r') {
	    LOGERR("Char 'r' pressed. Full reconfiguration.\n");
	    t.sleep_sec(3);
    	    reinit();
	} else
    	    printf("%s ccc %s \n", KRED, KNRM);
    else
	printf("!\n");
    fflush(stdout);

    t.start(x);
    t.sleep_ms(TMOUT);
    t.spent();
  }

  //   getr();

  return (EXIT_SUCCESS);
}

// eof
