/*
    Copyright © Stéphane Raimbault <stephane.raimbault@gmail.com>

    SPDX-License-Identifier: BSD-3-Clause
*/

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <modbus/modbus.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>

#include "./config.h"
#include "./libs.h"

int mb_slave_init();
int mb_slave_check();
void mb_slave_close();

static void close_sigint(int dummy) {
  mb_slave_close();
//  modbus_free(ctx);
//  modbus_mapping_free(mb_mapping);
  exit(dummy);
}

int main(void) {
  // ==============================================================
  signal(SIGINT, close_sigint);

  mb_slave_init();

  for (;;) {

    // =======================================================
    // Fill registers with data
    //    w++;
    //    for (int i = 0; i < 10; i++)
    //      mb_mapping->tab_registers[i] = w++;
    // =======================================================

    mb_slave_check();
  }
  return 0;
}
