#pragma once
// declaration.h ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <iomanip>
#include <iostream>

#include "include/plc/plc_class.h"
#include "include/opc/opc_class.h"

using cchar = const char;

using std::cout;
using std::endl;
using std::left;
using std::right;
using std::setw;
using std::string;

extern int timeout_sec;
extern cchar* mode;
extern map<string, Reg_c> REGmap;
extern vector<PLC_c> PLCvec;
extern PLC_c Slave;
extern OpcServer_c OPCs;
// extern regdata_t* P;

#define MODBUS_MODES "master", "slave", "scada"

bool isdebug();
void init_all();
void deinit_all();
void reinit();
void parse_char(int ch);

int cfg_master(const char* cfg_dir, const char* cfg_file,
               const char* cfg_mode = "master");
int cfg_slave(const char* cfg_dir, const char* cfg_file,
              const char* cfg_mode = "slave");

void plc_show1();
void plc_show2();
void mb_deinit();

int mb_read();
int mb_write();
int mb_update();
int mb_slave_init();
int mb_slave();

void regs_create_from_masters();
void regs_init_shm();
void regs_update();
void regs_update_shm();
void regs_deinit_shm();
void regs_deinit();
bool reg_exist(string);
// void reg_print(string, const reg_t*);

void opc_regs_init();
uint16_t opc_update_uint16(string name, reg_t* reg, uint16_t val);
void opc_init();
void opc_deinit();

int write_shm(string, uint16_t);
int write_rm(string rn, uint16_t val);
