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

extern std::map<string, rmap_t> REGmap;
extern std::vector<PLC> PLCset;

int read_console(time_t _sec = 0, suseconds_t _usec = 10000);
void wait_console(int _s = 0, int _us = 10000);

void init_all();
void reinit();

int cfg_master(const char* cfg_dir, const char* cfg_file);

void plc_show1();
void plc_show2();

int mb_read();
int mb_write();
int mb_update();
int mb_slave_init();
int mb_slave();

void regs_init();
void regs_update();
void reg_print(string, const reg_t*);

int write_shm(string, uint16_t);
int create_shm_fd(const char* fd);
int get_shm_fd(const char* fd);
void close_shm(int, reg_t*, size_t);
void close_fd(int);

void* create_shm_addr(int, size_t);
void* get_shm_addr(int, size_t);

// void mb_show_regs_full(int i);
// void mb_show_regs_name();
