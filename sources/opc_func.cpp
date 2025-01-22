// reg_func.cpp -----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <string.h>
#include <sys/prctl.h>

#include <map>
#include <string>
#include <vector>

#include "config.h"
#include "libs.h"

#define MB_READ

#define CAST(_XTYPE) static_cast<_XTYPE>


void opc_regs_init()
{
  printf("\n===== %s =====\n", __func__);

  for (auto &[name, rm] : REGmap) {
    // reg_print(n, rm.ptr_data_plc);
    // n - name, rm - RegMap_c rm.set_shm_val();
    string n, e;
    n = rm.ptr_reg->str_opcname;
    e = OPC_ERRORS_FOLDER + n + OPC_ERRORS_SUFFIX; // Kub.Temp1.errors

    OPCs.addVar(e, (uint16_t)0, 0); // Reg to keep NB of errors

    auto &rd = rm.ptr_data_plc;
    auto &t = rm.ptr_data_plc->rtype;

    if (t == TYPE_FLOAT_ABCD) {
      float fl = (int16_t)(rd->rvalue) * (float)1.0;
      OPCs.addVar(n, fl, rd->rmode);
    } else if (t == TYPE_FLOAT_CDAB) {
      float fl = (int16_t)(rd->rvalue) * (float)1.00;
      OPCs.addVar(n, fl, rd->rmode);
    } else if (t == TYPE_F10) {
      float fl = (int16_t)(rd->rvalue) * (float)0.1;
      OPCs.addVar(n, fl, rd->rmode);
    } else if (t == TYPE_F100) {
      float fl = (int16_t)(rd->rvalue) * (float)0.01;
      OPCs.addVar(n, fl, rd->rmode);
    } else if (t == TYPE_I16) {
      int16_t i16 = (int16_t)(rd->rvalue);
      OPCs.addVar(n, i16, rd->rmode);
    } else if (t == TYPE_U16) {
      uint16_t ui16 = (uint16_t)(rd->rvalue);
      OPCs.addVar(n, ui16, rd->rmode);
    }
  }
}

// uint16_t opc_update_uint16(string name, regdata_t* rd)
uint16_t opc_update_uint16(string name, RegShm_c* R)
{
  // printf("\n===== OPC_update_uint16 =====\n");
  regdata_t* rd = R->ptr_data_plc;
  uint16_t val_ui16 = R->get_value<uint16_t>(); // rd->rvalue;
  int16_t val_i16 = (int16_t)rd->rvalue;
  float val_fl = (int16_t)rd->rvalue * (float)0.01;
  bool isOK = (rd->rerrors == 0);
  int rtype = rd->rtype;

  string n = name; //(R->ptr_reg)->str_opcname;
  uint16_t val_get = 0;

  OPCs.updateVar(OPC_ERRORS_FOLDER + n + OPC_ERRORS_SUFFIX, rd->rerrors, true);

  if (rtype == 2)
    val_get = CAST(uint16_t)(100 * OPCs.updateVar(n, val_fl, isOK));
  else if (rtype == 1)
    val_get = CAST(uint16_t)(OPCs.updateVar(n, val_i16, isOK));
  else if (rtype == 0)
    val_get = CAST(uint16_t)(OPCs.updateVar(n, val_ui16, isOK));

  return val_get;
}

void opc_deinit() { OPCs.stop(); }

void opc_init() { OPCs.init(4840); }

void opc_start()
{
  std::thread opc_thread(opc_run_thread);
  opc_thread.detach();
  wait_console(timeout_sec);
}

void opc_run_thread()
{
  prctl(PR_SET_NAME, OPC_THREAD_NAME);
  OPCs.run();
}

/*
  template<typename T>
  uint16_t opc_update_var(string s, T Value_set, bool isOK)
  {
  T Value_get = Value_set;
  Value_get = OPCs.getVar(s, Value_get);
  if (OPCs.getType(s) == UA_TYPES_FLOAT) {
    Value_get = (uint16_t)(Value_get * 100);
    Value_set = Value_set * (T)0.01;
  }
  OPCs.setVar(s, Value_set, isOK);
  return (uint16_t)Value_get;
  }
*/

/*     string parent = name;
    string folder = rm.ptr_reg->str_title;
    size_t z = parent.find(".");
    parent.erase(z);
*/
// n = PLC_folder + parent + "/" + name;  // full - /PLC/Kub/Kub.Temp1
/* if (folder != "")
  n += "/" + folder;    // = /PLC/
  if (parent != "")
  n += + "/" + parent;  // = /PLC/Kub

  n += "/" + name;        // = /PLC/Kub/Kub.Temp1
*/

/*   string parent = name;
  string folder = rm.ptr_reg->str_title;
  size_t z = parent.find(".");
  parent.erase(z);
*/
// string n = PLC_folder + parent + "/" + name;

// eof
