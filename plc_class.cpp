
#include <errno.h>
#include <modbus/modbus.h>
#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <syslog.h>

#include "plc_class.h"

using namespace std;

//PLC::PLC() {}

int PLC::init(const char *_ip, int _port) {

  if (_port == 0) {
    _port = tcp_port;
    _ip = ip_addr;
  }

  modbus_close(ctx);
  modbus_free(ctx);
  ctx = NULL;

  ctx = modbus_new_tcp(_ip, _port);
  if (ctx == NULL) {
    fprintf(stderr, "MB: error allocate ctx for %s:%d\n", _ip, _port);
    rc = -1;
  }

  return rc;
}

int PLC::connect() {

  if (ctx == NULL) {
    init();
    if (rc == -1) return rc;
    printf("MB: new CTX - ok.\n");
  }

  rc = modbus_connect(ctx);
  if (rc == -1) {
    fprintf(stderr, "MB: connect err %s:%d: %s\n", ip_addr, tcp_port,
            modbus_strerror(errno));
    modbus_free(ctx);
  }

  return rc;
}


int PLC::read()
{

  connect();
  if (rc == -1) return rc;
  uint16_t *mbregs = new uint16_t[nb_regs+1];

  rc = modbus_read_registers(ctx, 0, nb_regs, mbregs);

  if (rc == -1) {
    fprintf(stderr, "MB: read error: %s \n", modbus_strerror(errno));
    return rc;
  }

  for (int j = 0; j < nb_regs; ++j)
    regs[j].rvalue = mbregs[regs[j].raddr];

  modbus_close(ctx);
  delete[] mbregs;

  return 0;
}


int PLC::set_timeout() {

  if (ctx == NULL)
    init();

  rc = modbus_set_response_timeout(ctx, 0, err_timeout);
  if (rc == -1) 
    fprintf(stderr, "MB: set timeout failed: %s\n", modbus_strerror(errno));

  return rc;
}


void PLC::deinit() {
  modbus_close(ctx);
//  modbus_free(ctx);
}
