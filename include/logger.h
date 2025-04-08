#pragma once
// logger.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

// #include <chrono>
// #include <iostream>
#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>

#include <mutex>

#include "config.h"  // to get definition of USE_SYSLOG & LOG_LEVEL_DEFAULT
#include "include/macros.h"

#define MESSAGE_MAX_LEN 254
#define LOG_LEVEL_DEFAULT 4

#ifdef SYSLOG_NAME
#undef SYSLOG_NAME
#endif
#define SYSLOG_NAME __FILENAME__

/*
  #define STRINGIFY_IMPL(x) #x
  #define STRINGIFY(x) STRINGIFY_IMPL(x)
*/

#define FILE_LINE __FILE__ ":" STR(__LINE__)
#define _FL_ FILE_LINE

#define LOGFORCE(...) logger(FILE_LINE, 0, __func__, __VA_ARGS__)       // 0 
#define LOGALWAYS(...) logger(FILE_LINE, 0, __func__, __VA_ARGS__)      // 0 
#define LOGA(...) logger(FILE_LINE, LOG_ALERT, __func__, __VA_ARGS__)   // 1
#define LOGC(...) logger(FILE_LINE, LOG_CRIT, __func__, __VA_ARGS__)    // 2
#define LOGE(...) logger(FILE_LINE, LOG_ERR, __func__, __VA_ARGS__)     // 3
#define LOGW(...) logger(FILE_LINE, LOG_WARNING, __func__, __VA_ARGS__) // 4
#define LOGN(...) logger(FILE_LINE, LOG_NOTICE, __func__, __VA_ARGS__)  // 5
#define LOGI(...) logger(FILE_LINE, LOG_INFO, __func__, __VA_ARGS__)    // 6
#define LOGD(...) logger(FILE_LINE, LOG_DEBUG, __func__, __VA_ARGS__)   // 7
#define LOGX(...) logger(FILE_LINE, 8, __func__, __VA_ARGS__)           // 8
#define LOGZ(...) logger(FILE_LINE, 9, __func__, __VA_ARGS__)           // 9

#define LOGIFA(...) logif(rc, FILE_LINE, LOG_ALERT, __func__, __VA_ARGS__)
#define LOGIFC(...) logif(rc, FILE_LINE, LOG_CRIT, __func__, __VA_ARGS__)
#define LOGIFE(...) logif(rc, FILE_LINE, LOG_ERR, __func__, __VA_ARGS__)
#define LOGIFW(...) logif(rc, FILE_LINE, LOG_WARNING, __func__, __VA_ARGS__)
#define LOGIFN(...) logif(rc, FILE_LINE, LOG_NOTICE, __func__, __VA_ARGS__)
#define LOGIFI(...) logif(rc, FILE_LINE, LOG_INFO, __func__, __VA_ARGS__)
#define LOGIFD(...) logif(rc, FILE_LINE, LOG_DEBUG, __func__, __VA_ARGS__)

#ifdef PRINT_DEBUG2
#define D(a) a
#else
#define D(a)
#endif

using namespace std;

extern int log_level;  // 0 - no messages at all, 9 - all on screen
// static mutex logger_mux;

// void logger(int prio, const char* format, ...);
// void logger(const char* logname, int prio, const char* format, ...);
void logger(const char* logname, int prio, const char* _func, const char* _fmt,
            ...);
void logdebug(const char* logname, int prio, const char* format, ...);

void logger_set_queue(bool to_queue);
void logger_flush();

char* get_new_char(const char*);

// eof

/*
  #define LOGE(...) logger(FILE_LINE, LOG_ERR, __VA_ARGS__)
  #define LOGW(...) logger(SYSLOG_NAME, LOG_WARNING, __VA_ARGS__)

  #define LOGI(...) logger(SYSLOG_NAME, LOG_INFO, __VA_ARGS__)
  #define LOGN(...) logger(SYSLOG_NAME, LOG_NOTICE, __VA_ARGS__)
*/
/*
  #ifdef LOGGER_DEBUG1
  #define STRF(S) string("%s(): " + (string)S).c_str()
  #define LOGD(F, ...) logdebug(_FL_, LOG_DEBUG, STRF(F), __func__, __VA_ARGS__)
  #else
  #define LOGD(...)
  #endif
*/
/*
  #ifdef USE_DEBUG1
  #define DEBUGF(msg) printf("%s/%s(): %s", __FILE__, __func__, msg)
  #define DEBUGL(msg) printf("%s/line:%d:, %s", __FILE__, __LINE__, msg)
  #else
  #define DEBUGF(msg) printf("%s/%s(): %s", __FILE__, __func__, msg)
  #define DEBUGL(msg) printf("%s, line: %d:, %s", __FILE__, __LINE__, msg)
  #endif
*/

// eof
