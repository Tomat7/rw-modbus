// cfg_func.cpp --------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <set>
#include <string>
#include <vector>

#include "config.h"
#include "libs.h"
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
  printf("\n======= cfg_read_Master =======\n");

  Config cfg;
  openlog("PLC_cfg", LOG_NDELAY, LOG_LOCAL1);
  LOGC("Mode: '%s'.", cfg_mode);

  cfg.setIncludeDir(cfg_dir);
  string cfile = (string)cfg_dir + "/" + (string)cfg_file;

  try {
    cfg.readFile(cfile.c_str());
    LOGN("I/O reading file OK: %s", cfile.c_str());
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
    LOGN("Config title: %s", name.c_str());
  } catch (const SettingNotFoundException &nfex) {
    LOGA("No '%s' setting in configuration file.\n", "nametitle");
    return (EXIT_FAILURE);
  }
  /*
    // Set LogLevel.
    try {
      int _log = cfg.lookup("loglevel");
      if (log_level != _log) {
        log_level = _log;
        LOGC("Set LOG_LEVEL to: %d.", log_level);
      } else
        LOGC("LOG_LEVEL is: %d.", log_level);
    } catch (const SettingNotFoundException &nfex) {
      log_level = LOG_LEVEL_DEFAULT;
      LOGA("No LOG_LEVEL configured. Set to LOG_LEVEL_DEFAULT: %d.", log_level);
    }
  */

  // Set LogLevel.
  if (log_level == 0) {
    try {
      log_level = cfg.lookup("loglevel");
      LOGC("Set LOG_LEVEL to: %d.", log_level);
    } catch (const SettingNotFoundException &nfex) {
      log_level = LOG_LEVEL_DEFAULT;
      LOGA("No LOG_LEVEL configured. Set to LOG_LEVEL_DEFAULT: %d.", log_level);
    }
  } else
    LOGC("LOG_LEVEL is: %d.", log_level);

  // Get list of PLC for configuration
  Setting* DEVlist;
  const char* dev_type = "PLC";
  try {
    DEVlist = &cfg.lookup("plc_list_" + string(dev_type) /* string(cfg_mode) */);
  } catch (const SettingNotFoundException &nfex) {
    LOGA("No 'plc_list_%s' configured!", dev_type /* cfg_mode */);
    return (EXIT_FAILURE);
  }

  // Fill a list of all PLCs in the inventory.
  try {
    cfg_init_plcset(cfg.lookup(dev_type), *DEVlist);
  } catch (const SettingNotFoundException &nfex) {
    LOGA("Great ERROR! (no '%s' settings?) Exiting.\n", dev_type);
    return (EXIT_FAILURE);
  }

  dev_type = "SCADA";
  try {
    DEVlist = &cfg.lookup("plc_list_" + string(dev_type));
  } catch (const SettingNotFoundException &nfex) {
    LOGA("No 'plc_list_%s' configured!", dev_type);
    return (EXIT_FAILURE);
  }

  try {
    cfg_init_plcset(cfg.lookup(dev_type), *DEVlist);
  } catch (const SettingNotFoundException &nfex) {
    LOGA("Great ERROR! (no '%s' settings?) Exiting.\n", dev_type);
    return (EXIT_FAILURE);
  }

  closelog();

  return (EXIT_SUCCESS);
}

