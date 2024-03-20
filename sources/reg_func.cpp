// reg_func.cpp -----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <map>
#include <string.h>
#include <string>
#include <vector>

#include "./config.h"
#include "./libs.h"

#define MB_READ

void regs_init();
void reg_print(string, const reg_t *);
// void reg_init_name(string devname, string regname, uint16_t *val);

void regs_init() {
  cout << endl << "===== reg_init =====" << endl;

  for (auto &D : PLCset)
    for (auto &R : D.regs) {
      string rn = R.fullname; // D.dev_name + "." + (string)R.ch_name;
      // R.fullname = rn.c_str();
      R.rvalue = 5757; // TODO: remove for production!!
      //      LOGINFO("SHM: try %s\n", R.fullname.c_str());

      rmap_t rm;
      rm.ptr_reg = &R;
      rm.rdata.rvalue = R.rvalue;
      rm.rdata.rerrors = R.rerrors;
      rm.rdata.rmode = R.rmode;
      rm.rdata.rtype = R.rtype;

      rm.fd = create_shm_fd(rn.c_str());
      if (rm.fd != -1) {
        rdata_t *addr = (rdata_t *)create_shm_addr(rm.fd, sizeof(rdata_t));
        if (addr != nullptr) {
          LOGINFO("SHM: created %s\n", rn.c_str());
          rm.ptr_shm = addr;
        }
      }

      REGmap[rn] = rm;
    }

  return;
}

void regs_update() {

  printf("\n===== regs_update =====\n");
  bool is_eol = false;

  for (auto &[rn, m] : REGmap) {
    reg_print(rn, m.ptr_reg);

    const auto &plc = m.ptr_reg; // "plc" is pointer to PLC regs struct
    auto &mem = m.rdata;
    rdata_t shm; // Temporary! For values from SHM. Now is empty.
    uint16_t remote_val = plc->rvalue; // Value from PLC
    uint16_t mem_val = mem.rvalue;     // Value in memory (in REGmap)
    uint16_t &shm_val = shm.rvalue;    // Value from SHM (will be!)

    if (plc->rmode) { // Is the Reg RW? If YES - get&check value from SHM.
      memcpy(&shm, m.ptr_shm, sizeof(rdata_t)); // Copy from SHM to tmp struct.

      if (mem_val != remote_val) // If new value got from PLC
        printf(" >");            // Print sign ">"
      else
        printf("  ");

      if (mem_val != shm_val) { // If new value got from SHM (SCADA?)
        plc->rupdate = 1;
        plc->rvalue = shm_val; // Put new value to PLC
        printf("< %d", shm_val);
      } else
        printf("  ");
    } else
      printf("    ");

    mem.rvalue = remote_val;    // Fill REGmap with PLC value
    mem.rerrors = plc->rerrors; // ... PLC errors
    mem.rstatus = plc->rstatus; // ... PLC status

    memcpy(m.ptr_shm, &m.rdata, sizeof(rdata_t));

    if (is_eol)
      printf("  + %s\n", KNRM);
    else
      printf("  +          %s", KNRM);

    is_eol = !is_eol;
  }

  return;
}

void reg_print(string rn, const reg_t *r) {

  const char *C = KNRM;
  if (r->rerrors > 0)
    C = KRED;

  if (r->rtype)
    printf("%s%-14s %7.2f", C, rn.c_str(), (int16_t)r->rvalue * 0.01);
  else
    printf("%s%-14s %7d", C, rn.c_str(), r->rvalue);

  return;
}

void regs_deinit() {
  REGmap.clear();
  return;
}

// eof
