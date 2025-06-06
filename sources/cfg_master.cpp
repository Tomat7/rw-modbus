// cfg_func.cpp --------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <libconfig.h++>
#include <set>
#include <string>
#include <vector>

#include "config.h"
#include "libs.h"
// #include "./plc_class.h"

using namespace libconfig;
using std::set;

static int total_regs = 0;

int cfg_init_plcset(const Setting &cfg, const Setting &pl);
int cfg_init_scadaset(const Setting &cfg, const Setting &pl);
int cfg_init_plcregs(const Setting &reg, PLC_c* pn);

/*
  void cfg_print_plc_details(const PLC_c &pn);
  void cfg_print_reg_details(const reg_t &rn);
*/
// Config cfg;

using cchar = const char;

int cfg_master(cchar* cfg_dir, cchar* cfg_file, cchar* cfg_mode)
{
  // Read the file. If there is an error, report it and exit.
  printf("\n======= cfg_read_Master =======\n");

  int _log = log_level;           // log_level = 0 at start (no logging)
  log_level = LOG_LEVEL_DEFAULT;  // Need "work" logging
  Config cfg;
  openlog("PLC_cfg", LOG_NDELAY, LOG_LOCAL1);
  LOGC("Mode: '%s'.", cfg_mode);

  // Set Include Directory for CFG files and full pathname
  cfg.setIncludeDir(cfg_dir);
  string cfile = (string)cfg_dir + "/" + (string)cfg_file;

  // Read CFG file by full pathname
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

  // Get the top name. (just for fan)
  try {
    string name = cfg.lookup("maintitle");
    LOGN("Config title: %s", name.c_str());
  } catch (const SettingNotFoundException &nfex) {
    LOGA("No '%s' setting in configuration file.\n", "nametitle");
    return (EXIT_FAILURE);
  }

  // Read/Set LogLevel from CFG file.
  log_level = _log;
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
    DEVlist =
      &cfg.lookup("plc_list_" + string(dev_type) /* string(cfg_mode) */);
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

  regs_create_from_plc();  // Init PLC reg in REGmap

  // The same for Scada (read it like a Modbus device)
  dev_type = "SCADA";
  try {
    DEVlist = &cfg.lookup("plc_list_" + string(dev_type));
  } catch (const SettingNotFoundException &nfex) {
    LOGA("No 'plc_list_%s' configured!", dev_type);
    return (EXIT_FAILURE);
  }

  try {
    cfg_init_scadaset(cfg.lookup(dev_type), *DEVlist);
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

  uint64_t vec_size_new = 0;  // PLCvec.size() + nb_plc_cfg;

  LOGW("Total PLCs in config: %d, in the list: %d.", nb_plc_cfg, nb_plc_list);

  const string &p0 = listPLC[0];
  if ((p0 == "all") || (p0 == "ALL")) {
    vec_size_new = PLCvec.size() + nb_plc_cfg;
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
    string _devname, _dfolder, _desc, _ip;
    int _port, _att, _ms, _us;

    // ===== Check the record which ex pect to get for CFG-file.
    if (cfgPLC[i].lookupValue("name", _devname) &&
        cfgPLC[i].lookupValue("folder", _dfolder) &&
        cfgPLC[i].lookupValue("desc", _desc) &&
        cfgPLC[i].lookupValue("ip", _ip) &&
        cfgPLC[i].lookupValue("port", _port) &&
        cfgPLC[i].lookupValue("attempts", _att) &&
        cfgPLC[i].lookupValue("polling", _ms) &&
        cfgPLC[i].lookupValue("timeout", _us)) {
      // It is PLC configuration!
      if (isCheckName && !PLClst.count(_devname))
        continue;  // get out of current iteration if PLC not in list

      PLCvec.emplace_back(_devname, _ip, _dfolder, _desc, _port, _att, _ms,
                          _us);
      PLCvec.back().reg_qty = cfgPLC[i]["regs"].getLength();

      int nb_regs = cfg_init_plcregs(cfgPLC[i]["regs"], &PLCvec.back());
      PLCvec.back().init_regs();  // Necessary to copy str to char* and others

      nb_plc_ready++;
      total_regs += nb_regs;

      LOGN("Configured PLC: %s, with: %d regs", PLCvec.back().dev_name,
           nb_regs);
      // ===== End PLC filling  =====
    } else {
      LOGA("Error reading PLC configuration: %d\n", i);
      continue;  // get out of current iteration if any field wrong in CFG-file
      // */
    }
  }

  LOGN("Total PLCs: %d, with %d regs", (int)PLCvec.size(), total_regs);

  if (isCheckName && (nb_plc_ready != nb_plc_list))
    LOGA("Wrong PLCs number! Processed: %d, in the list: %d.", nb_plc_ready,
         nb_plc_list);

  return 0;
}

int cfg_init_plcregs(const Setting &cfgREG, PLC_c* pn)
{
  int nb_regs = cfgREG.getLength();
  int nb_errors = 0;

  // ===== Cycle for REGs =====
  for (int j = 0; j < nb_regs; ++j) {
    reg_t r;

    if (cfgREG[j].lookupValue("rname", r.str_rname) &&
        cfgREG[j].lookupValue("raddr", r.raddr)) {
      // This is Modbus register

      if (!cfgREG[j].lookupValue("rmode", r.str_mode)) {
        LOGE("Error reading details on %s, REG: %d\n", r.str_rname.c_str(), j);
        exit(EXIT_FAILURE);
        continue;
      }

      if (!cfgREG[j].lookupValue("rfolder", r.str_rfolder))
        r.str_rfolder = "";

    } else {
      LOGE("Error reading 'rname' on %s: %s REG: %d\n",
           pn->str_top_folder.c_str(), pn->str_dev_name.c_str(), j);
      exit(EXIT_FAILURE);
      continue;
    }

    r.data.rvalue = 555;  // TODO: remove for production!
    r.str_type = "u";     // Process ALL PLC regs as uint16_t!!
    if (Reg_c::init_types(&r))
      pn->regs[r.raddr] = r;
    else
      nb_errors++;
  }
  return nb_regs - nb_errors;
}

// eof
