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

#include "../config.h"

// ANSI color codes
#ifndef C_NRM
#define C_NRM "\x1B[0m"
#endif
#ifndef C_RED
#define C_RED "\x1B[91m"
#endif
#ifndef C_GRN
#define C_GRN "\x1B[92m"
#endif
#ifndef C_YEL
#define C_YEL "\x1B[33m"
#endif
#ifndef C_BLU
#define C_BLU "\x1B[94m"
#endif

#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? \
        __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

#ifdef SYSLOG_NAME
#undef SYSLOG_NAME
#endif
#define SYSLOG_NAME __FILENAME__

#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)

#define FILE_LINE __FILE__ ":" STRINGIFY(__LINE__)
#define _FL_ FILE_LINE

#define LOGA(...) logger(FILE_LINE, LOG_ALERT, __func__, __VA_ARGS__)
#define LOGE(...) logger(FILE_LINE, LOG_ERR, __func__, __VA_ARGS__)
#define LOGW(...) logger(FILE_LINE, LOG_WARNING, __func__, __VA_ARGS__)
#define LOGN(...) logger(FILE_LINE, LOG_NOTICE, __func__, __VA_ARGS__)
#define LOGI(...) logger(FILE_LINE, LOG_INFO, __func__, __VA_ARGS__)

/*
  #define LOGE(...) logger(FILE_LINE, LOG_ERR, __VA_ARGS__)
  #define LOGW(...) logger(SYSLOG_NAME, LOG_WARNING, __VA_ARGS__)

  #define LOGI(...) logger(SYSLOG_NAME, LOG_INFO, __VA_ARGS__)
  #define LOGN(...) logger(SYSLOG_NAME, LOG_NOTICE, __VA_ARGS__)
*/

#ifdef LOGGER_DEBUG1
#define STRF(S) string("%s(): " + (string)S).c_str()
#define LOGD(F, ...) logdebug(_FL_, LOG_DEBUG, STRF(F), __func__, __VA_ARGS__)
#else
#define LOGD(...)
#endif

#ifdef PRINT_DEBUG2
#define D(a) a
#else
#define D(a)
#endif

/*
  #ifdef USE_DEBUG1
  #define DEBUGF(msg) printf("%s/%s(): %s", __FILE__, __func__, msg)
  #define DEBUGL(msg) printf("%s/line:%d:, %s", __FILE__, __LINE__, msg)
  #else
  #define DEBUGF(msg) printf("%s/%s(): %s", __FILE__, __func__, msg)
  #define DEBUGL(msg) printf("%s, line: %d:, %s", __FILE__, __LINE__, msg)
  #endif
*/


using namespace std;

extern int log_level;  // 0 - no messages at all, 9 - all on screen

static mutex logger_mux;

void logger(int prio, const char* format, ...);
//void logger(const char* logname, int prio, const char* format, ...);
void logger(const char* logname, int prio, const char* _func, const char* _fmt, ...);
void logdebug(const char* logname, int prio, const char* format, ...);

char* get_new_char(const char*);

// eof
