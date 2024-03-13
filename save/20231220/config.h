#pragma once

#include "libs.h"

#define CFG_FILE "./conf/modbus.cfg"

// https://31.44.7.12:55972/panel/
// http://85.117.235.21:55971/

using namespace std;
using namespace libconfig;

struct reg_t {
    int raddr;
    const char *ch_name;
    const char *ch_mode;
    uint16_t rvalue;
};

struct PLC {
    const char *dev_name;
    const char *ip_addr;
    int reg_qty;
    int poll_interval;
    int err_timeout;
    vector<reg_t> regs;
};

extern map<string, uint16_t *> MBreg;
extern vector<PLC> PLCset;

int cfg_read(const char *cfg_file);
//int cfg_fill_plc(const Setting &PLCs);

int plc_show1();

// void mb_show_regs_full(int i);
// void mb_show_regs_name();
