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
//void reg_print_shm(RegMap_c*);

void regs_update()
{
  printf("\n===== regs_update =====\n");
  bool is_eol = false;

  for (auto &[n, rm] : REGmap) {
    reg_print(n, rm.ptr_data_plc);

    uint16_t plc_val = rm.get_plc_val();  // Value from PLC
    uint16_t old_val = rm.value;          // Value in memory (in REGmap)
    uint16_t opc_val = opc_update_uint16(n, rm.ptr_reg, plc_val);
    //uint16_t shm_val = rm.get_local();    // Value in SHM
    uint16_t shm_val = opc_val;

    if (rm.get_mode()) {  // If the Reg RW - get&check value from SHM.

      if (plc_val != old_val)  // If new value got from PLC
        printf(">");           // Print sign ">"
      else
        printf(" ");

      if (shm_val != old_val) {  // If new value got from SHM (SCADA/OPC)
        rm.set_plc_val(shm_val);
        printf("<");
        //printf("<%5d", shm_val);
      } else {
        printf(" ");
        //printf("      ");
      }

    } else
      printf("  ");  // Reg is not RW
    //printf("       ");  // Reg is not RW

    printf("%5d %5d", opc_val, old_val);

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

void reg_print(string rn, const regdata_t* rd)
{
  // printf("\n===== regs_print =====\n");
  const char* C = C_WHIB; //NRM;
  const char* B = NRM;
  if (rd->rerrors > 0) {
    C = C_GRY; //C = C_RED;
    B = "\x1B[5m";  // Dark grey blym-blym
  }

  if (rd->rtype == 0)
    printf("%s%-12s %s%7d", C, rn.c_str(), B, (uint16_t)rd->rvalue);
  else if (rd->rtype == 1)
    printf("%s%-12s %s%7d", C, rn.c_str(), B, (int16_t)rd->rvalue);
  else if (rd->rtype == 2)
    printf("%s%-12s %s%7.2f", C, rn.c_str(), B, (int16_t)rd->rvalue * 0.01);

  printf(NRM);

  return;
}

/*
  void regs_update_shm()
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
*/


/*
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

// eof
