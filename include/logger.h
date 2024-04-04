// plc_class.cpp ----------------------------
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

// ANSI color codes
#ifdef KNRM
#undef KNRM
#endif
#define KNRM "\x1B[0m"

#ifdef KRED
#undef KRED
#endif
#define KRED "\x1B[91m"

#ifdef KGRN
#undef KGRN
#endif
#define KGRN "\x1B[32m"

#ifdef KYEL
#undef KYEL
#endif
#define KYEL "\x1B[33m"

#ifdef KBLU
#undef KBLU
#endif
#define KBLU "\x1B[94m"

using namespace std;

static mutex logger_mux;

void logger(int prio, const char* format, ...);


// eof
