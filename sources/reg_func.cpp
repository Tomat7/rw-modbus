// reg_func.cpp -----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <string.h>

#include <map>
#include <string>
#include <vector>

#include "config.h"
#include "libs.h"

#define MB_READ

struct regupd_t {
  bool upd_plc = false;    // need print ">"
  bool upd_opc = false;    // need print "<"
  uint16_t opc_value = 0;  // value to print
};

static map<string, regupd_t> STRmap;
static mutex regmap_mux;  // already defined in .h

void reg_print(string, const regdata_t*);

int task_regs_refresh_(void* params)
{
  UNUSED(params);
  LOGX(" ===== %s =====", __func__);
  regmap_mux.lock();
  int x = 0;

  for (auto& [n, rm] : REGmap) {
    if (rm.is_MB()) {
      uint16_t plc_val = rm.get_plc_val();  // Value from PLC
      uint16_t shm_val = rm.get_shm_val();  // Value from SHM
      uint16_t old_val = rm.value;          // Value in memory (in REGmap)
      uint16_t opc_val = opc_update_uint16(n, rm.ptr_data_plc);

      // printf("%7d %7d %7d %7d ==", plc_val, old_val, shm_val, opc_val);

      bool isNew_Plc = (plc_val != old_val);
      bool isNew_Opc = (opc_val != shm_val);

      rm.sync(plc_val);  // Save PLC value to REGmap (SHM & local)

      if (isNew_Plc) {
        if (!STRmap.count(n) || !STRmap[n].upd_plc)
          STRmap[n].upd_plc = true;
      }

      if (rm.get_mode() && isNew_Opc) {
        x++;
        rm.set_plc_val(opc_val);
        rm.value = opc_val;
        if (!STRmap.count(n) || !STRmap[n].upd_opc) {
          STRmap[n].upd_opc = true;
          STRmap[n].opc_value = opc_val;
        }
      }
      // try to fill referenced/virtual register
    } else if (rm.is_Ref()) {
      auto &rf = REGmap[rm.src_reference];
      uint16_t plc_val = rf.get_plc_val();  // Value from PLC
      uint16_t shm_val = rm.get_shm_val();  // Value from SHM
      uint16_t old_val = rm.value;          // Value in memory (in REGmap)
      uint16_t opc_val = opc_update_uint16(n, rf.ptr_data_plc);

      bool isNew_Plc = (plc_val != old_val);
      bool isNew_Opc = (opc_val != shm_val);

      rm.sync_regdata(rf.ptr_data_plc);  // Save referenced PLC value

      if (isNew_Plc) {
        if (!STRmap.count(n) || !STRmap[n].upd_plc)
          STRmap[n].upd_plc = true;
      }

      if (rf.get_mode() && isNew_Opc) {
        x++;
        rf.set_plc_val(opc_val);
        rf.value = opc_val;
        if (!STRmap.count(n) || !STRmap[n].upd_opc) {
          STRmap[n].upd_opc = true;
          STRmap[n].opc_value = opc_val;
        }
      }

    }
  }

  regmap_mux.unlock();
  LOGI("%s done: %d", __func__, x);

  return x;
}

void regs_update()
{
  printf("\n===== regs_update =====\n");
  // task_regs_refresh_();
  regmap_mux.lock();
  bool is_eol = false;
  string X;

  for (auto& [n, rm] : REGmap) {
    reg_print(n, rm.ptr_data_plc);

    X = (STRmap[n].upd_plc) ? ">" : " ";   // If new value got from PLC
    X += (STRmap[n].upd_opc) ? "<" : " ";  // If new value got from OPC
    printf("%s", X.c_str());

    if (STRmap[n].upd_opc)
      printf("%7d", STRmap[n].opc_value);
    else
      printf("%7s", "       ");

    if (is_eol)
      printf(" + %s\n", NRM);
    else
      printf(" +     %s", NRM);

    is_eol = !is_eol;
  }

  if (is_eol)
    printf("\n");

  STRmap.clear();
  regmap_mux.unlock();

  return;
}

void reg_print(string rn, const regdata_t* rd)
{
  // printf("\n===== regs_print =====\n");
  const char* C = getColor(rd->rerrors == 0);  // C_WHIB;  // NRM;
  const char* B = getBlynk(rd->rerrors == 0);

  if (rd->rtype == 0)
    printf("%s%-12s %s%7d", C, rn.c_str(), B, (uint16_t)rd->rvalue);
  else if (rd->rtype == 1)
    printf("%s%-12s %s%7d", C, rn.c_str(), B, (int16_t)rd->rvalue);
  else if (rd->rtype == 2)
    printf("%s%-12s %s%7.2f", C, rn.c_str(), B, (int16_t)rd->rvalue * 0.01);

  printf(C_NORM);

  return;
}

const char* getColor(bool noErrors) { return noErrors ? C_BOLD : C_DARK; }

const char* getBlynk(bool noErrors)
{
  return noErrors ? C_NORM : ESC_BLINK;  // Dark grey blym-blym
}

// eof
