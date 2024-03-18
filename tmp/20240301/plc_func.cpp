// plc_func.cpp --------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <vector>

#include "./config.h"
#include "./libs.h"

// using namespace std;
// using namespace libconfig;

void plc_show_regs(int i);
void plc_print_details(int i);
void plc_print_reg_details(int i, int j);

int plc_show1() {
  cout << endl << "===== plc_show =====" << endl;

  int nb_plcs = static_cast<int>(PLCset.size());
  cout << "Total PLCs: " << nb_plcs << endl;

  for (int i = 0; i < nb_plcs; ++i) { // Cycle for PLCs
    plc_print_details(i);

    for (int j = 0; j < PLCset[i].reg_qty; ++j) // Cycle for REGs
      plc_print_reg_details(i, j);

    cout << endl;
  }

  return 0;
}

void plc_print_details(int i) {
  cout << setw(5) << left << PLCset[i].dev_name << "  " << setw(5) << left
       << PLCset[i].reg_qty << "  " << setw(15) << left << PLCset[i].ip_addr
       << "  " << PLCset[i].mb.interval_ms << "  " << PLCset[i].mb.timeout_us
       << endl;
  return;
}

void plc_print_reg_details(int i, int j) {
  cout << "       " << setw(9) << left << PLCset[i].regs[j].ch_name << setw(3)
       << right << PLCset[i].regs[j].raddr << setw(7) << right
       << PLCset[i].regs[j].rvalue << "  " << left << PLCset[i].regs[j].ch_mode
       << " " << endl;
  return;
}

// eof
