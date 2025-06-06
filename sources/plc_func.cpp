// plc_func.cpp --------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <vector>

#include "config.h"
#include "libs.h"

void plc_print_details(int i);
void plc_print_reg_details(int i, int j);

void plc_show2()
{
  printf("\n======= %s =======\n", __func__);

  for (auto &D : PLCvec)
    for (auto &[a, R] : D.regs) {
      LOGI("%s: %s.%-8s (%d) %5d   [%s]", D.ip_addr, D.dev_name, R.ch_name,
           R.raddr, R.data.rvalue, R.rfullname.c_str());
    }
}

void plc_show1()
{
  printf("\n======= %s =======\n", __func__);

  int nb_plcs = static_cast<int>(PLCvec.size());
  LOGN("Total PLCs: %d", nb_plcs);

  for (int i = 0; i < nb_plcs; ++i) {  // Cycle for PLCs
    plc_print_details(i);

    for (auto &[a, R] : PLCvec[i].regs) // Cycle for REGs
      plc_print_reg_details(i, a);
  }

  return;
}

void plc_print_details(int i)
{
  // printf("\n===== plc_print_details =====\n");
  LOGN("%-8s %-4d %-16s %4d %7d", PLCvec[i].dev_name, PLCvec[i].reg_qty,
       PLCvec[i].ip_addr, PLCvec[i].mb.polling_ms, PLCvec[i].mb.timeout_us);
  /*
    cout << setw(7) << left << PLCset[i].str_dev_name << "  " << setw(3) << left
         << PLCset[i].reg_qty << "  " << setw(15) << left << PLCset[i].ip_addr
         << "  " << PLCset[i].mb.interval_ms << "  " << PLCset[i].mb.timeout_us
         << endl;
  */
  return;
}

void plc_print_reg_details(int i, int j)
{
  reg_t &R = PLCvec[i].regs[j];
  LOGI("   %-9s %2d %7d %-s", R.ch_name, R.raddr, R.data.rvalue,
       R.str_mode.c_str());
  /*
    cout << "       " << setw(9) << left << PLCset[i].regs[j].ch_name << setw(3)
         << right << PLCset[i].regs[j].raddr << setw(7) << right
         << PLCset[i].regs[j].data.rvalue << "  " << left
         << PLCset[i].regs[j].str_mode << " " << endl;
  */
  return;
}

// eof
