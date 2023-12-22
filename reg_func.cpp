// ----------------------------------------------------------------------------
#include <vector>

#include "config.h"
#include "libs.h"

using namespace std;
using namespace libconfig;

void reg_init();
void reg_print_name();
void reg_init_name(string devname, string regname, uint16_t *val);

/*
int plc_show() {
  cout << endl << "======= mb_show =======" << endl;
  // ===== Print ALL PLCs detailed.
  plc_show_full();
  // ===== Print ALL REGs names.
  reg_init();
  //  cout << PLCset[0].regs.size() << endl;
  //  cout << PLCset[1].regs.size() << endl;
  return 0;
}
*/

void reg_init() {
  cout << endl << "===== reg_init =====" << endl;
  int nb_plcs = static_cast<int>(PLCset.size());
  cout << "Total PLCs: " << nb_plcs << endl;

  for (int i = 0; i < nb_plcs; ++i) {             // Cycle for PLCs
    for (int j = 0; j < PLCset[i].nb_regs; ++j) { // Cycle for REGs
      PLC &D = PLCset[i];
      reg_init_name(D.dev_name, D.regs[j].rname, &D.regs[j].rvalue);
    }
//    cout << endl;
  }
  //  int nb_regs = static_cast<int>(PLCset[i].regs.size());
  // ===== Cycle to Show registers details =====
  //  for (int j = 0; j < nb_regs; ++j) {

  //    reg_fill_name(PLCset[i].dev_name, PLCset[i].regs[j].rname,
  //                  &PLCset[i].regs[j].rvalue);
  return;
}

void reg_init_name(string devname, string regname, uint16_t *val) {
  MBreg[devname + "." + regname] = val;
  *MBreg[devname + "." + regname] = 5757;
  return;
}

void reg_print_name() {
  cout << endl << "======= regs_print_name =======" << endl;
  for (const auto &[rname, rval] : MBreg)
    cout << "  " << setw(12) << left << rname << setw(7) << right << *rval
         << endl;
  return;
}




// eof
