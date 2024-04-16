// reg_func.cpp -----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <string.h>

#include <map>
#include <string>
#include <vector>

#include "./config.h"
#include "./libs.h"

#define MB_READ

void regs_init();
void reg_print(string, const regdata_t*);
// void reg_init_name(string devname, string regname, uint16_t *val);

void regs_init()
{
  cout << endl << "===== reg_init =====" << endl;

  for (auto &D : PLCset)
    for (auto &[n, R] : D.regs) {
      LOGD("try to create %s", R.fullname.c_str());
      RegMap_c rm(&R);
      REGmap[R.fullname] = rm;
    }
  return;
}

void regs_update()
{
  printf("\n===== regs_update =====\n");
  bool is_eol = false;

  for (auto &[n, rm] : REGmap) {

    reg_print(n, rm.ptr_data_plc);

    uint16_t plc_val = rm.get_plc_val(); // Value from PLC
    uint16_t shm_val = rm.get_local();   // Value in SHM
    uint16_t old_val = rm.value;         // Value in memory (in REGmap)

    if (rm.get_mode()) {
      // Is the Reg RW? If YES - get&check value from SHM.

      if (plc_val != old_val) // If new value got from PLC
        printf(">");          // Print sign ">"
      else
        printf(" ");

      if (shm_val != old_val) {
        // If new value got from SHM (SCADA?)
        rm.set_plc_val(shm_val);
        printf("<%5d", shm_val);
      } else
        printf("      ");
    } else
      printf("       "); // Reg is not RW

    D(printf("~%2d ", rm.fd);) // Show filedescriptor

    rm.value = plc_val; // Save PLC value to REGmap
    rm.sync(plc_val);

    if (is_eol)
      printf(" + %s\n", KNRM);
    else
      printf(" +     %s", KNRM);

    is_eol = !is_eol;
  }

  return;
}

void regs_update_shm()
{
  printf("\n===== regs_update_shm =====\n");
  bool is_eol = false;

  for (auto &[n, rm] : REGmap) {
    if (rm.is_shm())
      reg_print(n, rm.ptr_data_shm);

    // uint16_t plc_val = rm.get_plc_val(); // Value from PLC
    uint16_t shm_val = rm.get_local(); // Value in SHM
    uint16_t old_val = rm.value;       // Value in memory (in REGmap)

    if (rm.get_mode()) { // If the Reg RW - get&check value from SHM.

      if (shm_val != old_val)
        printf("<%5d", old_val);
      else
        printf("      ");

    } else
      printf("      "); // Reg is not RW

    D(printf(" ~%2d", rm.fd);) // Show filedescriptor
    rm.sync(shm_val);

    if (is_eol)
      printf(" + %s\n", KNRM);
    else
      printf(" +     %s", KNRM);

    is_eol = !is_eol;
  }

  return;
}

void reg_print(string rn, const regdata_t* rd)
{
  const char* C = KNRM;
  if (rd->rerrors > 0)
    C = KRED;

  if (rd->rtype)
    printf("%s%-14s %7.2f", C, rn.c_str(), (int16_t)rd->rvalue * 0.01);
  else
    printf("%s%-14s %7d", C, rn.c_str(), rd->rvalue);

  return;
}

void regs_deinit()
{
  for (auto &[n, rm] : REGmap) {
    close_shm(rm.fd, (void*)rm.ptr_data_shm, sizeof(regdata_t));
//    close_fd(rm.fd);
    unlink_shm(n.c_str());
  }
  REGmap.clear();
//  delete[] P;

  return;
}

// eof
