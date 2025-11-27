// main.cpp ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <locale>  // tolower
#include <map>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <iostream>

#include "config.h"
#include "libs.h"

#define TIMER_START_MSG nullptr
// using namespace libconfig;

void init_all()
{
  Timer t;
  int ret = 0;

  //  Console::save();
  //t.start(TIMER_START_MSG);
  ret = cfg_master(CFG_DIR, CFG_FILE, Cfg.mode);
  //t.spent_auto("============ Cfg Master finished in: ");
  if (ret == EXIT_FAILURE)
    exit(EXIT_FAILURE);
  console_wait_sec(Cfg.timeout_sec);
  // ==================================

  /*   t.start(TIMER_START_MSG);
    ret = cfg_slave(CFG_DIR, CFG_FILE, "slave");
    t.spent_auto("=== Cfg Slave finished in: ");
    if (ret == EXIT_FAILURE)
      exit(EXIT_FAILURE);
    wait_console(Cfg.timeout_sec); */
  // ==================================

  if (log_level > 5) {
    //t.start(TIMER_START_MSG);
    plc_show2();
    console_wait_sec(Cfg.timeout_sec);
    //t.spent_auto("=== PLC2 show finished in: ");
  }

  if (log_level > 3) {
    //t.start(TIMER_START_MSG);
    plc_show1();
    //t.spent_auto("=== PLC1 show finished in: ");
    console_wait_sec(Cfg.timeout_sec);
  }

  /*   t.start(TIMER_START_MSG);
    regs_create_from_masters();
    t.spent_auto("=== REG init finished in: ");
    wait_console(Cfg.timeout_sec);
    //  t.sleep_ms(TMOUT); */

  //t.start(TIMER_START_MSG);
  netsvc_init();
  opc_init();
  opc_regs_init();
  //t.spent_auto("=== OPC init finished in: ");   Console_c::c::wait(Cfg.timeout_sec);

  //t.start(TIMER_START_MSG);
  tasks_init();
  //t.spent_auto("=== TASKS init finished in: ");   Console_c::c::wait(Cfg.timeout_sec);

  tasks_start();
  opc_start();
  netsvc_start();
  //  Cfg.timeout_sec = TIMEOUT_SEC;

  return;
}

void reinit()
{
  deinit_all();
  LOGW("+++++++++++++++++++++++++++++++++");
  console_wait_sec(Cfg.timeout_sec);
  init_all();

  //  Cfg.timeout_sec = TIMEOUT_SEC;
  return;
}

void deinit_all()
{
  Task.stop();
  regs_deinit();
  opc_deinit();
  netsvc_deinit();
  LOGD("opc_deinit() - done");
  //  regs_deinit_shm();
  LOGD("regs_deinit() - done");
  mb_deinit();
  LOGD("mb_deinit() - done");
  Console::restore();
}