int cfg_init_plcset(const Setting &cfgPLC, const Setting &listPLC)
{
  bool isCheckName = false;
  int nb_plc_cfg = cfgPLC.getLength();
  int nb_plc_list = listPLC.getLength();
  int nb_plc_ready = 0;
  set<string> PLClst;

  uint64_t vec_size_new = PLCvec.size() + nb_plc_cfg;

  LOGW("Total PLCs in config: %d, in the list: %d.", nb_plc_cfg, nb_plc_list);

  const string &p0 = listPLC[0];
  if ((p0 == "all") || (p0 == "ALL")) {
    LOGC("List with %d PLCs ignored. Will read '%s' %d PLCs from configfile!",
         nb_plc_list, p0.c_str(), nb_plc_cfg);
  } else {
    isCheckName = true;
    vec_size_new = PLCvec.size() + nb_plc_list;
    for (int i = 0; i < nb_plc_list; ++i)
      PLClst.insert(listPLC[i]);
  }

  if (vec_size_new > PLCvec.capacity())
    PLCvec.reserve(vec_size_new + 1);

  for (int i = 0; i < nb_plc_cfg; ++i) {
    string _devname, _title, _desc, _ip;
    int _port, _att, _ms, _us;
    // ===== Check the record which expect to get for CFG-file.
    if (!(cfgPLC[i].lookupValue("name", _devname) &&
          cfgPLC[i].lookupValue("title", _title) &&
          cfgPLC[i].lookupValue("desc", _desc) &&
          cfgPLC[i].lookupValue("ip", _ip) &&
          cfgPLC[i].lookupValue("port", _port) &&
          cfgPLC[i].lookupValue("attempts", _att) &&
          cfgPLC[i].lookupValue("polling", _ms) &&
          cfgPLC[i].lookupValue("timeout", _us))) {
      LOGA("Error reading PLC configuration: %d\n", i);
      continue; // get out of current iteration if any field wrong in CFG-file
    }

    if (isCheckName && !PLClst.count(_devname)) {
      continue; // get out of current iteration if PLC not in list
    }

    PLCvec.emplace_back(_devname, _ip, _title, _desc, _port, _att, _ms, _us);
    PLCvec.back().reg_qty = cfgPLC[i]["regs"].getLength();

    cfg_init_regs(cfgPLC[i]["regs"], &PLCvec.back());
    PLCvec.back().init_regs(); // Necessary to copy str to char* and others
    nb_plc_ready++;

    LOGI("Configured PLC: %s, with: %d regs", PLCvec.back().dev_name,
         (int)PLCvec.back().regs.size());
    // ===== End PLC filling  =====
  }

  LOGI("Total PLCs: %d, with %d regs", (int)PLCvec.size(), total_regs);

  if (isCheckName && (nb_plc_ready != nb_plc_list))
    LOGA("Wrong PLCs number! Processed: %d, in the list: %d.",
         nb_plc_ready, nb_plc_list);

  return 0;
}

void cfg_init_regs(const Setting &cfgREG, PLC_c* pn)
{
  int nb_regs = cfgREG.getLength();
  total_regs += nb_regs;

  // ===== Cycle for REGs =====
  for (int j = 0; j < nb_regs; ++j) {
    reg_t r;

    // Check the record which expect to get for CFG-file.
    if (!(cfgREG[j].lookupValue("rname", r.str_name) &&
          cfgREG[j].lookupValue("raddr", r.raddr))) {
      LOGE("Error reading 'rname' on %s: %s REG: %d\n",
           pn->str_title.c_str(), pn->str_dev_name.c_str(), j);
      exit(EXIT_FAILURE);
      continue;
    }

    // Check the record which expected for SCADA reg.
    if (cfgREG[j].lookupValue("rsource", r.str_source) &&
        cfgREG[j].lookupValue("rfolder", r.str_folder)) {
      LOGI("Reading 'rsource'/'rfolder' on %s: %s/%s REG: %d",
           r.str_name.c_str(), r.str_source.c_str(), r.str_folder.c_str(), j);

      if (r.str_source == "-") {
        if (!(cfgREG[j].lookupValue("rmode", r.str_mode) &&
              cfgREG[j].lookupValue("rtype", r.str_type))) {
          LOGE("Error reading details on %s: %s REG: %d\n",
               r.str_name.c_str(), j);
          exit(EXIT_FAILURE);
          continue;
        }
      }
    } else {
      // Check records for Modbus register.
      if (!(cfgREG[j].lookupValue("rmode", r.str_mode) &&
            cfgREG[j].lookupValue("rtype", r.str_type))) {
        LOGE("Error reading details on %s, REG: %d\n", r.str_name.c_str(), j);
        exit(EXIT_FAILURE);
        continue;
      }

      if (!cfgREG[j].lookupValue("rfolder", r.str_folder))
        r.str_folder = ".";
    }

    r.data.rvalue = 555; // TODO: remove for production!
    pn->regs[r.raddr] = r;
  }

  return;
}

/*
  void cfg_deinit() { PLCvec.clear(); }
*/
// eof
