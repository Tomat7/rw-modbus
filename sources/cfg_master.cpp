// cfg_func.cpp --------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <string>
#include <vector>

#include "./config.h"
#include "./libs.h"
// #include "./plc_class.h"

using namespace std;
using namespace libconfig;

int _regs = 0;

int cfg_init_plcset(const Setting &cfg);
void cfg_init_regs(const Setting &reg, PLC_c* pn);

void cfg_print_plc_details(const PLC_c &pn);
void cfg_print_reg_details(const reg_t &rn);

// Config cfg;

int cfg_master(const char* cfg_dir, const char* cfg_file)
{
  // Read the file. If there is an error, report it and exit.
  cout << endl
       << "======= cfg_read_mbset =======" << endl;

  Config cfg;
  openlog("PLC_cfg", LOG_NDELAY, LOG_LOCAL1);

  cfg.setIncludeDir(cfg_dir);
  string cfile = (string)cfg_dir + "/" + (string)cfg_file;

  try {
    cfg.readFile(cfile.c_str());
    LOGW("I/O reading file OK: %s", cfile.c_str());
  } catch (const FileIOException &fioex) {
    LOGA("I/O error while reading file: %s\n", cfile.c_str());
    return (EXIT_FAILURE);
  } catch (const ParseException &pex) {
    LOGA("Parse error at %s:%d - %s\n", pex.getFile(), pex.getLine(),
         pex.getError());
    return (EXIT_FAILURE);
  }

  // Get the top name.
  try {
    string name = cfg.lookup("maintitle");
    LOGW("Config title: %s", name.c_str());
  } catch (const SettingNotFoundException &nfex) {
    LOGA("No '%s' setting in configuration file.\n", "nametitle");
    return (EXIT_FAILURE);
  }

  // Set LogLevel.
  if (log_level == 0) {
    try {
      int _log = cfg.lookup("loglevel");
      log_level = _log;
      LOGW("Set LOG_LEVEL to: %d.", log_level);
    } catch (const SettingNotFoundException &nfex) {
      LOGA("No LOG_LEVEL configured. Set to LOG_LEVEL_DEFAULT: %d.", log_level);
    }
  } else
    LOGW("LOG_LEVEL is: %d.", log_level);

// ========== TEST !!!
  try {
    Setting &L = cfg.lookup("plclisthome");
    string pl = L[0];
    LOGW("PLC list [0]: %s, total: %d.", pl.c_str(), L.getLength());
  } catch (const SettingNotFoundException &nfex) {
    LOGA("No LOG_LEVEL configured. Set to LOG_LEVEL_DEFAULT: %d.", log_level);
  }
// ==========  end of TEST

  // Output a list of all PLCs in the inventory.
  try {
    cfg_init_plcset(cfg.lookup("master"));
  } catch (const SettingNotFoundException &nfex) {
    LOGA("Great ERROR! (no 'plc' settings?) Exiting.\n");
    return (EXIT_FAILURE);
  }

  closelog();

  return (EXIT_SUCCESS);
}

int cfg_init_plcset(const Setting &cfgPLC)
{
  int nb_plcs = cfgPLC.getLength();
  PLCset.resize(nb_plcs);

  for (int i = 0; i < nb_plcs; ++i) {
    PLC_c &plc = PLCset[i];
    // ===== Check the record which expect to get for CFG-file.
    if (!(cfgPLC[i].lookupValue("title", plc.str_title) &&
          cfgPLC[i].lookupValue("desc", plc.str_desc) &&
          cfgPLC[i].lookupValue("name", plc.str_dev_name) &&
          cfgPLC[i].lookupValue("ip", plc.str_ip_addr) &&
          cfgPLC[i].lookupValue("port", plc.tcp_port) &&
          cfgPLC[i].lookupValue("attempts", plc.attempts) &&
          cfgPLC[i].lookupValue("polling", plc.mb.interval_ms) &&
          cfgPLC[i].lookupValue("timeout", plc.mb.timeout_us))) {
      LOGE("Error reading PLC configuration: %d\n", i);
      continue; // get out of current iteration if any field wrong in CFG-file
    }

    plc.reg_qty = cfgPLC[i]["regs"].getLength();
    cfg_init_regs(cfgPLC[i]["regs"], &plc /*PLCset[i]*/);

    plc.init_master(); // Absolutely necessary to copy str to char* and other
    LOGW("Configured PLC: %s, with: %d regs", plc.dev_name, (int)plc.regs.size());
    //    cout << endl;
    // ===== End PLC filling  =====
  }

  LOGW("Total PLCs: %d, with %d regs", (int)PLCset.size(), _regs);

  return 0;
}

void cfg_init_regs(const Setting &cfgREG, PLC_c* pn)
{
  int nb_regs = cfgREG.getLength();
  _regs += nb_regs;

  // ===== Cycle for REGs =====
  for (int j = 0; j < nb_regs; ++j) {
    reg_t regnow;

    // ===== Check the record which expect to get for CFG-file.
    if (!(cfgREG[j].lookupValue("rname", regnow.str_name) &&
          cfgREG[j].lookupValue("raddr", regnow.raddr) &&
          cfgREG[j].lookupValue("rmode", regnow.str_mode) &&
          cfgREG[j].lookupValue("rtype", regnow.str_type))) {
      LOGE("Error reading config on PLC: %s REG: %d\n",
           pn->str_dev_name.c_str(), j);
      exit(EXIT_FAILURE);
      continue;
    }

    regnow.data.rvalue = 555; // TODO: remove for production!
    pn->regs[regnow.raddr] = regnow;
  }

  return;
}

void cfg_print_plc_details(const PLC_c &D)
{
  // ===== Output PLC details
  cout << setw(10) << left << D.str_desc << "  " << setw(10) << left
       << D.str_dev_name << "  " << setw(20) << left << D.str_ip_addr << "  "
       << D.reg_qty << endl;
  return;
}

void cfg_print_reg_details(const reg_t &R)
{
  // ===== Output REG details
  cout << "       " << setw(9) << left << R.str_name << "" << setw(3) << right
       << R.raddr << " " << setw(5) << left << R.str_mode << "  " << endl;
  return;
}

void cfg_deinit()
{
  //    cfg.clear();
  PLCset.clear();
}

// eof
