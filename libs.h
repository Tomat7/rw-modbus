#pragma once
// config.h ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <modbus/modbus.h>
#include <syslog.h>

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include <map>
#include <string>
#include <vector>

#include <libconfig.h++>
#include <string.h>

#include "./decl.h"
#include "./in_class.h"
#include "./macros.h"
#include "./plc_class.h"
#include "./timer.h"
