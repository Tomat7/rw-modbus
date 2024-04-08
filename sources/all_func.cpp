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
  ret = cfg_master(CFG_DIR, CFG_FILE);
  t.spent_auto("============ Cfg finished in: ");
  if (ret == EXIT_FAILURE)
    exit(EXIT_FAILURE);
  wait_console(TIMEOUT_SEC);

  plc_show2();
  printf("===222\n");
  wait_console(TIMEOUT_SEC);

  t.start();
  plc_show1();
  t.spent_auto("============ PLC show finished in: ");
  wait_console(TIMEOUT_SEC);

  t.start();
  regs_init();
  t.spent_auto("============ REG init finished in: ");
  wait_console(TIMEOUT_SEC);
  //  t.sleep_ms(TMOUT);

  return;
}

void reinit()
{
  REGmap.clear();
  PLCset.clear();
  init_all();
}

int write_shm(string rn, uint16_t val)
{
  regdata_t* ptr_shm = nullptr;
  int fd = get_shm_fd(rn.c_str());
  LOGI("Reg: %s, got FD: %d", rn.c_str(), fd);

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

  LOGD("%s(): R:%s, FD: %d, addr: %x.", __func__, rn.c_str(), fd, ptr_shm);

  regdata_t rdata;
  memcpy(&rdata, ptr_shm, sizeof(regdata_t));
  rdata.rvalue = val;
  memcpy(ptr_shm, &rdata, sizeof(regdata_t));

  LOGD("%s(): R:%s, FD: %d. Finished - OK", __func__, rn.c_str(), fd);

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