void parse_char(int ch)
{
  int loglvl;

  if ((char)ch == 'Q') {
    LOGA("Char 'Q' pressed. Correct exit. Bye.\n");
    console_wait_sec(Cfg.timeout_sec);
    deinit_all();
    exit(EXIT_SUCCESS);
  } else if ((char)ch == 'C') {
    LOGA("Char 'C' pressed. Full reconfiguration.\n");
    console_wait_sec(Cfg.timeout_sec);
    reinit();
  } else if (((char)ch == 'F') || ((char)ch == 'f')) {
    Cfg.timeout_sec = 1;
    LOGA("Char 'F' pressed. Timeout set to: %d sec.\n", Cfg.timeout_sec);
    console_wait_sec(Cfg.timeout_sec);
  } else if (((char)ch == 'S') || ((char)ch == 's')) {
    Cfg.timeout_sec = 5;
    LOGA("Char 'S' pressed. Timeout set to: %d sec.\n", Cfg.timeout_sec);
    console_wait_sec(Cfg.timeout_sec);
  } else if ((char)ch == 'M') {
    Cfg.show_mb_regs = !Cfg.show_mb_regs;
    LOGA("Char 'M' pressed. Hide/unhide Modbus registers.\n", Cfg.timeout_sec);
    console_wait_sec(Cfg.timeout_sec);
    fflush(stdout);
  } else if ((char)ch == 'R') {
    Cfg.show_regs = !Cfg.show_regs;
    LOGA("Char 'R' pressed. Hide/unhide ANY registers.\n", Cfg.timeout_sec);
    console_wait_sec(Cfg.timeout_sec);
    fflush(stdout);
  } else if (isdigit((char)ch)) {
    loglvl = (char)ch - '0';  // new loglevel
    log_level = 2;
    LOGA("Digit pressed. Logging Level changed to '%d'.\n", loglvl);
    log_level = loglvl;
    console_wait_sec(Cfg.timeout_sec);
  } else if ((char)ch == ' ')
    PRINTF("%s %s %s \n", C_GRN, "=============================", C_NORM);
  else {
    PRINTF("Wow! What to do with: %s '%c'? %s \n", C_BLU, (char)ch, C_NORM);
    console_wait_sec(Cfg.timeout_sec * 2);
  }
}

void flush_logger()
{
  static size_t max_len;
  string logger_str;

  while (logger_get_string(logger_str)) {
    time_t timestamp = time(NULL);
    struct tm* local_tm = localtime(&timestamp);
    char time_str[22];
    strftime(time_str, 22, "%d.%m.%Y %H:%M:%S ", local_tm);

    string print_str = C_DARK;
    print_str.append(time_str);
    print_str.append(logger_str);

    size_t _len = print_str.length();

    if (_len < max_len) {
      print_str.pop_back();
      print_str.append(max_len - _len, ' ');
      print_str.append("\n");
    } else
      max_len = _len;

    Console::lines_add(print_str);
  }
}

/*
  void flush_logger()
  {
  string logged_;
  while (logger_get_string(logged_))
    Console::lines_add(logged_);

  //  logger_flush_printf();
  //  fflush(stdout);
  }
*/

void console_wait_sec(int _sec) { Console::read_sec(_sec); }

/* int write_shm(string rn, uint16_t val)
  {
  regdata_t* ptr_shm = nullptr;
  int fd = get_shm_fd(rn.c_str());
  LOGD("Reg: %s, got FD: %d", rn.c_str(), fd);

  if (fd == -1) {
    LOGE("Can't get_shm_fd: %s", rn.c_str());
    return -1;
  } else {
    ptr_shm = (regdata_t*)get_shm_addr(fd, sizeof(regdata_t));
    if (ptr_shm == nullptr) {
      close_fd(fd);
      LOGE("Can't get pointer to: %s\n", rn.c_str());
      return -1;
    }
  }

  LOGD("R:%s, FD: %d, addr: %x.", rn.c_str(), fd, ptr_shm);

  regdata_t rdata;
  memcpy(&rdata, ptr_shm, sizeof(regdata_t));
  rdata.rvalue = val;
  memcpy(ptr_shm, &rdata, sizeof(regdata_t));

  LOGD("R:%s, FD: %d. Finished - OK", rn.c_str(), fd);

  int rc = close_shm(fd, ptr_shm, sizeof(regdata_t));
  if (rc != 0)
    LOGE("Can't close_fd: %s", rn.c_str());

  LOGC("close_shm1: %x", ptr_shm);
  rc = close_shm(fd, ptr_shm, sizeof(regdata_t));
  LOGC("close_shm2: %x", ptr_shm);

  return rc;
  }
*/

/*
  int write_rm(string rn, uint16_t val)
  {
  //  REGmap[rn].set_shm_val(val);
  REGmap[rn].set_local(val);
  return 0;
  }
*/

bool isdebug() { return (log_level > 7); }

string to_lower(string str)
{
  for (auto &c : str)
    c = static_cast<char>(tolower(c));
  return str;
}

// eof
