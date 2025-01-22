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

/*
  using fl2ui_u = float2uint_u;
  using fl2_u = float2uint_u;
*/

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

struct regdata_t {
  uint16_t rvalue = 0;
  uint16_t rerrors = 0;  // number of errors on MB func (init/connect/read)
  int rupdate = 0;  // 1 - need to write/update remote register, 0 - no update
  int rstatus = 0;  // -1 mean ERROR, any positive - is OK
  int rmode = 0;    // 1 - mean RW, 0 - Read-only
  int rtype = 0;    // 0 - uint16_t, 1 - int16_t, 2 - float (enum?),
};

struct reg_t {
  int raddr = 0;
  regdata_t data;
  reg_t* r_next = nullptr;  // Float & (u)int32 - ptr to next part of value
  string fullname = "";     // PLC_name.reg_name
  string str_title = "";    // "PLC" or "SCADA" (or ??)
  string str_folder = "";   // /SCADA/folder/reg_name (optional)
  string str_opcname = "";  // OPC fullpath: /PLC/PLC_name/PLC_name.reg_name
  string str_source = "";   // reference to external register (optional)
  string str_name = "-";    // reg_name
  string str_mode = "*";  // "rw", "r", "w"
  string str_type = "x";  // "i", "f", "u"
  const char* ch_name = nullptr;
};

// eof
