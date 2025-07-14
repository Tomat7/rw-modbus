#pragma once

// reg_class.h ----------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//

#include <modbus/modbus.h>
#include <string.h>

#include <string>

#define NOTUA_TYPES_1ST 1001  // first reg of "long" (32/64 bits) word
#define NOTUA_TYPES_2ND 1002  // second reg of "long" (32/64 bits) word
#define NOTUA_TYPES_3RD 1003  // third reg of "long" (64 bits) word
#define NOTUA_TYPES_4TH 1004  // fourth reg of "long" (64 bits) word

using std::string;

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
//class reg_t {
//public:
  uint16_t rvalue = 0;
  uint16_t rerrors = 0;  // number of errors on MB func (init/connect/read)
  int rupdate = 0;  // 1 - need to write/update remote register, 0 - no update
  int rstatus = 0;  // -1 mean ERROR, any positive - is OK
  int rmode = 0;    // 1 - mean RW, 0 - Read-only
  // the next vars is out of Modbus level, but necessary for correct displaying
  // see https://www.modbustools.com/poll_display_formats.html
  // and https://www.simplymodbus.ca/FAQ.htm#Order
  // also see ../reg/reg_datatype.h
  int rtype = 0;       // see reg_datatype.h
  int rsize = 0;       // how much "connected " regs "in chain" (optional)
  int rbyteorder = 0;  // byte order "in chain" (Big-Endian, ABCD, etc)
};

//struct reg_t {
class reg_t
{
public:
  int raddr = 0;
  regdata_t data;
  reg_t* r_next = nullptr;  // keep pointer to next "connected" reg in chain
  string str_mode = "*";    // "rw", "r", "w" - (read from Config)
  // all next params is optional for raw/pure Modbus
  // Register name is necessary for naming and good look
  string str_rname = "-";         // reg_name
  string rfullname = "";          // PLC_name.reg_name
  const char* ch_name = nullptr;  // str_rname.c_str()
  // OPC name of folder (optional)
  // .../PLC_name/rfolder/PLC_name.reg_name (optional)
  string str_rfolder = "";
  // For SCADA/OPC: reg which/where referenced/pointed to Modbus reg
  // Reference to external register (optional)
  string str_source = "";
  // Type of variable "i", "f", "u", "f100", see "type_map" in reg_init.cpp
  string str_type = "*";
};

// TODO:  !!!
struct activity_t {
  bool modbus = false;   // enable Modbus requests
  bool opc = false;      // enable OPC mapping
  bool display = false;  // enable printing values on display
  bool summary = false;  // enable summary printing
};

// eof
