// scada_millis.cpp -----------------------------
// Copyright 2026 Tomat7 (star0413@gmail.com)

#include <string>

#include "config.h"
#include "libs.h"

float Tkub0 = 0.0;
float Tkub1 = 0.0;
float Tkub2 = 0.0;
float Tbuf = 0.0;
float Ttsa = 0.0;

bool AddAlarm(string Aname)
{
  uint16_t a = 0;
  OPCs.ReadNumber(Aname, a);
  a++;
  OPCs.WriteNumber(Aname, a, true);
}

bool ReadTemp(string s, float &t)
{

  float f = 0.0;
  if (OPCs.ReadNumber(s, f)) {
    // Check for T range
    t = f;
    if (f > 100)
      AddAlarm("Alarm.T");
    if (f < -54)
      AddAlarm("Alarm.DS");
  } else {
    // Update Alarm.MB
    AddAlarm("Alarm.MB");
  }

}

int task_scada_(void* params)
{
  (void)(params);
  float f = 0.0;

  ReadTemp("Ttsa", Ttsa);

  if (OPCs.ReadNumber("Tkub2", f))
    Tbuf = f;
  if (OPCs.ReadNumber("Tkub1", f))
    Tkub1 = f;
  if (OPCs.ReadNumber("Tkub2", f))
    Tkub2 = f;

  Tkub0 = (Tkub1 > Tkub2) ? Tkub1 : Tkub2;
  OPCs.WriteNumber("Tkub0", Tkub0, true);



  LOGI("%s done.", __func__);
  return 0;
}

// eof