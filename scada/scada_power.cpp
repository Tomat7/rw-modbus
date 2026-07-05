// scada_power.cpp -----------------------------
// Copyright 2026 Tomat7 (star0413@gmail.com)

#include "scada.h"

uint16_t Pset;
uint16_t Pnow;

uint16_t &Pset0 = Pset;
uint16_t &Pnow0 = Pnow;


void SetPower()
{
  if_init();
  Pset0 = 0;
  int Pshift = ReadValue("Pshift");

  P_boil();
  P_heat();
  P_dist();
  P_rect();

  if (Process0==1 || Process0==2)
    Pset0 = ReadValue("P.dist");
  else if (Process0==4)
    Pset0 = ReadValue("P.heat");
  else if (Process0==7)
    Pset0 = ReadValue("P.boil");
  else if (Process0 >= 9 && Process0 <= 79)
    Pset0 = ReadValue("P.rect");
  else
    Pset0 = 0;

  if (Process0 >= 9 && Pset0 > Pshift)
    Pset0 = Pset0 - Pshift;

  //  if (Alarm0 > 10) Pset0 = Pset0>>1;     // reduce if ModbusQuality!=192
  //if (Pset0 < 0) Pset0 = 0;

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
