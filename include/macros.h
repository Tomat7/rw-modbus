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
#ifndef NRM
#define NRM "\x1B[0m"
#endif
#ifndef C_NRM
#define C_NRM "\x1B[0m"
#endif
#ifndef C_RED
#define C_RED "\x1B[31m"
#endif
#ifndef C_REDB
#define C_REDB "\x1B[91m"
#endif
#ifndef C_GRN
#define C_GRN "\x1B[32m"
#endif
#ifndef C_GRNB
#define C_GRNB "\x1B[92m"
#endif
#ifndef C_YEL
#define C_YEL "\x1B[33m"
#endif
#ifndef C_YELB
#define C_YELB "\x1B[93m"
#endif
#ifndef C_BLU
#define C_BLU "\x1B[34m"
#endif
#ifndef C_BLUB
#define C_BLUB "\x1B[94m"
#endif
#ifndef C_GRY
#define C_GRY "\x1B[90m"
#endif
#ifndef C_WHIB
#define C_WHIB "\x1B[97m"
#endif

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
