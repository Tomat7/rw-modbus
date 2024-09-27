// cfg_func.cpp --------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#define COMPILE_SLAVE
#ifdef COMPILE_SLAVE

#include <string>
#include <vector>

#include "config.h"
#include "libs.h"
// #include "./plc_class.h"

using namespace std;
using namespace libconfig;

static int total_regs = 0;

int cfg_init_slave(const Setting &cfg);
void cfg_init_slave_regs(const Setting &reg, PLC_c* pn);

using cchar = const char;

int cfg_slave(const char* cfg_dir, const char* cfg_file, const char* cfg_mode)
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
    LOGA("No '%s' setting in cfg file for %s.\n", "nametitle", cfg_mode);
    return (EXIT_FAILURE);
  }

  // Set LogLevel.
  if (log_level == 0) {
    try {
      int _log = cfg.lookup("loglevel");
      log_level = _log;
      LOGC("Set LOG_LEVEL to: %d.", log_level);
    } catch (const SettingNotFoundException &nfex) {
      log_level = LOG_LEVEL_DEFAULT;
      LOGA("No LOG_LEVEL configured. Set to LOG_LEVEL_DEFAULT: %d.", log_level);
    }
  } else
    LOGC("LOG_LEVEL is: %d.", log_level);

  // Output a list of all PLCs in the inventory.
  try {
    cfg_init_slave(cfg.lookup("slave"));
  } catch (const SettingNotFoundException &nfex) {
    LOGA("Great ERROR! (no 'plc' settings?) Exiting.\n");
    return (EXIT_FAILURE);
  }

  closelog();

  return (EXIT_SUCCESS);
}

int cfg_init_slave(const Setting &cfgSlave)
{
  Slave.reg_qty = cfgSlave.getLength();
  cfg_init_slave_regs(cfgSlave, &Slave);
  Slave.init_regs();  // Absolutely necessary to copy str to char* and other
  LOGW("Configured Slave with: %d regs", (int)Slave.regs.size());
  //  LOGW("Total Slave's regs: %d", total_regs);

  return 0;
}

void cfg_init_slave_regs(const Setting &cfgREG, PLC_c* pn)
{
  int nb_regs = cfgREG.getLength();
  total_regs += nb_regs;

  // ===== Cycle for REGs =====
  for (int j = 0; j < nb_regs; ++j) {
    reg_t regnow;

    // ===== Check the record which expect to get for CFG-file.
    if (!(cfgREG[j].lookupValue("rname", regnow.str_name) &&
          cfgREG[j].lookupValue("raddr", regnow.raddr) &&
          cfgREG[j].lookupValue("source", regnow.fullname)  //&&
          //  cfgREG[j].lookupValue("rtype", regnow.str_type)
         )) {
      LOGE("Error reading config on Slave, REG: %d\n", pn->str_dev_name.c_str(),
           j);
      exit(EXIT_FAILURE);
      continue;
    }

    LOGE("-- %s %d %s", regnow.str_name.c_str(), regnow.raddr,
         regnow.fullname.c_str());

    if (regnow.fullname == "-") {
      if (!(cfgREG[j].lookupValue("rmode", regnow.str_mode) &&
            cfgREG[j].lookupValue("rtype", regnow.str_type))) {
        LOGE("Error reading config on Slave, vREG: %d\n",
             pn->str_dev_name.c_str(), j);
        exit(EXIT_FAILURE);
        continue;
      }
    } else {
      regnow.str_mode = "rw";
      regnow.str_type = "i";
    }

    regnow.data.rvalue = 555;  // TODO: remove for production!
    pn->regs[regnow.raddr] = regnow;
  }

  return;
}

void cfg_deinit_() { PLCvec.clear(); }

#endif
// eof
