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
  value_u opc_value;  // value to print
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
    value_u plc_val;  // Value from PLC
    value_u opc_val;  // from OPC
    value_u old_val = rm.value; // Value in memory (in REGmap)

    bool isNew_Plc = false; // Got new value from PLC
    bool isNew_Opc = false; // Got new value from OPC

    if (rm.visible) {
      opc_val = opc_get_value(rm.str_opcname);  // from OPC
      isNew_Opc = (opc_val.ui64 != old_val.ui64);
    }

    if (rm.is_modbus) {
      plc_val = rm.get_plc_value();  // Value from PLC
      isNew_Plc = (plc_val.ui64 != old_val.ui64);
    }

    if (isNew_Plc) {
      rm.sync(plc_val);  // Save PLC value to REGmap
      if (!STRmap.count(n) || !STRmap[n].upd_plc)
        STRmap[n].upd_plc = true;
    }

    if (rm.var_mode && isNew_Opc) {
      x++;
      rm.sync(opc_val);

      if (rm.is_modbus)
        rm.set_plc_value(opc_val);

      if (!STRmap.count(n) || !STRmap[n].upd_opc) {
        STRmap[n].upd_opc = true;
        STRmap[n].opc_value = opc_val;
      }
    }
    // try to fill referenced/virtual register
    if (rm.is_ref) {
      auto &rf = REGmap[rm.str_source];
      //uint16_t plc_val/*  = rf.get_plc_val() */;  // Value from PLC
      old_val = rm.value          // Value in memory (in REGmap)
                //uint16_t opc_val = opc_update_uint16(n, rf.ptr_data_plc);
                //uint16_t opc_val /*=  opc_update_uint16((rm.ptr_reg)->str_opcname, &rf) */;

      isNew_Plc = (plc_val != old_val);
      isNew_Opc = (opc_val != old_val);

//      rm.sync_regdata(rf.ptr_data_plc);  // Save referenced PLC value

      if (isNew_Plc) {
        if (!STRmap.count(n) || !STRmap[n].upd_plc)
          STRmap[n].upd_plc = true;
      }

      if (rf.var_mode && isNew_Opc) {
        x++;
        rf.set_plc_reg(opc_val);
        rf.value.ui16 = opc_val;
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

    if (!rm.visible)
      continue;

    reg_print(n, &rm.ptr_reg[0]->data);

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

  // TODO: full recode with new TYPE_*

  if (rd->rtype == UA_TYPES_UINT16)
    printf("%s%-14s %s%7d", C, rn.c_str(), B, (uint16_t)rd->rvalue);
  else if (rd->rtype == UA_TYPES_INT16)
    printf("%s%-14s %s%7d", C, rn.c_str(), B, (int16_t)rd->rvalue);
  else if (rd->rtype == NOTUA_TYPES_F100)
    printf("%s%-14s %s%7.2f", C, rn.c_str(), B, (int16_t)rd->rvalue * 0.01);

  printf(C_NORM);

  return;
}

const char* getColor(bool noErrors) { return noErrors ? C_BOLD : C_DARK; }

const char* getBlynk(bool noErrors)
{
  return noErrors ? C_NORM : ESC_BLINK;  // Dark grey blym-blym
}


/*  */

// eof
