#pragma once

//#include "config.h"
#include <modbus/modbus.h>
#include <vector>

using namespace std;

struct reg_t {
  int raddr;
  const char *rname;
  const char *rmode;
  uint16_t rvalue;
};

class PLC {

public:
  PLC(); // {}

  int init(const char *_ip, int _port);
  int set_timeout();

  const char *dev_title;
  const char *dev_desc;
  const char *dev_name;
  const char *ip_addr;
  int tcp_port = 0;
  uint32_t err_timeout = 0;
  int poll_interval = 0;
  int nb_regs = 0;
  vector<reg_t> regs;

private:
  int rc = 0;
  modbus_t *ctx = NULL;
};
