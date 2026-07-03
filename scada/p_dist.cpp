// p_dist.cpp -----------------------------
// Copyright 2026 Tomat7 (star0413@gmail.com)

#include <string.h>
//#include "config.h"
//#include "libs.h"

#include "scada.h"

using namespace OPC_server;

static uint16_t p_dist = 0;
static uint16_t mod_dist = 0;

#define T_IN_RANGE(T, MIN_T, MAX_T) (T >= MIN_T && T < MAX_T)

/* ============== P.dist ================= */

void Dist1()
{
  p_dist = 0;

  if (Tdef0 > 40) {
    if (T_IN_RANGE(Tkub0, 0, 97)) {
      p_dist = ReadValue("P_DIST");
      mod_dist=31;
    } else if (T_IN_RANGE(Tkub0, 97, 101)) {
      p_dist = ReadValue("P_TAIL");
      mod_dist=71;
    }
  } else if (Tdef0 > 30) {
    p_dist = ReadValue("P_WARM");
    p_dist = p_dist * 2;
    mod_dist=21;
  } else if (Tkub0 > 100) {
    p_dist = 0;  // на всякий случай
    mod_dist=91;
  } else {
    p_dist = ReadValue("P_MAX");
    mod_dist=11;
  }
}


void Dist2()
{
  p_dist = 0;
  if (Tdef0 < 40) {
    if (T_IN_RANGE(Tkub0, 0, 80)) {
      p_dist = ReadValue("P_MAX");
      mod_dist=12;
    } else if (T_IN_RANGE(Tkub0, 80, 100)) {
      p_dist = ReadValue("P_WARM");
      mod_dist=22;
    }
  } else {
    if (T_IN_RANGE(Tkub0, 0, 82)) {
      p_dist = ReadValue("P_WARM");
      mod_dist=12;
    } else if (T_IN_RANGE(Tkub0, 82, 97)) {
      p_dist = ReadValue("P_DIST2");
      mod_dist=32;
    } else if (T_IN_RANGE(Tkub0, 97, 100)) {
      p_dist = ReadValue("P_DIST3");
      mod_dist=33;
    }
  }
}


void CheckBoil()
{
  if_init();
  float Tboil_shift = 0.15f;
  float Tboil0=ReadValue("T_boil");
  float Tstop0=ReadValue("T_heat");
  if (Tkub0>Tstop0 || Tkub0>(Tboil0+Tboil_shift))
    mod_dist=91;
}


void P_dist()
{
  if_init();
  p_dist = 0;

  if (Process0==1 || Process0==2) {
    if (Process0==1)
      Dist1();
    else if (Process0==2)
      Dist2();

    CheckBoil();
    WriteValue("Mode", mod_dist);
  }

  WriteValue("P.dist", p_dist);
}


// eof
