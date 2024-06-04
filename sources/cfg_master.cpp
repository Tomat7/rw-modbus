// cfg_func.cpp --------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <string>
#include <vector>
#include <set>

#include "./config.h"
#include "./libs.h"
// #include "./plc_class.h"

using namespace std;
using namespace libconfig;

static int total_regs = 0;

int cfg_init_plcset(const Setting &cfg, const Setting &pl);
void cfg_init_regs(const Setting &reg, PLC_c* pn);

void cfg_print_plc_details(const PLC_c &pn);
void cfg_print_reg_details(const reg_t &rn);

// Config cfg;

using cchar = const char;

int cfg_master(cchar* cfg_dir, cchar* cfg_file, cchar* cfg_mode)
{
  // Read the file. If there is an error, report it and exit.
  printf("\n======= cfg_read_mbset =======\n");

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
      LOGC("Set LOG_LEVEL to: %d.", log_level);
    } catch (const SettingNotFoundException &nfex) {
      LOGA("No LOG_LEVEL configured. Set to LOG_LEVEL_DEFAULT: %d.", log_level);
    }
  } else
    LOGC("LOG_LEVEL is: %d.", log_level);

  // Get list of PLC for configuration
  Setting* PLClist;
  try {
    PLClist = &cfg.lookup("plc_list_" + string(cfg_mode));
    LOGW("Mode: '%s', PLCs in list: %d.", cfg_mode, PLClist->getLength());
  } catch (const SettingNotFoundException &nfex) {
    LOGA("No 'plc_list_%s' configured!", cfg_mode);
    return (EXIT_FAILURE);
  }

  // Fill a list of all PLCs in the inventory.
  try {
    cfg_init_plcset(cfg.lookup("PLC"), *PLClist);
  } catch (const SettingNotFoundException &nfex) {
    LOGA("Great ERROR! (no 'PLC' settings?) Exiting.\n");
    return (EXIT_FAILURE);
  }

  closelog();

  return (EXIT_SUCCESS);
}

int cfg_init_plcset(const Setting &cfgPLC, const Setting &listPLC)
{
  bool isCheckName = true;
  int nb_plc_cfg = cfgPLC.getLength();
  int nb_plc_list = listPLC.getLength();
  printf("\n %d %d \n", nb_plc_list, nb_plc_cfg);

  set<string> PLClst;

  const string &p0 = listPLC[0];
  if ((p0 == "all") || (p0 == "ALL")) {
    isCheckName = false;
    PLCset.resize(nb_plc_cfg);
    LOGC("List with %d PLCs ignored. Will read '%s' %d PLCs from configfile!",
         nb_plc_list, p0.c_str(), nb_plc_cfg);
  } else {
    PLCset.resize(nb_plc_list);
    for (int i = 0; i < nb_plc_list; ++i) {
      const string &p = listPLC[i];
      printf("%s \n", p.c_str());
      PLClst.insert(p);
    }
  }

  for (int i = 0; i < nb_plc_cfg; ++i) {
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

    printf("+ %s \n", plc.str_dev_name.c_str());
    if (isCheckName && !PLClst.count(plc.str_dev_name)) {
      //plc.init_master();
      continue;
    }
    printf("++ %s \n", plc.str_dev_name.c_str());

    plc.reg_qty = cfgPLC[i]["regs"].getLength();
    cfg_init_regs(cfgPLC[i]["regs"], &plc /*PLCset[i]*/);

    plc.init_master(); // Absolutely necessary to copy str to char* and other
    LOGW("Configured PLC: %s, with: %d regs", plc.dev_name, (int)plc.regs.size());
    // ===== End PLC filling  =====
  }

  LOGC("Total PLCs: %d, with %d regs", (int)PLCset.size(), total_regs);

  if (isCheckName && (nb_plc_cfg != nb_plc_list))
    LOGA("Wrong PLCs number! Configured: %d, required: %d.", nb_plc_cfg, nb_plc_list);

  return 0;
}

void cfg_init_regs(const Setting &cfgREG, PLC_c* pn)
{
  int nb_regs = cfgREG.getLength();
  total_regs += nb_regs;

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

void cfg_deinit()
{
  PLCset.clear();
}

// eof
