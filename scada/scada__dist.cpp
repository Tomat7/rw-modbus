// scada_power.cpp -----------------------------
// Copyright 2026 Tomat7 (star0413@gmail.com)

#include <string.h>
// #include <sys/prctl.h>
// #include <cmath>
// #include <map>
// #include <string>
// #include <vector>

//#include "config.h"
//#include "libs.h"

#include "scada.h"

using namespace OPC_server;

static uint16_t p_dist = 0;
static uint16_t mod_dist = 0;

/* ============== P.dist ================= */

void Dist1()
{
  if (Tdef0 > 40) {
    switch ((int)Tkub0) {
    case 0 ... 97:
      p_dist = ReadValue("P_DIST");
      mod_dist=31;
      break;
    case 98 ... 101:
      p_dist = ReadValue("P_TAIL");
      mod_dist=71;
      break;
    default:
      p_dist = 0;
    }
  } else if (Tdef0 > 30)  {
    p_dist = ReadValue("P_WARM");
    p_dist = p_dist * 2;
    mod_dist=21;
  } else if (Tkub0 > 100) {
    p_dist = 0;  // на всякий случай
    mod_dist=91;
  } else                  {
    p_dist = ReadValue("P_MAX");
    mod_dist=11;
  }
}


void Dist2()
{
  if (Tdef0 > 40) {
    switch ((int)Tkub0) {
    case 0 ... 81:
    { p_dist = ReadValue("P_WARM");  mod_dist=12; break; }
    case 82 ... 96:
    { p_dist = ReadValue("P_DIST2"); mod_dist=32; break; }
    case 97 ... 100:
    { p_dist = ReadValue("P_DIST3"); mod_dist=33; break; }
    default:
      p_dist = 0;
    }
  } else {
    switch ((int)Tkub0) {
    case 0 ... 80:
    { p_dist = ReadValue("P_MAX");   mod_dist=12; break; }
    case 81 ... 100:
    { p_dist = ReadValue("P_WARM");  mod_dist=22; break; }
    default:
      p_dist = 0;
    }
  }
}

void CheckBoil()
{
  if_init();
  float Tboil_shift = 0.15;
  float Tboil0=ReadValue("T_boil");
  float Tstop0=ReadValue("T_heat");
  if (Tkub0>Tstop0 || Tkub0>(Tboil0+Tboil_shift))
    mod_dist=91;
}


void P_dist()
{
  if_init();
  p_dist = 0;

  if (Process0==1)
    Dist1();
  else if (Process0==2)
    Dist2();

  if (Process0==1 || Process0==2) {
    CheckBoil();
    WriteValue("Mode", mod_dist);
  }

  WriteValue("P.dist", p_dist);
}


// eof
