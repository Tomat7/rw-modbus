
#include <errno.h>
#include <modbus/modbus.h>
#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <syslog.h>

#include "plc_class.h"

using namespace std;

PLC::PLC() {}

int PLC::init(const char *_ip, int _port) {

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

int PLC::set_timeout() {

  rc = modbus_set_response_timeout(ctx, 0, err_timeout);
  if (rc == -1) {
    fprintf(stderr, "MB set timeout failed: %s\n", modbus_strerror(errno));
    // modbus_free(mb);
  }

  return rc;
}
