/*
    Copyright © Stéphane Raimbault <stephane.raimbault@gmail.com>

    SPDX-License-Identifier: BSD-3-Clause
*/

#include <arpa/inet.h>
#include <errno.h>
#include <modbus/modbus.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include "./config.h"
#include "./libs.h"

#include "include/plc_class.h"

//int mb_slave_init();
//int mb_slave_check(int usec = 1000000);
//void mb_slave_close();
//void mb_slave_print_reg(int /*reg_address*/);

Timer t;
PLC_c Slave(1502, 9);
uint16_t w;


static void close_sigint(int dummy)
{
//  mb_slave_close();
  //  modbus_free(ctx);
  //  modbus_mapping_free(mb_mapping);
  Slave.mb_deinit();
  exit(dummy);
}

int main(void)
{
  // ==============================================================
  signal(SIGINT, close_sigint);

//  PLC Slave(1502);

//  mb_slave_init();

  uint64_t ms = t.millis();

  for (;;) {
    if (t.millis() > (ms + 2000)) {
      ms = t.millis();
//      mb_slave_print_reg(0);
//      mb_slave_print_reg(9);
      printf("%d   %d    |\n", Slave.read_raw(1), Slave.read_raw(7));
      fflush(stdout);
    }
    // =======================================================
    // Fill registers with data
    w++;
    //    for (int i = 0; i < 10; i++)
    Slave.write_raw(1, w++);
    Slave.write_raw(7, w++);
    //mb_mapping->tab_registers[i] = w++;
    // =======================================================
    //    printf(".\n");
//    mb_slave_check(1);
    Slave.handle_slave(100);
  }
  return 0;
}
