// cfg_func.cpp --------------------------------------------------------------
#include "config.h"
#include "libs.h"
#include <vector>

using namespace std;
using namespace libconfig;

int cfg_fill_plcset(const Setting &PLCs);

Config cfg;
PLC plcnow;
//reg_t regnow;

// This example reads the configuration file and displays
// some of its contents.

int cfg_read(const char *cfg_file)
{

    // Read the file. If there is an error, report it and exit.
    cout << endl << "======= cfg_read_mbset =======" << endl;

    try {
        cfg.readFile(cfg_file);
        std::cout << "I/O reading file OK: " << cfg_file << std::endl;
    } catch (const FileIOException &fioex) {
        std::cerr << "I/O error while reading file " << cfg_file << std::endl;
        return (EXIT_FAILURE);
    } catch (const ParseException &pex) {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                  << " - " << pex.getError() << std::endl;
        return (EXIT_FAILURE);
    }

    // Get the top name.
    try {
        string name = cfg.lookup("maintitle");
        cout << "Config title: " << name << endl << endl;
    } catch (const SettingNotFoundException &nfex) {
        cerr << "No 'nametitle' setting in configuration file." << endl;
    }

    // Output a list of all PLCs in the inventory.
    try {
        cfg_fill_plcset(cfg.lookup("plc"));
    } catch (const SettingNotFoundException &nfex) {
        cout << "Great ERROR! Exiting." << endl;
        // Ignore.
    }

    return (EXIT_SUCCESS);
}

int cfg_fill_plcset(const Setting &PLCs)
{
    //  const Setting &PLCs = cfg.lookup("plc");
    int nb_plcs = PLCs.getLength();

    // ===== Cycle for PLCs =====
    for (int i = 0; i < nb_plcs; ++i) {
//    plc_t plcnow;
        plcnow.regs.clear();
        const Setting &plc = PLCs[i];
//    const Setting &REGs = plc["regs"];

        const Setting &REGs = PLCs[i]["regs"];
        int nb_regs = REGs.getLength();
        plcnow.nb_regs = nb_regs;

        const char *ptitle; // Just to show
        // ===== Check the record which expect to get for CFG-file.
        if (!(plc.lookupValue("title", ptitle) &&
                plc.lookupValue("name", plcnow.dev_name) &&
                plc.lookupValue("ip", plcnow.ip_addr) &&
                plc.lookupValue("polling", plcnow.poll_interval) &&
                plc.lookupValue("timeout", plcnow.err_timeout))) {
            cout << "Warning!! Error reading PLC configuration: " << i << endl;
            continue; // get out of current cycle iteration if any field wrong in
            // CFG-file
        }

        // ===== Output PLC details
        cout << setw(10) << left << ptitle << "  " << setw(10) << left
             << plcnow.dev_name << "  " << setw(20) << left << plcnow.ip_addr
             << "  " << plcnow.nb_regs << endl;

        // ===== Cycle for REGs =====
        for (int j = 0; j < nb_regs; ++j) {
            const Setting &reg = REGs[j];
            reg_t regnow;

            // ===== Check the record which expect to get for CFG-file.
            if (!(reg.lookupValue("rname", regnow.rname) &&
                    reg.lookupValue("addr", regnow.raddr) &&
                    reg.lookupValue("access", regnow.rmode))) {
                cout << "error reading REG " << j << endl;
                continue;
            }

            regnow.rvalue = 555;
            //	plcnow.reg[regnow.rname] = &regnow.rvalue;
            //	cout << *(plcnow.reg[regnow.rname]) << endl; //   -------------
            plcnow.regs.push_back(regnow);

            // ===== Output REG details
            cout << "       " << setw(9) << left << regnow.rname << "" << setw(3)
                 << right << regnow.raddr << " " << setw(5) << left << regnow.rmode
                 << "  " << endl;
        }
        // ===== END registers details =====

        cout << endl;
        cout << "Configured REGs now: " << plcnow.regs.size() << endl;
        PLCset.push_back(plcnow);
        // ===== END PLs details =====
    }
    cout << "Configured PLCs: " << PLCset.size() << endl;
    return 0;
}

// eof
