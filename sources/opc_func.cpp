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
    n = rm.str_opcname;
    e = Cfg.opc.ErrFolder + n + Cfg.opc.ErrSuffix;  // Kub.Temp1.errors

    OPCs.addVar(e, (uint16_t)0, 0);  // Reg to keep NB of errors

    value_u v;
    v.ui64 = 0;
    int t = rm.var_type;
    if (rm.is_modbus)
      t = UA_TYPES_UINT16;

    if (t == UA_TYPES_UINT16)
      OPCs.addVar(n, rm.get_local_value().ui16, rm.var_mode);
    else if (t == UA_TYPES_INT16)
      OPCs.addVar(n, rm.get_local_value().i16, rm.var_mode);
    else if (t == NOTUA_TYPES_F100)
      OPCs.addVar(n, rm.get_local_value().fl, rm.var_mode);
    else if (t == NOTUA_TYPES_F10)
      OPCs.addVar(n, rm.get_local_value().fl, rm.var_mode);
    else if (t == UA_TYPES_FLOAT)
      OPCs.addVar(n, rm.get_local_value().fl, rm.var_mode);
    else if (t == UA_TYPES_DOUBLE)
      OPCs.addVar(n, rm.get_local_value().dbl, rm.var_mode);
    else
      LOGE("Wrong type: %d, Var: %s", t, n.c_str());
  }
}

// uint16_t opc_update_uint16(string name, regdata_t* rd)
/*
  uint16_t opc_update_uint16(string name, Reg_c* R)
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

  OPCs.updateVar(Cfg.opc.ErrFolder + n + Cfg.opc.ErrSuffix, rd->rerrors, true);

  // TODO: full recode with new TYPE_*
  if (rtype == 2)
    val_get = CAST(uint16_t)(100 * OPCs.updateVar(n, val_fl, isOK));
  else if (rtype == 1)
    val_get = CAST(uint16_t)(OPCs.updateVar(n, val_i16, isOK));
  else if (rtype == 0)
    val_get = CAST(uint16_t)(OPCs.updateVar(n, val_ui16, isOK));

  return val_get;
  }
*/

value_u opc_get_value(string s) { return OPCs.ReadRawValue(OPCs.getVarFullName(s)); }

bool opc_set_value(string s, value_u val, bool isOK)
{
  return OPCs.WriteRawUnion(OPCs.getVarFullName(s), val, isOK);
}

void opc_deinit() { OPCs.stop(); }

void opc_init() { OPCs.init(Cfg.opc.SrvPort); }

void opc_start()
{
  std::thread opc_thread(opc_run_thread);
  opc_thread.detach();
  wait_console(Cfg.timeout_sec);
}

void opc_run_thread()
{
  prctl(PR_SET_NAME, Cfg.opc.SrvName);
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
