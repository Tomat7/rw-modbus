// plc_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <errno.h>
#include <modbus/modbus.h>
#include <stdio.h>
#include <syslog.h>
// #include <stdlib.h>
// #include <string.h>

#include <chrono>

#include "./plc_class.h"

PLC::PLC() {
  openlog("Modbus", LOG_NDELAY, LOG_LOCAL1);
  LOGINFO("+ New PLC created.\n");
}

PLC::~PLC() { deinit(); }

int PLC::init(const char *_ip, int _port) {
  rc = 0;

  if (_port == 0) {
    _port = tcp_port;
    _ip = ip_addr;
  }

  LOGINFO("%s: try to init \n", _ip);

  if (ctx != nullptr) {
    LOGINFO("%s: try to close \n", _ip);
    modbus_close(ctx);
    LOGINFO("%s: try to free \n", _ip);
    modbus_free(ctx);
    ctx = nullptr;
  }

  ctx = modbus_new_tcp(_ip, _port);

  if (ctx == nullptr) {
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
  if (ctx == nullptr) {
    rc = init();
    if (rc == -1)
      return rc;
  }

  rc = modbus_connect(ctx);
  if (rc == -1) {
    LOGERR("%s %s connect error: %s\n", ip_addr, dev_name,
           modbus_strerror(errno));
    modbus_free(ctx);
    ctx = nullptr;
  }

  return rc;
}

int PLC::read() {
  rc = connect();
  if (rc == -1)
    mb_errors++;
  else
    rc = read_allregs();

  mb_timestamp_ms = millis();
  modbus_close(ctx);
  mb_status = rc;

  for (auto &r : regs)
    r.rstatus = mb_status;

  return rc;
}

int PLC::read_allregs() {
  int nb_regs = reg_max - reg_min + 1; // WARNING!! May be too much!
  uint16_t *mbregs = new uint16_t[nb_regs];
  rc = modbus_read_registers(ctx, reg_min, nb_regs, mbregs);

  if (rc == -1) {
    mb_errors++;
    LOGERR("%s %s read error: %s \n", ip_addr, dev_name,
           modbus_strerror(errno));
  } else if (rc != nb_regs) {
    mb_errors++;
    rc = -2;
    LOGERR("%s %s qty regs mismatch: expect %d, got %d\n", ip_addr, dev_name,
           nb_regs, rc);
  } else {
    mb_errors = 0;
    for (auto &r : regs) // (int j = 0; j < reg_qty; ++j)
      r.rvalue = mbregs[r.raddr - reg_min];
    // regs[j].rvalue = mbregs[regs[j].raddr - reg_min];
  }

  delete[] mbregs;
  return rc;
}

int PLC::set_timeout() {
  if (ctx == nullptr)
    init();

  rc = modbus_set_response_timeout(ctx, 0, mb_timeout_us);
  if (rc == -1) {
    LOGERR("%s %s set timeout failed: %s\n", ip_addr, dev_name,
           modbus_strerror(errno));
  }

  return rc;
}

void PLC::deinit() {
  if (ctx != nullptr) {
    LOGINFO("%s %s close and free. \n", ip_addr, dev_name);
    modbus_close(ctx);
    modbus_free(ctx);
  }

  LOGINFO("- PLC deleted: %s. \n", dev_name);
}

uint64_t PLC::millis() {
#define CAST_MILLIS std::chrono::duration_cast<std::chrono::milliseconds>

  // using namespace std::chrono;
  uint64_t t, old = mb_timestamp_ms;
  t = CAST_MILLIS(std::chrono::system_clock::now().time_since_epoch()).count();
  printf("%s _dT: %ld  errors: %d  rc: %d\n", dev_name, t - old, mb_errors, rc);

  return t;
}
