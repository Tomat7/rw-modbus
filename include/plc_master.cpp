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

#include <string>

#include "./logger.h"

PLC_c::PLC_c(string _ip, string _name) // Master only
{
  ip_addr = _ip.c_str();
  dev_name = _name.c_str();
  LOGW("+ New PLC created: %s %s", ip_addr, dev_name);
}

// Destructor in plc_common.cpp

void PLC_c::init_master() // Master only
{
  ip_addr = str_ip_addr.c_str();
  dev_name = str_dev_name.c_str();
  LOGN("+ PLC init: %s %-7s %-7s %-20s", ip_addr, dev_name,
       str_title.c_str(), str_desc.c_str());

  for (auto &[n, R] : regs) {
    auto &rd = R.data;
    R.fullname = str_dev_name + "." + R.str_name;
    R.ch_name = R.str_name.c_str();

    rd.rmode = (R.str_mode == "rw") ? 1 : 0;
    rd.rtype = (R.str_type == "f") ? 1 : 0;

    if (R.raddr < reg_min)
      reg_min = R.raddr;

    if (R.raddr > reg_max)
      reg_max = R.raddr;

    rd.rvalue = 777; // TODO: remove for production
    LOGN("+ REG init: %-7s %2d %2s [%s]", R.ch_name, R.raddr,
         R.str_mode.c_str(), R.fullname.c_str());
  }
}

int PLC_c::read_master() // Master only
{
  rc = 0;
  att = 0;

  while (att < attempts && rc <= 0) {
    att++;
    rc = mb_connect();
    if (rc == 0)
      rc = read_allregs();
    else
      mb.errors++;
  }


  mb.status = rc;
  mb.timestamp_try_ms = millis();
  if (rc > 0)
    mb.timestamp_ok_ms = millis();

  for (auto &[n, R] : regs) {
    auto &rd = R.data;
    rd.rstatus = rc;
    rd.rerrors = mb.errors;
  }



  return rc;
}

int PLC_c::mb_connect() // Master only
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
        LOGE("%s %s connect error: %s ", ip_addr, dev_name,
             modbus_strerror(errno));
    }
  }
  modbus_flush(ctx);

  return rc;
}

int PLC_c::read_allregs() // Master only
{
  int nb_regs = reg_max - reg_min + 1; // WARNING!! May be too much!
  uint16_t* mbregs = new uint16_t[nb_regs];

  rc = modbus_read_registers(ctx, reg_min, nb_regs, mbregs);

  if (rc == -1) {
    mb.errors++;
    mb.errors_rd++;
    if (att >= attempts)
      LOGE("%s %s read error: %s ", ip_addr, dev_name,
           modbus_strerror(errno));
  } else if (rc != nb_regs) {
    mb.errors++;
    mb.errors_rd++;
    rc = -2;
    if (att >= attempts)
      LOGE("%s %s qty: expect %d, got %d", ip_addr, dev_name,
           nb_regs, rc);
  } else {
    mb.errors = 0;
    for (auto &[n, R] : regs)
      R.data.rvalue = mbregs[R.raddr - reg_min];
  }

  delete[] mbregs;

  return rc;
}

int PLC_c::write_master() // Master only
{
  for (auto &[n, R] : regs) {
    auto &rd = R.data;
    if (rd.rmode && rd.rupdate) {
      rc = 0;
      att = 0;
      while (att < attempts && rc <= 0) {
        att++;
        rc = mb_connect();
        if (rc == 0)
          rc = write_reg(R);
        else
          mb.errors++;
      }
    }
  }
  return rc;
}

int PLC_c::write_reg(reg_t &R)
{
  auto &rd = R.data;
  rc = modbus_write_register(ctx, R.raddr, rd.rvalue);

  if (rc == -1) {
    mb.errors++;
    mb.errors_wr++;
    if (att >= attempts)
      LOGE("%s %s write reg: %s error: %s", ip_addr, dev_name,
           R.ch_name, modbus_strerror(errno));
  } else {
    mb.errors = 0;
    rd.rupdate = 0;
  }

  rd.rstatus = rc;
  rd.rerrors = mb.errors;

  return rc;
}

int PLC_c::update_master() // Master only
{
  rc = 0;
  uint64_t interval_ms = mb.interval_ms;
  if (mb.errors > 2)
    interval_ms = mb.interval_ms * 3;

  if (millis() - mb.timestamp_try_ms > interval_ms) {
    rc = write_master();
    // LOGD("Write_master finished... %s", dev_name);
    rc = read_master();
    // LOGD("Read_master finished... %s", dev_name);
  }
  return rc;
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
