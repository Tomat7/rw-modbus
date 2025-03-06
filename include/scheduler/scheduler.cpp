// scheduler.cpp ----------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//

#include "scheduler.h"

#include <fcntl.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <syslog.h>
#include <unistd.h>

#include <chrono>
#include <functional>
#include <map>
#include <mutex>
#include <thread>

#include "include/logger.h"
#include "include/plc/plc_class.h"
//#include "include/shmem/shmem.h"

#ifdef SYSLOG_NAME
#undef SYSLOG_NAME
#endif
#define SYSLOG_NAME "Task-scheduler"

volatile bool Schedule_c::isRunning = false;
/* int Schedule_c::nb_max = 0; */
vector<Task_c> Schedule_c::tasks;
// vector<thread> Schedule_c::threads;
mutex Schedule_c::scheduler_mux;

uint64_t millis()
{
#define CAST_MILLIS std::chrono::duration_cast<std::chrono::milliseconds>
  uint64_t t;
  t = CAST_MILLIS(std::chrono::system_clock::now().time_since_epoch()).count();
  return t;
}

Task_c::Task_c(function<int(void*)> _func, uint64_t _ms, string _name,
               void* _ptr)
  : func(_func), interval_ms(_ms), task_name(_name), params(_ptr)
{
  task_mux = new mutex;
  LOGD("Construct Tasks_c: %s. %x", task_name.c_str(), this);
}

Task_c::~Task_c()
{
  delete this->task_mux;
  LOGD("DEstruct Tasks_c: %s. %x", this->task_name.c_str(), this);
}

Schedule_c::Schedule_c(int nb_)
{
  tasks.reserve(nb_);
  LOGD("Construct Schedule_c: %d  %x", nb_, this);
}

Schedule_c::~Schedule_c()
{
  stop();
  LOGD("DEstruct Schedule_c! %x", this);
}

void Schedule_c::init(int _nb)
{
  if (_nb) {
    stop();
    tasks.reserve(_nb);
    LOGD("Init Schedule_c capacity: %d", tasks.capacity());
  } else
    LOGC("Wrong Schedule_c capacity: %d", _nb);
}

int Schedule_c::add_task(function<int(void*)> _func, uint64_t _ms, string _name,
                         void* _ptr)
{
  scheduler_mux.lock();
  uint64_t nb_tasks = tasks.size();
  uint64_t nb_capct = tasks.capacity();
  int ret = 0;

  if (nb_capct > nb_tasks) {
    for (uint64_t i = 0; i < nb_tasks; i++)
      tasks[i].task_mux->lock();
    for (uint64_t i = 0; i < nb_tasks; i++)
      tasks[i].task_mux->unlock();

    if (_ms > 10) {
      tasks.emplace_back(_func, _ms, _name, _ptr);
      nb_tasks = tasks.size();
      LOGN("New task: %s, ms: %d, total: %d", _name.c_str(), _ms, nb_tasks);
    } else
      LOGE("Too short interval! Task: %s, ms: %d - IGNORED!", _name.c_str(),
           _ms);

  } else
    LOGA("Can't add task! Now: %d, capacity: %d", nb_tasks, nb_capct);

  scheduler_mux.unlock();
  return ret;
}

void Schedule_c::run()
{
  if (!isRunning) {
    isRunning = true;
    thread run_cycle(run_cycle_);
    run_cycle.detach();
    LOGN("RunCycle: Detached");
  } else
    LOGA("RunCycle: already detached");
}

void Schedule_c::run_cycle_()
{
  scheduler_mux.lock();
  prctl(PR_SET_NAME, "Scheduler:cycle");
  uint64_t nb_tasks = tasks.size();
  vector<thread> threads(nb_tasks);

  while (isRunning) {
    this_thread::sleep_for(10ms);

    for (uint64_t i = 0; i < nb_tasks; i++) {
      Task_c &t = tasks[i];
      if ((millis() - t.millis_last_run) > t.interval_ms) {
        if ((!t.taskRunning) && isRunning) {
          // LOGD("Task-ready: %s \n", t.task_name.c_str());
          threads[i] = thread(run_task_, i);
          threads[i].detach();
        } else {
          LOGC("Task-error: %s still running!", t.task_name.c_str());
          t.counter_errors++;
          t.counter_run = 0;
        }
      }
    }

    this_thread::yield();
  }

  for (uint64_t i = 0; i < nb_tasks; i++)
    tasks[i].task_mux->lock();

  scheduler_mux.unlock();

  for (uint64_t i = 0; i < nb_tasks; i++)
    tasks[i].task_mux->unlock();

  LOGN("RunCycle: Finished");
}

void Schedule_c::stop()
{
  isRunning = false;
  this_thread::sleep_for(20ms);
  scheduler_mux.lock();

  LOGD("Stop: Try to lock.");

  uint64_t nb_tasks = tasks.size();
  for (uint64_t i = 0; i < nb_tasks; i++)
    tasks[i].task_mux->lock();
  for (uint64_t i = 0; i < nb_tasks; i++)
    tasks[i].task_mux->unlock();

  LOGD("Stop: Try to clear.");
  this_thread::sleep_for(10ms);

  tasks.clear();
  scheduler_mux.unlock();
  LOGN("Stop tasks: %d", nb_tasks);
}

void Schedule_c::clear() { tasks.clear(); }

void Schedule_c::run_task_(uint64_t i)
{
  LOGD("Task-run: %s", tasks[i].task_name.c_str());
  tasks[i].task_mux->lock();
  prctl(PR_SET_NAME, tasks[i].task_name.c_str());
  tasks[i].taskRunning = true;
  tasks[i].millis_last_run = millis();
  int ret = tasks[i].func(tasks[i].params);
  tasks[i].counter_errors = 0;
  tasks[i].counter_run++;
  tasks[i].taskRunning = false;
  tasks[i].task_mux->unlock();
  LOGD("Task-done: %s %d", tasks[i].task_name.c_str(), ret);
}

// eof
