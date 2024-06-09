// reg_func.cpp -----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <string.h>

#include <map>
#include <string>
#include <vector>

#include "./config.h"
#include "./libs.h"

#define MB_READ

void regs_create_from_masters()
{
  printf("\n===== reg_init_from_masters =====\n");

  for (auto &D : PLCvec)
    for (auto &[a, R] : D.regs) {
      LOGD("(Master) try to create %s", R.fullname.c_str());
      // RegMap_c rm(&R);
      // REGmap[R.fullname] = rm;
      REGmap[R.fullname] = {&R};
    }
  return;
}

void regs_create_from_slave()
{
  printf("\n===== reg_init_from_slave =====\n");

  for (auto &[a, R] : Slave.regs) {
    LOGD("(Slave) try to create %s", R.fullname.c_str());
    if (!reg_exist(R.fullname))
      REGmap[R.fullname] = {&R};
    else
      LOGW("(Slave) REG %s already exist", R.fullname.c_str());
  }
  return;
}

void regs_init_shm()
{
  printf("\n===== reg_init =====\n");

  for (auto &D : PLCvec)
    for (auto &[a, R] : D.regs) {
      LOGD("(Slave) try to create %s", R.fullname.c_str());
      REGmap[R.fullname] = {R.fullname};
    }
  return;
}

bool reg_exist(string _rn)
{
  return REGmap.count(_rn);
}

void regs_deinit_shm()
{
  for (auto &[n, rm] : REGmap) {
    close_shm(rm.fd, rm.ptr_data_shm, sizeof(regdata_t));
    unlink_shm(n.c_str());
  }
  return;
}

void regs_deinit()
{
  REGmap.clear();
  return;
}

// eof
