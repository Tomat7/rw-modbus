/* Pset */

#include "config.cpp"

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
  case 1:
    Pset0 = ReadValue("P.dist");
  case 2:
    Pset0 = ReadValue("P.dist");
  case 4:
    Pset0 = ReadValue("P.heat");
  case 7:
    Pset0 = ReadValue("P.boil");
  case 9..79:
    Pset0 = ReadValue("P.rect");
  case 80..99:
    Pset0 = 0;
  default:
    Pset0 = 0;
  }

  if (job0 >= 9)
    Pset0 = Pset0 - Pshift;

  //  if (Alarm0 > 10) Pset0 = Pset0>>1;     // reduce if ModbusQuality!=192

  if (Pset0 < 0)
    Pset0 = 0;
  WriteValue(Pset, Pset0);
}