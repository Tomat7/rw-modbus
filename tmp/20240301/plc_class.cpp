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

int PLC::mb_new_master() {
  rc = 0;

  //  if (ctx != nullptr) {
  LOGINFO("%s: try to close/free \n", ip_addr);
  modbus_close(ctx);
  modbus_free(ctx);
  ctx = nullptr;
  // } else {
  //      LOGINFO("%s: try to close \n", _ip);
  //      modbus_close(ctx);
  //  }

  ctx = modbus_new_tcp(ip_addr, tcp_port);
  if (ctx == nullptr) {
    LOGINFO("%s:%d %s CTX allocate error. \n", ip_addr, tcp_port, dev_name);
    rc = -1;
  } else {
    LOGINFO("%s:%d %s CTX allocate OK. \n", ip_addr, tcp_port, dev_name);
  }

  return rc;
}

int PLC::mb_connect() {
  if ((rc < 0) || (ctx == nullptr)) {
    rc = mb_new_master();
    //    if (rc == -1)
    //      return rc;
    rc = modbus_connect(ctx);
    if (rc == -1) {
      LOGERR("%s %s connect error: %s\n", ip_addr, dev_name,
             modbus_strerror(errno));
      modbus_free(ctx);
      ctx = nullptr;
    }
  }
  modbus_flush(ctx);

  return rc;
}

int PLC::read_master() {

  rc = 0;
  for (int i = 0; i < try_qty && rc <= 0; i++)
    rc = read_allregs();

  mb.status = rc;

  for (auto &r : regs) {
    r.rstatus = rc;
    r.rerrors = mb.errors;
  }

  //  uint64_t old = mb.timestamp_ms;
  mb.timestamp_ms = millis();

  /*
    printf("%s _dT: %ld  err: %d cn: %d rd: %d wr: %d rc: %d\n", dev_name,
           mb.timestamp_ms - old, mb.errors, mb.errors_cn, mb.errors_rd,
           mb.errors_wr, rc);
  */

  /*
    modbus_flush(ctx);
    modbus_close(ctx);
    modbus_free(ctx);
    ctx = nullptr;
  */

  return rc;
}

int PLC::read_allregs() {

  int nb_regs = reg_max - reg_min + 1; // WARNING!! May be too much!
  uint16_t *mbregs = new uint16_t[nb_regs];

  rc = mb_connect();
  if (rc == -1) {
    mb.errors++;
    mb.errors_cn++;
    return rc;
  }

  rc = modbus_read_registers(ctx, reg_min, nb_regs, mbregs);

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

int PLC::write_master() {
  /*
    rc = connect();
    if (rc == -1) {
      mb.errors++;
      mb.errors_cn++;
    } else {
      for (auto &r : regs)
        rc = write_reg(r);
    }
  */

  for (auto &r : regs) {
    rc = 0;
    for (int i = 0; i < try_qty && rc <= 0; i++)
      rc = write_reg(r);
  }

  //  mb.status = rc;

  /*
    modbus_flush(ctx);
    modbus_close(ctx);
    modbus_free(ctx);
    ctx = nullptr;
  */
  return rc;
}

int PLC::write_reg(reg_t &r) {

  if (r.rmode && r.rupdate) {
    mb_connect();
    rc = modbus_write_register(ctx, r.raddr, r.rvalue);

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

int PLC::update_master() {

  rc = 0;
  if (millis() - mb.timestamp_ms > mb.interval_ms) {
    rc = write_master();
    rc = read_master();
  }
  return rc;
}

int PLC::set_timeout() {

  if (ctx == nullptr)
    mb_new_master();

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
