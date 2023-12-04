// ----------------------------------------------------------------------------

#include "config.h"
#include "libs.h"

using namespace std;
using namespace libconfig;

int mb_show(const vector<plc_t> &plcset) {

  int count_PLCs = static_cast<int>(plcset.size());
  cout << "Total PLCs: " << count_PLCs << endl;

  for (int i = 0; i < count_PLCs; ++i) {

    int count_REGs = plcset[i].nb_regs;

    cout << setw(5) << left << plcset[i].preffix << "  " << setw(5) << left
         << plcset[i].nb_regs << "  " << setw(15) << left << plcset[i].ipaddr
         << "  " << plcset[i].polling << "  " << plcset[i].timeout << endl;

    // ===== Show registers details =====
    for (int j = 0; j < count_REGs; ++j) {
      cout << "       " << setw(9) << left << plcset[i].regs[j].rname << setw(3)
           << right << plcset[i].regs[j].raddr << setw(7) << right
           << plcset[i].regs[j].rvalue << "  " << left
           << plcset[i].regs[j].rmode << " " << endl;
    }
    // =====
    cout << endl;
  }
  cout << endl;
  return 0;
}

// eof
