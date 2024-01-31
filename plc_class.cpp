//
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <chrono>
#include <errno.h>
#include <modbus/modbus.h>
#include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#include <syslog.h>

#include "plc_class.h"

#define CAST_MILLIS duration_cast<milliseconds>

using namespace std;

// PLC::PLC() {}

PLC::PLC() {
  openlog("Modbus", LOG_NDELAY, LOG_LOCAL0);
  LOGINFO("+ New PLC created.");
}

PLC::~PLC() { deinit(); }

int PLC::init(const char *_ip, int _port) {
  rc = 0;

  if (_port == 0) {
    _port = tcp_port;
    _ip = ip_addr;
  }

  LOGINFO("%s: try to init \n", _ip);

  if (ctx != NULL) {
    LOGINFO("%s: try to close \n", _ip);
    modbus_close(ctx);
    LOGINFO("%s: try to free \n", _ip);
    modbus_free(ctx);
    ctx = NULL;
  }

  ctx = modbus_new_tcp(_ip, _port);

  if (ctx == NULL) {
    LOGINFO("%s:%d %s CTX allocate error. \n", _ip, _port, dev_name);
    rc = -1;
  } else {
    LOGINFO("%s:%d %s CTX allocate OK. \n", _ip, _port, dev_name);
    rc = modbus_set_response_timeout(ctx, 0, mb_timeout_us);
    if (rc == -1) {
      LOGERR("%s %s set timeout failed: %s\n", ip_addr, dev_name,
             modbus_strerror(errno));
    }
  }

  return rc;
}

int PLC::connect() {
  if (ctx == NULL) {
    rc = init();
    if (rc == -1)
      return rc;
  }

  rc = modbus_connect(ctx);

  if (rc == -1) {
    LOGERR("%s %s connect error: %s\n", ip_addr, dev_name,
           modbus_strerror(errno));
    modbus_free(ctx);
    ctx = NULL;
  }

  return rc;
}

int PLC::read() {
  rc = connect();

  if (rc == -1) {
    mb_errors++;
  } else {
    uint16_t *mbregs = new uint16_t[nb_regs + 1];
    rc = modbus_read_registers(ctx, 0, nb_regs, mbregs);

    if (rc == -1) {
      mb_errors++;
      LOGERR("%s %s read error: %s \n", ip_addr, dev_name,
             modbus_strerror(errno));
    } else {
      mb_errors = 0;
      for (int j = 0; j < nb_regs; ++j)
        regs[j].rvalue = mbregs[regs[j].raddr];  
    }

    delete[] mbregs;
  }
  
  mb_timestamp_ms = millis();
  modbus_close(ctx);
  mb_status = rc;

  for (auto &r : regs)
    r.rstatus = mb_status;

  return rc;
}

int PLC::set_timeout() {
  if (ctx == NULL)
    init();

  rc = modbus_set_response_timeout(ctx, 0, mb_timeout_us);
  if (rc == -1) {
    LOGERR("%s %s set timeout failed: %s\n", ip_addr, dev_name,
           modbus_strerror(errno));
  }

  return rc;
}

void PLC::deinit() {
  if (ctx != NULL) {
    LOGINFO("%s %s close and free. \n", ip_addr, dev_name);
    modbus_close(ctx);
    modbus_free(ctx);
  }
  LOGINFO("- PLC deleted: %s. \n", dev_name);
}

uint64_t PLC::millis() {
#define CAST_MILLIS duration_cast<milliseconds>

  using namespace std::chrono;
  uint64_t t, old = mb_timestamp_ms;
  t = CAST_MILLIS(system_clock::now().time_since_epoch()).count();
  printf("%s __dT: %ld  errors: %d  rc: %d\n", dev_name, t - old, mb_errors, rc);

  return t;
}
