#pragma once

// reg_datatype.h ----------------------------------
// Copyright 2025 Tomat7 (star0413@gmail.com)

#include "include/opc_server/opcs_class.h"
#include "include/plc/plc_class.h"

/*
  Defines of data types. (incuding TWO and FOUR words types)
   _BE - High byte first, high word first. (Big-endian, HH)
   _LE - Low byte first, low word first. (Little-endian, LL)
   _BES - Low byte first, high word first. (Big-endian Swap, LH)
   _LES - High byte first, low word first. (Little-endian Swap, HL)
  See details - https://www.modbustools.com/poll_display_formats.html
*/
enum byteorder_t {
  BO_ZERO,
  BO_SNGL,
  BO_BE,
  BO_BES,
  BO_LE,
  BO_LES,
  BO_OTHER = 10,
  BO_F10,   // floatVar = ModbusVar_int16 / 10.0;
  BO_F100,  // floatVar = ModbusVar_int16 / 100.0;
  BO_HEX,
  BO_BINARY,
  BO_1ST = 101,
  BO_2ND = 102,
  BO_3RD = 103,
  BO_4TH = 104,
  BO_REF = 200,
  BO_NOTHING = 255,
};

// { variable type, number of MB-words, byte-order }
#define TYPE_U16 { UA_TYPES_UINT16, 1, BO_SNGL }
#define TYPE_I16 { UA_TYPES_INT16, 1, BO_SNGL }

#define NOTUA_TYPES_1ST 1001  // first reg of "long" (32/64 bits) word
#define NOTUA_TYPES_2ND 1002  // second reg of "long" (32/64 bits) word
#define NOTUA_TYPES_3RD 1003  // third reg of "long" (64 bits) word
#define NOTUA_TYPES_4TH 1004  // fourth reg of "long" (64 bits) word

#define NOTUA_TYPES_F10 1010   // floatVar = ModbusVar_int16 / 10.0;
#define NOTUA_TYPES_F100 1100  // floatVar = ModbusVar_int16 / 100.0;
#define TYPE_F10 { NOTUA_TYPES_F10, 1, BO_F10 }
#define TYPE_F100 { NOTUA_TYPES_F100, 1, BO_F100 }

#define NOTUA_TYPES_HEX 2001
#define NOTUA_TYPES_BINARY 2002

#define NOTUA_TYPES_REF 3000
#define NOTUA_TYPES_NOTHING 4000

/*
  TWO Modbus-words types
  See details - https://www.simplymodbus.ca/FAQ.htm#Order
*/
#define TYPE_U32_BE { UA_TYPES_UINT32, 2, BO_BE }
#define TYPE_U32_LES { UA_TYPES_UINT32, 2, BO_LES }
#define TYPE_U32_BES { UA_TYPES_UINT32, 2, BO_BES }
#define TYPE_U32_LE { UA_TYPES_UINT32, 2, BO_LE }
#define TYPE_U32 TYPE_U32_BE  // Big-endian as default

#define TYPE_I32_BE { UA_TYPES_INT32, 2, BO_BE }
#define TYPE_I32_LES { UA_TYPES_INT32, 2, BO_LES }
#define TYPE_I32_BES { UA_TYPES_INT32, 2, BO_BES }
#define TYPE_I32_LE { UA_TYPES_INT32, 2, BO_LE }
#define TYPE_I32 TYPE_I32_BE  // Big-endian as default

// Float 32-bits (float Float_var = 123.00f)
#define TYPE_FLOAT_BE { UA_TYPES_FLOAT, 2, BO_BE }
#define TYPE_FLOAT_LES { UA_TYPES_FLOAT, 2, BO_LES }
#define TYPE_FLOAT_BES { UA_TYPES_FLOAT, 2, BO_BES }
#define TYPE_FLOAT_LE { UA_TYPES_FLOAT, 2, BO_LE }
#define TYPE_FLOAT TYPE_FLOAT_BE  // Big-endian as default

