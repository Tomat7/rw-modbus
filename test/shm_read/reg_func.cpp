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
      string reg = (string)D.dev_name + "." + (string)R.rname;
      REGmap[reg] = &R;
      REGmap[reg]->rvalue = 5757;
    }
  return;
}

void reg_print_name() {
  cout << endl << "======= regs_print_name =======" << endl;

  for (const auto &[rn, r] : REGmap) {

    rshm_t rm;
    int fd = get_shm_fd(rn.c_str());

    if (fd != -1) {
      reg_t *addr = get_shm_addr(fd, sizeof(rshm_t));
      if (addr != nullptr) {
        memcpy(&rm, addr, sizeof(rshm_t));
        const char *C = KNRM;

        if (rm.rstatus < 0)
          C = KRED;

        if (rm.rtype == 0)
          printf("%s%-12s %7d" NRM, C, rn.c_str(), rm.rvalue);
        else
          printf("%s%-12s %7.2f" NRM, C, rn.c_str(), (int16_t)rm.rvalue * 0.01);

        close_shm(fd, addr, sizeof(rshm_t));
        printf("   %ld +\n", sizeof(rshm_t));
      } else {
        close_fd(fd);
        printf("   --\n");
      }
    } else
      printf("   -\n");
  }

  return;
}

// eof
