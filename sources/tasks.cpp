// tasks.cpp ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

/* #include <map>
  #include <set>

  #include <vector>
  #include <variant>
*/

#include <string>
#include "config.h"
#include "libs.h"


int millis_()
{
  int i = 0;
  for (auto& [n, rm] : REGmap) {
    if (n.find("millis") != std::string::npos) {
      WriteValue(n, 0);
      i++;
    }
  }
  LOGC("%s done: %d\n", __func__, i);
  return i;
}

int opc_refresh_()
{
  OPCs.refreshValues();
  LOGC("%s: Done.\n", __func__);
  return 1;
}

int begin_()
{
  //OPCs.refreshValues();
  LOGC("%s: Done.\n", __func__);
  return 1;
}

