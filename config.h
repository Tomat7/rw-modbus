#pragma once
// config.h ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
// https://31.44.7.12:55972/panel/
// http://85.117.235.21:55971/
//

//#define USE_NCURSES

#define PI 3.1415926535897932384626433832795

#define CFG_DIR "/var/tmp/conf"
#define CFG_FILE "modbus.cfg"

#define OPC_URL "opc.tcp://localhost:4840"
#define OPC_SERVER_PORT 4840
// #define OPC_PLC_FOLDER "/PLC/"
// #define OPC_SCADA_FOLDER "/SCADA/"
#define OPC_ERRORS_FOLDER "/ERRORS"
#define OPC_ERRORS_SUFFIX ".error"
#define OPC_THREAD_NAME "OPC:server"
#define OPC_POLLING_MS 700

#ifdef DEBUG_FLAG
#define TIMEOUT_SEC 1
#else
#define TIMEOUT_SEC 3
#endif

#define MB_SLAVE_CONN_MAX 5
#define MB_SLAVE_PORT 1502
#define MB_SLAVE_REGS_NB 50
#define TASKS_NB_MAX 500
#define REGS_REFRESH_MS 900
#define MILLIS_HEARTBEAT_MS 2900
#define CONSOLE_WAIT_MS 100

#define USE_SYSLOG
#define LOG_STR_LEN 75

#ifndef SYSLOG_NAME
#define SYSLOG_NAME "RW-modbus"
#endif

#define LOGGER_DEBUG1
// #define PRINT_DEBUG2
