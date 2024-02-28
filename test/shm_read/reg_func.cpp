// reg_func.cpp -----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <string.h>

#include <string>
#include <vector>

#include "./config.h"
#include "./libs.h"

#define MB_READ

// using ::std;
// using ::libconfig;

void regs_init();
void reg_print_name();
// void reg_init_name(string devname, string regname, uint16_t *val);

void regs_init() {
  cout << endl << "===== reg_init =====" << endl;

  for (auto &D : PLCset)
    for (auto &R : D.regs) {
      string rn = (string)D.dev_name + "." + (string)R.ch_name;
      R.fullname = rn.c_str();
      R.rvalue = 5757; // TODO: remove for production!!

      rmap_t rm;
      rm.p_reg = &R;
      rm.rdata.rvalue = R.rvalue;
      rm.rdata.rerrors = R.rerrors;
      rm.rdata.rmode = (strcmp(R.ch_mode, "rw") == 0) ? 1 : 0;
      rm.rdata.rtype = (strcmp(R.ch_type, "f") == 0) ? 1 : 0;

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
  /*
        rshm_t rm;
        rm.rvalue = R.rvalue;
        rm.rstatus = R.rstatus;
        rm.rmode = (strcmp(R.rmode, "rw") == 0) ? 1 : 0;
        rm.rtype = (strcmp(R.rtype, "f") == 0) ? 1 : 0;
        SHMmap[reg] = rm;
  */
  /*
        int fd = create_shm_fd(reg.c_str());
        if (fd != -1) {
          reg_t *addr = create_shm_addr(fd, sizeof(rshm_t));

          if (addr != nullptr) {
            LOGINFO("SHM: created %s\n", reg.c_str());
            REGmap[reg]->rfd = fd;
            REGmap[reg]->rshm = addr;
  //          close_shm(fd, addr, sizeof(rshm_t));
          } //else
  //          close_fd(fd);
        }
      }
  */
  return;
}

void reg_print_name() {

  cout << endl << "======= regs_print_name =======" << endl;

  for (auto &[rn, m] : REGmap) {
    const auto &r = m.p_reg;
    auto &d = m.rdata;

    const char *C = KNRM;
    if (r->rerrors < 0)
      C = KRED;

    if (strcmp(m.p_reg->ch_type, "i") == 0)
      printf("%s%-12s %7d" NRM, C, rn.c_str(), d.rvalue);
    else
      printf("%s%-12s %7.2f" NRM, C, rn.c_str(), (int16_t)d.rvalue * 0.01);

    d.rvalue = r->rvalue;
    d.rerrors = r->rerrors;
    const void *ptr = &m.rdata;

    memcpy(m.p_shm, ptr, sizeof(rdata_t));
    printf("   +\n");

    /*
        int fd  = get_shm_fd(rn.c_str());
        if (fd != -1) {
          reg_t *addr = get_shm_addr(fd, sizeof(rshm_t));

          if (addr != nullptr) {
            memcpy(addr, ptr, sizeof(rshm_t));
          close_shm(fd, addr, sizeof(rshm_t));
            memcpy(r->rshm, ptr, sizeof(rshm_t));
            printf("   +\n");
          } else {
            close_fd(fd);
            printf("   --\n");
          }

        } else
          printf("   -\n");
    */
  }

  return;
}

// eof
