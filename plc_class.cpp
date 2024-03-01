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
#include <string.h>

#include <chrono>

#include "./plc_class.h"

PLC::PLC() {
  openlog("Modbus", LOG_NDELAY, LOG_LOCAL1);
  LOGINFO("+ New PLC created.\n");
}

PLC::~PLC() { deinit(); }

int PLC::init() {
  rc = 0;

  LOGINFO("%s: try to close/free \n", ip_addr);
  modbus_close(ctx);
  modbus_free(ctx);
  ctx = nullptr;

  ctx = modbus_new_tcp(ip_addr, tcp_port);
  if (ctx == nullptr) {
    LOGINFO("%s:%d %s CTX allocate error. \n", ip_addr, tcp_port, dev_name);
    rc = -1;
  } else {
    LOGINFO("%s:%d %s CTX allocate OK. \n", ip_addr, tcp_port, dev_name);
  }

  return rc;
}

int PLC::connect() {
  if ((rc < 0) || (ctx == nullptr)) {
    rc = init();
    rc = modbus_connect(ctx);

    if (rc == -1) {
      LOGERR("%s %s connect error: %s\n", ip_addr, dev_name,
             modbus_strerror(errno));
      modbus_free(ctx);
      ctx = nullptr;
      mb.errors++;
      mb.errors_cn++;
    }
  }
  modbus_flush(ctx);

  return rc;
}

int PLC::read() {

  rc = read_allregs();
  mb.status = rc;

  for (auto &r : regs) {
    r.rstatus = rc;
    r.rerrors = mb.errors;
  }

  mb.timestamp_ms = millis();

  return rc;
}

int PLC::read_allregs() {

  int nb_regs = reg_max - reg_min + 1; // WARNING!! May be too much!
  uint16_t *mbregs = new uint16_t[nb_regs];
  rc = 0;
  
  for (int i = 0; i < attempts && rc <= 0; i++) {
    connect();
    rc = modbus_read_registers(ctx, reg_min, nb_regs, mbregs);
  }

  if (rc == -1) {
    mb.errors++;
    mb.errors_rd++;
    LOGERR("%s %s read error: %s \n", ip_addr, dev_name,
           modbus_strerror(errno));
  } else if (rc != nb_regs) {
    mb.errors++;
    mb.errors_rd++;
    rc = -2;
    LOGERR("%s %s qty regs mismatch: expect %d, got %d\n", ip_addr, dev_name,
           nb_regs, rc);
  } else {
    mb.errors = 0;
    for (auto &r : regs)
      r.rvalue = mbregs[r.raddr - reg_min];
  }

  delete[] mbregs;
  return rc;
}

int PLC::write() {

  for (auto &r : regs)
      rc = write_reg(r);

  return rc;
}

int PLC::write_reg(reg_t &r) {

  if (r.rmode && r.rupdate) {
    rc = 0;
    for (int i = 0; i < attempts && rc <= 0; i++) {
      connect();
      rc = modbus_write_register(ctx, r.raddr, r.rvalue);
    }

    if (rc == -1) {
      mb.errors++;
      mb.errors_wr++;
      LOGERR("%s %s write reg: %s error: %s \n", ip_addr, dev_name, r.ch_name,
             modbus_strerror(errno));
    } else {
      mb.errors = 0;
      r.rupdate = 0;
    }

    r.rstatus = rc;
    r.rerrors = mb.errors;
  }

  return rc;
}

int PLC::update() {

  rc = 0;
  if (millis() - mb.timestamp_ms > mb.interval_ms) {
    rc = write();
    rc = read();
  }
  return rc;
}

int PLC::set_timeout() {

  if (ctx == nullptr)
    init();

  rc = modbus_set_response_timeout(ctx, 0, mb.timeout_us);
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
  uint64_t t;
  t = CAST_MILLIS(std::chrono::system_clock::now().time_since_epoch()).count();
  return t;
}

int PLC::get_rc() { return rc; }
