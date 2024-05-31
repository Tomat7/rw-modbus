#pragma once

// macros.h ----------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

// ANSI Escape Sequences
#define ESC "\x1B"
#define CLS ESC "[2J"
#define HOME ESC "[H"

// ANSI color codes
#define NRM "\x1B[0m"
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[94m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

#ifdef USE_NCURSES
#define PRINTF(...) printw(__VA_ARGS__)
#else
#define PRINTF(...) printf(__VA_ARGS__)
#endif

/*
  #ifdef USE_SYSLOG
  #define LOGERR(...)                                                            \
  (PRINTF("\x1B[91m\n"), fprintf(stderr, __VA_ARGS__), PRINTF(KNRM),           \
   syslog(LOG_ERR, __VA_ARGS__))
  #define LOGINFO(...) (printf(__VA_ARGS__), syslog(LOG_INFO, __VA_ARGS__))
  #else
  #define LOGERR(...) fprintf(stderr, __VA_ARGS__)
  #define LOGINFO(...) printf(__VA_ARGS__)
  #endif
*/
