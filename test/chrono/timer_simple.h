
#include <iostream>
#include <iomanip>
#include <chrono> // для функций из std::chrono

using namespace std;
using namespace chrono;

class Timer {
private:

  system_clock::time_point begin, end;
  duration<double> sec;

public:
  Timer() // : m_beg(clock_t::now())
  {}

  void start(const char *txt = "Timer started: ") {
    cout << txt;
    begin = system_clock::now();
    auto current_time = system_clock::to_time_t(begin);
//    cout.imbue(locale("ru_RU.utf8"));
    cout << put_time(localtime(&current_time), "%d.%m.%Y %T %Z") << endl;
  }

  void stop(const char *txt = "Timer stopped: ") {
    cout << txt;
    end = system_clock::now();
    sec = end - begin;
    auto current_time = system_clock::to_time_t(begin);
//    cout.imbue(locale("ru_RU.utf8"));
    cout << put_time(localtime(&current_time), "%d.%m.%Y %T %Z") << endl;
  }

  double elapsed_second() const {
    return sec.count();
  }

  void spent_second(const char *txt = "Time spent (seconds): ") {
    cout << txt << elapsed_second() << '\n';
  }


  int elapsed_msec() const {
    auto msec = sec * 1000;
    return int(msec.count());
  }

  void spent_msec(const char *txt = "Time spent (msec): ") {
    cout << txt << elapsed_msec() << '\n';
  }


  int elapsed_us() const {
    auto us = sec*1000000;
    return int(us.count());
  }

  void spent_us(const char *txt = "Time spent (us): ") {
    cout << txt << elapsed_us() << '\n';
  }

};

Timer t;
