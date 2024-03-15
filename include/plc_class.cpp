// plc_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <errno.h>
#include <modbus/modbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include <chrono>
#include <string>

#include "./plc_class.h"

void PLC::logerr(const char *s, ...) {
  va_list va;
  printf(s, va);
}

PLC::PLC() {
  openlog("Modbus", LOG_NDELAY, LOG_LOCAL1);
  ip_addr = "x.x.x.x";
  dev_name = "tmp";
  LOGINFO("+ New PLC created: %s %s \n", ip_addr, dev_name);
  //  cout << ip_addr << endl;
}

PLC::~PLC() { deinit(); }

void PLC::init() {

  //  cout << ip_addr << endl;
  ip_addr = str_ip_addr.c_str();
  dev_name = str_dev_name.c_str();
  LOGINFO("+ PLC init: %s %-7s %-7s %-20s \n", ip_addr, dev_name,
          str_title.c_str(), str_desc.c_str());

  for (auto &R : regs) {
    R.fullname = str_dev_name + "." + R.str_name;
    R.ch_name = R.str_name.c_str();

    R.rmode = (R.str_mode == "rw") ? 1 : 0;
    R.rtype = (R.str_type == "f") ? 1 : 0;

    if (R.raddr < reg_min)
      reg_min = R.raddr;

    if (R.raddr > reg_max)
      reg_max = R.raddr;

    R.rvalue = 777; // TODO: remove for production
    LOGINFO("+ REG init: %-7s %2d %2s [%s] \n", R.ch_name, R.raddr,
            R.str_mode.c_str(), R.fullname.c_str());
  }
}

int PLC::mb_new() {
  rc = 0;

  LOGINFO("%s:%d %s try to close/free.\n", ip_addr, tcp_port, dev_name);
  modbus_close(ctx);
  modbus_free(ctx);
  ctx = nullptr;

  ctx = modbus_new_tcp(ip_addr, tcp_port);
  if (ctx == nullptr) {
    LOGINFO("%s:%d %s CTX allocate error.\n", ip_addr, tcp_port, dev_name);
    //  init();
    rc = -1;
  } else
    LOGINFO("%s:%d %s CTX allocate OK.\n", ip_addr, tcp_port, dev_name);

  return rc;
}

int PLC::mb_connect() {

  if ((mb.errors > 0) || (ctx == nullptr)) {
    rc = mb_new();
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
    mb_connect();
    rc = modbus_read_registers(ctx, reg_min, nb_regs, mbregs);
    mb.errors++;
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
    LOGERR("%s %s qty: expect %d, got %d\n", ip_addr, dev_name, nb_regs, rc);
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
      mb_connect();
      rc = modbus_write_register(ctx, r.raddr, r.rvalue);
      mb.errors++;
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
    mb_new();

  rc = modbus_set_response_timeout(ctx, 0, mb.timeout_us);
  if (rc == -1) {
    LOGERR("%s %s set timeout failed: %s\n", ip_addr, dev_name,
           modbus_strerror(errno));
  }

  return rc;
}

void PLC::deinit() {
  //  if (ctx != nullptr) {
  //  LOGINFO("%s %s close and free. \n", ip_addr, dev_name);
  modbus_close(ctx);
  modbus_free(ctx);
  //  }
  LOGINFO("- PLC close, free and deleted: %s %s. \n", ip_addr, dev_name);
}

uint64_t PLC::millis() {
#define CAST_MILLIS std::chrono::duration_cast<std::chrono::milliseconds>
  uint64_t t;
  t = CAST_MILLIS(std::chrono::system_clock::now().time_since_epoch()).count();
  return t;
}

/*
    void PLC::new_str(const char *ch) {
    char *new_ch = nullptr;
    const char *save_ch = ch;
    string s = (string)ch;
    int len = strlen(ch) + 1;
    new_ch = (char *)malloc(len);
    //  strcpy(new_ch, ch);
    //  new_ch[0] = (char)"_";

    for (int i = 0; i <= len; i++)
    new_ch[i] = ch[i];

    ch = new_ch;
    //  LOGINFO("String copied: %s %s \n",  ch, new_ch);
    //  ch = s.c_str();
    LOGINFO("String copied: %s %s \n", ch, new_ch);

    if (save_ch == ch)
    LOGINFO("save_ch == ch! \n");
    if (save_ch == new_ch)
    LOGINFO("save_ch == new_ch! \n");
    if (ch == new_ch)
    LOGINFO("ch == new_ch! \n");

    return;
    }
*/

int PLC::get_rc() { return rc; }
