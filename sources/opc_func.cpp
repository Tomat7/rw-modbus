// reg_func.cpp -----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <string.h>

#include <map>
#include <string>
#include <vector>
#include <sys/prctl.h>

#include "config.h"
#include "libs.h"

#define MB_READ

#define CAST(_XTYPE) static_cast<_XTYPE>

// void regs_init();
// void reg_print(string, const regdata_t*);
// void reg_print_shm(RegMap_c*);

void opc_regs_init()
{
  printf("\n===== OPC_regs_init =====\n");

  string n, e;

  for (auto& [name, rm] : REGmap) {
    // reg_print(n, rm.ptr_data_plc);
    // n - name, rm - RegMap_c rm.set_shm_val();

    string parent = name;
    size_t z = parent.find(".");
    parent.erase(z);

    n = PLC_folder + parent + "/" + name;  // full - /PLC/Kub/Kub.Temp1
    e = n + PLC_ERRORS;  // Kub.Temp1.errors

    OPCs.addVar(e, (uint16_t)0, 0); // Reg to keep NB of errors

    if (rm.ptr_reg->str_type == "f") {
      float fl = (int16_t)(rm.ptr_data_plc->rvalue) * (float)0.01;
      OPCs.addVar(n, fl, rm.ptr_data_plc->rmode);
    } else if (rm.ptr_reg->str_type == "i") {
      int16_t i16 = (int16_t)(rm.ptr_data_plc->rvalue);
      OPCs.addVar(n, i16, rm.ptr_data_plc->rmode);
    } else if (rm.ptr_reg->str_type == "u") {
      uint16_t ui16 = (uint16_t)(rm.ptr_data_plc->rvalue);
      OPCs.addVar(n, ui16, rm.ptr_data_plc->rmode);
    }
  }
}

uint16_t opc_update_uint16(string name, regdata_t* rd)
{
  // printf("\n===== OPC_update_uint16 =====\n");
  uint16_t val_ui16 = rd->rvalue;
  int16_t val_i16 = (int16_t)rd->rvalue;
  float val_fl = (int16_t)rd->rvalue * (float)0.01;
  bool isOK = (rd->rerrors == 0);
  int rtype = rd->rtype;

  string parent = name;
  size_t z = parent.find(".");
  parent.erase(z);

  string n = PLC_folder + parent + "/" + name;
  uint16_t val_get = 0;

  OPCs.updateVar(n + PLC_ERRORS, rd->rerrors, true);

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

void opc_run()
{
  prctl(PR_SET_NAME, "OPC_server");
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

// eof
