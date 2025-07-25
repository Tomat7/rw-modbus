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


numeric_u Reg_c::pull_plc_value32()
{
  for (int i = 0; i < var_size; i++)
    value.mb32u[i] = get_plc_reg(var_size - i - 1);  // Big-endian now!

  if ((byte_order == BO_LE) || (byte_order == BO_LS)) // to Less-endian
    value.ui32 = be32toh(value.ui32);

  if ((byte_order == BO_LS) || (byte_order == BO_BS)) { // swap here
    value.mb32u[0] = bswap_16(value.mb32u[0]);
    value.mb32u[1] = bswap_16(value.mb32u[1]);
  }

  return value;
}

numeric_u Reg_c::pull_plc_value64()
{

  for (int i = 0; i < var_size; i++)
    value.mb64u[i] = get_plc_reg(var_size - i - 1); // Big-endian now!

  if ((byte_order == BO_LE) || (byte_order == BO_LS)) // to Less-endian
    value.ui64 = be64toh(value.ui64);

  if ((byte_order == BO_LS) || (byte_order == BO_BS)) { // swap here
    value.mb64u[0] = bswap_16(value.mb64u[0]);
    value.mb64u[1] = bswap_16(value.mb64u[1]);
    value.mb64u[2] = bswap_16(value.mb64u[2]);
    value.mb64u[3] = bswap_16(value.mb64u[3]);
  }

  return value;
}

// eof