/*
  FOUR Modbus-words types
  See details - https://www.modbustools.com/poll_display_formats.html
*/
#define TYPE_U64_BE { UA_TYPES_UINT64, 4, BO_BE }
#define TYPE_U64_LES { UA_TYPES_UINT64, 4, BO_LES }
#define TYPE_U64_BES { UA_TYPES_UINT64, 4, BO_BES }
#define TYPE_U64_LE { UA_TYPES_UINT64, 4, BO_LE }
#define TYPE_U64 TYPE_U64_BE  // Big-endian as default

#define TYPE_I64_BE { UA_TYPES_INT64, 4, BO_BE }
#define TYPE_I64_LES { UA_TYPES_INT64, 4, BO_LES }
#define TYPE_I64_BES { UA_TYPES_INT64, 4, BO_BES }
#define TYPE_I64_LE { UA_TYPES_INT64, 4, BO_LE }
#define TYPE_I64 TYPE_I64_BE  // Big-endian as default

// Double 64-bits (double Dbl_var = 12345.6789)
#define TYPE_DOUBLE_BE { UA_TYPES_DOUBLE, 4, BO_BE }
#define TYPE_DOUBLE_LES { UA_TYPES_DOUBLE, 4, BO_LES }
#define TYPE_DOUBLE_BES { UA_TYPES_DOUBLE, 4, BO_BES }
#define TYPE_DOUBLE_LE { UA_TYPES_DOUBLE, 4, BO_LE }
#define TYPE_DOUBLE TYPE_DOUBLE_BE  // Big-endian as default

#define TYPE_1ST { NOTUA_TYPES_1ST, 1, BO_1ST }  // ??
#define TYPE_2ND { NOTUA_TYPES_2ND, 0, BO_2ND }  // Second word of 2-words type
#define TYPE_3RD { NOTUA_TYPES_3RD, 0, BO_3RD }  // Third word of 4-words type
#define TYPE_4TH { NOTUA_TYPES_4TH, 0, BO_4TH }  // Fourth word of 4-words type

#define TYPE_REFERENCED { NOTUA_TYPES_REF, 0, BO_REF }  // Reference to other
#define TYPE_NOTHING { NOTUA_TYPES_NOTHING, 0, BO_NOTHING }  // No-type type

#define TYPE_HEX { NOTUA_TYPES_HEX, 1, BO_HEX }  // HEX display type
#define TYPE_BINARY { NOTUA_TYPES_BINARY, 1, BO_BINARY }  // Binary display type

#define ENUMCAST(x) static_cast<int>(x)  // ??
/*
  #define TYPE_REF TYPE_REFERENCED
  #define TYPE_NO TYPE_NOTHING
*/
// FLOAT data types (two Modbus words)
#define FLOAT_BE TYPE_FLOAT_BE
#define FLOAT_ABCD TYPE_FLOAT_BE
#define FLOAT_BIGENDIAN TYPE_FLOAT_BE

// High byte first, low word first. (Little-endian Swap)
#define FLOAT_LES TYPE_FLOAT_LES
#define FLOAT_CDAB TYPE_FLOAT_LES

// Low byte first, high word first. (Big-endian Swap)
#define FLOAT_BES TYPE_FLOAT_BES
#define FLOAT_BADC TYPE_FLOAT_BES

// Low byte first, low word first. (Little-endian)
#define FLOAT_LE TYPE_FLOAT_LE
#define FLOAT_DCBA TYPE_FLOAT_LE

// DOUBLE data types (four Modbus words)
// See details https://www.simplymodbus.ca/FAQ.htm#Order

// High byte first, high word first. (Big-endian)
#define DOUBLE_BE TYPE_FLOAT_BE
#define DOUBLE_ABCD TYPE_FLOAT_BE
#define DOUBLE_BIGENDIAN TYPE_FLOAT_BE

// High byte first, low word first. (Little-endian Swap)
#define DOUBLE_LES TYPE_FLOAT_LES
#define DOUBLE_CDAB TYPE_FLOAT_LES

// Low byte first, high word first. (Big-endian Swap)
#define DOUBLE_BES TYPE_FLOAT_BES
#define DOUBLE_BADC TYPE_FLOAT_BES

// Low byte first, low word first. (Little-endian)
#define DOUBLE_LE TYPE_FLOAT_LE
#define DOUBLE_DCBA TYPE_FLOAT_LE

// eof
