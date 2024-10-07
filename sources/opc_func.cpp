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
void reg_print(string, const regdata_t*);
void reg_print_shm(RegMap_c*);

string folder = "/PLC/";

void opc_regs_init()
{
  printf("\n===== OPC_regs_init =====\n");

  for (auto &[name, rm] : REGmap) {
    // reg_print(n, rm.ptr_data_plc);
    // n - name, rm - RegMap_c rm.set_shm_val();

    string n = folder + name;
    //string n = name;

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
  /*
    folder = "PLC";

    for (auto &[name, rm] : REGmap) {
      // reg_print(n, rm.ptr_data_plc);
      // n - name, rm - RegMap_c, rm.set_shm_val();
      string n = "/" + folder + "/" + name;

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
  */
  /*
    folder = "PLC";

    for (auto &[name, rm] : REGmap) {
      // reg_print(n, rm.ptr_data_plc);
      // n - name, rm - RegMap_c, rm.set_shm_val();
      string n = "/" + folder + "/" + name;

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

    folder = "Scada";

    for (auto &[name, rm] : REGmap) {
      // reg_print(n, rm.ptr_data_plc);
      // n - name, rm - RegMap_c, rm.set_shm_val();
      string n = "/" + folder + "/" + name;

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
  */
}


uint16_t opc_update_uint16(string name, string str_type, uint16_t val)
{
  printf("\n===== OPC_update_uint16 =====\n");

  string n = folder + name;
  //string n = name;

  uint16_t res = 0;

  printf("\nTry: %s\n", n.c_str());

  if (str_type == "f") {
    float fl = OPCs.getVar(n, fl);
    //res = (uint16_t)(fl * 100);
    OPCs.setVar(n, (int16_t)val * (float)0.01);
  } else if (str_type == "i") {
    int16_t i16 = OPCs.getVar(n, i16);
    res = (uint16_t)i16;
    OPCs.setVar(n, (int16_t)val);
  } else if (str_type == "u") {
    uint16_t ui16 = OPCs.getVar(n, ui16);
    //res = (uint16_t)ui16;
    OPCs.setVar(n, (uint16_t)val);
  }

  return res;
}
/*
  void opc_update(string n, uint16_t val, )
  {
  printf("\n===== OPC_regs_update =====\n");
  bool is_eol = false;

  for (auto &[n, rm] : REGmap) {
    reg_print(n, rm.ptr_data_plc);

    uint16_t plc_val = rm.get_plc_val();  // Value from PLC
    uint16_t shm_val = rm.get_local();    // Value in SHM
    uint16_t old_val = rm.value;          // Value in memory (in REGmap)

    if (rm.get_mode()) {  // If the Reg RW - get&check value from SHM.

      if (plc_val != old_val)  // If new value got from PLC
        printf(">");           // Print sign ">"
      else
        printf(" ");

      if (shm_val != old_val) {  // If new value got from SHM (SCADA?)
        rm.set_plc_val(shm_val);
        printf("<%5d", shm_val);
      } else
        printf("      ");
    } else
      printf("       ");  // Reg is not RW

    D(printf("~%2d ", rm.fd);)  // Show filedescriptor

    rm.value = plc_val;  // Save PLC value to REGmap
    rm.sync(plc_val);

    if (is_eol)
      printf(" + %s\n", NRM);
    else
      printf(" +     %s", NRM);

    is_eol = !is_eol;
  }

  if (is_eol)
    printf("\n");

  return;
  }
*/

/* void regs_update_shm()
  {
  printf("\n===== regs_update_shm =====\n");
  bool is_eol = false;

  for (auto &[n, rm] : REGmap) {
    reg_print_shm(&rm);

    uint16_t shm_val = rm.get_local();  // Value in SHM
    uint16_t old_val = rm.value;        // Value in memory (in REGmap)

    if (rm.get_mode()) {  // If the Reg RW - get&check value from SHM.

      if (shm_val != old_val)
        printf("<%5d", old_val);
      else
        printf("      ");

    } else
      printf("      ");  // Reg is not RW

    D(printf(" ~%2d", rm.fd);)  // Show filedescriptor
    rm.sync(shm_val);

    if (is_eol)
      printf(" + %s\n", NRM);
    else
      printf(" +     %s", NRM);

    is_eol = !is_eol;
  }

  if (is_eol)
    printf("\n");

  return;
  }

  void reg_print(string rn, const regdata_t* rd)
  {
  const char* C = NRM;
  if (rd->rerrors > 0)
    C = C_RED;

  if (rd->rtype)
    printf("%s%-14s %7.2f", C, rn.c_str(), (int16_t)rd->rvalue * 0.01);
  else
    printf("%s%-14s %7d", C, rn.c_str(), rd->rvalue);

  return;
  }

  void reg_print_shm(RegMap_c* rm)
  {
  int rerrors = 1;
  int rtype = 0;
  uint16_t rvalue = 0;

  if (rm->is_shm()) {
    regdata_t* rd = rm->ptr_data_shm;
    rerrors = rd->rerrors;
    rtype = rd->rtype;
    rvalue = rd->rvalue;
  }

  const char* C = NRM;
  if (rerrors > 0)
    C = C_RED;

  if (rtype)
    printf("%s%-14s %7.2f", C, rm->rn, (int16_t)rvalue * 0.01);
  else
    printf("%s%-14s %7d", C, rm->rn, rvalue);

  return;
  }

*/

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
