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
value_u Reg_c::pull_plc_regs_by_order(byteorder_t _bo)
{
  value_u _val;
  uint16_t mb2u[4] = {0};

  for (int i = 0; i < var_size; i++)
    mb2u[i] = get_plc_reg(i);

  if (_bo == BO_BE)
    for (int i = 0; i < var_size; i++)
      _val.dbl2u[i] = mb2u[var_size - 1 - i];
  else if (_bo == BO_LES)
    for (int i = 0; i < var_size; i++)
      _val.dbl2u[i] = mb2u[i];

  return _val;
}


value_u Reg_c::pull_plc_value32()
{
  uint32_t _val32;

  union ui32_u {
    uint32_t mb32;
    uint16_t mb2u[2] = {0};
  } _u32;

  for (int i = 0; i < var_size; i++)
    _u32.mb2u[i] = get_plc_reg(i);

  if ((byte_order == BO_BE) || (byte_order == BO_BES))
    _val32 = be32toh(_u32.mb32);
  else if ((byte_order == BO_LE) || (byte_order == BO_LES))
    _val32 = le32toh(_u32.mb32);

  if ((byte_order == BO_BES) || (byte_order == BO_LES))
    value.ui32 = bswap_32(_val32);
  else
    value.ui32 = _val32;

  return value;
}

value_u Reg_c::pull_plc_value64()
{
  uint64_t _val64;

  union ui64_u {
    uint64_t mb64;
    uint16_t mb2u[4] = {0};
  } _u64;


  for (int i = 0; i < var_size; i++)
    _u64.mb2u[i] = get_plc_reg(i);

  if ((byte_order == BO_BE) || (byte_order == BO_BES))
    _val64 = be64toh(_u64.mb64);
  else if ((byte_order == BO_LE) || (byte_order == BO_LES))
    _val64 = le64toh(_u64.mb64);

  if ((byte_order == BO_BES) || (byte_order == BO_LES))
    value.ui64 = bswap_64(_val64);
  else
    value.ui64 = _val64;

  return value;
}


// eof
