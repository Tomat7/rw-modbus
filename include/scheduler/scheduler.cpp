// scheduler.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//

#include "scheduler.h"

#include <fcntl.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <syslog.h>
#include <unistd.h>

#include <chrono>
#include <map>
#include <mutex>
#include <thread>
#include <functional>

#include "include/logger.h"
#include "include/plc/plc_class.h"
#include "include/shmem/shmem.h"

#ifdef SYSLOG_NAME
#undef SYSLOG_NAME
#endif
#define SYSLOG_NAME "Task-scheduler"

volatile bool Schedule_c::isRunning = false;
vector<Task_c> Schedule_c::tasks;
vector<thread> Schedule_c::threads;
mutex Schedule_c::scheduler_mux;

uint64_t millis()
{
#define CAST_MILLIS std::chrono::duration_cast<std::chrono::milliseconds>
  uint64_t t;
  t = CAST_MILLIS(std::chrono::system_clock::now().time_since_epoch()).count();
  return t;
}

Task_c::Task_c(function<int()> _func, uint64_t _ms, string _name) :
  func(_func), interval_ms(_ms), task_name(_name)
{
  task_mux = new mutex;
  LOGD("Construct Tasks_c! %x", this);
}

Task_c::~Task_c()
{
  delete task_mux;
  LOGD("DEstruct Tasks_c: %s. %x", this->task_name.c_str(), this);
}

Schedule_c::Schedule_c() { LOGD("Construct Schedule_c! %x", this); }
Schedule_c::~Schedule_c()
{
  stop();
  LOGD("DEstruct Schedule_c! %x", this);
}



int Schedule_c::add_task(function<int()> _func, uint64_t _ms, string _name)
{
  scheduler_mux.lock();
  tasks.emplace_back(_func, _ms, _name); //, new mutex);
  scheduler_mux.unlock();
  LOGN("PushBack/Added task: %s\n", _name.c_str());
  return 1;
}

void Schedule_c::run()
{
  isRunning = true;
  thread run_cycle(_run_cycle);
  run_cycle.detach();
  LOGN("MainRun: Detached\n");
}

void Schedule_c::_run_cycle()
{
  while (isRunning) {
    this_thread::sleep_for(10ms);
    scheduler_mux.lock();
    uint64_t nb_tasks = tasks.size();
    threads.resize(nb_tasks);

    for (uint64_t i = 0; i < nb_tasks; i++) {
      Task_c &t = tasks[i];
      if ((millis() - t.millis_last_run) > t.interval_ms) {
        if (!t.taskRunning && isRunning) {
          //  t.millis_last_run = millis();
          //  t.counter_errors = 0;
          //  t.counter_run++;
          thread thr(_run_task, i);
          thr.detach();
        } else {
          t.counter_errors++;
          t.counter_run = 0;
        }
      }
    }
    scheduler_mux.unlock();
    this_thread::yield();
  }
}

void Schedule_c::stop()
{
  isRunning = false;
  this_thread::sleep_for(20ms);
  clear();
}

void Schedule_c::clear() { tasks.clear(); }

void Schedule_c::_run_task(uint64_t i)
{
  tasks[i].task_mux->lock();
  tasks[i].taskRunning = true;
  tasks[i].func();
  tasks[i].millis_last_run = millis();
  tasks[i].counter_errors = 0;
  tasks[i].counter_run++;
  tasks[i].taskRunning = false;
  LOGD("run-task-done: %s\n", tasks[i].task_name.c_str());
  tasks[i].task_mux->unlock();
}


//eof
