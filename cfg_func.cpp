// cfg_func.cpp --------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <string>
#include <vector>

#include "./config.h"
#include "./libs.h"
#include "./plc_class.h"

using namespace std;
using namespace libconfig;

int cfg_init_plcset(const Setting &cfg);
void cfg_init_regs(const Setting &reg, PLC *pn);

void cfg_print_plc_details(const PLC &pn);
void cfg_print_reg_details(const reg_t &rn);

Config cfg;

// Function to copy the string

int cfg_read(const char *cfg_dir, const char *cfg_file) {
  // Read the file. If there is an error, report it and exit.
  cout << endl << "======= cfg_read_mbset =======" << endl;

  //  Config cfg;
  openlog("PLC_cfg", LOG_NDELAY, LOG_LOCAL1);

  cfg.setIncludeDir(cfg_dir);
  string cfile = (string)cfg_dir + "/" + (string)cfg_file;

  try {
    cfg.readFile(cfile.c_str());
    LOGINFO("I/O reading file OK: %s\n", cfile.c_str());
  } catch (const FileIOException &fioex) {
    LOGERR("I/O error while reading file: %s\n", cfile.c_str());
    return (EXIT_FAILURE);
  } catch (const ParseException &pex) {
    LOGERR("Parse error at %s:%d - %s\n", pex.getFile(), pex.getLine(),
           pex.getError());
    return (EXIT_FAILURE);
  }

  // Get the top name.
  try {
    string name = cfg.lookup("maintitle");
    LOGINFO("Config title: %s\n", name.c_str());
  } catch (const SettingNotFoundException &nfex) {
    LOGERR("No 'nametitle' setting in configuration file.\n");
    return (EXIT_FAILURE);
  }

  // Output a list of all PLCs in the inventory.
  try {
    cfg_init_plcset(cfg.lookup("plc"));
    cout << "+++++" << endl;
  } catch (const SettingNotFoundException &nfex) {
    LOGERR("Great ERROR! Exiting.\n");
    return (EXIT_FAILURE);
  }

  closelog();
  /*
    for (auto &D : PLCset)
      for (auto &R : D.regs) {
        printf("%s: %s.%s \n", D.ip_addr, D.dev_name, R.ch_name);
      }
    printf("===111\n");
  */
  return (EXIT_SUCCESS);
}

int cfg_init_plcset(const Setting &cfgPLC) {
  //  const Setting &cfgPLC = cf->lookup("plc");
  int nb_plcs = cfgPLC.getLength();

  // ===== Cycle for PLCs =====
  for (int i = 0; i < nb_plcs; ++i) {
    cout << "plcnow will be..." << endl;
    PLC plcnow;
    cout << "plcnow created!" << endl;
    // ===== Check the record which expect to get for CFG-file.
    if (!(cfgPLC[i].lookupValue("title", plcnow.str_title) &&
          cfgPLC[i].lookupValue("desc", plcnow.str_desc) &&
          cfgPLC[i].lookupValue("name", plcnow.str_dev_name) &&
          cfgPLC[i].lookupValue("ip", plcnow.str_ip_addr) &&
          cfgPLC[i].lookupValue("port", plcnow.tcp_port) &&
          cfgPLC[i].lookupValue("attempts", plcnow.attempts) &&
          cfgPLC[i].lookupValue("polling", plcnow.mb.interval_ms) &&
          cfgPLC[i].lookupValue("timeout", plcnow.mb.timeout_us))) {
      LOGERR("Warning!! Error reading PLC configuration: %d\n", i);
      continue; // get out of current iteration if any field wrong in CFG-file
    }

    plcnow.reg_qty = cfgPLC[i]["regs"].getLength();
    cfg_print_plc_details(plcnow);
    cfg_init_regs(cfgPLC[i]["regs"], &plcnow);

    cout << endl;
    LOGINFO("Configured REGs now: %d\n", (int)plcnow.regs.size());
    PLCset.push_back(plcnow);
    cout << "PB done" << endl;
    PLCset[i].init(); // Absolutely necessary to copy str to char* and other
    // ===== End PLC filling  =====
  }

  LOGINFO("Configured PLCs: %d\n", (int)PLCset.size());
  /*
    for (auto &D : PLCset)
      for (auto &R : D.regs) {
        printf("%s: %s.%s \n", D.ip_addr, D.dev_name, R.ch_name);
      }
  */
  return 0;
}

void cfg_init_regs(const Setting &cfgREG, PLC *pn) {
  int nb_regs = cfgREG.getLength();

  // ===== Cycle for REGs =====
  for (int j = 0; j < nb_regs; ++j) {
    reg_t regnow;

    // ===== Check the record which expect to get for CFG-file.
    if (!(cfgREG[j].lookupValue("rname", regnow.str_name) &&
          cfgREG[j].lookupValue("raddr", regnow.raddr) &&
          cfgREG[j].lookupValue("rmode", regnow.str_mode) &&
          cfgREG[j].lookupValue("rtype", regnow.str_type))) {
      LOGERR("error reading REG %d\n", j);
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
  cout << setw(10) << left << D.str_desc << "  " << setw(10) << left
       << D.str_dev_name << "  " << setw(20) << left << D.str_ip_addr << "  "
       << D.reg_qty << endl;
  return;
}

void cfg_print_reg_details(const reg_t &R) {
  // ===== Output REG details
  cout << "       " << setw(9) << left << R.str_name << "" << setw(3) << right
       << R.raddr << " " << setw(5) << left << R.str_mode << "  " << endl;
  return;
}

void cfg_deinit() {
  //    cfg.clear();
  PLCset.clear();
}

// eof
