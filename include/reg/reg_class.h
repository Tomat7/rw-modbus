#pragma once

// reg_class.h ----------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// Other (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <modbus/modbus.h>
#include <string.h>

#include <atomic>
#include <ctime>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <vector>

#define MB_SLAVE_CONN_MAX 5
//#define USE_SYSLOG

#define TYPE_U16 0
#define TYPE_I16 1
#define TYPE_F100 2
#define TYPE_F10 3

#define TYPE_FLOAT_ABCD 21
#define TYPE_FLOAT_CDAB 22

using namespace std;

union float2uint_u {
  float fl;
  uint16_t ui[2];
};

union value_u {
  int16_t i16;
  int32_t i32;
  int64_t i64;
  uint16_t ui16;
  uint32_t ui32;
  uint64_t ui64;
  int64_t dt;
  float fl;
  double dbl;
  uint16_t fl2u[2];
  uint16_t dbl2u[4];
};

using fl2ui_u = float2uint_u;
using fl2_u = float2uint_u;

struct regdata_t {
  uint16_t rvalue = 0;
  uint16_t* rv_next = nullptr; // Float & (u)int32 - ptr to next part of value
  uint16_t rerrors = 0;  // number of errors on MB func (init/connect/read)
  int rupdate = 0;  // 1 - need to write/update remote register, 0 - no update
  int rstatus = 0;  // -1 mean ERROR, any positive - is OK
  int rmode = 0;    // 1 - mean RW, 0 - Read-only
  int rtype = 0;    // 0 - uint16_t, 1 - int16_t, 2 - float (enum?),
};

struct reg_t {
  int raddr = 0;
  regdata_t data;
  string fullname = "";     // PLC_name.reg_name
  string str_title = "";    // "PLC" or "SCADA" (or ??)
  string str_folder = "";   // /SCADA/folder/reg_name (optional)
  string str_opcname = "";  // OPC fullpath: /PLC/PLC_name/PLC_name.reg_name
  string str_source = "";   // reference to external register (optional)
  string str_name = "-";     // reg_name
  string str_mode = "*";  // "rw", "r", "w"
  string str_type = "x";  // "i", "f", "u"
  const char* ch_name = nullptr;
};

class Reg_c
{
public:
  Reg_c(int _fd, regdata_t* _shm, regdata_t* _plc, reg_t* _reg);
  Reg_c(reg_t* _reg);      // for PLC master
  Reg_c(const char* _rn, string src_ref);  // for Scada regs.
  Reg_c(string _rn, string src_ref);       // for Scada regs.
  Reg_c();
  ~Reg_c();

  uint16_t get_plc_val();
  uint16_t get_shm_val();
  uint16_t get_local();

  void set_plc_val(uint16_t _val);
  void set_shm_val(uint16_t _val);
  void set_local(uint16_t _val);

  void sync();
  void sync(uint16_t _val);
  void sync_regdata(regdata_t* prt_data);

  int get_mode(); // 1 = "rw"
  int get_type();
  bool is_shm();
  bool is_MB();
  bool is_Scada();  // Calculated
  bool has_Ref();    // Referenced to Modbus

  // set MODBUS value and return LOCAL
  template <typename T> T set_value(T _val);

  // read LOCAL saved value
  template <typename T> T read_value();

  // ask MODBUS for current value
  template <typename T> T get_value();


  string src_reference;  // keeps "reference" to MB-reg (Tdef = DEF.Temp3)
  // "-" mean no reference - Scada calculated reg
  // "" mean no reference - Modbus reg only

  int fd = -1;                        // descriptor of SHARED MEMORY
  const char* rn = nullptr;           // just for FUN! (copy)
  value_u value;                 // just for FUN! (to print with PLC & SHM)
  regdata_t* ptr_data_shm = nullptr;  // ptr to SHARED MEMORY (local) data
  regdata_t* ptr_data_plc = nullptr;  // ptr to SHARED MEMORY (PLC/MB) data
  reg_t* ptr_reg = nullptr;           // ptr to PLC reg
};

// eof
