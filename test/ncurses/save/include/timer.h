#pragma once
// timer.h ----------------------------------
// Copyright 2024 Tomat7 (star0413@gmail.com)
//
// Other (c)
// https://www.techiedelight.com/ru/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
//

#include <chrono> // для функций из std::chrono
#include <thread>

#define CAST_SECONDS duration_cast<second_t>
#define CAST_MILLIS duration_cast<milliseconds>
#define CAST_MICROS duration_cast<microseconds>

using namespace std;
using namespace chrono;

class Timer
{
private:
  // Псевдонимы типов используются для удобного доступа к вложенным типам
  //  using clock_t = std::chrono::high_resolution_clock;
  //  using second_t = std::chrono::duration<double, std::ratio<1> >;

  using second_t = duration<double, std::ratio<1>>;
  time_point<steady_clock> begin;
  time_point<steady_clock> end;

public:
  Timer() {}

  void start(const char* txt = "Timer started... \n")
  {
    if (txt != nullptr)
      PRINTF("%s", txt);
    begin = steady_clock::now();
  }

  void stop(const char* txt = "Timer stopped... \n")
  {
    if (txt != nullptr)
      PRINTF("%s", txt);
    end = steady_clock::now();
  }

  double elapsed_sec() const
  {
    auto sec = duration_cast<second_t>(end - begin);
    return sec.count();
  }

  int64_t elapsed_ms() const
  {
    auto sec = duration_cast<milliseconds>(end - begin);
    return sec.count();
  }

  int64_t elapsed_us() const
  {
    auto sec = duration_cast<microseconds>(end - begin);
    return sec.count();
  }

  // ============================

  void spent_sec(int p = 3, const char* txt1 = "Time spent: ",
                 const char* txt2 = " second.")
  {
    printf("%s %.*f %s\n", txt1, p, elapsed_sec(), txt2);
//    cout << txt1 << fixed << setprecision(p) << (elapsed_sec()) << txt2 << '\n';
  }

  void spent_ms(int p = 1, const char* txt1 = "Time spent: ",
                const char* txt2 = " msec.")
  {
    printf("%s %d %s\n", txt1, elapsed_ms(), txt2);
//    cout << txt1 << fixed << setprecision(p) << (elapsed_sec() * 1000) << txt2
//         << '\n';
  }

  void spent_us(int p = 1,
                const char* txt1 = "Time spent: ", const char* txt2 = " us.")
  {
    printf("%s %d %s\n", txt1, elapsed_us(), txt2);
//    cout << txt1 << fixed << setprecision(p) << (elapsed_sec() * 1000000)
//         << txt2 << '\n';
  }

  void spent_auto(const char* txt1 = "Time spent: ")
  {
    end = steady_clock::now();
    double _duration = elapsed_sec();

    if (_duration > 1)
      spent_sec(2, txt1);
    else if (_duration * 1000 > 1)
      spent_ms(1, txt1);
    else
      spent_us(0, txt1);
  }

  void spent() { spent_auto(); }

  // ===========================

  void sleep_sec(int s)
  {
    this_thread::sleep_for(seconds(s)); // std::chrono::
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

  uint64_t millis()
  {
    return CAST_MILLIS(system_clock::now().time_since_epoch()).count();
  }

  uint64_t micros()
  {
    return CAST_MICROS(system_clock::now().time_since_epoch()).count();
  }

};

// eof
