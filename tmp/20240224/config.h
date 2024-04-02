#pragma once
// config.h ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <map>
#include <string>
#include <vector>

#include "./libs.h"
#include "./plc_class.h"

#define CFG_FILE "./conf/modbus.cfg"

// ANSI Escape Sequences
#define ESC "\x1B"
#define CLS ESC "[2J"
#define HOME ESC "[H"

// ANSI color codes
#define NRM "\x1B[0m"
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

#define USE_SYSLOG
#ifdef LOGERR
#undef LOGERR
#endif
#ifdef LOGINFO
#undef LOGINFO
#endif

#ifdef USE_SYSLOG
#define LOGERR(...)                                                            \
  printf(__VA_ARGS__);                                                         \
  syslog(LOG_ERR, __VA_ARGS__)
#define LOGINFO(...)                                                           \
  printf(__VA_ARGS__);                                                         \
  syslog(LOG_INFO, __VA_ARGS__)
#else
#define LOGERR(...) printf(__VA_ARGS__)
#define LOGINFO(...) printf(__VA_ARGS__)
#endif

// https://31.44.7.12:55972/panel/
// http://85.117.235.21:55971/

// using namespace std;
// using namespace libconfig;

extern std::map<string, reg_t *> REGmap;
extern std::map<string, rshm_t> SHMmap;
extern std::vector<PLC_c> PLCset;

int cfg_read(const char *cfg_file);

int plc_show1();
int mb_read();
void regs_init();
void reg_print_name();

int create_shm_fd(const char *fd);
int get_shm_fd(const char *fd);
void close_shm(int, reg_t *, size_t);
void close_fd(int);

reg_t *create_shm_addr(int, size_t);
reg_t *get_shm_addr(int, size_t);

// void mb_show_regs_full(int i);
// void mb_show_regs_name();
