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

int cfg_init_scadaset(const Setting &cfg, const Setting &pl);
void cfg_init_scadaregs(const Setting &reg, string _dname, string _dfolder);
/*
  void cfg_print_plc_details(const PLC_c &pn);
  void cfg_print_reg_details(const reg_t &rn);
*/
// Config cfg;

using cchar = const char;

int cfg_init_scadaset(const Setting &cfgPLC, const Setting &listPLC)
{
  bool isCheckName = false;
  int nb_plc_cfg = cfgPLC.getLength();
  int nb_plc_list = listPLC.getLength();
  int nb_plc_ready = 0;
  set<string> PLClst;

  uint64_t vec_size_new = 0; // PLCvec.size() + nb_plc_cfg;

  LOGW("Total SCADA sets in config: %d, in the list: %d.", nb_plc_cfg, nb_plc_list);

  const string &p0 = listPLC[0];
  if ((p0 == "all") || (p0 == "ALL") || (p0 == "All")) {
    vec_size_new = PLCvec.size() + nb_plc_cfg;
    LOGC("List with %d SCADA sets ignored.", nb_plc_list);
    LOGC("Will read '%s' %d SCADA sets from configfile!",
         p0.c_str(), nb_plc_cfg);
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
//    int _port, _att, _ms, _us;

    // ===== Check the record which expect to get for CFG-file.
    if (cfgPLC[i].lookupValue("name", _devname) &&
        cfgPLC[i].lookupValue("folder", _dfolder) &&
        cfgPLC[i].lookupValue("desc", _desc)) {
      // It is SCADA registers configuration!
      // //PLCvec.emplace_back(_devname, "", _folder, _desc, 0, 0, 0, 0);
      // //PLCvec.back().reg_qty = cfgPLC[i]["regs"].getLength();

      cfg_init_scadaregs(cfgPLC[i]["regs"], _devname, _dfolder);
      //regs_create(&PLCvec.back());
      // //PLCvec.back().init_regs(); // Necessary to copy str to char* and others
      nb_plc_ready++;

      LOGI("Configured SCADA: %s, with: %d regs", PLCvec.back().dev_name,
           (int)PLCvec.back().regs.size());
      //PLCvec.erase(PLCvec.end());
      // ===== End SCADA virtual filling =====
    } else {
      LOGA("Error reading SCADA configuration: %d\n", i);
      continue; // get out of current iteration if any field wrong in CFG-file */
    }
  }

  LOGI("Total PLCs: %d, with %d regs", (int)PLCvec.size(), total_regs);

  if (isCheckName && (nb_plc_ready != nb_plc_list))
    LOGA("Wrong PLCs number! Processed: %d, in the list: %d.",
         nb_plc_ready, nb_plc_list);

  return 0;
}



void cfg_init_scadaregs(const Setting &cfgREG, string _dname, string _dfolder)
{
  int nb_regs = cfgREG.getLength();
  total_regs += nb_regs;

  // ===== Cycle for REGs =====
  for (int j = 0; j < nb_regs; ++j) {
    reg_t r;

    if (cfgREG[j].lookupValue("rsource", r.str_source) &&
        cfgREG[j].lookupValue("rfolder", r.str_rfolder) &&
        cfgREG[j].lookupValue("rname", r.str_name)) {
      // This is SCADA register/variable/tag
      if (!(cfgREG[j].lookupValue("rmode", r.str_mode) &&
            cfgREG[j].lookupValue("rtype", r.str_type))) {
        if (r.str_source == "-") {
          LOGE("Error reading 'rmode'/'rtype' on %s: %s REG: %d\n",
               r.str_name.c_str(), j);
          exit(EXIT_FAILURE);
          continue;
        }
      }
      r.raddr = j;
      LOGI("Read 'rsource'/'rfolder' on %s: %s/%s REG: %d",
           r.str_name.c_str(), r.str_source.c_str(), r.str_rfolder.c_str(), j);
    } else {
      LOGE("Error reading 'rname' on %s: %s REG: %d\n",
           _dfolder.c_str(), _dname.c_str(), j);
      exit(EXIT_FAILURE);
      continue;
    }

    Reg_c::init_types(&r);
    r.data.rmode = (r.str_mode == "rw") ? 1 : 0;
    r.ch_name = r.str_name.c_str();
    r.data.rvalue = 888; // TODO: remove for production!

    if (_dname == "-" || _dname == ".") // Scada!
      r.fullname = r.str_name;
    else
      r.fullname = _dname + "." + r.str_name;


    string str_opcbase = "/" + _dfolder + "/" + _dname + "/";
    //_D->regs[r.raddr] = r;

    REGmap[r.fullname] = {&r, str_opcbase};
  }
  return;
}

/*
    // Check the record which expect to get for CFG-file.
    if (!(cfgREG[j].lookupValue("rname", r.str_name) &&
          cfgREG[j].lookupValue("raddr", r.raddr)))
    {
      LOGE("Error reading 'rname' on %s: %s REG: %d\n",
           pn->str_folder.c_str(), pn->str_dev_name.c_str(), j);
      exit(EXIT_FAILURE);
      continue;
    }

    // Check the record which expected for SCADA reg.
    if (cfgREG[j].lookupValue("rsource", r.str_source) &&
        cfgREG[j].lookupValue("rfolder", r.str_rfolder))
    {
      LOGI("Reading 'rsource'/'rfolder' on %s: %s/%s REG: %d",
           r.str_name.c_str(), r.str_source.c_str(), r.str_rfolder.c_str(), j);

      if (r.str_source == "-")
      {
        if (!(cfgREG[j].lookupValue("rmode", r.str_mode) &&
              cfgREG[j].lookupValue("rtype", r.str_type)))
        {
          LOGE("Error reading details on %s: %s REG: %d\n",
               r.str_name.c_str(), j);
          exit(EXIT_FAILURE);
          continue;
        }
      }
      else
      {
        cfgREG[j].lookupValue("rmode", r.str_mode);
        cfgREG[j].lookupValue("rtype", r.str_type);
      }
    }
    else
    {
      // Check records for Modbus register.
      if (!(cfgREG[j].lookupValue("rmode", r.str_mode) &&
            cfgREG[j].lookupValue("rtype", r.str_type)))
      {
        LOGE("Error reading details on %s, REG: %d\n", r.str_name.c_str(), j);
        exit(EXIT_FAILURE);
        continue;
      }

      if (!cfgREG[j].lookupValue("rfolder", r.str_rfolder))
        r.str_rfolder = ".";

      if (r.str_type == "*")
        r.str_mode = pn->regs[r.raddr - 1].str_mode;
    }
*/

/*
  void cfg_deinit() { PLCvec.clear(); }
*/
// eof
