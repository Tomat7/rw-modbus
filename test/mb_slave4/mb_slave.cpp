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

static modbus_t* ctx = NULL;
static modbus_mapping_t* mb_mapping;

static int server_socket = -1;
static uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
static int master_socket;
static int rc;
static fd_set refset;
static fd_set rdset;
static int fdmax;  // Maximum file descriptor number
static uint16_t w = 0;

int mb_slave_init(void)
{
  /*
    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
    int master_socket;
    int rc;
    fd_set refset;
    fd_set rdset;
    int fdmax;
  */
  ctx = modbus_new_tcp(NULL, MB_SLAVE_PORT);
  if (ctx == NULL) {
    LOGERR("Unable to allocate libmodbus context\n");
    return -1;
  }

  mb_mapping = modbus_mapping_new(0, 0, MB_SLAVE_REGS_NB, 0);
  if (mb_mapping == NULL) {
    LOGERR("Failed to allocate the mapping: %s\n", modbus_strerror(errno));
    modbus_free(ctx);
    return -1;
  }

  server_socket = modbus_tcp_listen(ctx, MB_SLAVE_CONN_MAX);
  if (server_socket == -1) {
    LOGERR("Unable to listen TCP connection\n");
    modbus_free(ctx);
    return -1;
  }

//  int x = (_modbus)ctx->slave;

  // ==============================================================
  FD_ZERO(&refset);                // Clear the reference set of socket
  FD_SET(server_socket, &refset);  // Add the server socket
  fdmax = server_socket;           // Keep track of the max file descriptor
  LOGINFO("MB Slave ready on port %d.\n",  MB_SLAVE_PORT);
  // ==============================================================

  return 1;
}

int mb_slave_check(int usec /*= 1000000*/)
{
  // for (;;) {
  // =======================================================
  // Fill registers with data
  w++;
  //      for (int i = 0; i < 10; i++)
  mb_mapping->tab_registers[1] = w++;
  mb_mapping->tab_registers[7] = w++;
  // =======================================================

  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = usec;

  //  printf("_0\n");
  rdset = refset;
  if (select(fdmax + 1, &rdset, NULL, NULL, &tv) == -1) {
    LOGERR("Server select() failure.\n");
    perror("Server select() failure.");
    return -1;  // close_sigint(1);
  }

  // Run through the existing connections looking for data to be read

  //  printf("_1\n");
  for (master_socket = 0; master_socket <= fdmax; master_socket++) {
    if (!FD_ISSET(master_socket, &rdset)) {
      //      printf("--");
      continue;
    }
    //    printf("+2\n");

    if (master_socket == server_socket) {  // Client asking a new connection
      socklen_t addrlen;
      struct sockaddr_in clientaddr;
      int newfd;

      // Handle new connections
      addrlen = sizeof(clientaddr);
      memset(&clientaddr, 0, sizeof(clientaddr));
      newfd = accept(server_socket, (struct sockaddr*)&clientaddr, &addrlen);

      if (newfd == -1) {
        LOGERR("Server accept() error\n");
        perror("Server accept() error");
      } else {
        FD_SET(newfd, &refset);
        if (newfd > fdmax)
          fdmax = newfd;  // Keep track of the maximum
        LOGINFO("New connection from %s:%d on socket %d\n",
                inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port, newfd);
      }

    } else {  // Existing connection
      modbus_set_socket(ctx, master_socket);
      rc = modbus_receive(ctx, query);

      if (rc > 0)
        modbus_reply(ctx, query, rc, mb_mapping);
      else if (rc == -1) {  // connection closing or any errors.
        LOGINFO("Connection closed on socket %d\n", master_socket);
        close(master_socket);
        FD_CLR(master_socket, &refset);  // Remove from reference set
        if (master_socket == fdmax)
          fdmax--;
      }  // closing connection
    }    // not new (existing) connection
  }      // for (master_sockets...) handling each socket

  return fdmax;
}

void mb_slave_close()
{
  if (server_socket != -1)
    close(server_socket);
  modbus_free(ctx);
  modbus_mapping_free(mb_mapping);
  return;
}

void mb_slave_print_reg(int addr)
{
  printf(" %d ", mb_mapping->tab_registers[addr]);
  return;
}