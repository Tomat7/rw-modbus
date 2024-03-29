// ----------------------------------------------------------------------------

#include "config.h"
#include "libs.h"

using namespace std;
using namespace libconfig;

int plc_show1()
{

    int count_PLCs = static_cast<int>(PLCset.size());
    cout << "Total PLCs: " << count_PLCs << endl;

    // ===== Cycle to Show PLCs details =====
    for (int i = 0; i < count_PLCs; ++i) {
        int count_REGs = PLCset[i].reg_qty;
        cout << setw(5) << left << PLCset[i].dev_name << "  " << setw(5) << left
             << PLCset[i].reg_qty << "  " << setw(15) << left << PLCset[i].ip_addr
             << "  " << PLCset[i].poll_interval << "  " << PLCset[i].err_timeout << endl;

        // ===== Cycle to Show registers details =====

        plc_show_regs(i);

        // ===== Next PLC ...
        cout << endl;
    }
    // ===== All PLC done.
    cout << endl;
    regs_init();

    cout << PLCset[0].regs.size() << endl;
    cout << PLCset[1].regs.size() << endl;

    return 0;
}

void plc_show_regs(int i)
{
    int nbregs = static_cast<int>(PLCset[i].regs.size());
    cout << nbregs << endl;

    for (int j = 0; j < nbregs; ++j) {
        string devn = PLCset[i].dev_name;
        string regn = PLCset[i].regs[j].ch_name;
        MBreg[devn + "." + regn] = &PLCset[i].regs[j].rvalue;

        cout << "       " << setw(9) << left << PLCset[i].regs[j].ch_name << setw(3)
             << right << PLCset[i].regs[j].raddr << setw(7) << right
             << PLCset[i].regs[j].rvalue << "  " << left
             << PLCset[i].regs[j].ch_mode << " " << endl;

        *MBreg[devn + "." + regn] = 5757;
    }
    return;
}

void regs_init()
{
    for (const auto &[rname, rval] : MBreg)
        cout << rname << " " << *rval << endl;
}

// eof
