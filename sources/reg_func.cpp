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
void reg_print(string, const reg_t*);
// void reg_init_name(string devname, string regname, uint16_t *val);

void regs_init()
{
  cout << endl
       << "===== reg_init =====" << endl;

  for (auto &D : PLCset)
    for (auto &[n, R] : D.regs) {
      // auto &rd = R.data;
      // R.data.rvalue = 5757;  // TODO: remove for production!!
      // RegMap_c rm;
      // rm.ptr_reg = &R;
      // rm.ptr_data_plc = &R.data;
      /*
        rm.rdata.rvalue = rd.rvalue;
        rm.rdata.rerrors = rd.rerrors;
        rm.rdata.rmode = rd.rmode;
        rm.rdata.rtype = rd.rtype;
      */

      regdata_t* addr;
      int fd = create_shm_fd(R.fullname.c_str());
      if (fd != -1) {
        addr = (regdata_t*)create_shm_addr(fd, sizeof(regdata_t));
        if (addr != nullptr) {
          LOGINFO("SHM: created %s\n", R.fullname.c_str());
          RegMap_c rm(fd, addr, &R.data, &R);
          REGmap[R.fullname] = rm;
        }
      }
    }

  return;
}

void regs_update()
{
  printf("\n===== regs_update =====\n");
  bool is_eol = false;

  for (auto &[n, rm] : REGmap) {
    reg_print(n, rm.ptr_reg);

    // const auto &plc = rm.ptr_data_plc;  // "plc" is pointer to PLC regs struct
    // auto &shm = rm.ptr_data_shm;
    // regdata_t mem;  // Temporary! For values from SHM. Now is empty.
    uint16_t plc_val = rm.get_remote(); // Value from PLC
    uint16_t shm_val = rm.get_local();  // Value in memory (in REGmap)
    uint16_t old_val = rm.value;        // Value in memory (in REGmap)
    // uint16_t &shm_val = shm.rvalue;     // Value from SHM (will be!)

    if (rm.get_mode()) {
      // Is the Reg RW? If YES - get&check value from SHM.
      //      memcpy(&shm, m.ptr_data_shm, sizeof(regdata_t));  // Copy from SHM to tmp struct.

      if (plc_val != old_val) // If new value got from PLC
        printf(" >");           // Print sign ">"
      else
        printf("  ");

      if (shm_val != old_val) {
        // If new value got from SHM (SCADA?)
        rm.set_remote(shm_val);
        printf("< %d", shm_val);
      } else
        printf("  ");
    } else
      printf("    "); // Reg is not RW

    rm.value = plc_val; // Put PLC value to REGmap
    rm.sync(plc_val);
    // mem.rerrors = plc->data.rerrors;  // ... PLC errors
    // mem.rstatus = plc->data.rstatus;  // ... PLC status
    // memcpy(m.ptr_data_shm, &m.rdata, sizeof(regdata_t));

    if (is_eol)
      printf("  + %s\n", KNRM);
    else
      printf("  +          %s", KNRM);

    is_eol = !is_eol;
  }

  return;
}

void reg_print(string rn, const reg_t* r)
{
  const char* C = KNRM;
  if (r->data.rerrors > 0)
    C = KRED;

  if (r->data.rtype)
    printf("%s%-14s %7.2f", C, rn.c_str(), (int16_t)r->data.rvalue * 0.01);
  else
    printf("%s%-14s %7d", C, rn.c_str(), r->data.rvalue);

  return;
}

void regs_deinit()
{
  REGmap.clear();
  return;
}

// eof
