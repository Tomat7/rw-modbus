// plc_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <stdarg.h>
#include <syslog.h>
#include <string.h>
#include <unistd.h>
#include <chrono>
#include <mutex>

#include "./logger.h"

// std::mutex logger_mux;
int log_level = 7; // 0 - no messages at all, 9 - all on screen

void logger(int prio, const char* format, ...)
{
  logger_mux.lock();
  FILE* fout = stdout;

  if (prio == LOG_ERR) {
    fout = stderr;
    fprintf(fout, C_RED);
  } else if (prio == LOG_WARNING)
    fprintf(fout, C_GRN);

  openlog("RW-modbus", LOG_NDELAY, LOG_LOCAL1);

  va_list arg1;
  va_list arg2;

  va_start(arg1, format);
  va_copy(arg2, arg1);
  vfprintf(fout, format, arg1);
  vsyslog(prio, format, arg2);
  va_end(arg1);
  va_end(arg2);

  fprintf(fout, "%s\n", C_NRM);
  closelog();
  logger_mux.unlock();
}

/*
  void logger(const char* logname, int prio, const char* format, ...)
  {
  logger_mux.lock();
  FILE* fout = stdout;

  if (prio == LOG_ERR) {
    fout = stderr;
    fprintf(fout, C_BLU);
    fprintf(fout, "%s ", logname);
    fprintf(fout, C_RED);
  } else if (prio == LOG_WARNING)
    fprintf(fout, C_GRN);

  openlog(logname, LOG_NDELAY, LOG_LOCAL1);

  va_list arg1;
  va_list arg2;

  va_start(arg1, format);
  va_copy(arg2, arg1);
  vfprintf(fout, format, arg1);
  vsyslog(prio, format, arg2);
  va_end(arg1);
  va_end(arg2);

  fprintf(fout, "%s\n", C_NRM);
  closelog();
  logger_mux.unlock();
  }
*/

void logger(const char* logname, int prio, const char* _func, const char* _fmt, ...)
{
  logger_mux.lock();

  FILE* fout = stdout;
  const char* format = nullptr;
  const char* fname = nullptr;
  const char* color = C_NRM;

  bool no_file = (prio != 7 && log_level < 7);
  bool no_func = (log_level < 9);
  bool no_syslog = (prio > 5 && log_level < 9);
  bool no_print = (prio > log_level);
  string fmt = (string)_func + "(): " + (string)_fmt;

  if (prio == LOG_ALERT) {
    fout = stderr;
    color = C_REDB;
  } else if (prio == LOG_ERR) {
    fout = stderr;
    color = C_REDB;
  } else if (prio == LOG_NOTICE)
    color = C_GRN;
  else if (prio == LOG_DEBUG)
    color = C_YEL;
  else
    color = C_NRM;

  if (no_file && no_func) {
    fname = "";
    format = _fmt;
  } else if (no_func) {
    fname = strrchr(logname, '/') ? strrchr(logname, '/') + 1 : logname;
    format = _fmt;
  } else {
    fname = logname;
    format = fmt.c_str();
  }

  if (!no_print)
    fprintf(fout, "%s%s%s ", C_BLU, fname, color);

  openlog(logname, LOG_NDELAY, LOG_LOCAL1);

  va_list arg1;
  va_list arg2;

  va_start(arg1, _fmt);
  va_copy(arg2, arg1);
  if (!no_print)
    vfprintf(fout, format, arg1);
  if (!no_syslog)
    vsyslog(prio, format, arg2);
  va_end(arg1);
  va_end(arg2);

  fprintf(fout, "%s\n", C_NRM);
  closelog();
  logger_mux.unlock();
}

void logdebug(const char* logname, int prio, const char* format, ...)
{
  logger_mux.lock();
  FILE* fout = stdout;

  if (prio == LOG_DEBUG) {
    fout = stderr;
    fprintf(fout, C_BLU);
    fprintf(fout, "%s ", logname);
    fprintf(fout, C_YEL);
  }

  openlog(logname, LOG_NDELAY, LOG_LOCAL1);

  va_list arg1;
  va_list arg2;

  va_start(arg1, format);
  va_copy(arg2, arg1);
  vfprintf(fout, format, arg1);
  vsyslog(prio, format, arg2);
  va_end(arg1);
  va_end(arg2);

  fprintf(fout, "%s\n", C_NRM);
  closelog();
  logger_mux.unlock();
}

char* get_new_char(const char* _oldch)
{
  char* _newch = new char[1 + strlen(_oldch)];
  strcpy(_newch, _oldch);
  return _newch;
}

const char* add_funcname(const char* _fmt, const char* _func)
{
  string fmt = (string)_func + "(): " + (string)_fmt;
  _fmt = fmt.c_str();
  return _fmt;
}

const char* extract_filename(const char* f)
{
  f = strrchr(f, '/') ? strrchr(f, '/') + 1 : f;
  return f;
}


// eof
