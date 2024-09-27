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

#include "include/timer.h"
#include "include/console.h"
#include "include/decl.h"
#include "include/logger.h"
#include "include/macros.h"
#include "include/inotify/in_class.h"
#include "include/plc/plc_class.h"
#include "include/shmem/shmem.h"
#include "include/opc/opc_class.h"
