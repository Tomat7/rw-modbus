// reg_func.cpp -----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <string.h>

#include <map>
#include <string>
#include <vector>

#include "config.h"
#include "libs.h"

#define MB_READ

// void regs_init();
// void reg_print(string, const regdata_t*);
// void reg_print_shm(RegMap_c*);

string folder = "/PLC/";

void opc_regs_init()
{
  printf("\n===== OPC_regs_init =====\n");

  string n;

  for (auto &[name, rm] : REGmap) {
    // reg_print(n, rm.ptr_data_plc);
    // n - name, rm - RegMap_c rm.set_shm_val();

    n = folder + name;

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

    //folder = "/Scada/PLC/";

    string parent = name;
    size_t z = parent.find(".");
    parent.erase(z);

    n = folder + parent + "/" + name;

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


uint16_t opc_update_uint16(string name, reg_t* ptr_r, uint16_t val)
{
  // printf("\n===== OPC_update_uint16 =====\n");
  string str_type = ptr_r->str_type;
  bool reg_is_OK = ((ptr_r->data).rerrors == 0);
  string parent = name;
  size_t z = parent.find(".");
  parent.erase(z);

  string n = folder + parent + "/" + name;
  uint16_t res = 0;

  if (str_type == "f") {
    float fl = OPCs.getVar(n, fl);
    res = (uint16_t)(fl * 100);
    OPCs.setVar(n, (int16_t)val * (float)0.01, reg_is_OK);
  } else if (str_type == "i") {
    int16_t i16 = OPCs.getVar(n, i16);
    res = (uint16_t)i16;
    OPCs.setVar(n, (int16_t)val, reg_is_OK);
  } else if (str_type == "u") {
    uint16_t ui16 = OPCs.getVar(n, ui16);
    res = (uint16_t)ui16;
    OPCs.setVar(n, (uint16_t)val, reg_is_OK);
  }

  return res;
}


void opc_deinit()
{
  OPCs.stop();
  return;
}

void opc_init()
{
  OPCs.init(4840);
  OPCs.run();
  return;
}

// eof
