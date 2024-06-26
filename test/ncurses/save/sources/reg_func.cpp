// reg_func.cpp -----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <string.h>

#include <map>
#include <string>
#include <vector>

#include "./config.h"
#include "./libs.h"

#define MB_READ

// void regs_init();
void reg_print(string, const regdata_t*);
void reg_print_shm(RegMap_c*);

void regs_create_from_masters()
{
  PRINTF("\n===== reg_init =====\n");

  for (auto &D : PLCvec)
    for (auto &[a, R] : D.regs) {
      LOGD("(Master) try to create %s", R.fullname.c_str());
      // RegMap_c rm(&R);
      // REGmap[R.fullname] = rm;
      REGmap[R.fullname] = {&R};
    }
  return;
}

void regs_init_shm()
{
  PRINTF("\n===== reg_init =====\n");

  for (auto &D : PLCvec)
    for (auto &[a, R] : D.regs) {
      LOGD("(Slave) try to create %s", R.fullname.c_str());
      REGmap[R.fullname] = {R.fullname};
    }
  return;
}

void regs_update()
{
  PRINTF("\n===== regs_update =====\n");
  bool is_eol = false;
  refresh();

  for (auto &[n, rm] : REGmap) {

    reg_print(n, rm.ptr_data_plc);

    uint16_t plc_val = rm.get_plc_val(); // Value from PLC
    uint16_t shm_val = rm.get_local();   // Value in SHM
    uint16_t old_val = rm.value;         // Value in memory (in REGmap)

    if (rm.get_mode()) { // If the Reg RW - get&check value from SHM.

      if (plc_val != old_val) // If new value got from PLC
        PRINTF(">");          // Print sign ">"
      else
        PRINTF(" ");

      if (shm_val != old_val) {
        // If new value got from SHM (SCADA?)
        rm.set_plc_val(shm_val);
        PRINTF("<%5d", shm_val);
      } else
        PRINTF("      ");
    } else
      PRINTF("       "); // Reg is not RW

    D(PRINTF("~%2d ", rm.fd);) // Show filedescriptor

    rm.value = plc_val; // Save PLC value to REGmap
    rm.sync(plc_val);

    if (is_eol)
      PRINTF(" + %s\n", KNRM);
    else
      PRINTF(" +     %s", KNRM);

    is_eol = !is_eol;
  }

  if (is_eol)
    PRINTF("\n");
  
  refresh();

  return;
}

void regs_update_shm()
{
  PRINTF("\n===== regs_update_shm =====\n");
  bool is_eol = false;

  for (auto &[n, rm] : REGmap) {

    reg_print_shm(&rm);

    uint16_t shm_val = rm.get_local(); // Value in SHM
    uint16_t old_val = rm.value;       // Value in memory (in REGmap)

    if (rm.get_mode()) { // If the Reg RW - get&check value from SHM.

      if (shm_val != old_val)
        PRINTF("<%5d", old_val);
      else
        PRINTF("      ");

    } else
      PRINTF("      "); // Reg is not RW

    D(PRINTF(" ~%2d", rm.fd);) // Show filedescriptor
    rm.sync(shm_val);

    if (is_eol)
      PRINTF(" + %s\n", KNRM);
    else
      PRINTF(" +     %s", KNRM);

    is_eol = !is_eol;
  }

  if (is_eol)
    PRINTF("\n");

  return;
}

void reg_print(string rn, const regdata_t* rd)
{
  const char* C = KNRM;
  if (rd->rerrors > 0)
    C = KRED;

  if (rd->rtype)
    PRINTF("%s%-14s %7.2f", C, rn.c_str(), (int16_t)rd->rvalue * 0.01);
  else
    PRINTF("%s%-14s %7d", C, rn.c_str(), rd->rvalue);

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
  const char* C = KNRM;
  if (rerrors > 0)
    C = KRED;

  if (rtype)
    PRINTF("%s%-14s %7.2f", C, rm->rn, (int16_t)rvalue * 0.01);
  else
    PRINTF("%s%-14s %7d", C, rm->rn, rvalue);

  return;
}

void regs_deinit_shm()
{
  for (auto &[n, rm] : REGmap) {
    close_shm(rm.fd, rm.ptr_data_shm, sizeof(regdata_t));
    unlink_shm(n.c_str());
  }
  return;
}

void regs_deinit()
{
  REGmap.clear();
  return;
}

// eof
