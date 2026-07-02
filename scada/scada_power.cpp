// scada_power.cpp -----------------------------
// Copyright 2026 Tomat7 (star0413@gmail.com)

#include <string.h>
#include <sys/prctl.h>

#include <cmath>
#include <map>
#include <string>
#include <vector>

#include "config.h"
#include "libs.h"

float Tkub0 = 0.0;
float Tkub1 = 0.0;
float Tkub2 = 0.0;
float Tbuf0 = 0.0;
float Ttsa0 = 0.0;


int Pset0 = 0;
int Mode0 = 0;

void if_init() {};

void SetP1()
{
  float Tdef0 = ReadValue("Tdef");
  //float Tkub1 = ReadValue("Tkub1");
  //float Tkub2 = ReadValue("Tkub2");
  float Tkub0 = ReadValue("Tkub0");

  if (Tdef0 > 40) {
    switch ((int)Tkub0) {
    case 0 ... 97:
    { Pset0 = ReadValue("P_DIST"); Mode0=31; break; }
    case 98 ... 101:
    { Pset0 = ReadValue("P_TAIL"); Mode0=71; break; }
    default:
      Pset0 = 0;
    }
  } else if (Tdef0 > 30)  {
    Pset0 = ReadValue("P_WARM");
    Pset0 = Pset0 * 2;
    Mode0=21;
  } else if (Tkub0 > 100) {
    Pset0 = 0;  // на всякий случай
    Mode0=91;
  } else                  {
    Pset0 = ReadValue("P_MAX");
    Mode0=11;
  }
}

void SetP2()
{
  float Tdef0 = ReadValue("Tdef");
  //float Tkub1 = ReadValue("Tkub1");
  //float Tkub2 = ReadValue("Tkub2");
  float Tkub0 = ReadValue("Tkub0");

  if (Tdef0 > 40) {
    switch ((int)Tkub0) {
    case 0 ... 81:
    { Pset0 = ReadValue("P_WARM");  Mode0=12; break; }
    case 82 ... 96:
    { Pset0 = ReadValue("P_DIST2"); Mode0=32; break; }
    case 97 ... 100:
    { Pset0 = ReadValue("P_DIST3"); Mode0=33; break; }
    default:
      Pset0 = 0;
    }
  } else {
    switch ((int)Tkub0) {
    case 0 ... 80:
    { Pset0 = ReadValue("P_MAX");   Mode0=12; break; }
    case 81 ... 100:
    { Pset0 = ReadValue("P_WARM");  Mode0=22; break; }
    default:
      Pset0 = 0;
    }
  }
}

void ChkBoil()
{
  if_init();
  //float Tkub1 = ReadValue("Tkub1");
  //float Tkub2 = ReadValue("Tkub2");
  //float Tkub0 = (Tkub1 > Tkub2) ? Tkub1 : Tkub2;
  float Tkub0 = ReadValue("Tkub0");

  float Tboil0=ReadValue("T_boil");
  Tboil0 += 0.15;
  float Tstop0=ReadValue("T_heat");
  if ((Tkub0>Tstop0) || (Tkub0>Tboil0))
    Mode0=91;
}

// =========================================================


/* P.dist */

void P_dist()
{
  if_init();
  int job0 = ReadValue("Process");

  if (job0==1) {
    SetP1();
    ChkBoil();
  }
  if (job0==2) {
    SetP2();
    ChkBoil();
  }

  WriteValue("Mode", Mode0);
  WriteValue("P.dist", Pset0);
}


