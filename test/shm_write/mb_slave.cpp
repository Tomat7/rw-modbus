// mb_func.cpp ------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)

#include <vector>

#include "./config.h"
#include "./libs.h"
//#include "./timer.h" // Timer t; - already initialised h
// using namespace std;
// using namespace libconfig;

// Timer tt;

int s = -1;
modbus_t* ctx;
modbus_mapping_t* mb_mapping;

int mb_slave_init()
{
  cout << endl << "===== mb_slave_init =====" << endl;
  //  int ret = 0;

  LOGINFO("slave init started\n");

  ctx = modbus_new_tcp(NULL, 502);
  LOGINFO("new CTX done\n");
  modbus_set_debug(ctx, TRUE);
  LOGINFO("Debug ON\n");
  mb_mapping = modbus_mapping_new(0, 0, 50, 0);
  LOGINFO("Mapping done\n");

  if (mb_mapping == NULL) {
    LOGINFO("Mapping BAD\n");
    fprintf(stderr, "Failed to allocate the mapping: %s\n",
            modbus_strerror(errno));
    modbus_free(ctx);
    return -1;
  }

  s = modbus_tcp_listen(ctx, 1);
  LOGINFO("slave listen\n");

  modbus_tcp_accept(ctx, &s);
  LOGINFO("slave accept\n");

  cout << "===== mb_slave_init finished - OK" << endl;

  return 0;
}

int mb_slave()
{
  //  cout << "===== mb_slave =====" << endl;

  uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
  int rc;

  rc = modbus_receive(ctx, query);
  if (rc > 0) {
    // rc is the query size
    modbus_reply(ctx, query, rc, mb_mapping);
  }
  //  else if (rc == -1) {
  // Connection closed by the client or error
  //  break;
  // }
  return rc;
}

/*
  int mb_update() {
  //  cout << "===== mb_update =====" << endl;
  int ret = 0;

  for (auto &D : PLCset) {
    uint64_t old = D.mb.timestamp_ms;
    ret = D.update();
    printf("%-7s_dT: %4ld ret: %2d err: %d cn: %d rd: %d wr: %d rc: %2d\n",
           D.dev_name, D.mb.timestamp_ms - old, ret, D.mb.errors,
           D.mb.errors_cn, D.mb.errors_rd, D.mb.errors_wr, D.get_rc());
    //    tt.sleep_ms(10);
  }

  return ret;
  }
*/
// eof
