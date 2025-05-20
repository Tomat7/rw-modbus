#pragma once

// plc_class.h ----------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// Other (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <modbus/modbus.h>
#include <string.h>

#include <atomic>
#include <ctime>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#define SCHEDULE_MAX_TASK 50
//#define USE_SYSLOG

using namespace std;

class Task_c
{
public:
  Task_c(function<int(void*)> _func, uint64_t _ms, string _name, void* _ptr);
  ~Task_c();

  function<int(void*)> func;
  volatile bool taskRunning = false;
  volatile uint64_t millis_last_run = 0;  // last run millis
  uint64_t interval_ms = 0;
  uint64_t counter_run = 0;     // counter of run
  uint64_t counter_errors = 0;  // cntr of err. run while not finished previous
  mutex* task_mux = nullptr;
  string task_name;
  void* params;  // optional ptr to function's parameter
};

class Schedule_c
{
public:
  Schedule_c(int nb_ = 1);
  ~Schedule_c();

  static int add_task(function<int(void*)> _func, uint64_t _ms,
                      string _name = "Noname", void* _ptr = nullptr);
  static void init(int _nb = 0);
  static void run();
  static void stop();

private:
  static vector<Task_c> tasks;
  // vector<thread> threads;
  volatile static bool isRunning;
  static mutex scheduler_mux;
  /* static  int nb_max; */

  static void scheduler_cycle_();
  static void run_task_(/* function<int()> task, */ uint64_t i);  //* isRun);
  static void clear();
};

// eof
