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
#define ESC "\033["
#define CLS ESC "2J"
#define HOME ESC "H"

// ANSI color codes
#define NRM "\033[0m"
#define STD_ "\033[0m"
#define C_STD "\033[0m"
#define C_NORM "\033[0m"
#define C_DARK "\033[90m"
#define C_BOLD "\033[97m"
#define C_HIGH "\033[1;37m"
#define C_HIGH2 "\033[1;97m"

#define C_RED "\033[0;31m"
#define C_GRN "\033[32m"
#define C_YEL "\033[33m"
#define C_BLU "\033[34m"
#define C_MAG "\033[35m"
#define C_CYN "\033[36m"
#define C_WHI "\033[37m"
#define C_GRY "\033[38m"
#define C_BLC "\033[39m"

#define C_REDD "\033[1;31m"
#define C_GRND "\033[2;32m"
#define C_YELD "\033[2;33m"
#define C_BLUD "\033[2;34m"
#define C_MAGD "\033[2;35m"
#define C_CYND "\033[2;36m"
#define C_WHID "\033[2;37m"
#define C_GRYD "\033[2;38m"
#define C_BLCD "\033[2;39m"

#define C_REDB "\033[91m"
#define C_GRNB "\033[92m"
#define C_YELB "\033[93m"
#define C_BLUB "\033[94m"
#define C_MAGB "\033[95m"
#define C_CYNB "\033[96m"
#define C_WHIB "\033[97m"
#define C_GRYB "\033[98m"
#define C_BLCB "\033[99m"

#define C_REDH "\033[1;91m"
#define C_GRNH "\033[1;92m"
#define C_YELH "\033[1;93m"
#define C_BLUH "\033[1;94m"
#define C_MAGH "\033[1;95m"
#define C_CYNH "\033[1;96m"
#define C_WHIH "\033[1;97m"
#define C_GRYH "\033[1;98m"
#define C_BLCH "\033[1;99m"


#ifdef USE_SYSLOG
#define LOGERR(...)                                                   \
  (printf("\033[91m\n"), fprintf(stderr, __VA_ARGS__), printf(C_NORM), \
   syslog(LOG_ERR, __VA_ARGS__))
#define LOGINFO(...) (printf(__VA_ARGS__), syslog(LOG_INFO, __VA_ARGS__))
#else
#define LOGERR(...) fprintf(stderr, __VA_ARGS__)
#define LOGINFO(...) printf(__VA_ARGS__)
#endif