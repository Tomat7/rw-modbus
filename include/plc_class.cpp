// plc_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// timer & millis() (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include "./plc_class.h"

#include <errno.h>
#include <modbus/modbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include <chrono>
#include <string>

void PLC::logerr(const char* s, ...)
{
  va_list va;
  printf(s, va);
}

PLC::PLC(string _ip, string _name)  // Master only
{
  openlog("MB_master", LOG_NDELAY, LOG_LOCAL1);
  ip_addr = _ip.c_str();
  dev_name = _name.c_str();
  LOGINFO("+ New PLC created: %s %s \n", ip_addr, dev_name);
  closelog();
}

PLC::PLC(int _port, string _name)  // Slave only
{
  openlog("MB_slave", LOG_NDELAY, LOG_LOCAL1);
  ip_addr = "0.0.0.0";  // Slave always listening on ALL addresses!
  tcp_port = _port;
  str_desc = _name;
  str_dev_name = _name;
  dev_name = str_dev_name.c_str();
  is_slave = true;
  LOGINFO("+ New PLC created: %s:%d %s \n", ip_addr, tcp_port, dev_name);
  closelog();
}

PLC::~PLC() { deinit(); }

void PLC::init_master()  // Master only
{
  openlog("MB_master", LOG_NDELAY, LOG_LOCAL1);
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

    R.rvalue = 777;  // TODO: remove for production
    LOGINFO("+ REG init: %-7s %2d %2s [%s] \n", R.ch_name, R.raddr,
            R.str_mode.c_str(), R.fullname.c_str());
  }

  closelog();
}

int PLC::mb_new_master()
{
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

int PLC::mb_connect()  // Master only
{
  if ((mb.errors > 0) || (ctx == nullptr)) {
    rc = mb_new_master();
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

int PLC::read_master()  // Master only
{
  openlog("MB_master", LOG_NDELAY, LOG_LOCAL1);
  rc = read_allregs();
  mb.status = rc;

  for (auto &r : regs) {
    r.rstatus = rc;
    r.rerrors = mb.errors;
  }

  mb.timestamp_ms = millis();
  closelog();

  return rc;
}

int PLC::read_allregs()  // Master only
{
  int nb_regs = reg_max - reg_min + 1;  // WARNING!! May be too much!
  uint16_t* mbregs = new uint16_t[nb_regs];
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

int PLC::write_master()  // Master only
{
  openlog("MB_master", LOG_NDELAY, LOG_LOCAL1);
  for (auto &r : regs)
    rc = write_reg(r);

  closelog();
  return rc;
}

int PLC::write_reg(reg_t &r)
{
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

int PLC::update_master()  // Master only
{
  rc = 0;
  if (millis() - mb.timestamp_ms > mb.interval_ms) {
    rc = write_master();
    rc = read_master();
  }
  return rc;
}

int PLC::set_timeout()
{
  if (ctx == nullptr)
    mb_new_master();

  rc = modbus_set_response_timeout(ctx, 0, mb.timeout_us);
  if (rc == -1) {
    LOGERR("%s %s set timeout failed: %s\n", ip_addr, dev_name,
           modbus_strerror(errno));
  }

  return rc;
}

void PLC::deinit()
{
  if (is_slave) 
    openlog("MB_slave", LOG_NDELAY, LOG_LOCAL1);
  else
    openlog("MB_master", LOG_NDELAY, LOG_LOCAL1);

  modbus_close(ctx);
  modbus_free(ctx);
  LOGINFO("- PLC closed, free and deleted: %s %s. \n", ip_addr, dev_name);
  closelog();
}

uint64_t PLC::millis()
{
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
