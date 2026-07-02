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

/* ============== P.rect ================= */

void P_rect()
{
  if_init();

  // int Pset0 = 0;
  // int m_rect;
  // Process0 = ReadValue("Process");
  // int Mode0 = ReadValue("Mode");
  // Int Pshift = ReadValue("Check");

  if (Process0 > 90)
    mod_rect = 99;
  else if (Process0 == 9)
    mod_rect = Mode0;
  else if (Process0 > 9)
    mod_rect = Process0;
  else
    mod_rect = 99;

  switch (mod_rect) {
  case 0 ... 9:
    p_rect = 0;
    break;
  case 10 ... 19:
    p_rect = ReadValue("P_MAX"); // RUN
    break;
  case 20 ... 29:
    p_rect = ReadValue("P_WARM"); // HEAT
    break;
  case 30 ... 39:
    p_rect = ReadValue("P_HEAD"); // STAB
    break;
  case 40 ... 49:
    p_rect = ReadValue("P_HEAD"); // HEAD
    break;
  case 50 ... 59:
    p_rect = ReadValue("P_BODY"); // BODY
    break;
  case 60 ... 69:
    p_rect = ReadValue("P_BODY"); // after-BODY
    break;
  case 70 ... 79:
    p_rect = ReadValue("P_TAIL"); // TAIL
    break;
  case 80 ... 99:
    p_rect = 0; // finish-pause-error
    break;
  default:
    p_rect = 0;
  }
  //  Pset0 = Pset0 - Pshift;
  //if (p_rect < 0)
  //  p_rect = 0;

  WriteValue("P.rect", p_rect);
}


// eof
