#pragma once
// timer.h ----------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// Other (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <chrono>  // для функций из std::chrono
#include <iomanip>
#include <iostream>
#include <thread>

// using namespace chrono;

using cchar = const char;

// Псевдонимы типов используются для удобного доступа к вложенным типам
using second_t = std::chrono::duration<double, std::ratio<1>>;
using millis_t = std::chrono::milliseconds;
using micros_t = std::chrono::microseconds;

#define DURATION_CAST std::chrono::duration_cast
#define CAST_SECOND std::chrono::duration_cast<second_t>
#define CAST_MILLIS std::chrono::duration_cast<millis_t>
#define CAST_MICROS std::chrono::duration_cast<micros_t>

using hi_clock_t = std::chrono::high_resolution_clock;    // just to save
using sys_clock_t = std::chrono::system_clock;            // just to save here
using std_clock_t = std::chrono::steady_clock;            // just to save here

using cclock_t = std_clock_t;  // make a choise - what clock to use now?!

class Timer
{
private:
  std::chrono::time_point<cclock_t> begin;
  std::chrono::time_point<cclock_t> end;
  uint64_t start_millis, start_micros;
  double start_seconds;

public:
  Timer()
  {
    start_seconds = CAST_SECOND(cclock_t::now().time_since_epoch()).count();
    start_millis = CAST_MILLIS(cclock_t::now().time_since_epoch()).count();
    start_micros = CAST_MICROS(cclock_t::now().time_since_epoch()).count();
  }

  void start(cchar* txt = "Timer started... \n")
  {
    if (txt != nullptr)
      printf("%s", txt);
    begin = cclock_t::now();
  }

  void stop(cchar* txt = "Timer stopped... \n")
  {
    if (txt != nullptr)
      printf("%s", txt);
    end = cclock_t::now();
  }

  // ===== is_passed_* =====

  bool is_passed_sec(double sec) { return passed_sec() > sec; }

  bool is_passed_ms(int64_t ms) { return passed_ms() > ms; }

  bool is_passed_us(int64_t us) { return passed_us() > us; }

  // ===== passed_* =====

  double passed_sec() const
  {
    string n;
    return DURATION_CAST<second_t>(cclock_t::now() - begin).count();
  }

  int64_t passed_ms() const
  {
    return DURATION_CAST<millis_t>(cclock_t::now() - begin).count();
  }

  int64_t passed_us() const
  {
    return DURATION_CAST<micros_t>(cclock_t::now() - begin).count();
  }

  // ===== elapsed_* =====

  double elapsed_sec() const
  {
    // auto sec = DURATION_CAST<second_t>(end - begin);
    return DURATION_CAST<second_t>(end - begin).count();
  }

  int64_t elapsed_ms() const
  {
    // auto msec = DURATION_CAST<millis_t>(end - begin);
    return DURATION_CAST<millis_t>(end - begin).count();
  }

  int64_t elapsed_us() const
  {
    // auto usec = DURATION_CAST<micros_t>(end - begin);
    return DURATION_CAST<micros_t>(end - begin).count();
  }

  // ============================

  void spent_sec(cchar* txt1 = "Time spent: ", cchar* txt2 = " second.")
  {
    /* cout << txt1 << fixed << setprecision(p) << (elapsed_sec())
            << txt2 << '\n'; */
    printf("%s %7.2f %s\n", txt1, elapsed_sec(), txt2);
  }

  void spent_ms(cchar* txt1 = "Time spent: ", cchar* txt2 = " msec.")
  {
    /* cout << txt1 << fixed << setprecision(p) << (elapsed_sec() * 1000) <<
       txt2
         << '\n'; */
    printf("%s %8.1f %s\n", txt1, elapsed_sec() * 1000, txt2);
  }

  void spent_us(cchar* txt1 = "Time spent: ", cchar* txt2 = " us.")
  {
    /* cout << txt1 << fixed << setprecision(p) << (elapsed_sec() * 1000000)
         << txt2 << '\n'; */
    printf("%s %7.0f %s\n", txt1, elapsed_sec() * 1000000, txt2);
  }

  void spent_auto(const char* txt1 = "Time spent: ")
  {
    end = cclock_t::now();
    double _duration = elapsed_sec();

    if (_duration > 1)
      spent_sec(txt1);
    else if (_duration * 1000 > 1)
      spent_ms(txt1);
    else
      spent_us(txt1);
  }

  void spent() { spent_auto(); }

  // ===========================

  void sleep_sec(int s)
  {
    std::this_thread::sleep_for(std::chrono::seconds(s));  // std::chrono::
  }

  void sleep_ms(int ms)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
  }

  void sleep_us(int us)
  {
    std::this_thread::sleep_for(std::chrono::microseconds(us));
  }

  // ============================

  double seconds() { return epoch_seconds() - start_seconds; }

  int64_t millis() { return epoch_millis() - start_millis; }

  int64_t micros() { return epoch_micros() - start_micros; }

  // ============================

  auto now_since_epoch() { return cclock_t::now().time_since_epoch(); }
  // ===========================

  double epoch_seconds()
  {
    return DURATION_CAST<second_t>(now_since_epoch()).count();
  }

  int64_t epoch_millis()
  {
    return CAST_MILLIS(cclock_t::now().time_since_epoch()).count();
  }

  int64_t epoch_micros()
  {
    return CAST_MICROS(cclock_t::now().time_since_epoch()).count();
  }
};

// eof
