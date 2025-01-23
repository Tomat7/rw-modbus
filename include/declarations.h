#pragma once
// declaration.h ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <iomanip>
#include <iostream>

#include "include/opc/opc_class.h"
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

extern int timeout_sec;
extern cchar* mode;
extern map<string, Reg_c> REGmap;
extern vector<PLC_c> PLCvec;
// extern PLC_c Slave;
extern OpcServer_c OPCs;
extern Schedule_c Task;
extern string PLC_folder;
extern string SCADA_folder;

#define MODBUS_MODES "master", "slave", "scada"

bool isdebug();
void init_all();
void deinit_all();
void reinit();
void parse_char(int ch);
string to_lower(string str);

const char* getColor(bool isErrors);
const char* getBlynk(bool isErrors);

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
int mb_add_tasks();
int mb_print_summary();
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
void opc_run_thread();
void opc_init();
void opc_deinit();
void opc_start();
//uint16_t opc_update_uint16(string name, regdata_t* rd);
uint16_t opc_update_uint16(string name, Reg_c* R);
// uint16_t opc_update_uint16(string name, reg_t* reg, uint16_t val);
int write_shm(string, uint16_t);
int write_rm(string rn, uint16_t val);

// ===== Scheduled Tasks =====
void tasks_init();
int task_millis_(void* params);
int task_opc_refresh_(void* params);
int task_regs_refresh_(void* params);
int task_begin_(void* params);
int task_mb_update_(void* params);


template <typename T>
void WriteValue(string s_, T val_, bool isOK = true)
{
  OPCs.updateVar(OPCs.lookupVar(s_), val_, isOK);
}

struct ReadValue {
  string _s;  // Full path to variable
  ReadValue(string svar)
  {
    _s = OPCs.lookupVar(svar);  // Try to find fullpath-name
  }
  template <typename T>
  operator T()
  {
    return OPCs.readValue<T>(_s);
  }
};

/*     if (OPCs.isVar(svar))        // if fullpath exist = "/PLC/Kub/Kub.Temp1"
        _s = svar;
*/
/*     else if (( != "")) // varname only - "Tkub0"
        _s = SCADA_folder + sv;
    else {                  // Only "Buf.Temp3"
        string _plc = ss.substr(0, ss.find(".")); // = "Buf"
        _s = PLC_folder + _plc + "/" + ss;        // = "/PLC/Buf/Buf.Temp3"
*/
//        string _plc = ss;
// auto _dot = _plc.find(".");
//        _plc.erase(ss.find("."));
//        auto _dot = ss.find(".");

// eof
