#pragma once
// macros.h ----------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

//#define ef(a) else if (a)

#ifndef XSTR
#define XSTR(x) #x
#endif
#ifndef STR
#define STR(x) XSTR(x)
#endif

#ifndef LOCK_GUARD
#define LOCK_GUARD(lg) const std::lock_guard<std::mutex> lock(lg)
#endif

#ifdef FULL_DEBUG
#define D(a) a
#else
#define D(a)
#endif

#ifndef __SHORT_FILENAME__
#define __SHORT_FILENAME__                                                 \
  (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 \
                                    : __FILE__)
#endif

#define UNUSED(x) (void)(x)

// ANSI Escape Sequences
#define ESC "\x1B"
#define CLS ESC "[2J"
#define HOME ESC "[H"

// ANSI color codes
#define NRM "\x1B[0m"
//#define STD_ "\x1B[0m"
#define C_NRM "\x1B[0m"
#define C_RED "\x1B[31m"
#define C_GRN "\x1B[32m"
#define C_YEL "\x1B[33m"
#define C_BLU "\x1B[34m"
#define C_MAG "\x1B[35m"
#define C_CYN "\x1B[36m"
#define C_WHI "\x1B[37m"

#define C_GRY "\x1B[90m"
#define C_REDB "\x1B[91m"
#define C_GRNB "\x1B[92m"
#define C_YELB "\x1B[93m"
#define C_BLUB "\x1B[94m"
#define C_MAGB "\x1B[95m"
#define C_CYNB "\x1B[96m"
#define C_WHIB "\x1B[97m"

#define STD_ "\033[0;39m"
#define STD_B "\033[1;39m"

#define GRL_ "\033[0;37m"
#define GRL_B "\033[1;37m"

#define GRD_ "\033[0;90m"
#define GRD_B "\033[1;90m"

#define qwrerwerwert C_GRY

#ifdef USE_SYSLOG
#define LOGERR(...)                                                   \
  (printf("\x1B[91m\n"), fprintf(stderr, __VA_ARGS__), printf(C_NRM), \
   syslog(LOG_ERR, __VA_ARGS__))
#define LOGINFO(...) (printf(__VA_ARGS__), syslog(LOG_INFO, __VA_ARGS__))
#else
#define LOGERR(...) fprintf(stderr, __VA_ARGS__)
#define LOGINFO(...) printf(__VA_ARGS__)
#endif
