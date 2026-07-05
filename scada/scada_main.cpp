// scada_check.cpp -----------------------------
// Copyright 2026 Tomat7 (star0413@gmail.com)

#include <string>

#include "scada.h"

uint16_t Mode = 0;
uint16_t Process = 0;
uint16_t &Mode0 = Mode;
uint16_t &Process0 = Process;

void UpdateProcess();

int task_scada_(void* params)
{
  (void)(params);

  UpdateProcess();

  uint16_t err = UpdateTemps();
  if (err)
    LOGW("Update TEMPs error: %i", err);

  SetPower();

  LOGI("%s done.", __func__);
  return 0;
}

void UpdateProcess()
{
  //OPCs.ReadNumber("Process", Process0);
  //OPCs.ReadNumber("Mode", Mode0);
  Process0 = ReadValue("Process");
  Mode0 = ReadValue("Mode");
}

void if_init() {};

// eof