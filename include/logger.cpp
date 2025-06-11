// plc_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

// #include <chrono>
#include "logger.h"

#include <stdarg.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include <cstring>
#include <string>
#include <mutex>
#include <queue>

int log_level = 0;
// 0 - no messages at all - will get from Config
// 9 - all messages on screen

static std::mutex logger_mux;  // already defined in .h
static std::queue<std::string> Print_queue;
static bool print_to_queue = false;

static const char* ch_color[10] = {
//  0    Alert-1 Crit-2  Error-3 Warn-4 Notice-5 Info-6 Debug-7  X-8    Z-9
  C_MAGB, C_RED, C_MAGB, C_REDB, C_YELB, C_GRN,  C_BLUB, C_CYN, C_STD, C_STD
};


void logger(const char* _logname, int _prio, const char* _func,
            const char* _fmt, ...)
{
  LOCK_GUARD(logger_mux);

  _prio = (_prio > 7) ? 7 : _prio;
  bool no_syslog = (_prio > 4 && log_level < 9);
  bool no_print = (_prio > log_level);

  if (no_syslog && no_print)
    return;

  bool no_filename = !(_prio == 7 || log_level > 7);
  bool no_funcname = (log_level < 9);
  const char* format = _fmt;
  const char* fname = _logname;
  const char* color = C_NORM;
  char buffer[MESSAGE_MAX_LEN + 1] = {0};
  char buff_va[MESSAGE_MAX_LEN + 1] = {0};
  char buff_fn[MESSAGE_MAX_LEN + 1] = {0};

  std::string fmt = (std::string)_func + "(): " + (std::string)_fmt;

  if (!no_print) {
    color = ch_color[_prio];

    if (no_filename && no_funcname)
      fname = "";
    else if (no_funcname)
      fname = strrchr(_logname, '/') ? strrchr(_logname, '/') + 1 : _logname;
    else
      format = fmt.c_str();

    snprintf(buff_fn, MESSAGE_MAX_LEN, "%s%s%s ", C_BLU, fname, color);
  }

  va_list arg1;
  va_start(arg1, _fmt);
  vsnprintf(buff_va, MESSAGE_MAX_LEN, format, arg1);
  va_end(arg1);

  if (!no_syslog) {
    openlog(_logname, LOG_NDELAY, LOG_LOCAL1);
    syslog(_prio, "%s", buff_va);
    closelog();
  }

  if (!no_print) {
    snprintf(buffer, MESSAGE_MAX_LEN, "%s%s%s\n", buff_fn, buff_va, C_NORM);

    if (print_to_queue)
      Print_queue.emplace(std::string(buffer));
    else
      printf("%s", buffer);
  }

  return;
}

void logger_set_queue(bool to_queue) { print_to_queue = to_queue; }

void logger_flush()
{
  LOCK_GUARD(logger_mux);
  while (!Print_queue.empty()) {
    printf("%s", Print_queue.front().c_str());
    Print_queue.pop();
  }
}

/*
  void logger_fout(const char* _logname, int _prio, const char* _func,
                 const char* _fmt, ...)
  {
  LOCK_GUARD(logger_mux);
  openlog(_logname, LOG_NDELAY, LOG_LOCAL1);

  // https://www.gnu.org/software/libc/manual/html_node/Creating-a-Pipe.html
  // FILE* fout = stdout;
  int pipefd[2];
  pipe(pipefd);                         // make a pipe
  FILE* fout = fdopen(pipefd[1], "w");  // pipe to stream

  const char* format = _fmt;
  const char* fname = _logname;
  const char* color = C_NORM;

  bool no_filename = !(_prio == 7 || log_level > 7);
  bool no_funcname = (log_level < 9);
  bool no_syslog = (_prio > 4 && log_level < 9);
  bool no_print = (_prio > log_level);
  string fmt = (string)_func + "(): " + (string)_fmt;

  if (!no_print) {
    if (_prio == LOG_ALERT)
      color = C_REDB;
    else if (_prio == LOG_CRIT)
      color = C_YELB;
    else if (_prio == LOG_ERR)
      color = C_REDB;
    else if (_prio == LOG_WARNING)
      color = C_GRN;
    else if (_prio == LOG_NOTICE)
      color = C_BLUB;
    else if (_prio == LOG_DEBUG)
      color = C_YEL;
    else
      color = C_NORM;

    if (no_filename && no_funcname)
      fname = "";
    else if (no_funcname)
      fname = strrchr(_logname, '/') ? strrchr(_logname, '/') + 1 : _logname;
    else
      format = fmt.c_str();

    fprintf(fout, "%s%s%s ", C_BLU, fname, color);
  }

  va_list arg1;
  va_list arg2;

  va_start(arg1, _fmt);
  va_copy(arg2, arg1);
  if (!no_print)
    vfprintf(fout, format, arg1);
  if (!no_syslog)
    vsyslog(_prio, format, arg2);
  va_end(arg1);
  va_end(arg2);

  if (!no_print)
    fprintf(fout, "%s\n", C_NORM);
  else
    fprintf(fout, "%s", C_NORM);

  closelog();
  fclose(fout);
  close(pipefd[1]);

  fout = fdopen(pipefd[0], "r");
  char buffer[MESSAGE_MAX_LEN + 1] = {0};
  char* bufc = fgets(buffer, MESSAGE_MAX_LEN, fout);

  if (bufc != nullptr)
    if (print_to_queue)
      Print_queue.emplace(string(bufc));
    else
      printf("%s", bufc);
  else
    printf("bufc overload");

  fclose(fout);
  close(pipefd[0]);
  }
*/

/*
  void logdebug(const char* logname, int prio, const char* format, ...)
  {
  //  logger_mux.lock();
  LOCK_GUARD(logger_mux);
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

  fprintf(fout, "%s\n", C_NORM);
  closelog();
  //  logger_mux.unlock();
  }
*/

char* get_new_char(const char* _oldch)
{
  char* _newch = new char[1 + strlen(_oldch)];
  strcpy(_newch, _oldch);
  return _newch;
}

char* add_slash(const char* _rn)
{
  char* _newch = new char[2 + strlen(_rn)];
  strcat(_newch, "/");
  strcat(_newch, _rn);
  return _newch;
}

const char* add_funcname(const char* _fmt, const char* _func)
{
  std::string fmt = (std::string)_func + "(): " + (std::string)_fmt;
  _fmt = fmt.c_str();
  return _fmt;
}

const char* extract_filename(const char* f)
{
  f = strrchr(f, '/') ? strrchr(f, '/') + 1 : f;
  return f;
}

// eof
