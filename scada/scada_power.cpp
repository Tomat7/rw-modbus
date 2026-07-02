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

#include "scada.h"

using namespace OPC_server;

uint16_t Pset;
uint16_t Pnow;

uint16_t &Pset0;
uint16_t &Pnow0;


void P_set()
{
  if_init();
//  Process0 = ReadValue("Process");
//  int Pset0; //=ReadValue("Pset");
//  int Alarm0 = ReadValue("Alarm_");
//  int Mode0=ReadValue("Mode");
  uint16_t Pshift = ReadValue("Check");

  switch (Process0) {
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

  if (Process0 >= 9)
    Pset0 = Pset0 - Pshift;

  //  if (Alarm0 > 10) Pset0 = Pset0>>1;     // reduce if ModbusQuality!=192

  if (Pset0 < 0)
    Pset0 = 0;
  WriteValue("Pset", Pset0);
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
