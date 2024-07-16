#pragma once
// config.h ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <ctype.h>
#include <modbus/modbus.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <termios.h>
#include <unistd.h>

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <libconfig.h++>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "include/console.h"
#include "include/decl.h"
#include "include/in_class.h"
#include "include/logger.h"
#include "include/macros.h"
#include "include/plc_class.h"
#include "include/shmem.h"
#include "include/timer.h"