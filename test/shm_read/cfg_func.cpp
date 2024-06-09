// cfg_func.cpp --------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <string>
#include <vector>

#include "./config.h"
#include "./libs.h"
#include "./plc_class.h"

// using namespace std;
using namespace libconfig;

int cfg_init_plcset();
void cfg_init_regs(const Setting &reg, PLC_c *pn);

void cfg_print_plc_details(const PLC_c &pn);
void cfg_print_reg_details(const reg_t &rn);

Config cfg;

// This example reads the configuration file and displays
// some of its contents.

int cfg_read(const char *cfg_file) {
  // Read the file. If there is an error, report it and exit.
  cout << endl << "======= cfg_read_mbset =======" << endl;

  openlog("PLC_cfg", LOG_NDELAY, LOG_LOCAL1);

  try {
    cfg.readFile(cfg_file);
    LOGINFO("I/O reading file OK: %s\n", cfg_file);
    // std::cout << "I/O reading file OK: " << cfg_file << std::endl;
  } catch (const FileIOException &fioex) {
    LOGERR("I/O error while reading file: %s\n", cfg_file);
    // std::cerr << "I/O error while reading file " << cfg_file << std::endl;
    return (EXIT_FAILURE);
  } catch (const ParseException &pex) {
    LOGERR("Parse error at %s:%d - %s\n", pex.getFile(), pex.getLine(),
           pex.getError());
    // std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
    //          << " - " << pex.getError() << std::endl;
    return (EXIT_FAILURE);
  }

  // Get the top name.
  try {
    string name = cfg.lookup("maintitle");
    LOGINFO("Config title: %s\n", name.c_str());
    // cout << "Config title: " << name << endl << endl;
  } catch (const SettingNotFoundException &nfex) {
    LOGERR("No 'nametitle' setting in configuration file.\n");
    // cerr << "No 'nametitle' setting in configuration file." << endl;
  }

  // Output a list of all PLCs in the inventory.
  try {
    cfg_init_plcset();
    cout << "+++++" << endl;
  } catch (const SettingNotFoundException &nfex) {
    LOGERR("Great ERROR! Exiting.\n");
    // cout << "Great ERROR! Exiting." << endl;
  }

  closelog();
  return (EXIT_SUCCESS);
}

int cfg_init_plcset() {
  const Setting &cfgPLC = cfg.lookup("plc");
  int nb_plcs = cfgPLC.getLength();

  // ===== Cycle for PLCs =====
  for (int i = 0; i < nb_plcs; ++i) {
    PLC_c plcnow;

    // ===== Check the record which expect to get for CFG-file.
    if (!(cfgPLC[i].lookupValue("title", plcnow.dev_title) &&
          cfgPLC[i].lookupValue("desc", plcnow.dev_desc) &&
          cfgPLC[i].lookupValue("name", plcnow.dev_name) &&
          cfgPLC[i].lookupValue("ip", plcnow.ip_addr) &&
          cfgPLC[i].lookupValue("port", plcnow.tcp_port) &&
          cfgPLC[i].lookupValue("polling", plcnow.mb_interval_ms) &&
          cfgPLC[i].lookupValue("timeout", plcnow.mb_timeout_us))) {
      LOGERR("Warning!! Error reading PLC configuration: %d\n", i);
      // cout << "Warning!! Error reading PLC configuration: " << i << endl;
      continue; // get out of current iteration if any field wrong in CFG-file
    }

    plcnow.reg_qty = cfgPLC[i]["regs"].getLength();
    cfg_print_plc_details(plcnow);
    cfg_init_regs(cfgPLC[i]["regs"], &plcnow);

    cout << endl;
    LOGINFO("Configured REGs now: %d\n", (int)plcnow.regs.size());
    // cout << "Configured REGs now: " << plcnow.regs.size() << endl;
    PLCvec.push_back(plcnow);
    // ===== End PLC filling  =====
  }

  LOGINFO("Configured PLCs: %d\n", (int)PLCvec.size());
  // cout << "Configured PLCs: " << PLCset.size() << endl;
  return 0;
}

void cfg_init_regs(const Setting &cfgREG, PLC_c *pn) {
  int nb_regs = cfgREG.getLength();

  // ===== Cycle for REGs =====
  for (int j = 0; j < nb_regs; ++j) {
    //    const Setting &reg = cfgREG[j];
    reg_t regnow;

    // ===== Check the record which expect to get for CFG-file.
    if (!(cfgREG[j].lookupValue("rname", regnow.ch_name) &&
          cfgREG[j].lookupValue("raddr", regnow.raddr) &&
          cfgREG[j].lookupValue("rmode", regnow.ch_mode) &&
          cfgREG[j].lookupValue("rtype", regnow.ch_type))) {
      LOGERR("error reading REG %d\n", j);
      // cout << "error reading REG " << j << endl;
      continue;
    }

    if (regnow.raddr < pn->reg_min)
      pn->reg_min = regnow.raddr;

    if (regnow.raddr > pn->reg_max)
      pn->reg_max = regnow.raddr;

/*
    //  ===== Shared memory init.
    regnow.rfd = create_shm_fd(regnow.rname);
    if (regnow.rfd != -1) {
      regnow.rshm = create_shm_addr(regnow.rfd, sizeof(regnow));

      if (regnow.rshm != nullptr)
        close_shm(regnow.rfd, regnow.rshm, sizeof(regnow));
    }
*/
    regnow.rvalue = 555;
    cfg_print_reg_details(regnow);
    pn->regs.push_back(regnow);
  }

  return;
}

void cfg_print_plc_details(const PLC_c &D) {
  // ===== Output PLC details
  cout << setw(10) << left << D.dev_desc << "  " << setw(10) << left
       << D.dev_name << "  " << setw(20) << left << D.ip_addr << "  "
       << D.reg_qty << endl;
  return;
}

void cfg_print_reg_details(const reg_t &R) {
  // ===== Output REG details
  cout << "       " << setw(9) << left << R.ch_name << "" << setw(3) << right
       << R.raddr << " " << setw(5) << left << R.ch_mode << "  " << endl;
  return;
}

// eof
