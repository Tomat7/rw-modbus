/*
 * Copyright © Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <errno.h>
#include <stdlib.h>

#include <modbus/modbus.h>

int main(void) {
  int s = -1;
  modbus_t *ctx;
  modbus_mapping_t *mb_mapping;

  ctx = modbus_new_tcp(NULL, 502);
  /* modbus_set_debug(ctx, TRUE); */

  mb_mapping = modbus_mapping_new(500, 500, 500, 500);
  if (mb_mapping == NULL) {
    fprintf(stderr, "Failed to allocate the mapping: %s\n",
            modbus_strerror(errno));
    modbus_free(ctx);
    return -1;
  }

  s = modbus_tcp_listen(ctx, 10);
  printf("mb listen - OK\n");

  modbus_tcp_accept(ctx, &s);

  printf("mb accept - OK\n");


  for (;;) {
    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
    int rc;
  
    printf("in the loop...\n");

    rc = modbus_receive(ctx, query);
    if (rc > 0) {
      /* rc is the query size */
      modbus_reply(ctx, query, rc, mb_mapping);
    } else if (rc == -1) {
      /* Connection closed by the client or error */
      break;
    }
  }

  printf("Quit the loop: %s\n", modbus_strerror(errno));

  if (s != -1) {
    close(s);
  }
  modbus_mapping_free(mb_mapping);
  modbus_close(ctx);
  modbus_free(ctx);

  return 0;
}
