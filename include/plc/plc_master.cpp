// plc_class.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//

#include <errno.h>
#include <math.h>
#include <modbus/modbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include <string>

#include "include/logger.h"
#include "plc_class.h"
/*
  PLC_c::PLC_c(string _ip, string _name)  // Master only
  {
  // lock_init();
  lock_mux = new mutex;
  ip_addr = _ip.c_str();
  dev_name = _name.c_str();
  LOGI("+ New PLC created: %s %s", ip_addr, dev_name);
  }
*/

PLC_c::PLC_c(string _devname, string _ip, string _folder, string _desc,
             int _port, int _att, int _ms, int _us)
{
  // lock_init();
  lock_mux = new mutex;

  str_dev_name = _devname;
  dev_name = str_dev_name.c_str();

  str_ip_addr = _ip;
  ip_addr = str_ip_addr.c_str();

  str_dev_folder = _folder;
  str_dev_desc = _desc;
  tcp_port = _port;
  attempts = _att;
  mb.polling_ms = _ms;
  mb.timeout_us = _us;

  if (tcp_port == 0 || attempts == 0 || mb.polling_ms == 0)
    Enabled = false;
  else
    Enabled = true;

  LOGN("+ New PLC created: %s:%i %-7s %-7s %-20s", ip_addr, tcp_port, dev_name,
       str_dev_folder.c_str(), str_dev_desc.c_str());
  //  LOGI("+ New PLC created: %s:%i %s", ip_addr, tcp_port, dev_name);
  if (!Enabled)
    LOGA("- PLC will be ignored: %s:%i %s", ip_addr, tcp_port, dev_name);
}

// Destructor in plc_common.cpp

int PLC_c::mb_connect()  // Master only
{
  if ((mb.errors > 0) || (ctx == nullptr)) {
    rc = mb_ctx();
    rc = modbus_connect(ctx);

    if (rc == -1) {
      modbus_free(ctx);
      ctx = nullptr;
      mb.errors++;
      mb.errors_cn++;
      if (att >= attempts)
        LOGE("%s %s connect: %s ", ip_addr, dev_name,
             modbus_strerror(errno));
    }
  }
  modbus_flush(ctx);

  return rc;
}

int PLC_c::read_master()  // Master only. Read directly from PLC.
{
  //  LOCK_GUARD(network_mux);
  rc = 0;
  att = 0;

  while (att < attempts && rc <= 0) {
    lock_mux->lock();
    att++;
    rc = mb_connect();
    if (rc == 0)
      rc = read_allregs();
    else
      mb.errors++;
    lock_mux->unlock();
  }

  mb.status = rc;
  // mb.timestamp_try_ms = millis();
  if (rc > 0)
    mb.timestamp_ok_ms = millis();

  for (auto &[a, R] : regs) {
    auto &rd = R.data;
    // rd.rstatus = rc;
    rd.rerrors = mb.errors;
  }

  return rc;
}

int PLC_c::read_allregs()  // Master only. Read (raw) directly from PLC.
{
  int nb_regs = reg_max - reg_min + 1;  // WARNING!! May be too much!
  uint16_t* mbregs = new uint16_t[nb_regs];

  rc = modbus_read_registers(ctx, reg_min, nb_regs, mbregs);

  if (rc == -1) {
    mb.errors++;
    mb.errors_rd++;
    if (att >= attempts)
      LOGE("%s %s read: %s ", ip_addr, dev_name, modbus_strerror(errno));
  } else if (rc != nb_regs) {
    mb.errors++;
    mb.errors_rd++;
    rc = -2;
    if (att >= attempts)
      LOGE("%s %s qty: expect %d, got %d", ip_addr, dev_name, nb_regs, rc);
  } else {
    mb.errors = 0;
    for (auto &[a, R] : regs)
      if (!R.data.rmode || !R.data.rupdate)  // !! Do not update !!
        R.data.rvalue = mbregs[R.raddr - reg_min];
  }

  delete[] mbregs;

  return rc;
}

int PLC_c::write_master()  // Master only. Write all regs directly to PLC.
{
  //  LOCK_GUARD(network_mux);
  rc_write = 0;
  for (auto &[a, R] : regs) {
    auto &rd = R.data;
    if (rd.rmode && rd.rupdate) {
      rc = 0;
      att = 0;

      while (att < attempts && rc <= 0) {
        lock_mux->lock();
        att++;
        rc = mb_connect();
        if (rc == 0)
          rc = write_reg(R);
        else
          mb.errors++;
        lock_mux->unlock();
      }

      if (rc == -1) {
        rc_write--;
        LOGW("%s %s write reg %s: %s", ip_addr, dev_name, R.ch_name,
             modbus_strerror(errno));
      }
    }
  }

  if (rc_write < 0)
    LOGE("%s %s write regs errors: %i", ip_addr, dev_name, rc_write * -1);

  return rc_write;
}

int PLC_c::write_reg(mbreg_t &R)  // Master only. Write (raw) reg directly to PLC.
{
  auto &rd = R.data;
  rc = modbus_write_register(ctx, R.raddr, rd.rvalue);

  if (rc == -1) {
    mb.errors++;
    mb.errors_wr++;
  } else {
    mb.errors = 0;
    rd.rupdate = 0;
  }

  //rd.rstatus = rc;
  rd.rerrors = mb.errors;

  return rc;
}

// int PLC_c::refresh_master() { update_master(); } // Master only.

int PLC_c::update_master()  // Master only.
{
  int ret = 0;
  if (tcp_port == 0)
    return ret;

  uint64_t interval_ms = mb.polling_ms;

  if (mb.errors > 2)
    interval_ms = mb.polling_ms * 3;

  if (millis() - mb.timestamp_try_ms > interval_ms) {
    mb.timestamp_try_ms = millis();
    rc = 0;
    /*     rc = write_master();
        rc = read_master(); */

    rc = read_master();
    rc_read = rc;

    if (rc > 0)
      rc = write_master();

    rc_write = rc;
    ret = rc;
  }
  return ret;
}

int PLC_c::set_timeout()
{
  if (ctx == nullptr)
    mb_ctx();

  rc = modbus_set_response_timeout(ctx, 0, mb.timeout_us);
  if (rc == -1) {
    LOGE("%s %s set timeout failed: %s", ip_addr, dev_name,
         modbus_strerror(errno));
  }

  return rc;
}

// eof
