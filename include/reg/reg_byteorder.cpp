// reg_plc.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <byteswap.h>
#include "reg_class.h"

#include <string>

#include "include/logger.h"
#include "include/plc/plc_class.h"
// #include "include/shmem/shmem.h"

#ifdef SYSLOG_NAME
#undef SYSLOG_NAME
#endif
#define SYSLOG_NAME "REG-class"


// Get reg's local value != read PLC.
uint32_t Reg_c::mb_words_swap32(ui32_u _u32)
{
  //ui32_u _u32;
  //_u32.mb32 = _x32;
  uint16_t _x16 = _u32.mb2u[0];
  _u32.mb2u[0] = _u32.mb2u[1];
  _u32.mb2u[1] = _x16;
  return _u32.mb32;
}

// Get reg's local value != read PLC.
value_u Reg_c::pull_plc_regs_by_order(byteorder_t _bo)
{
  value_u _val;
  uint16_t mb2u[4] = {0};

  for (int i = 0; i < var_size; i++)
    mb2u[i] = get_plc_reg(i);

  if (_bo == BO_BE)
    for (int i = 0; i < var_size; i++)
      _val.dbl2u[i] = mb2u[var_size - 1 - i];
  else if (_bo == BO_LS)
    for (int i = 0; i < var_size; i++)
      _val.dbl2u[i] = mb2u[i];

  return _val;
}


value_u Reg_c::pull_plc_value32()
{
  ui32_u _u32;

  for (int i = 0; i < var_size; i++)
    _u32.mb2u[i] = get_plc_reg(var_size - i - 1);  // Big-endian now!

  if ((byte_order == BO_LE) || (byte_order == BO_LS))
    _u32.mb32 = be32toh(_u32.mb32);  // = bswap_32(_u32.mb32);

  if ((byte_order == BO_LS) || (byte_order == BO_BS)) {
    _u32.mb2u[0] = bswap_16(_u32.mb2u[0]);
    _u32.mb2u[1] = bswap_16(_u32.mb2u[1]);
  }

  value.ui32 =_u32.mb32;
  return value;
}

value_u Reg_c::pull_plc_value64()
{
  ui64_u _u64;

  for (int i = 0; i < var_size; i++)
    _u64.mb2u[i] = get_plc_reg(var_size - i - 1); // Big-endian now!

  if ((byte_order == BO_LE) || (byte_order == BO_LS))
    _u64.mb64 = be64toh(_u64.mb64);

  if ((byte_order == BO_LS) || (byte_order == BO_BS)) {
    _u64.mb2u[0] = bswap_16(_u64.mb2u[0]);
    _u64.mb2u[1] = bswap_16(_u64.mb2u[1]);
    _u64.mb2u[2] = bswap_16(_u64.mb2u[2]);
    _u64.mb2u[3] = bswap_16(_u64.mb2u[3]);
  }

  value.ui64 = _u64.mb64;
  return value;
}

/*
  if (byte_order == BO_BE)
    value.ui64 = _u64.mb64;
  else if (byte_order == BO_BES) {
    _u64.mb2u[0] = bswap_16(_u64.mb2u[0]);
    _u64.mb2u[1] = bswap_16(_u64.mb2u[1]);
    _u64.mb2u[2] = bswap_16(_u64.mb2u[2]);
    _u64.mb2u[3] = bswap_16(_u64.mb2u[3]);
    value.ui64 = _u64.mb64;
  } else if (byte_order == BO_LE) {
    value.ui64 = be64toh(_u64.mb64);
  } else if (byte_order == BO_LES) {
    uint16_t _x16 = _u64.mb2u[0];
    _u64.mb2u[0] = _u64.mb2u[3];
    _u64.mb2u[3] = _x16;
    _x16 = _u64.mb2u[1];
    _u64.mb2u[1] = _u64.mb2u[2];
    _u64.mb2u[2] = _x16;
    value.ui64 =_u64.mb64;
  }

  return value;
  }
*/
/*   else if (byte_order == BO_BES) {
  _u32.mb2u[0] = bswap_16(_u32.mb2u[0]);
  _u32.mb2u[1] = bswap_16(_u32.mb2u[1]);
  value.ui32 = _u32.mb32;
  } else if (byte_order == BO_LE) {
*/
//  _val32 = le32toh(_u32.mb32);  // doing nothing on Intel/AMD64!
//    value.ui32 = bswap_32(_u32.mb32);
//  } else

/*
  if ((byte_order == BO_BE) || (byte_order == BO_LE))
    _val32 = bswap_32(_u32.mb32);
  else
    _val32 = _u32.mb32;
*/
/*
  if ((byte_order == BO_BE) || (byte_order == BO_BES))
    value.ui32 = be32toh(_val32);
  else if ((byte_order == BO_LE) || (byte_order == BO_LES))
    value.ui32 = le32toh(_val32);
*/

/*
  if ((byte_order == BO_BE) || (byte_order == BO_BES))
    _val32 = _u32.mb32; //be32toh(_u32.mb32);
  else if ((byte_order == BO_LE) || (byte_order == BO_LES))
    _val32 = le32toh(_u32.mb32);

  if ((byte_order == BO_BES) || (byte_order == BO_LE))
    value.ui32 = bswap_32(_val32);
  else
    value.ui32 = _val32;
*/


// eof
