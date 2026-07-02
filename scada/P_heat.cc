/* P.heat */

#include "config.cpp"

{
  if_init();

  Int Mode0;
  Int Pmax0, Pset0 = 0, Pmin0 = 300;
  float Theat0 = ReadValue("T_heat");
  Int job0 = ReadValue("Process");
  Int PID0;

  if (job0 == 4) {
    Pmax0 = ReadValue("P_MAX");
    PID0 = ReadValue("PID");
    Pmin0 = Round(Theat0 * 10);

    if (PID0 == 0) {
      Mode0 = 84;
    } // pause
    else if (PID0 > 99) {
      Mode0 = 14;
    } // run!
    else {
      Mode0 = 24;
    } // slow

    if (PID0 > 0)
      Pset0 = Pmin0 + Round(PID0 * Pmax0 / 100);
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