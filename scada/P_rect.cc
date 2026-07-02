/* P.rect */

#include "config.cpp"

{
  if_init();

  Int Pset0 = 0;
  Int Pmode;
  Int job0 = ReadValue("Process");
  Int Mode0 = ReadValue("Mode");
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
  case 0..9:
    Pset0 = 0;
  case 10..19:
    Pset0 = ReadValue("P_MAX"); // RUN
  case 20..29:
    Pset0 = ReadValue("P_WARM"); // HEAT
  case 30..39:
    Pset0 = ReadValue("P_HEAD"); // STAB
  case 40..49:
    Pset0 = ReadValue("P_HEAD"); // HEAD
  case 50..59:
    Pset0 = ReadValue("P_BODY"); // BODY
  case 60..69:
    Pset0 = ReadValue("P_BODY"); // after-BODY
  case 70..79:
    Pset0 = ReadValue("P_TAIL"); // TAIL
  case 80..99:
    Pset0 = 0; // finish-pause-error
  default:
    Pset0 = 0;
  }
  //  Pset0 = Pset0 - Pshift;

  if (Pset0 < 0)
    Pset0 = 0;
  WriteValue("P.rect", Pset0);
}
