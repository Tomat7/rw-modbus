// scada.h -----------------------------
// Copyright 2026 Tomat7 (star0413@gmail.com)

#include <string>

#include "config.h"
#include "libs.h"

using namespace OPC_server;

extern float Tkub0;
extern float Tkub1;
extern float Tkub2;
extern float Tbuf;
extern float Tdef;
extern float Ttsa;

extern float &Tbuf0;
extern float &Tdef0;
extern float &Ttsa0;

extern uint16_t Pset;
extern uint16_t Pnow;
extern uint16_t &Pset0;
extern uint16_t &Pnow0;

extern uint16_t Process;
extern uint16_t Mode;

extern uint16_t &Process0;
extern uint16_t &Mode0;

void if_init();

void AddAlarm(string Aname);
bool ReadTemp(string s, float &t);
uint16_t UpdateTemps();

void SetPower();
void P_boil();
void P_heat();
void P_dist();
void P_rect();

// eof