// scada_p_heat.cpp -----------------------------
// Copyright 2026 Tomat7 (star0413@gmail.com)

#include <string.h>

//#include "config.h"
//#include "libs.h"
#include "scada.h"

using namespace OPC_server;

static uint16_t p_heat = 0;
static uint16_t mod_heat = 0;

/* ============== P.heat ================= */

void P_heat()
{
  if_init();
  p_heat = 0;

  if (Process0 == 4) {

    float Theat0 = ReadValue("T_heat");
    uint16_t PID0 = ReadValue("PID");
    uint16_t Pmax0 = ReadValue("P_MAX");
    uint16_t Pmin0 = (uint16_t)(Theat0 * 10);

    if (PID0 == 0)
      mod_heat = 84; // pause
    else if (PID0 > 99)
      mod_heat = 14; // run!
    else
      mod_heat = 24; // slow

    if (PID0 > 0)
      p_heat = Pmin0 + (uint16_t)(PID0 * Pmax0 / 100);
    else
      p_heat = 0;

    if ((p_heat > 0) && (p_heat < Pmin0))
      p_heat = Pmin0;

    if (p_heat > Pmax0)
      p_heat = Pmax0;

    WriteValue("Mode", mod_heat);
  }

  WriteValue("P.heat", p_heat);
}


// eof
