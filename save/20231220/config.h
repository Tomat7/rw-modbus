#pragma once

#include "libs.h"

#define CFG_FILE "./conf/modbus.cfg"

// https://31.44.7.12:55972/panel/
// http://85.117.235.21:55971/

using namespace std;
using namespace libconfig;

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

extern map<string, uint16_t *> MBreg;
extern vector<plc_t> PLCset;

int cfg_read_mbset(const char *cfg_file);
//int cfg_fill_plc(const Setting &PLCs);

int mb_show();

// void mb_show_regs_full(int i);
// void mb_show_regs_name();
