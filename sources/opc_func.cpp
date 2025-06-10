// reg_func.cpp -----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <string.h>
#include <sys/prctl.h>

#include <cmath>
#include <map>
#include <string>
#include <vector>

#include "config.h"
#include "libs.h"

#define MB_READ

#define CAST(_XTYPE) static_cast<_XTYPE>

void opc_regs_init()
{
  printf("\n===== %s =====\n", __func__);

  for (auto &[name, rm] : REGmap) {
    // reg_print(n, rm.ptr_data_plc);
    // n - name, rm - RegMap_c rm.set_shm_val();

    string n, e;
    n = rm.str_opcname;
    e = Cfg.opc.ErrFolder + n + Cfg.opc.ErrSuffix;  // Kub.Temp1.errors

    OPCs.AddVar(e, (uint16_t)0, 0);  // Reg to keep NB of errors

    //  value_u v;
    //  v.ui64 = 0;
    int t = rm.var_type;
    if (rm.is_modbus)
      t = UA_TYPES_UINT16;

    if (t == UA_TYPES_UINT16)
      OPCs.AddVar(n, rm.get_local_value().ui16, rm.var_mode);
    else if (t == UA_TYPES_INT16)
      OPCs.AddVar(n, rm.get_local_value().i16, rm.var_mode);
    else if (t == UA_TYPES_UINT32)
      OPCs.AddVar(n, rm.get_local_value().ui32, rm.var_mode);
    else if (t == UA_TYPES_INT32)
      OPCs.AddVar(n, rm.get_local_value().i32, rm.var_mode);
    else if (t == UA_TYPES_UINT64)
      OPCs.AddVar(n, rm.get_local_value().ui64, rm.var_mode);
    else if (t == UA_TYPES_INT64)
      OPCs.AddVar(n, rm.get_local_value().i64, rm.var_mode);
    else if (t == NOTUA_TYPES_F100)
      OPCs.AddVar(n, rm.get_local_value().fl, rm.var_mode);
    else if (t == NOTUA_TYPES_F10)
      OPCs.AddVar(n, rm.get_local_value().fl, rm.var_mode);
    else if (t == UA_TYPES_FLOAT)
      OPCs.AddVar(n, rm.get_local_value().fl, rm.var_mode);
    else if (t == UA_TYPES_DOUBLE)
      OPCs.AddVar(n, rm.get_local_value().dbl, rm.var_mode);
    else
      LOGE("Wrong type: %d, Var: %s", t, n.c_str());
  }
}

value_u opc_get_value(string s) { return OPCs.ReadRawUnion(OPCs.GetVarFullName(s)); }

bool opc_set_value(string s, value_u val, bool isOK)
{
  return OPCs.WriteRawUnion(OPCs.GetVarFullName(s), val, isOK);
}

void opc_deinit() { OPCs.stop(); }

void opc_init() { OPCs.init(Cfg.opc.SrvPort); }

void opc_start()
{
  std::thread opc_thread(opc_run_thread);
  opc_thread.detach();
  wait_console(Cfg.timeout_sec);
}

void opc_run_thread()
{
  prctl(PR_SET_NAME, Cfg.opc.SrvName.c_str());
  OPCs.run();
}


// ======= OPC server operations =======

void opc_server_()
{
  using OPC_server::ReadValue;
  using OPC_server::WriteValue;
  /*
          i++;
          string s;
          s = "/PLC/Kub/Kub.millis";
          OPCs.setVar(s, i);
          s = "/PLC/Kub/Kub.Temp1";
          OPCs.setVar(s, (float)i);
  */

  string s = "/PLC/Kub/Kub.millis";
  printf("Kub.millis: %d, ", OPCs.ReadRawUnion(s).ui16);

  s = "/PLC/Kub/Kub.Temp1";
  printf("T1: %5.2f, ", OPCs.ReadRawUnion(s).fl);

  s = "/PLC/Kub/Kub.Temp2";
  float fl;
  OPCs.ReadNumber(s, fl);
  printf("T2: %5.2f", fl);

  s = "Kub.Temp3";
  float myfl = ReadValue(s);
  const char* C = getColor(OPCs.isVariable(s));
  const char* B = getBlynk(OPCs.isGood(s));
  printf("%sT3a: %s%5.2f%s, ", C, B, myfl, NRM);

  s = "Kub.Temp3";
  myfl = ReadValue(s);
  C = getColor(OPCs.isVariable(s));
  B = getBlynk(OPCs.isGood(s));
  printf("%sT3b: %s%5.2f%s, ", C, B, myfl, NRM);
  // printf("T4: %5.3f, ", myfl /*(float)ReadValue(s)*/);
  int16_t t16 = (int16_t)round(myfl * 100);
  myfl = t16 / 100;
}

// ======= OPC CLIENT operations =======
void opc_client_()
{
  using OPC_client::ReadValue;
  using OPC_client::WriteValue;

  static uint16_t cnt = 0;
  static uint16_t ccc = 0;

  cnt++;
  string s = OPCs.GetVarFullName("Millis");

//    t.start();
//  OPCclient.ReadNumber(s, ccc);
  ccc = ReadValue(s);
//    t.spent_auto("OPC Client read ONE reg in: ");

  /*
    if (OPCclient.WriteNumber(s, cnt)) {
      OPCclient.ReadNumber(s, ccc);
      OPCs.RefreshAllValues();
      printf("%s: %d %d %d\n", s.c_str(), ccc, cnt, OPCs.ReadRawUnion(s).ui16);
    } else
      printf("%s: %d %d %d error!\n", s.c_str(), ccc, cnt, OPCs.ReadRawUnion(s).ui16);
  */
  if (OPCclient.WriteNumber(s, cnt)) {
    // OPCclient.ReadNumber(s, ccc);
    ccc = ReadValue(s);
    OPCs.RefreshAllValues();
    printf("%s: %d %d %d\n", s.c_str(), ccc, cnt, OPCs.ReadRawUnion(s).ui16);
  } else
    printf("%s: %d %d %d error!\n", s.c_str(), ccc, cnt, OPCs.ReadRawUnion(s).ui16);
// ======= Read CLIENT =======

  float t1 = 0;
  s = OPCs.GetVarFullName("Tkub1");

  if (OPCclient.ReadNumber(s, t1))
    printf("%s: %f %f\n", s.c_str(), t1, OPCs.ReadRawUnion(s).fl);
  else
    printf("%s: %f %f error!\n", s.c_str(), t1, OPCs.ReadRawUnion(s).fl);
}

// eof
