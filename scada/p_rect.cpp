// scada_p_rect.cpp -----------------------------
// Copyright 2026 Tomat7 (star0413@gmail.com)

// #include <string.h>
// #include <sys/prctl.h>

// #include <cmath>
// #include <map>
// #include <string>
// #include <vector>

//#include "config.h"
//#include "libs.h"
#include "scada.h"

using namespace OPC_server;

static uint16_t p_rect = 0;
static uint16_t mod_rect = 0;

#define MOD_IN_RANGE(M, MIN_M, MAX_M) (M >= MIN_M && M <= MAX_M)

/* ============== P.rect ================= */

void P_rect()
{
  if_init();
  p_rect = 0;

  if (Process0 > 90)
    mod_rect = 99;
  else if (Process0 == 9)
    mod_rect = Mode0;
  else if (Process0 > 9)
    mod_rect = Process0;
  else
    mod_rect = 99;

  //if (MOD_IN_RANGE(mod_rect,0,9))
  //  p_rect = 0;
  //else
  if (MOD_IN_RANGE(mod_rect, 10, 19))
    p_rect = ReadValue("P_MAX"); // RUN
  else if (MOD_IN_RANGE(mod_rect, 20, 29))
    p_rect = ReadValue("P_WARM"); // HEAT
  else if (MOD_IN_RANGE(mod_rect, 30, 39))
    p_rect = ReadValue("P_HEAD"); // STAB
  else if (MOD_IN_RANGE(mod_rect, 40, 49))
    p_rect = ReadValue("P_HEAD"); // HEAD
  else if (MOD_IN_RANGE(mod_rect, 50, 59))
    p_rect = ReadValue("P_BODY"); // BODY
  else if (MOD_IN_RANGE(mod_rect, 60, 69))
    p_rect = ReadValue("P_BODY"); // after-BODY
  else if (MOD_IN_RANGE(mod_rect, 70, 79))
    p_rect = ReadValue("P_TAIL"); // TAIL

  //else if (MOD_IN_RANGE(mod_rect,80,89))
  //  p_rect = 0; // finish-pause-error
  //  Pset0 = Pset0 - Pshift;
  //if (p_rect < 0) p_rect = 0;

  WriteValue("P.rect", p_rect);
}


// eof
