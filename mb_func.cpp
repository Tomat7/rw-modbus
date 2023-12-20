// ----------------------------------------------------------------------------
#include <vector>

#include "config.h"
#include "libs.h"

using namespace std;
using namespace libconfig;

int mb_show()
{

  cout << endl << "======= mb_show =======" << endl;

  int count_PLCs = static_cast<int>(PLCset.size());
  cout << "Total PLCs: " << count_PLCs << endl;

  // ===== Cycle to Show PLCs details =====
  for (int i = 0; i < count_PLCs; ++i)
  {
    int count_REGs = PLCset[i].nb_regs;
    cout << setw(5) << left << PLCset[i].dev_name << "  " << setw(5) << left
         << PLCset[i].nb_regs << "  " << setw(15) << left << PLCset[i].ip_addr
         << "  " << PLCset[i].poll_interval << "  " << PLCset[i].err_timeout << endl;

    // ===== Cycle to Show registers details =====
    mb_show_regs_full(i);

    // ===== Next PLC ...
    cout << endl;
  }
  // ===== All PLC done.
  mb_show_regs_name();
  
//  cout << PLCset[0].regs.size() << endl;
//  cout << PLCset[1].regs.size() << endl;
  
  return 0;
}

void mb_show_regs_full(int i)
{
  int nbregs = static_cast<int>(PLCset[i].regs.size());
//  cout << nbregs << endl;

  for (int j = 0; j < nbregs; ++j)
  {
    string devn = PLCset[i].dev_name;
    string regn = PLCset[i].regs[j].rname;
    MBreg[devn + "." + regn] = &PLCset[i].regs[j].rvalue;

    cout << "       " << setw(9) << left << PLCset[i].regs[j].rname << setw(3)
         << right << PLCset[i].regs[j].raddr << setw(7) << right
         << PLCset[i].regs[j].rvalue << "  " << left
         << PLCset[i].regs[j].rmode << " " << endl;

    *MBreg[devn + "." + regn] = 5757;
  }
  return;
}

void mb_show_regs_name()
{
  cout << endl << "======= mb_show_regs_name =======" << endl;
  for (const auto &[rname, rval] : MBreg)
    cout << "  " << setw(12) << left << rname << setw(7) << right << *rval << endl;
}

// eof
