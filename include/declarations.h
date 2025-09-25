#pragma once
// declaration.h ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <iomanip>
#include <iostream>

#include "include/opc_server/opcs_class.h"
#include "include/opc_client/opc_client.h"
#include "include/plc/plc_class.h"
#include "include/reg/reg_class.h"
#include "include/scheduler/scheduler.h"

using cchar = const char;

using std::cout;
using std::endl;
using std::left;
using std::right;
using std::setw;
using std::string;

struct opc_t {
  uint16_t SrvPort = OPC_SERVER_PORT;
  string SrvName = OPC_THREAD_NAME;
  string ErrFolder = OPC_ERRORS_FOLDER;
  string ErrSuffix = OPC_ERRORS_SUFFIX;
  int polling_ms = OPC_POLLING_MS;
  UA_LogLevel LogLevel = UA_LOGLEVEL_INFO;
};

struct mb_t {
  int SlaveMaxConn = MB_SLAVE_CONN_MAX;
  int SlavePort = MB_SLAVE_PORT;
  int SlaveMaxRegs = MB_SLAVE_REGS_NB;
};

struct cfg_t {
  opc_t opc;
  mb_t mb;
  //  int log_level;
  bool show_mb_regs = true;
  bool show_regs = true;
  int timeout_sec = TIMEOUT_SEC;
  int tasks_nb = TASKS_NB_MAX;
  int regs_refresh_ms = REGS_REFRESH_MS;
  int millis_heartbeat_ms = MILLIS_HEARTBEAT_MS;
  cchar* mode = nullptr;
};

extern cfg_t Cfg;

// extern int Cfg.timeout_sec;
// extern cchar* mode;
extern map<string, Reg_c> REGmap;
extern vector<PLC_c> PLCvec;
// extern PLC_c Slave;

extern OpcServer_c OPCs;
extern OPC::OpcClient_c OPCclient;
extern Schedule_c Task;
// extern string PLC_folder;
// extern string SCADA_folder;

#define MODBUS_MODES "master", "slave", "scada"
bool operator<(const numeric_u &v1, const numeric_u &v2);
bool operator>(const numeric_u &v1, const numeric_u &v2);
bool operator!=(const numeric_u &v1, const numeric_u &v2);

bool isdebug();
void init_all();
void deinit_all();
void reinit();
//void init_ncurses();
//void refresh_ncurses();
void flush_logger();
void parse_char(int ch);
string to_lower(string str);

void console_wait_sec(int _sec);

const char* getColor(bool isErrors);
const char* getBlynk(bool isErrors);

int cfg_master(cchar* cfg_dir, cchar* cfg_file, cchar* cfg_mode = "master");
/*
  int cfg_master(const char* cfg_dir, const char* cfg_file,
               const char* cfg_mode = "master");
  int cfg_slave(const char* cfg_dir, const char* cfg_file,
              const char* cfg_mode = "slave");
*/

void plc_show1();
void plc_show2();
void mb_deinit();

int mb_add_refresh_tasks();
int mb_print_summary();
void mb_print_help();
// int mb_read();
// int mb_write();
// int mb_update();
// int mb_slave_init();
// int mb_slave();

void regs_create_from_plc();
void regs_update();
void regs_deinit();
bool reg_exist(string);
// void regs_create(PLC_c* D);
// void regs_init_shm();
// void regs_update_shm();
// void regs_deinit_shm();
// void reg_print(string, const reg_t*);

void opc_regs_init();
void opc_run_thread();
void opc_init();
void opc_deinit();
void opc_start();
numeric_u opc_get_value(string s);
bool opc_set_value(string s, numeric_u val, bool isOK);
void opc_client_();
void opc_server_();
// uint16_t opc_update_uint16(string name, regdata_t* rd);
// uint16_t opc_update_uint16(string name, Reg_c* R);
// uint16_t opc_update_uint16(string name, reg_t* reg, uint16_t val);
// int write_shm(string, uint16_t);
// int write_rm(string rn, uint16_t val);

// ===== Scheduled Tasks =====
void tasks_init();
void tasks_start();
int task_millis_(void* params);
int task_opc_refresh_(void* params);
int task_regs_refresh_(void* params);
int task_plc_refresh_(void* params);
// int task_begin_(void* params);


// ===== OPC SERVER functions "fold" =====

namespace OPC_server
{

template <typename T>
void WriteValue(string s_, T val_, bool isOK = true)
{
  OPCs.UpdateVar(OPCs.GetVarFullName(s_), val_, isOK);
}

struct ReadValue {
  string _s;  // Full path to variable
  ReadValue(string svar)
  {
    _s = OPCs.GetVarFullName(svar);  // Try to find fullpath-name
  }
  template <typename T>
  operator T()
  {
    T x;
    OPCs.ReadNumber(_s, x);
    return x;
  }
};

} // namespace OPC_server

// ===== OPC SERVER functions "fold" =====

namespace OPC_client
{

template <typename T>
void WriteValue(string s_, T val_)
{
  OPCclient.WriteNumber(s_, val_);
}

struct ReadValue {
  string _s;  // Full path to variable
  ReadValue(string svar)
  {
    _s = svar;  // Try to find fullpath-name
  }
  template <typename T>
  operator T()
  {
    T x;
    OPCclient.ReadNumber(_s, x);
    return x;
  }
};

} // namespace OPC_client


// eof
