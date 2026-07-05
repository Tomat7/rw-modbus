// scada_check.cpp -----------------------------
// Copyright 2026 Tomat7 (star0413@gmail.com)

#include <string>

#include "scada.h"

/* Check */

void check()
{
  if_init();

  int Pshift=0;
  uint16_t Stop0;
  Process0=ReadValue("Process");
  Mode0=ReadValue("Mode");
  uint16_t OverPressure=ReadValue("DiffPress");
  uint16_t AbsPressure=ReadValue("AbsPress");


  //uint16_t Alarm0=ReadValue("Alarm_");
  uint16_t AlarmMB=ReadValue("Alarm.MB");
  uint16_t AlarmDS=ReadValue("Alarm.DS");
  uint16_t AlarmT=ReadValue("Alarm.T");
  uint16_t AlarmP=ReadValue("Alarm.P");

  float Tboil2=0.12f+(float)ReadValue("T_boil");

  if (AbsPressure < 755)
    Pshift = (755 - AbsPressure) * 10;
  if (OverPressure > 20)
    Pshift += (OverPressure - 20) * 10;
  if (OverPressure > 30)
    Pshift += (OverPressure - 30) * 20;
  if (OverPressure > 40)
    Pshift += (OverPressure - 40) * 40;
  WriteValue("Pshift", Pshift);

  Stop0 = Process0;
  if (Process0 < 90) {
    if (Mode0 > 90)
      Stop0=Mode0; // correct FINISH (see Mode)
    else if (Tkub0 > Tboil2)
      Stop0=91;    // stop by KUB
    else if (Tdef0 > 99)
      Stop0=92;    // stop by DEF (99 for DIST)
    else if (Tbuf0 > 98)
      Stop0=93;    // stop by BUF (98 for RECT)
    else if (OverPressure > 59)
      Stop0=94;    // Alarm! OverPressure
    else if ((Process0 == 88) || (Process0 == 89))
      Stop0=Process0; // cleaning and washing
    else if (Ttsa0   > 50)
      Stop0=95;    // Alarm! TSA too hot
    else if (AlarmP  > 10)
      Stop0=96;    // Alarm! Pnow out of set
    else if (AlarmT  > 5 )
      Stop0=97;    // Alarm! T>105
    else if (AlarmDS > 10)
      Stop0=98;    // Alarm! DS lost
    else if (AlarmMB > 100)
      Stop0=99;    // Alarm! ModbusQuality!=192
  }

  if (Stop0 != Process0)
    WriteValue("Process", Stop0);

}

// eof