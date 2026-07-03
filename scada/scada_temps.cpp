// scada_millis.cpp -----------------------------
// Copyright 2026 Tomat7 (star0413@gmail.com)

#include <string>
#include <map>

#include "scada.h"

float Tkub0 = 0.0;
float Tkub1 = 0.0;
float Tkub2 = 0.0;
float Tdef = 0.0;
float Tbuf = 0.0;
float Ttsa = 0.0;

float &Tbuf0 = Tbuf;
float &Tdef0 = Tbuf;
float &Ttsa0 = Ttsa;

#define FQN(S) OPCs.GetVarFullName(S)

static std::map<string, float*> TempMap {
  { "Tkub1", &Tkub1 },
  { "Tkub2", &Tkub2 },
  { "Tbuf", &Tbuf  },
  { "Tdef", &Tdef  },
  { "Ttsa", &Ttsa  }
};

void AddAlarm(string Alarm_name)
{
  uint16_t a = 0;
  OPCs.ReadNumber(FQN(Alarm_name), a);
  a++;
  OPCs.WriteNumber(FQN(Alarm_name), a, true);
}

bool ReadTemp(string s, float &t)
{
  bool ret = false;
  float f = 0.0;

  if (OPCs.ReadNumber(FQN(s), f)) { // Check for T range
    t = f;
    if (f > 100)
      AddAlarm("Alarm.T");
    if (f < -54)
      AddAlarm("Alarm.DS");
    ret = true;
  } else       // Update Alarm.MB
    AddAlarm("Alarm.MB");

  return ret;
}

uint16_t UpdateTemps()
{
  uint16_t error_counter = 0;

  for (auto& [S, t_] : TempMap) {
    if (!ReadTemp(S, *t_))
      error_counter++;
  }

  Tkub0 = (Tkub1 > Tkub2) ? Tkub1 : Tkub2;
  OPCs.WriteNumber(FQN("Tkub0"), Tkub0, true);

  return error_counter;
}


// eof