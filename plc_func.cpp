// ----------------------------------------------------------------------------
#include "config.h"
#include "libs.h"
#include <vector>

using namespace std;
using namespace libconfig;

void plc_show_regs(int i);
void plc_print_details(int i);
void plc_print_reg_details(int i, int j);

int plc_show() {
  cout << endl << "===== plc_show =====" << endl;

  int nb_plcs = static_cast<int>(PLCset.size());
  cout << "Total PLCs: " << nb_plcs << endl;

  for (int i = 0; i < nb_plcs; ++i) { // Cycle for PLCs
    plc_print_details(i);
    //    int nb_regs = static_cast<int>(PLCset[i].regs.size());

    for (int j = 0; j < PLCset[i].nb_regs; ++j) // Cycle for REGs
      plc_print_reg_details(i, j);

    cout << endl;
  }

  return 0;
}

/*
void plc_show_regs(int i) {
  int nb_regs = static_cast<int>(PLCset[i].regs.size());
  // ===== Cycle to Show registers details =====
  for (int j = 0; j < nb_regs; ++j) {
        reg_fill_name(PLCset[i].dev_name, PLCset[i].regs[j].rname,
                         &PLCset[i].regs[j].rvalue);

    plc_print_reg_details(i, j);
  }
  return;
}
*/

void plc_print_details(int i) {
  cout << setw(5) << left << PLCset[i].dev_name << "  " << setw(5) << left
       << PLCset[i].nb_regs << "  " << setw(15) << left << PLCset[i].ip_addr
       << "  " << PLCset[i].mb_interval << "  " << PLCset[i].mb_timeout << endl;
  return;
}

void plc_print_reg_details(int i, int j) {
  cout << "       " << setw(9) << left << PLCset[i].regs[j].rname << setw(3)
       << right << PLCset[i].regs[j].raddr << setw(7) << right
       << PLCset[i].regs[j].rvalue << "  " << left << PLCset[i].regs[j].rmode
       << " " << endl;
  return;
}

// eof
