#pragma once
// config.h ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
// https://31.44.7.12:55972/panel/
// http://85.117.235.21:55971/

#define CFG_DIR "/var/tmp/conf"
#define CFG_FILE "modbus.cfg"

#ifdef DEBUG_FLAG
#define TIMEOUT_SEC 1
#else
#define TIMEOUT_SEC 3
#endif

#define MB_SLAVE_CONN_MAX 5
#define MB_SLAVE_PORT 1502
#define MB_SLAVE_REGS_NB 50
#define OPC_SERVER_PORT 4840
#define TASKS_NB_MAX 1

#define USE_SYSLOG

#ifndef SYSLOG_NAME
#define SYSLOG_NAME "RW-modbus"
#endif

#define LOGGER_DEBUG1
//#define PRINT_DEBUG2
