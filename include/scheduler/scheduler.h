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
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <vector>
#include <thread>
#include <functional>

#define SCHEDULE_MAX_TASK 50
//#define USE_SYSLOG

using namespace std;

class Task_c
{
public:
  Task_c(function<int()> _func, uint64_t _ms, /* string _name, */ mutex* _mux);
  /*   Thread_c(const Thread_c&) = delete;
    Thread_c(Thread_c&&) = default; */
  ~Task_c();

  function<int()> func;
  uint64_t interval_ms = 0;
  uint64_t millis_last_run = 0;  // last run millis
  uint64_t counter_run = 0;      // counter of run
  uint64_t counter_errors = 0;   // counter of errors (run while not finished previous)
  volatile bool taskRunning = false;
  mutex* task_mux = nullptr;
//  string task_name; // = "Noname";
//  thread *thr = nullptr;
};

class Schedule_c
{
public:
  Schedule_c();
  ~Schedule_c();

  int add_task(function<int()> _func, uint64_t _ms, string _name = "Noname");
  void run();

  void stop();

private:

  static vector<Task_c> tasks;
  static vector<thread> threads;
  volatile static bool isRunning;
  static mutex scheduler_mux;

  static void _run_cycle();
  static void _run_task(/* function<int()> task, */ uint64_t i); //* isRun);
  void clear();

};

//eof
