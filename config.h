#pragma once
// config.h ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
// https://31.44.7.12:55972/panel/
// http://85.117.235.21:55971/

#define CFG_DIR "./conf"
#define CFG_FILE "modbus.cfg"
#define USE_SYSLOG
#define SYSLOG_NAME "RW-modbus"
#define TIMEOUT_SEC 3

#define MB_SLAVE_CONN_MAX 5
#define MB_SLAVE_PORT 1502
#define MB_SLAVE_REGS_NB 50

#define USE_DEBUG1
