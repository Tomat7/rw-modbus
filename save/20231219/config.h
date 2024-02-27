#pragma once

#include "libs.h"

#define CFG_FILE "./conf/modbus.cfg"

using namespace std;

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

extern map<string, uint16_t*> MBreg;
extern vector<PLC> PLCset;

int cfg_read(const char *cfg_file);
int plc_show();

void plc_show_regs(int i);
void reg_init();
