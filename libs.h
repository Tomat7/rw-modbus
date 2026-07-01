#pragma once
// libs.h ---------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

//#include "include/inotify/in_class.h"
//#include "include/shmem/shmem.h"
//#include "include/logger.h"

#include "include/declarations.h"
#include "include/numeric.h"
#include "include/macros.h"
#include "include/timer.h"

#include "libs/logger/logger.h"
#include "libs/console/console.h"
#include "libs/opc_server/opcs_class.h"
#include "libs/opc_client/opc_client.h"
#include "libs/mb_plc/mbplc_class.h"
#include "libs/number/number_class.h"
//#include "libs/net_service/net_service.h"
//#include "include/scheduler/scheduler.h"

#ifdef USE_NCURSES
#include <ncurses.h>
#endif

// eof
