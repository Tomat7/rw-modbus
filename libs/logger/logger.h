#pragma once
// logger.h ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>
#include <string.h>

#include <mutex>
#include <string>

#include "colors_esc.h"
// #include "macros.h"

#define MESSAGE_MAX_LEN 254
#define LOG_LEVEL_DEFAULT 4
#define LOG_LEVEL_MINIMAL 1 // Log ALERTS only

#ifndef XSTR
#define XSTR(x) #x
#endif
#ifndef STR
#define STR(x) XSTR(x)
#endif

#ifdef SYSLOG_NAME
#undef SYSLOG_NAME
#endif
#define SYSLOG_NAME __FILENAME__

#define NEWLOGGER

struct logmessage_t {
  std::string file_name;
  std::string func_name;
  int line_num;
  int log_prio;
  std::string message;
};

extern int log_level;  // 0 - no messages at all, 9 - all on screen

//char* get_new_char(const char*);
void logger_set_queue(bool to_queue);
void logger_flush_printf();
bool logger_get_string(std::string &logged_string);

#define FILE_LINE __FILE__ ":" STR(__LINE__)
#define _FFL_ __FILE__ "/" __func__ ":" STR(__LINE__)

//#define LOGFORCE(...) logger(FILE_LINE, 0, __func__, 0, __VA_ARGS__)       // 0
#define LOGBROADCAST(...) logger(FILE_LINE, 0, __func__, 0, __VA_ARGS__)   // 0 

#ifdef NEWLOGGER
void newlogger(const char* fn, const char* func, int ln, int prio, const char* msg, ...);
#define LOGA(...) newlogger(__FILE__, __func__, __LINE__, LOG_ALERT,   __VA_ARGS__) // 1
#define LOGC(...) newlogger(__FILE__, __func__, __LINE__, LOG_CRIT,    __VA_ARGS__) // 2
#define LOGE(...) newlogger(__FILE__, __func__, __LINE__, LOG_ERR,     __VA_ARGS__) // 3
#define LOGW(...) newlogger(__FILE__, __func__, __LINE__, LOG_WARNING, __VA_ARGS__) // 4
#define LOGN(...) newlogger(__FILE__, __func__, __LINE__, LOG_NOTICE,  __VA_ARGS__) // 5
#define LOGI(...) newlogger(__FILE__, __func__, __LINE__, LOG_INFO,    __VA_ARGS__) // 6
#define LOGD(...) newlogger(__FILE__, __func__, __LINE__, LOG_DEBUG,   __VA_ARGS__) // 7
#else
void logger(const char* _fname, int _prio, const char* _func, int _rgb,
            const char* _fmt, ...);
#define LOGA(...) logger(FILE_LINE, LOG_ALERT,   __func__, 0, __VA_ARGS__) // 1
#define LOGC(...) logger(FILE_LINE, LOG_CRIT,    __func__, 0, __VA_ARGS__) // 2
#define LOGE(...) logger(FILE_LINE, LOG_ERR,     __func__, 0, __VA_ARGS__) // 3
#define LOGW(...) logger(FILE_LINE, LOG_WARNING, __func__, 0, __VA_ARGS__) // 4
#define LOGN(...) logger(FILE_LINE, LOG_NOTICE,  __func__, 0, __VA_ARGS__) // 5
#define LOGI(...) logger(FILE_LINE, LOG_INFO,    __func__, 0, __VA_ARGS__) // 6
#define LOGD(...) logger(FILE_LINE, LOG_DEBUG,   __func__, 0, __VA_ARGS__) // 7
//#define LOGX(...) logger(FILE_LINE, 8, __func__, __VA_ARGS__)           // 8
//#define LOGZ(...) logger(FILE_LINE, 9, __func__, __VA_ARGS__)           // 9
#define LOG_(...) // no logging!
#endif

// DEBUG logging (color printing only!)
#ifndef NDEBUG
#define LOG_RED(...) logger(FILE_LINE, 9, __func__, 3, __VA_ARGS__)  // red  
#define LOG_YEL(...) logger(FILE_LINE, 9, __func__, 4, __VA_ARGS__)  // yellow
#define LOG_GRN(...) logger(FILE_LINE, 9, __func__, 5, __VA_ARGS__)  // green
#define LOG_CYN(...) logger(FILE_LINE, 9, __func__, 7, __VA_ARGS__)  // cyan
#define LOG_BLU(...) logger(FILE_LINE, 9, __func__, 8, __VA_ARGS__)  // blue
#else // no DEBUG logging
#define LOGXR(...)
#define LOGXY(...)
#define LOGXG(...)
#define LOGXC(...)
#define LOGXB(...)
#endif // NDEBUG

#ifdef PRINT_DEBUG2
#define D(a) a
#else
#define D(a)
#endif


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
