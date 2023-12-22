#pragma once

#include "libs.h"
#include "plc_class.h"

#define CFG_FILE "./conf/modbus.cfg"

// https://31.44.7.12:55972/panel/
// http://85.117.235.21:55971/

using namespace std;
using namespace libconfig;

extern map<string, uint16_t *> MBreg;
extern vector<PLC> PLCset;

int cfg_read(const char *cfg_file);

int plc_show();
int mb_read();
void reg_init();

// void mb_show_regs_full(int i);
// void mb_show_regs_name();
