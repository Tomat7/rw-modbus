#pragma once
// declaration.h ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include "./plc_class.h"

using std::cout;
using std::endl;
using std::left;
using std::right;
using std::setw;
using std::string;

extern std::map<string, RegMap_c> REGmap;
extern std::vector<PLC_c> PLCset;
extern regdata_t* P;

int read_console(time_t _sec = 0, suseconds_t _usec = 10000);
void wait_console(int _s = 0, int _us = 10000);

void init_all();
void reinit();
void parse_char(int ch);

int cfg_master(const char* cfg_dir, const char* cfg_file);

void plc_show1();
void plc_show2();

int mb_read();
int mb_write();
int mb_update();
int mb_slave_init();
int mb_slave();

void regs_init();
void regs_init_shm();
void regs_update();
void regs_update_shm();
void regs_deinit();
//void reg_print(string, const reg_t*);

int write_shm(string, uint16_t);
int write_rm(string rn, uint16_t val);

