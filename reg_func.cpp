// reg_func.cpp -----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
#include <string.h>

#include <string>
#include <vector>

#include "./config.h"
#include "./libs.h"

#define MB_READ

// using ::std;
// using ::libconfig;

void reg_init();
void reg_print_name();
// void reg_init_name(string devname, string regname, uint16_t *val);

void reg_init() {
  cout << endl << "===== reg_init =====" << endl;

  for (auto &D : PLCset)
    for (auto &R : D.regs) {
      string reg = (string)D.dev_name + "." + (string)R.rname;
      REGmap[reg] = &R;
      REGmap[reg]->rvalue = 5757;
    }

  /*  Old style cycles
      int nb_plcs = static_cast<int>(PLCset.size());
      cout << "Total PLCs: " << nb_plcs << endl;
      for (int i = 0; i < nb_plcs; ++i) {             // Cycle for PLCs
      for (int j = 0; j < PLCset[i].nb_regs; ++j) { // Cycle for REGs
        PLC &D = PLCset[i];
        reg_init_name(D.dev_name, R.rname, &R.rvalue);
      }
      }
      //  int nb_regs = static_cast<int>(PLCset[i].regs.size());  // old style
  */

  return;
}

/*
    void reg_init_name(string devname, string regname, uint16_t *val) {
    MBreg[devname + "." + regname] = val;
    MBreg[devname + "." + regname] = 5757;
    return;
    }
*/

void reg_print_name() {
  cout << endl << "======= regs_print_name =======" << endl;

  for (const auto &[rn, r] : REGmap) {
    const char *C = KNRM;
    if (r->rstatus < 0)
      C = KRED;

    if (strcmp(r->rtype, "i") == 0)
      printf("%s%-12s %7d\n" NRM, C, rn.c_str(), r->rvalue);
    else
      printf("%s%-12s %7.2f\n" NRM, C, rn.c_str(), (int16_t)r->rvalue * 0.01);
  }

  return;
}

/*
    void reg_print_name_c()
    {
    cout << endl << "======= regs_print_name =======" << endl;
    for (const auto &[rname, ra] : REGmap) {
        if (strcmp(ra->rtype, "i") == 0)
            cout << setw(12) << left << rname << setw(7) << right << ra->rvalue
                 << endl;
        else
            cout << setw(12) << left << rname << setw(7) << right << fixed
                 << setprecision(2) << (int16_t)(ra->rvalue) * 0.01 << endl;
    }
    return;
    }
*/

// eof
