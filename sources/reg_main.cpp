// reg_func.cpp -----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <string.h>

#include <map>
#include <string>
#include <vector>

#include "config.h"
#include "libs.h"

#define MB_READ

/*
  void replace_str(string& str, const string& from, const string& to)
  {
  size_t start_pos = 0;
  // Цикл ищет подстроку 'from' до тех пор, пока она не перестанет встречаться
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length(); // Продвигаем позицию, чтобы избежать бесконечного цикла
  }
  }
*/

void regs_create_from_plc()
{
  printf("\n======= %s =======\n", __func__);

  for (auto &D : PLCvec) {
    for (auto &[a, r] : D.regs) {
      REGmap[r.rfullname] = {&r, &D};
    }
//    mbreg_t status_reg = D.regs[0];
//    replace_str(status_reg.rfullname, ".millis", ".status");
//  string reg_status_name = "/PLC/" + D.str_dev_name + "/";
//  reg_status_name += D.str_dev_name + ".status";
//    REGmap[status_reg.rfullname] = {&status_reg, &D, "i"};
//    REGmap[status_reg.rfullname].str_type = "i";
//    REGmap[status_reg.rfullname].var_type_ua = 3;
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
