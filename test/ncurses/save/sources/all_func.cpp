// main.cpp ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <map>
#include <string>
#include <vector>

#include "./config.h"
#include "./libs.h"

// using namespace std;
// using namespace libconfig;

void init_all()
{
  Timer t;
  int ret = 0;

  t.start();
  PRINTF("TEST 111111111111");
  ret = cfg_master(CFG_DIR, CFG_FILE);
  t.spent_auto("============ Cfg finished in: ");
  PRINTF("TEST 222222222222");
  refresh();
  
  if (ret == EXIT_FAILURE)
    exit(EXIT_FAILURE);
  wait_console(timeout_sec);
  sleep(3);

  plc_show2();
  PRINTF("=============   33333333333\n");
  wait_console(timeout_sec);
  sleep(3);

  t.start();
  plc_show1();
  t.spent_auto("============ PLC show finished in: ");
  wait_console(timeout_sec);
  sleep(3);

  t.start();
  regs_init();
  t.spent_auto("============ REG init finished in: ");
  wait_console(timeout_sec);
  //  t.sleep_ms(TMOUT);
  sleep(3);

  return;
}

void reinit()
{
  deinit_all();
  wait_console(timeout_sec);
  init_all();
}

void deinit_all()
{
  regs_deinit_shm();
  regs_deinit();
  plc_deinit();
}

void parse_char(int ch)
{
  int loglvl;

  if (((char)ch == 'e') || ((char)ch == 'q')) {
    LOGC("Char 'e' or 'q' pressed. Correct shutdown. Bye.\n");
    wait_console(timeout_sec);
    deinit_all();
    exit(EXIT_SUCCESS);
  } else if ((char)ch == 'r') {
    LOGC("Char 'r' pressed. Full reconfiguration.\n");
    wait_console(timeout_sec);
    reinit();
  } else if (isdigit((char)ch)) {
    loglvl = (char)ch - '0'; // new loglevel
    log_level = 2;
    LOGC("Digit pressed. Logging Level changed to '%d'.\n", loglvl);
    log_level = loglvl;
    wait_console(timeout_sec);
  } else if ((char)ch == ' ')
    PRINTF("%s %s %s \n", KGRN, "=============================", KNRM);
  else {
    PRINTF("Wow! What to do with: %s '%c'? %s \n", KBLU, (char)ch, KNRM);
    wait_console(timeout_sec * 2);
  }
}

int write_shm(string rn, uint16_t val)
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

  return rc;
}

int write_rm(string rn, uint16_t val)
{
  //  REGmap[rn].set_shm_val(val);
  REGmap[rn].set_local(val);

  return 0;
}

// eof
