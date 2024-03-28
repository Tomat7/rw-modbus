// plc_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include "./plc_class.h"

#include <errno.h>
#include <modbus/modbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include <string>


PLC::PLC(int _port, string _name) // Slave only
{
  ip_addr = "0.0.0.0"; // Slave always listening on ALL addresses!
  tcp_port = _port;
  str_desc = _name;
  str_dev_name = _name;
  dev_name = str_dev_name.c_str();
  is_slave = true;
  logger(LOG_INFO, "+ New PLC created: %s:%d %s", ip_addr, tcp_port, dev_name);
}

// Destructor in plc_common.cpp

// eof
