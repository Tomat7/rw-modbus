// reg_func.cpp -----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <string.h>

#include <map>
#include <string>
#include <vector>

#include "config.h"
#include "libs.h"

#define MB_READ


void regs_create_from_plc()
{
  printf("\n======= %s =======\n", __func__);

  for (auto &D : PLCvec) {
    for (auto &[a, R] : D.regs) {
      //if (R.str_source == "" || R.str_source == "-") {
      // LOGI("(Master) try to create %s, src: %s",
      //     R.rfullname.c_str(), R.str_source.c_str());
      REGmap[R.rfullname] = {&R, &D};
      //}
    }
  }

  return;
}

bool reg_exist(string _rn)
{
  return REGmap.count(_rn);
}


void regs_deinit()
{
  REGmap.clear();
  return;
}


/*
  void regs_create(PLC_c* D)
  {
  for (auto &[a, r] : D->regs) {
    if (r.str_source == "" || r.str_source == "-") {
      LOGI("(Master) try to create %s, src: %s",
           r.fullname.c_str(), r.str_source.c_str());
      REGmap[r.fullname] = {&r, D};
    }
  }
  }
*/


/*
  for (auto &D : PLCvec) {
    for (auto &[a, R] : D.regs) {
      if (R.str_source != "" && R.str_source != "-") {
        auto &Rsrc = REGmap[R.str_source];
        LOGN("(Referenced) try to create %s, src: %s",
             R.rfullname.c_str(), R.str_source.c_str());
        LOGD("- %s %d", __func__, 0);
        R.str_mode = Rsrc.ptr_reg->str_mode;
        R.str_type = Rsrc.ptr_reg->str_type;
        LOGD("- %s %d", __func__, 00);
        REGmap[R.rfullname] = {&R, &D};
      }
    }
  }
*/


/* void regs_deinit_shm()
  {
  for (auto &[n, rm] : REGmap) {
    close_shm(rm.fd, rm.ptr_data_shm, sizeof(regdata_t));
    unlink_shm(n.c_str());
  }
  return;
  }
*/


// REGmap[R.fullname].ptr_reg->str_title = D.str_title;
/*         R.str_mode = REGmap[R.str_source].ptr_reg->str_mode;
        R.str_type = REGmap[R.str_source].ptr_reg->str_type;
        R.data.rmode = REGmap[R.str_source].ptr_data_plc->rmode;
        R.data.rtype = REGmap[R.str_source].ptr_data_plc->rtype; */

/* void regs_create_from_slave()
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
*/

/*
  void regs_init_shm()
  {
  printf("\n===== reg_init =====\n");

  for (auto &D : PLCvec)
    for (auto &[a, R] : D.regs) {
      LOGD("(Master) try to create %s", R.fullname.c_str());
      REGmap[R.fullname] = {R.fullname};
    }
  return;
  }
*/

// eof
