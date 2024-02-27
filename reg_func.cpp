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

void reg_init();
void reg_print_name();
// void reg_init_name(string devname, string regname, uint16_t *val);

void reg_init() {
  cout << endl << "===== reg_init =====" << endl;

  for (auto &D : PLCset)
    for (auto &R : D.regs) {
      string rn = (string)D.dev_name + "." + (string)R.ch_name;
      R.fullname = rn.c_str();
      R.rvalue = 5757; // TODO: remove for production!!

      rmap_t rm;
      rm.preg = &R;
      rm.rdata.rvalue = R.rvalue;
      rm.rdata.rstatus = R.rstatus;
      rm.rdata.rmode = R.rmode;
      rm.rdata.rtype = R.rtype;
      //      rm.rdata.rmode = (strcmp(R.ch_mode, "rw") == 0) ? 1 : 0;
      //      rm.rdata.rtype = (strcmp(R.ch_type, "f") == 0) ? 1 : 0;

      rm.fd = create_shm_fd(rn.c_str());
      if (rm.fd != -1) {
        rdata_t *addr = (rdata_t *)create_shm_addr(rm.fd, sizeof(rdata_t));
        if (addr != nullptr) {
          LOGINFO("SHM: created %s\n", rn.c_str());
          rm.pshm = addr;
        }
      }

      REGmap[rn] = rm;
    }

  return;
}

void reg_print_name() {

  cout << endl << "======= regs_print_name =======" << endl;

  for (auto &[rn, m] : REGmap) {
    const auto &r = m.preg;
    auto &d = m.rdata;

    const char *C = KNRM;
    if (r->rstatus < 0)
      C = KRED;

    if (r->rtype)
      printf("%s%-12s %7.2f" NRM, C, rn.c_str(), (int16_t)r->rvalue * 0.01);
    else
      printf("%s%-12s %7d" NRM, C, rn.c_str(), r->rvalue);

    uint16_t x = r->rvalue;

    if (d.rmode)
    {
      memcpy(&m.rdata, m.pshm, sizeof(rdata_t));
      if (d.rvalue != r->rvalue)
      {
        r->rupdate = 1;
        r->rvalue = d.rvalue;
        printf("*");
      }
    }

    d.rvalue = x;
    d.rstatus = r->rstatus;
//    const void *ptr = &m.rdata;

    memcpy(m.pshm, &m.rdata, sizeof(rdata_t));
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
