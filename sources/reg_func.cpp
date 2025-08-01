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
  bool upd_plc = false;  // need print ">"
  bool upd_opc = false;  // need print "<"
  bool err_plc = false;  // need blynk
  bool err_opc = false;  // need ...
  numeric_u opc_value;     // value to print
};

static map<string, regupd_t> STRmap;
static mutex regmap_mux;  // already defined in .h

// void reg_print(string, const mbregdata_t*);
void reg_print(Reg_c &);

int task_regs_refresh_(void* params)
{
  UNUSED(params);
  LOG_BLU(" ===== %s =====", __func__);
  regmap_mux.lock();
  int x = 0;

  for (auto& [n, rm] : REGmap) {
    numeric_u plc_val;  // Value from PLC
    bool plc_err = false;
    numeric_u opc_val = opc_get_value(rm.str_opcname);  // from OPC
    bool opc_err = !OPCs.isGood(rm.str_opcname);
    numeric_u old_val = rm.get_local_value();  // Value in memory (in REGmap)
    bool old_err = rm.var_errors;

    bool isNew_Plc = false;                 // Got new value from PLC
    bool isNew_Opc = (opc_val != old_val);  // Got new value from OPC?

    if (rm.is_modbus || rm.is_ref) {
      plc_val = rm.get_plc_value();  // Value from PLC, will update var_errors
      plc_err = rm.get_plc_errors();

      STRmap[n].err_plc = plc_err;
      isNew_Plc = (plc_val != old_val);

      // Check the NewValue from PLC or errors change
      if (isNew_Plc || (plc_err != old_err)) {
        rm.set_local_value(plc_val);  // Save PLC value to REGmap
        rm.var_errors = plc_err;
        opc_set_value(rm.str_opcname, plc_val, !plc_err);
        if (!STRmap.count(n) || !STRmap[n].upd_plc)
          STRmap[n].upd_plc = true;
      } else if (old_err && plc_err)
        rm.var_errors++;

      // Check the writable regs
      if (rm.var_mode_rw && isNew_Opc && !plc_err && !opc_err) {
        x++;
        rm.set_plc_value(opc_val);
        rm.set_local_value(opc_val);
        rm.var_errors = plc_err || opc_err;
        if (!STRmap.count(n) || !STRmap[n].upd_opc) {
          STRmap[n].upd_opc = true;
          STRmap[n].opc_value = opc_val;
        }
      }
      // ++ is_Modbus or is_Referenced
    } else if (rm.is_scada) {
      STRmap[n].err_opc = opc_err;
      rm.var_errors = opc_err;

      if (/* rm.var_mode &&  */ isNew_Opc) {
        x++;
        rm.set_local_value(opc_val);
        if (!STRmap.count(n) || !STRmap[n].upd_opc) {
          STRmap[n].upd_opc = true;
          STRmap[n].opc_value = opc_val;
        }
      } // ++ isNew_Opc

    } // ++ is_Scada
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
    //  reg_print(n, &rm.ptr_reg[0]->data);
    if (!rm.is_modbus || Cfg.show_mb_regs) {

      reg_print(rm);

      X = (STRmap[n].upd_plc) ? ">" : " ";   // If new value got from PLC
      X += (STRmap[n].upd_opc) ? "<" : " ";  // If new value got from OPC
      printf("%s", X.c_str());

      if (STRmap[n].upd_opc)
        printf("%7d", STRmap[n].opc_value.ui16);
      else
        printf("%7s", "       ");

      if (is_eol)
        printf(" + %s\n", NRM);
      else
        printf(" +     %s", NRM);

      is_eol = !is_eol;
    }
  }

  if (is_eol)
    printf("\n");

  STRmap.clear();
  regmap_mux.unlock();

  return;
}


void reg_print(Reg_c &rm)
{
  // printf("\n===== regs_print =====\n");
  const char* C = getColor(rm.var_errors == 0);  // C_WHIB;  // NRM;
  const char* B = getBlynk(rm.var_errors == 0);
  // char ch[50];

  //printf("%s%-14s %s%14s", C, rm.rn, B, rm.get_local_value_chars(ch));
  printf("%s%-16s %i %s%16s", C, rm.rn, rm.var_errors,
         B, rm.c_str() /* rm.get_local_value_chars(ch) */);

  printf(C_NORM);

  return;
}


void mb_print_help()
{
  printf("%s 'f'/'s' - fast/slow refresh, 'h' - (un)hide Modbus regs", C_BOLD);
  printf("%s 'r' - reread config, %s'q'||'e' - exit, 1..7 - set loglevel,%s\n",
         C_HIGH, C_HIGH2, C_NORM);
}

// BOLD or Dark grey if error
const char* getColor(bool noErrors) { return noErrors ? C_BOLD : C_DARK; }

// NORN or blym-blym if error
const char* getBlynk(bool noErrors) { return noErrors ? C_NORM : ESC_BLINK; }


/*
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
*/


// eof
