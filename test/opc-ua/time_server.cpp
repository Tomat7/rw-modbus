/* This work is licensed under a Creative Commons CCZero 1.0 Universal License.
   See http://creativecommons.org/publicdomain/zero/1.0/ for more information. */

#include <thread>
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>

#include "time_server.h"


int main(void)
{
  var_t myI32;
  myI32.name = "myInt";
  myI32.type = UA_TYPES_INT32;
  myI32.value.i32 = 77555;

  var_t myFloat;
  myFloat.name = "myFloat";
  myFloat.type = UA_TYPES_FLOAT;
  myFloat.value.fl = 3.14;

  var_t myDateTime;
  myDateTime.name = "myDateTime";
  myDateTime.type = UA_TYPES_DATETIME;
  myDateTime.value.dt = UA_DateTime_now();

  UA_Server* server = UA_Server_new();

  myInt = 420;
  myFl = 9.81;

  addVariable(server, (void*)&myI32);
  addVariable(server, (void*)&myFloat);
  addVariable(server, (void*)&myDateTime);

  addCurrentTimeDataSourceVariable(server);
  addCurrentTimeExternalDataSource(server);

  std::thread thr(UA_Server_runUntilInterrupt, server);

  for (;;) {

    getVariable(server, (void*)&myI32, true);
    myInt = myI32.value.i32;
    myInt++;
    myI32.value.i32 = myInt;
    myI32.value.i32++;
    setVariable(server, (void*)&myI32);


    myFloat.value.fl += 0.123;
    setVariable(server, (void*)&myFloat);

    myDateTime.value.dt = UA_DateTime_now();
    setVariable(server, (void*)&myDateTime);

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  thr.join();

  UA_Server_delete(server);
  return 0;
}