#pragma once

#include "libs.h"

#define CFG_FILE "./conf/modbus.cfg"

using namespace std;

struct reg_t {
  int raddr;
  const char *rname;
  const char *rmode;
  uint16_t rvalue;
};

struct plc_t {
  const char *dev_name;
  const char *ip_addr;
  int nb_regs;
  int poll_interval;
  int err_timeout;
  vector<reg_t> regs;
};

extern map<string, uint16_t*> MBreg;
extern vector<plc_t> PLCset;

int cfg_read_mbset(const char *cfg_file);
int mb_show();

void mb_show_regs_full(int i);
void mb_show_regs_name();
