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

#include "config.h"
#include "libs.h"

#include "include/plc/plc_class.h"

//int mb_slave_init();
//int mb_slave_check(int usec = 1000000);
//void mb_slave_close();
//void mb_slave_print_reg(int /*reg_address*/);

Timer t;
PLC_c Slave(1502, 9);
uint16_t w;

/*
union value_u {
  int16_t i16;
  int32_t i32;
  int64_t i64;
  uint16_t ui16;
  uint32_t ui32;
  uint64_t ui64;
  int64_t dt;
  float fl;
  double dbl;
  uint16_t fl2u[2];
  uint16_t dbl2u[4];
  uint8_t byte2u[8];
}
*/
 value_u val;

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
  uint64_t msStart = t.millis();
  uint16_t secPassed = 0; 
  float fl = (float)1.234;
  double dbl = 123.4567;

  for (;;) {
    if (t.millis() > (ms + 2000)) {
      ms = t.millis();
//      mb_slave_print_reg(0);
//      mb_slave_print_reg(9);
      fl *= (float)1.001;
      dbl *= 1.01;
      printf("millis: %d,  %d   %f  %f  |\n", secPassed, Slave.read_raw(0), fl, dbl);
      fflush(stdout);
    }
    // =======================================================
    // Fill registers with data
    w++;
    //    for (int i = 0; i < 10; i++)
    Slave.write_raw(1, w++);
    
    secPassed = (uint16_t)((t.millis() - msStart)/1000);
    Slave.write_raw(0, secPassed);

    //
    val.fl = fl;
    Slave.write_raw(2, val.fl2u[1]);
    Slave.write_raw(3, val.fl2u[0]);

    val.dbl = dbl;
    Slave.write_raw(4, val.dbl2u[3]);
    Slave.write_raw(5, val.dbl2u[2]);
    Slave.write_raw(6, val.dbl2u[1]);
    Slave.write_raw(7, val.dbl2u[0]);

    //mb_mapping->tab_registers[i] = w++;
    // =======================================================
    //    printf(".\n");
//    mb_slave_check(1);
    Slave.handle_slave(100);
  }
  return 0;
}
