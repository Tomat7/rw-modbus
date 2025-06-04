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


class Task_c
{
public:
  Task_c(std::function<int(void*)> _func, uint64_t _ms, std::string _name, void* _ptr);
  ~Task_c();
  // static void run(Task_c*);
  void run();

  std::function<int(void*)> func;
  volatile bool taskRunning = false;
  volatile uint64_t millis_last_run = 0;  // last run millis
  uint64_t interval_ms = 0;
  uint64_t counter_run = 0;     // counter of run
  uint64_t counter_errors = 0;  // cntr of err. run while not finished previous
  std::mutex* task_mux = nullptr;
  std::string task_name;
  int rc = 0; // Return code for... future use
  void* params;  // optional ptr to function's parameter
};


class Schedule_c
{
public:
  Schedule_c(int nb_ = 1);
  ~Schedule_c();

  static int add_task(std::function<int(void*)> _func, uint64_t _ms,
                      std::string _name = "Noname", void* _ptr = nullptr);
  static void init(int _nb = 0);
  static void start();
  static void stop();

private:
  static std::vector<Task_c> tasks;
  // vector<thread> threads;
  volatile static bool isRunning;
  static std::mutex scheduler_mux;
  /* static  int nb_max; */

  static void scheduler_cycle_();
  static void run_task_(/* function<int()> task, */ uint64_t i);  //* isRun);
  static void clear();
};

// eof
