// cfg_func.cpp --------------------------------------------------------------

#include "config.h"
#include "libs.h"
#include "plc_class.h"
#include <string>
#include <vector>

using namespace std;
using namespace libconfig;

int cfg_init_plcset();
void cfg_init_regs(const Setting &reg, PLC *pn);

void cfg_print_plc_details(const PLC &pn);
void cfg_print_reg_details(const reg_t &rn);

Config cfg;

// This example reads the configuration file and displays
// some of its contents.

int cfg_read(const char *cfg_file) {

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
    cfg_init_plcset();
    cout << "+++++" << endl;
  } catch (const SettingNotFoundException &nfex) {
    cout << "Great ERROR! Exiting." << endl;
    // Ignore.
  }

  return (EXIT_SUCCESS);
}

int cfg_init_plcset() {
  const Setting &cfgPLC = cfg.lookup("plc");
  int nb_plcs = cfgPLC.getLength();

  // ===== Cycle for PLCs =====
  for (int i = 0; i < nb_plcs; ++i) {
    PLC plcnow;

    // ===== Check the record which expect to get for CFG-file.
    if (!(cfgPLC[i].lookupValue("title", plcnow.dev_title) &&
          cfgPLC[i].lookupValue("desc", plcnow.dev_desc) &&
          cfgPLC[i].lookupValue("name", plcnow.dev_name) &&
          cfgPLC[i].lookupValue("ip", plcnow.ip_addr) &&
          cfgPLC[i].lookupValue("port", plcnow.tcp_port) &&
          cfgPLC[i].lookupValue("polling", plcnow.mb_interval) &&
          cfgPLC[i].lookupValue("timeout", plcnow.mb_timeout))) {
      cout << "Warning!! Error reading PLC configuration: " << i << endl;
      continue; // get out of current cycle iteration if any field wrong in
                // CFG-file
    }

    plcnow.nb_regs = cfgPLC[i]["regs"].getLength();
    cfg_print_plc_details(plcnow);
    cfg_init_regs(cfgPLC[i]["regs"], &plcnow);

    cout << endl;
    cout << "Configured REGs now: " << plcnow.regs.size() << endl;
    PLCset.push_back(plcnow);
    // ===== End PLC filling  =====
  }

  cout << "Configured PLCs: " << PLCset.size() << endl;
  return 0;
}

void cfg_init_regs(const Setting &cfgREG, PLC *pn) {
  int nb_regs = cfgREG.getLength();

  // ===== Cycle for REGs =====
  for (int j = 0; j < nb_regs; ++j) {
    //    const Setting &reg = cfgREG[j];
    reg_t regnow;

    // ===== Check the record which expect to get for CFG-file.
    if (!(cfgREG[j].lookupValue("rname", regnow.rname) &&
          cfgREG[j].lookupValue("raddr", regnow.raddr) &&
          cfgREG[j].lookupValue("rmode", regnow.rmode) &&
          cfgREG[j].lookupValue("rtype", regnow.rtype))) {
      cout << "error reading REG " << j << endl;
      continue;
    }

    regnow.rvalue = 555;
    cfg_print_reg_details(regnow);
    pn->regs.push_back(regnow);
  }

  return;
}

void cfg_print_plc_details(const PLC &D) {
  // ===== Output PLC details
  cout << setw(10) << left << D.dev_desc << "  " << setw(10) << left
       << D.dev_name << "  " << setw(20) << left << D.ip_addr << "  "
       << D.nb_regs << endl;
  return;
}

void cfg_print_reg_details(const reg_t &R) {
  // ===== Output REG details
  cout << "       " << setw(9) << left << R.rname << "" << setw(3) << right
       << R.raddr << " " << setw(5) << left << R.rmode << "  " << endl;
  return;
}

// eof