void P_set()
{
  if_init();

  int job0 = ReadValue("Process");
  int Pset0; //=ReadValue("Pset");
  int Alarm0 = ReadValue("Alarm_");
  int Pshift = ReadValue("Check");
  // int Mode0=ReadValue("Mode");

  switch (job0) {
  case 0:
    Pset0 = 0;
    break;
  case 1:
    Pset0 = ReadValue("P.dist");
    break;
  case 2:
    Pset0 = ReadValue("P.dist");
    break;
  case 4:
    Pset0 = ReadValue("P.heat");
    break;
  case 7:
    Pset0 = ReadValue("P.boil");
    break;
  case 9 ... 79:
    Pset0 = ReadValue("P.rect");
    break;
  case 80 ... 99:
    Pset0 = 0;
    break;
  default:
    Pset0 = 0;
  }

  if (job0 >= 9)
    Pset0 = Pset0 - Pshift;

  //  if (Alarm0 > 10) Pset0 = Pset0>>1;     // reduce if ModbusQuality!=192

  if (Pset0 < 0)
    Pset0 = 0;
  WriteValue("Pset", Pset0);
}

void P_rect()
{
  if_init();

  int Pset0 = 0;
  int pMode;
  int job0 = ReadValue("Process");
  int Mode0 = ReadValue("Mode");
  //  Int Pshift = ReadValue("Check");

  if (job0 > 90)
    pMode = 99;
  else if (job0 == 9)
    pMode = Mode0;
  else if (job0 > 9)
    pMode = job0;
  else
    pMode = 99;

  switch (pMode) {
  case 0 ... 9:
    Pset0 = 0;
    break;
  case 10 ... 19:
    Pset0 = ReadValue("P_MAX"); // RUN
    break;
  case 20 ... 29:
    Pset0 = ReadValue("P_WARM"); // HEAT
    break;
  case 30 ... 39:
    Pset0 = ReadValue("P_HEAD"); // STAB
    break;
  case 40 ... 49:
    Pset0 = ReadValue("P_HEAD"); // HEAD
    break;
  case 50 ... 59:
    Pset0 = ReadValue("P_BODY"); // BODY
    break;
  case 60 ... 69:
    Pset0 = ReadValue("P_BODY"); // after-BODY
    break;
  case 70 ... 79:
    Pset0 = ReadValue("P_TAIL"); // TAIL
    break;
  case 80 ... 99:
    Pset0 = 0; // finish-pause-error
    break;
  default:
    Pset0 = 0;
  }
  //  Pset0 = Pset0 - Pshift;

  if (Pset0 < 0)
    Pset0 = 0;
  WriteValue("P.rect", Pset0);
}

void P_heat()
{
  if_init();

  int Mode0;
  int Pmax0, Pset0 = 0, Pmin0 = 300;
  float Theat0 = ReadValue("T_heat");
  int job0 = ReadValue("Process");
  int PID0;

  if (job0 == 4) {
    Pmax0 = ReadValue("P_MAX");
    PID0 = ReadValue("PID");
    Pmin0 = (int)(Theat0 * 10);

    if (PID0 == 0)
      Mode0 = 84; // pause
    else if (PID0 > 99)
      Mode0 = 14; // run!
    else
      Mode0 = 24; // slow

    if (PID0 > 0)
      Pset0 = Pmin0 + (int)(PID0 * Pmax0 / 100);
    else
      Pset0 = 0;

    if ((Pset0 > 0) && (Pset0 < Pmin0))
      Pset0 = Pmin0;
    if (Pset0 > Pmax0)
      Pset0 = Pmax0;

    WriteValue("Mode", Mode0);
  }

  WriteValue("P.heat", Pset0);
}


// old version ==============================================
/*
  void SetP1()
  {
  switch (Tkub0)
  {
    case 0..94:     { Pset0 = ReadValue("P_MAX"); Mode0=11; }
    case 94..100:   { Pset0 = ReadValue("P_DIST"); Mode0=21; }
    default:        Pset0 = 0;
  }
  switch (Tdef0)
  {
    case 0..40:    { Pset0 = Pset0; }
    case 40..97:   { Pset0 = ReadValue("P_DIST"); Mode0=31; }
    case 97..99:   { Pset0 = ReadValue("P_TAIL"); Mode0=71; }
    default:       Pset0 = 0;
  }
  }
*/

// eof
