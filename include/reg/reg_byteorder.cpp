// reg_plc.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

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

  if (_bo == BO_HH)
    for (int i = 0; i < var_size; i++)
      _val.dbl2u[i] = mb2u[var_size - 1 - i];
  else if (_bo == BO_HL)
    for (int i = 0; i < var_size; i++)
      _val.dbl2u[i] = mb2u[i];

  return _val;
}

/* value_u Reg_c::push_plc_regs_by_order(byteorder_t _bo); */

// eof
