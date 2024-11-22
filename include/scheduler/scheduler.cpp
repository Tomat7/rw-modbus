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
//vector<Task_c> Schedule_c::tasks;
//vector<thread> Schedule_c::threads;
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
  LOGD("Construct Tasks_c: %s. %x", task_name.c_str(), this);
}

Task_c::~Task_c()
{
  delete this->task_mux;
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

  uint64_t nb_tasks = tasks.size();
  for (uint64_t i = 0; i < nb_tasks; i++)
    tasks[i]->task_mux->lock();

  for (uint64_t i = 0; i < nb_tasks; i++)
    tasks[i]->task_mux->unlock();

  Task_c* t = new Task_c(_func, _ms, _name);

  tasks.emplace_back(t); //, new mutex);
  nb_tasks++;
  //threads.clear();
  //threads.resize(nb_tasks);
  //tasks.resize(nb_tasks);

  scheduler_mux.unlock();
  LOGN("PushBack/Added task: %d - %s\n", nb_tasks, _name.c_str());
  return 1;
}

void Schedule_c::run()
{
  isRunning = true;
  thread run_cycle(&Schedule_c::run_cycle_, this);
  run_cycle.detach();
  LOGN("MainRun: Detached\n");
}

void Schedule_c::run_cycle_()
{
  while (isRunning) {
    this_thread::sleep_for(10ms);
    scheduler_mux.lock();
    uint64_t nb_tasks = (this->tasks).size();
    vector<thread> threads(nb_tasks);
    //threads.resize(nb_tasks);

    for (uint64_t i = 0; i < nb_tasks; i++) {
      Task_c &t = *tasks[i];
      if ((millis() - t.millis_last_run) > t.interval_ms) {
        if (!t.taskRunning && isRunning) {
          //  t.millis_last_run = millis();
          //  t.counter_errors = 0;
          //  t.counter_run++;
          LOGD("Task-ready: %s \n", t.task_name.c_str());
          //thread thr(_run_task, i);
          threads[i] = thread(&Schedule_c::run_task_, this, i);
          threads[i].detach();
        } else {
          t.counter_errors++;
          t.counter_run = 0;
        }
      }
    }
    //threads.clear();
    scheduler_mux.unlock();
    this_thread::yield();
  }
}

void Schedule_c::stop()
{
  isRunning = false;
  this_thread::sleep_for(20ms);
  uint64_t nb_tasks = tasks.size();
  for (uint64_t i = 0; i < nb_tasks; i++)
    delete tasks[i];
  clear();
}

void Schedule_c::clear() { tasks.clear(); }

void Schedule_c::run_task_(uint64_t i)
{
  /*   LOGD("Task-run: %s \n", tasks[i].task_name.c_str());
    tasks[i].task_mux->lock();
    tasks[i].taskRunning = true;
    tasks[i].func();
    tasks[i].millis_last_run = millis();
    tasks[i].counter_errors = 0;
    tasks[i].counter_run++;
    tasks[i].taskRunning = false;
    tasks[i].task_mux->unlock();
    LOGD("Task-done: %s\n", tasks[i].task_name.c_str()); */
}


//eof
