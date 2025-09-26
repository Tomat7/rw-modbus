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
    for (auto &[a, r] : D.regs) {
      REGmap[r.rfullname] = {&r, &D};
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

// eof
