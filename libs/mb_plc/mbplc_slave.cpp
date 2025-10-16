// plc_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//

#include <arpa/inet.h>
#include <errno.h>
#include <modbus/modbus.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <syslog.h>
#include <unistd.h>

#include <string>

#include "include/logger.h"
#include "mbplc_class.h"

// Destructor in plc_common.cpp

ModbusPLC_c::ModbusPLC_c(int _port, int _m, string _name)  // Slave only
{
  // lock_init();
  lock_mux = new mutex;
  str_ip_addr = "0.0.0.0";  // Slave always listening on ALL addresses!
  tcp_port = _port;
  reg_max = _m;
  str_dev_desc = _name;
  str_dev_folder = _name;
  str_dev_name = _name;
  dev_name = str_dev_name.c_str();
  is_slave = true;
  LOGN("+ New Slave PLC created: %s:%d %s", str_ip_addr.c_str(), tcp_port,
       dev_name);
}

int ModbusPLC_c::renew_mapping()  // Slave only
{
  rc = 0;
  if (mbm != nullptr) {
    modbus_mapping_free(mbm);
    mbm = nullptr;
  }

  mbm = modbus_mapping_new(0, 0, reg_max + 1, 0);
  if (mbm == nullptr) {
    modbus_free(ctx);
    ctx = nullptr;
    mb.errors++;
    mb.errors_cn++;
    rc = -1;
    if (att >= attempts)
      LOGE("PLC_slave::%s: failed to allocate: %s", __func__, modbus_strerror(errno));
  }

  return rc;
}

int ModbusPLC_c::renew_listen()  // Slave only
{
  rc = 0;
  if (server_socket != -1)
    close(server_socket);

  server_socket = modbus_tcp_listen(ctx, MB_SLAVE_CONN_MAX);
  if (server_socket == -1) {
    modbus_free(ctx);
    ctx = nullptr;
    mb.errors++;
    mb.errors_rd++;
    rc = -1;
    if (att >= attempts)
      LOGE("PLC_slave::%s: unable on port: %d", __func__, tcp_port);
    return rc;
  }

  modbus_flush(ctx);
  FD_ZERO(&refset);                // Clear the reference set of socket
  FD_SET(server_socket, &refset);  // Add the server socket
  fdmax = server_socket;           // Keep track of the max file descriptor
  LOGN("MB Slave listen on port %d.", tcp_port);
  return rc;
}

int ModbusPLC_c::write_raw(int r, uint16_t val)
{
  if (r > reg_max)
    return -1;
  rc = check_slave();
  if (rc != -1)
    mbm->tab_registers[r] = val;

  return rc;
}

uint16_t ModbusPLC_c::read_raw(int addr)
{
  if (addr > reg_max)
    return -1;

  uint16_t val = 0;
  rc = check_slave();
  if (rc != -1)
    val = mbm->tab_registers[addr];

  return val;
}

int ModbusPLC_c::check_slave()
{
  rc = 0;
  if ((mb.errors > 0) || (ctx == nullptr)) {
    rc = mb_ctx();
    if (rc != -1)
      rc = renew_mapping();
    if (rc != -1)
      rc = renew_listen();
  }
  return rc;
}

int ModbusPLC_c::handle_slave(int usec)
{
  //  LOCK_GUARD(network_mux);
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = usec;

  // lock_now();
  lock_mux->lock();

  rc = check_slave();
  if (rc == -1)
    return rc;
  else
    mb.errors = 0;

  rdset = refset;
  if (select(fdmax + 1, &rdset, NULL, NULL, &tv) == -1) {
    mb.errors++;
    rc = -1;
    LOGE("PLC_slave::%s: server select() failure", __func__);
    return rc;
  }

  // Run through the existing connections looking for data to be read
  for (master_socket = 0; master_socket <= fdmax; master_socket++) {
    if (!FD_ISSET(master_socket, &rdset))
      continue;

    if (master_socket == server_socket)  // Client asking a new connection
      new_client();
    else  // Existing connection
      work_client();
  }

  // unlock_now();
  lock_mux->unlock();

  return fdmax;
}

void ModbusPLC_c::new_client()  // Handle new connections
{
  socklen_t addrlen;
  struct sockaddr_in clientaddr;
  int newfd;

  addrlen = sizeof(clientaddr);
  memset(&clientaddr, 0, sizeof(clientaddr));
  newfd = accept(server_socket, (struct sockaddr*)&clientaddr, &addrlen);

  if (newfd == -1) {
    mb.errors_wr++;
    LOGE("PLC_slave::%s: server accept() error", __func__);
  } else {
    FD_SET(newfd, &refset);
    if (newfd > fdmax)
      fdmax = newfd;  // Keep track of the maximum
    LOGW("PLC_slave::%s: connection from %s:%d on socket %d", __func__,
         inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port, newfd);
  }

  return;
}

void ModbusPLC_c::work_client()
{
  modbus_set_socket(ctx, master_socket);
  rc = modbus_receive(ctx, query);

  if (rc > 0)
    modbus_reply(ctx, query, rc, mbm);
  else if (rc == -1) {  // connection closing or any errors.
    close(master_socket);
    FD_CLR(master_socket, &refset);  // Remove from reference set
    if (master_socket == fdmax)
      fdmax--;
    LOGW("PLC_slave::%s: connection closed on socket %d", __func__, master_socket);
  }

  return;
}

// eof
