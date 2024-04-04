#pragma once
// config.h ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <modbus/modbus.h>
#include <syslog.h>

#include <cstdlib>
#include <iomanip>
#include <iostream>
//#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include <libconfig.h++>
#include <map>
#include <string>
#include <vector>

#include "include/decl.h"
#include "include/in_class.h"
#include "include/macros.h"
#include "include/plc_class.h"
#include "include/timer.h"
#include "include/logger.h"
#include "include/shmem.h"
