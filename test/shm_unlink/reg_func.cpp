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

//    memcpy(r, r->rshm, sizeof(reg_t));

//    int fd = get_shm_fd(rn.c_str());
//    reg_t * a = get_shm_addr(fd, sizeof(reg_t));

/*
    const char *C = KNRM;
    if (a->rstatus < 0)
      C = KRED;

    if (strcmp(a->rtype, "i") == 0)
      printf("%s%-12s %7d\n" NRM, C, rn.c_str(), a->rvalue);
    else
      printf("%s%-12s %7.2f\n" NRM, C, rn.c_str(), (int16_t)a->rvalue * 0.01);
*/

//    close_shm(fd, a, sizeof(reg_t));
    unlink_shm(rn.c_str());

//    memcpy(r->rshm, r, sizeof(reg_t));
  }

  return;
}

// eof
