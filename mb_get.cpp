// ----------------------------------------------------------------------------
#include <vector>

#include "config.h"
#include "libs.h"

using namespace std;
using namespace libconfig;

void mb_get_plcs_full();
void mb_get_regs_byname();


int mb_get() {
  cout << endl << "======= mb_get =======" << endl;
  // ===== Get ALL PLCs details.
  mb_get_plcs_full();

  // ===== Print ALL REGs names.
  mb_get_regs_byname();

  return 0;
}

void mb_get_plcs_full() {
  int nb_plcs = static_cast<int>(PLCset.size());
  cout << "Total PLCs: " << nb_plcs << endl;

  for (int i = 0; i < nb_plcs; ++i) {
    //    mb_set_response_timeout(i);
    PLCset[i].read();
  }

  return;
}


void mb_get_regs_byname() {
  cout << endl << "======= mb_get_regs_byname =======" << endl;
  for (const auto &[rname, rval] : MBreg)
    cout << "  " << setw(12) << left << rname << setw(7) << right << *rval
         << endl;
  return;
}


// eof
