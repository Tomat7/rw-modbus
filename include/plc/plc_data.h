#pragma once

// reg_class.h ----------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// Other (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <modbus/modbus.h>
#include <string.h>

#include <string>

#include "plc_datatype.h"

using namespace std;

struct mbdata_t {
  int status = 0;                 // rc value of last func (init/connect/read)
  uint64_t timestamp_try_ms = 0;  // milliseconds since the Epoch on last TRY
  uint64_t timestamp_ok_ms = 0;   // ms since the Epoch on last GOOD read
  uint32_t polling_ms = 0;        // milliseconds between read request
  uint32_t timeout_us = 0;        // miCRo seconds (!!) Modbus respose timeout
  uint16_t errors = 0;            // counter of any current ERRORS (reset if OK)
  uint16_t errors_rd = 0;         // counter of READ errors (summ from start)
  uint16_t errors_wr = 0;         // counter of WRITE errors (summ from start)
  uint16_t errors_cn = 0;         // counter of CONNECT errors (summ from start)
};

struct regdata_t {
  uint16_t rvalue = 0;
  uint16_t rerrors = 0;  // number of errors on MB func (init/connect/read)
  int rupdate = 0;  // 1 - need to write/update remote register, 0 - no update
  int rstatus = 0;  // -1 mean ERROR, any positive - is OK
  int rmode = 0;    // 1 - mean RW, 0 - Read-only
  regtype_t rtype;    // 0 - uint16_t, 1 - int16_t, 2 - float100 (see plc_datatype.h)
};

struct reg_t {
  int raddr = 0;
  regdata_t data;
  reg_t* r_next = nullptr;  // Float & (u)int32 - ptr to next reg_t
  string fullname = "";     // PLC_name.reg_name
  string str_rfolder = "";   // /PLC/PLC_name/folder/PLC_name.reg_name (opt)
  string str_source = "";   // reference to external register (optional)
  string str_name = "-";    // reg_name
  string str_mode = "*";    // "rw", "r", "w"
  string str_type = "*";    // "i", "f", "u"
  const char* ch_name = nullptr;
};

struct activity_t {
  bool modbus = false;            // enable Modbus requests
  bool opc = false;               // enable OPC mapping
  bool display = false;           // enable printing values on display
  bool summary = false;           // enable summary printing
};


// eof
