// cfg_func.cpp --------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <libconfig.h++>
#include <set>
#include <utility>
#include <map>
#include <string>
#include <vector>

#include "config.h"
#include "libs.h"
// #include "./plc_class.h"

using cchar = const char;

using std::set;
using libconfig::Setting;

static int total_regs = 0;

int cfg_init_scadaset(const Setting &cfg, const Setting &pl);
int cfg_init_scadaregs(const Setting &reg, string _dname, string _dfolder);

int cfg_init_scadaset(const Setting &cfgPLC, const Setting &listPLC)
{
  bool isCheckName = false;
  int nb_plc_cfg = cfgPLC.getLength();
  int nb_plc_list = listPLC.getLength();
  int nb_plc_ready = 0;
  set<string> PLClst;

  // uint64_t vec_size_new = 0; // PLCvec.size() + nb_plc_cfg;

  LOGW("Total SCADA sets in config: %d, in the list: %d.",
       nb_plc_cfg, nb_plc_list);

  const string &p0 = listPLC[0];
  if ((p0 == "all") || (p0 == "ALL") || (p0 == "All")) {
    //  vec_size_new = PLCvec.size() + nb_plc_cfg;
    LOGC("List with %d SCADA sets ignored.", nb_plc_list);
    LOGC("Will read '%s' %d SCADA sets from configfile!", p0.c_str(),
         nb_plc_cfg);
  } else {
    isCheckName = true;
    //  vec_size_new = PLCvec.size() + nb_plc_list;
    for (int i = 0; i < nb_plc_list; ++i)
      PLClst.insert(listPLC[i]);
  }

  for (int i = 0; i < nb_plc_cfg; ++i) {
    string _devname, _dfolder, _desc, _ip;
    //    int _port, _att, _ms, _us;

    // ===== Check the record which expect to get for CFG-file.
    if (cfgPLC[i].lookupValue("name", _devname) &&
        cfgPLC[i].lookupValue("folder", _dfolder) &&
        cfgPLC[i].lookupValue("desc", _desc)) {
      // It is SCADA registers configuration!

      int nb_regs = cfg_init_scadaregs(cfgPLC[i]["regs"], _devname, _dfolder);
      total_regs += nb_regs;
      nb_plc_ready++;

      LOGI("Configured SCADA set: %s, with: %d regs", _devname.c_str(),
           nb_regs);

      // ===== End SCADA virtual filling =====
    } else {
      LOGA("Error reading SCADA configuration: %d\n", i);
      continue;  // get out of current iteration if any field wrong in CFG-file
      // */
    }
  }

  LOGI("Total SCADA sets: %d, with %d regs", nb_plc_cfg, total_regs);

  if (isCheckName && (nb_plc_ready != nb_plc_list))
    LOGA("Wrong PLCs number! Processed: %d, in the list: %d.",
         nb_plc_ready, nb_plc_list);

  return 0;
}

int cfg_init_scadaregs(const Setting &cfgREG, string _dname, string _dfolder)
{
  int nb_regs = cfgREG.getLength();
  int nb_errors = 0;

  // ===== Cycle for REGs =====
  for (int j = 0; j < nb_regs; ++j) {
    mbreg_t r;
    string s_source, s_type;

    if (cfgREG[j].lookupValue("rsource", s_source) &&
        cfgREG[j].lookupValue("rfolder", r.str_rfolder) &&
        cfgREG[j].lookupValue("rname", r.str_rname)) {
      // This is SCADA register/variable/tag

      if (!(cfgREG[j].lookupValue("rmode", r.str_mode) &&
            cfgREG[j].lookupValue("rtype", s_type))) {

        LOGE("Error reading 'rmode'/'rtype' on %s/%s REG: %d\n",
             _dname.c_str(), r.str_rname.c_str(), j);
        continue;
      }

      r.raddr = j;
      // LOGI("Read 'rsource'/'rfolder' on %s: %s/%s REG: %d", r.str_rname.c_str(),
      // r.str_source.c_str(), r.str_rfolder.c_str(), j);
    } else {
      LOGE("Error reading 'rname' on %s: %s REG: %d\n",
           _dfolder.c_str(), _dname.c_str(), j);
      exit(EXIT_FAILURE);
      continue;
    }

    r.data.rvalue = 888;  // TODO: remove for production!

    if (_dname == "-" || _dname == "." || _dname == "")  // No prefix.name?
      r.rfullname = r.str_rname;
    else
      r.rfullname = _dname + "." + r.str_rname;

    string str_opcbase = "/" + _dfolder + "/" + _dname + "/";
    mbreg_t* ptr_source = nullptr;

    if (!(s_source == "") && !(s_source == "-")) {
      if (reg_exist(s_source))
        ptr_source = REGmap[s_source].get_ptr(0);
      else {
        LOGE("Wrong 'rsource': '%s' on reg: '%s' - IGNORED!",
             s_source.c_str(), r.str_rname.c_str());
        continue;
      }
    }

    if (Reg_c::check_type(s_type)) {
      //Reg_c R(&r, ptr_source, s_source, s_type, str_opcbase);
      //REGmap[r.rfullname] = R;
      REGmap[r.rfullname] = { &r, ptr_source, s_source, s_type, str_opcbase };
    } else {
      nb_errors++;
      LOGE("Wrong type: '%s', reg: '%s' - IGNORED!",
           s_type.c_str(), r.str_rname.c_str());
    }

  }

  return nb_regs - nb_errors;
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
