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
      string rn = (string)R.fullname; // D.dev_name + "." + (string)R.ch_name;
      // R.fullname = rn.c_str();
      R.rvalue = 5757; // TODO: remove for production!!
      LOGINFO("SHM: try %s\n", R.fullname);

      rmap_t rm;
      rm.p_reg = &R;
      rm.rdata.rvalue = R.rvalue;
      rm.rdata.rerrors = R.rerrors;
      rm.rdata.rmode = R.rmode;
      rm.rdata.rtype = R.rtype;

      rm.fd = create_shm_fd(rn.c_str());
      if (rm.fd != -1) {
        rdata_t *addr = (rdata_t *)create_shm_addr(rm.fd, sizeof(rdata_t));
        if (addr != nullptr) {
          LOGINFO("SHM: created %s\n", rn.c_str());
          rm.p_shm = addr;
        }
      }

      REGmap[rn] = rm;
    }

  return;
}

void regs_update() {
  printf("\n===== regs_update =====\n");

  for (auto &[rn, m] : REGmap) {
    reg_print(rn, m.p_reg);

    const auto &plc = m.p_reg;
    auto &mem = m.rdata;
    rdata_t shm;
    uint16_t remote_val = plc->rvalue;
    uint16_t mem_val = mem.rvalue;
    uint16_t &shm_val = shm.rvalue;

    if (plc->rmode) {
      memcpy(&shm, m.p_shm, sizeof(rdata_t));

      if (mem_val != remote_val)
        printf(" >");
      else
        printf("  ");

      if (mem_val != shm_val) {
        plc->rupdate = 1;
        plc->rvalue = shm_val;
        printf("< %d", shm_val);
      } else
        printf("  ");
    } else
      printf("    ");

    mem.rvalue = remote_val;
    mem.rerrors = plc->rerrors;
    mem.rstatus = plc->rstatus;

    memcpy(m.p_shm, &m.rdata, sizeof(rdata_t));
    printf("  +\n");
  }

  return;
}

void reg_print(string rn, const reg_t *r) {

  const char *C = KNRM;
  if (r->rerrors > 0)
    C = KRED;

  if (r->rtype)
    printf("%s%-14s %7.2f" NRM, C, rn.c_str(), (int16_t)r->rvalue * 0.01);
  else
    printf("%s%-14s %7d" NRM, C, rn.c_str(), r->rvalue);

  return;
}

void regs_deinit() { REGmap.clear(); }

// eof
