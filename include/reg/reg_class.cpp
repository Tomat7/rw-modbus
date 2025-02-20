// reg_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include "reg_class.h"

#include <fcntl.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <syslog.h>
#include <unistd.h>

#include <chrono>
#include <map>
#include <mutex>
#include <string>

#include "include/logger.h"
#include "include/plc/plc_class.h"
#include "include/shmem/shmem.h"

#ifdef SYSLOG_NAME
#undef SYSLOG_NAME
#endif
#define SYSLOG_NAME "REG-class"


bool Reg_c::is_MB() { return (str_source == ""); }

bool Reg_c::is_Scada() { return (str_source == "-"); }

bool Reg_c::has_Ref() { return !(is_MB() || is_Scada()); }

bool Reg_c::has_Str(string SS, string fs) { return SS.find(fs) != std::string::npos; }


// ========================================

value_u Reg_c::get_local_value() { return value; }

// value_u Reg_c::get_scada_value() { return OPCs.readRawValue(str_opcname); }

void Reg_c::sync(value_u _val)
{
  value = _val;
}

void Reg_c::sync()
{
  /*   sync(get_plc_val()); */
  return;
}

void Reg_c::sync_regdata(regdata_t* ptr_data)
{

}

uint16_t Reg_c::get_local()
{
  /*   if (is_shm()) {
      regdata_t mem;
      memcpy(&mem, ptr_data_shm, sizeof(regdata_t));
      return mem.rvalue;
    } */
  return 0;
}


void Reg_c::set_local(uint16_t _val)
{

}

// eof
