// plc_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>
#include <chrono>
#include <mutex>

#include "./logger.h"

// std::mutex logger_mux;

void logger(int prio, const char* format, ...)
{
  logger_mux.lock();
  FILE* fout = stdout;

  if (prio == LOG_ERR) {
    fout = stderr;
    fprintf(fout, C_RED);
  }

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


void logger(const char* logname, int prio, const char* format, ...)
{
  logger_mux.lock();
  FILE* fout = stdout;

  if (prio == LOG_ERR) {
    fout = stderr;
    fprintf(fout, C_RED);
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

void logdebug(const char* logname, int prio, const char* format, ...)
{
  logger_mux.lock();
  FILE* fout = stdout;

  if (prio == LOG_ERR) {
    fout = stderr;
    fprintf(fout, C_RED);
  }

  openlog(logname, LOG_NDELAY, LOG_LOCAL1);

  fprintf(fout, "%s ", logname);

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

// eof
