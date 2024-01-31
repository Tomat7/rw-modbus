// ----------------------------------------------------------------------------
#include <vector>

#include "config.h"
#include "libs.h"

using namespace std;
using namespace libconfig;

void plc_show_full();
void plc_show_regs(int i);

void plc_print_details(int i);
void plc_print_reg_details(int i, int j);

void reg_init();
void reg_fill_name(string devname, string regname, uint16_t *val);


int plc_show()
{
    cout << endl << "======= mb_show =======" << endl;
    // ===== Print ALL PLCs detailed.
    plc_show_full();
    // ===== Print ALL REGs names.
    reg_init();
    //  cout << PLCset[0].regs.size() << endl;
    //  cout << PLCset[1].regs.size() << endl;
    return 0;
}

void plc_show_full()
{
    int nb_plcs = static_cast<int>(PLCset.size());
    cout << "Total PLCs: " << nb_plcs << endl;
    // ===== Cycle to Show PLCs details =====
    for (int i = 0; i < nb_plcs; ++i) {
        int count_REGs = PLCset[i].reg_qty;
        plc_print_details(i);
        plc_show_regs(i);
        cout << endl;
    }
    return;
}

void plc_print_details(int i)
{
    cout << setw(5) << left << PLCset[i].dev_name << "  " << setw(5) << left
         << PLCset[i].reg_qty << "  " << setw(15) << left << PLCset[i].ip_addr
         << "  " << PLCset[i].poll_interval << "  " << PLCset[i].err_timeout
         << endl;
    return;
}

void plc_show_regs(int i)
{
    int nb_regs = static_cast<int>(PLCset[i].regs.size());
    // ===== Cycle to Show registers details =====
    for (int j = 0; j < nb_regs; ++j) {
        /*
            string devn = PLCset[i].dev_name;
            string regn = PLCset[i].regs[j].rname;
            &PLCset[i].regs[j].rvalue;
        */
        reg_fill_name(PLCset[i].dev_name, PLCset[i].regs[j].rname,
                      &PLCset[i].regs[j].rvalue);
        plc_print_reg_details(i, j);
    }
    return;
}

void plc_print_reg_details(int i, int j)
{
    cout << "       " << setw(9) << left << PLCset[i].regs[j].rname << setw(3)
         << right << PLCset[i].regs[j].raddr << setw(7) << right
         << PLCset[i].regs[j].rvalue << "  " << left << PLCset[i].regs[j].rmode
         << " " << endl;
    return;
}

void reg_init()
{
    cout << endl << "======= mb_show_regs_name =======" << endl;
    for (const auto &[rname, rval] : MBreg)
        cout << "  " << setw(12) << left << rname << setw(7) << right << *rval
             << endl;
    return;
}

void reg_fill_name(string devname, string regname, uint16_t *val)
{
    MBreg[devname + "." + regname] = val;
    *MBreg[devname + "." + regname] = 5757;
    return;
}

// eof
