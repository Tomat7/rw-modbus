#pragma once
// logger.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <mutex>

// ANSI color codes
#ifndef C_NRM
#define C_NRM "\x1B[0m"
#endif
#ifndef C_RED
#define C_RED "\x1B[91m"
#endif
#ifndef C_GRN
#define C_GRN "\x1B[32m"
#endif
#ifndef C_YEL
#define C_YEL "\x1B[33m"
#endif
#ifndef C_BLU
#define C_BLU "\x1B[94m"
#endif

#define FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#ifndef SYSLOG_NAME
#define SYSLOG_NAME FILENAME
#endif

#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)

#define FILE_LINE __FILE__ ":" STRINGIFY(__LINE__)
#define _FL_ FILE_LINE

#define LOGI(...) logger(SYSLOG_NAME, LOG_INFO, __VA_ARGS__)
#define LOGE(...) logger(SYSLOG_NAME, LOG_ERR, __VA_ARGS__)

#define LOGIFL(...) logger(FILE_LINE, LOG_INFO, __VA_ARGS__)
#define LOGEFL(...) logger(FILE_LINE, LOG_ERR, __VA_ARGS__)

#ifdef USE_DEBUG1
#define LOGD(...) logdebug(_FL_, LOG_DEBUG, __VA_ARGS__)
#else
#define LOGD(...)
#endif

#ifdef USE_DEBUG1
#define DEBUGF(msg) printf("%s/%s(): %s", __FILE__, __func__, msg)
#define DEBUGL(msg) printf("%s/line:%d:, %s", __FILE__, __LINE__, msg)
#else
#define DEBUGF(msg) printf("%s/%s(): %s", __FILE__, __func__, msg)
#define DEBUGL(msg) printf("%s, line: %d:, %s", __FILE__, __LINE__, msg)
#endif

#ifdef USE_DEBUG1
#define D(a) a
#else
#define D(a)
#endif

using namespace std;

static mutex logger_mux;

void logger(int prio, const char* format, ...);
void logger(const char* logname, int prio, const char* format, ...);
void logdebug(const char* logname, int prio, const char* format, ...);

// eof
