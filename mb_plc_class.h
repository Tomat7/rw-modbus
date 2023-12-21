#pragma once

#include "config.h"
#include <modbus/modbus.h>

using namespace std;


struct reg_t {
  int raddr;
  const char *rname;
  const char *rmode;
  uint16_t rvalue;
};


class plc_t {
private:
  int rc = 0;
  modbus_t *ctx = NULL;

public:
  plc_t() {}

  int init(const char *_ip, int _port) {
    rc = 0;
    ctx = modbus_new_tcp(_ip, _port);
    if (ctx == NULL) {
      fprintf(stderr, "Unable to allocate libmodbus context\n");
      rc = -1;
    }

    rc = modbus_connect(ctx);
    if (rc == -1) {
      fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
      // modbus_free(ctx);
    }

    return rc;
  }

  int set_timeout() {
    rc = modbus_set_response_timeout(ctx, 0, err_timeout);
    if (rc == -1) {
      fprintf(stderr, "MB set timeout failed: %s\n", modbus_strerror(errno));
      // modbus_free(mb);
    }
    return rc;
  }

  const char *dev_desc;
  const char *dev_name;
  const char *ip_addr;
  int tcp_port = 0;
  int poll_interval = 0;
  int err_timeout = 0;
  int nb_regs = 0;
  vector<reg_t> regs;

};
