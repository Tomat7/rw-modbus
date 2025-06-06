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

using std::string;
using std::mutex;
using std::vector;
using std::function;
using std::thread;
using std::this_thread::sleep_for;

using namespace std::chrono_literals;

volatile bool Schedule_c::isRunning = false;
/* int Schedule_c::nb_max = 0; */
vector<Task_c> Schedule_c::tasks;
// vector<thread> Schedule_c::threads;
mutex Schedule_c::scheduler_mux;


static uint64_t millis()
{
  using namespace std::chrono;
#define CAST_MILLIS duration_cast<milliseconds>
  return CAST_MILLIS(system_clock::now().time_since_epoch()).count();
}


// ======= Tasks ===============================================

Task_c::Task_c(function<int(void*)> _func, uint64_t _ms, string _name, void* _ptr)
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


void Task_c::run()  // for way 3&4
{
  LOGD("Task-run: %s", task_name.c_str());
  task_mux->lock();
  prctl(PR_SET_NAME, task_name.c_str());
  taskRunning = true;
  millis_last_run = millis();
  rc = func(params);
  counter_errors = 0;
  counter_run++;
  taskRunning = false;
  task_mux->unlock();
  LOGD("Task-done: %s %d", task_name.c_str(), rc);
}

/*
  void Task_c::run(Task_c* t)  // for way 1&2
  {
  LOGD("Task-run: %s", t->task_name.c_str());
  t->task_mux->lock();
  prctl(PR_SET_NAME, t->task_name.c_str());
  t->taskRunning = true;
  t->millis_last_run = millis();
  t->rc = t->func(t->params);
  t->counter_errors = 0;
  t->counter_run++;
  t->taskRunning = false;
  t->task_mux->unlock();
  LOGD("Task-done: %s %d", t->task_name.c_str(), t->rc);
  }
*/
// ======= Scheduler ===============================================

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

int Schedule_c::add_task(function<int(void*)> _func, uint64_t _ms,
                         string _name, void* _ptr)
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
      LOGE("Too short! Task: %s, ms: %d - IGNORED!", _name.c_str(), _ms);

  } else
    LOGA("Can't add task! Now: %d, capacity: %d", nb_tasks, nb_capct);

  scheduler_mux.unlock();
  return ret;
}

void Schedule_c::start()
{
  if (!isRunning) {
    isRunning = true;
    thread run_cycle(scheduler_cycle_);
    run_cycle.detach();
    LOGN("RunCycle: Detached");
  } else
    LOGA("RunCycle: already detached");
}

void Schedule_c::scheduler_cycle_()
{
  scheduler_mux.lock();
  prctl(PR_SET_NAME, "Scheduler:cycle");
  uint64_t nb_tasks = tasks.size();
  vector<thread> threads(nb_tasks);
  // function<void(Task_c*)> f; // for other way1
  function<void()> fn_task; // for other way 3

  while (isRunning) {
    sleep_for(10ms);

    for (uint64_t i = 0; i < nb_tasks; i++) {
      Task_c &t = tasks[i];
      // f = tasks[i].run; // for other way1
      fn_task = std::bind_front(&Task_c::run, &tasks[i]); // for other way3
      if ((millis() - t.millis_last_run) > t.interval_ms) {
        if ((!t.taskRunning) && isRunning) {
          // LOGD("Task-ready: %s \n", t.task_name.c_str());
          // threads[i] = thread(f, &tasks[i]); // way 1
          // threads[i] = thread(t.run, &tasks[i]); // way2
          threads[i] = thread(fn_task); // way3
          // threads[i] = thread(std::bind(&Task_c::run, &tasks[i])); // way 4a
          // threads[i] = thread(std::bind_front(&Task_c::run, &tasks[i])); // 4b
          // threads[i] = thread(run_task_, i);  // classic & stupid
          threads[i].detach();
        } else {
          LOGC("Task-error: %s still running!", t.task_name.c_str());
          t.counter_errors++;
          t.counter_run = 0;
        }
      }
    }

    std::this_thread::yield();
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
  sleep_for(20ms);
  scheduler_mux.lock();

  LOGD("Stop: Try to lock.");

  uint64_t nb_tasks = tasks.size();
  for (uint64_t i = 0; i < nb_tasks; i++)
    tasks[i].task_mux->lock();
  for (uint64_t i = 0; i < nb_tasks; i++)
    tasks[i].task_mux->unlock();

  LOGD("Stop: Try to clear.");
  sleep_for(10ms);

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
  tasks[i].rc = tasks[i].func(tasks[i].params);
  tasks[i].counter_errors = 0;
  tasks[i].counter_run++;
  tasks[i].taskRunning = false;
  tasks[i].task_mux->unlock();
  LOGD("Task-done: %s %d", tasks[i].task_name.c_str(), tasks[i].rc);
}

// eof
