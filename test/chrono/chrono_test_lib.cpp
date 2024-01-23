
#include "timer_simple.h"
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>

#define CLKSIZE 300

using namespace std;
using namespace chrono;

int main()
{
    // Получаем момент времени_1
    t.start();

    // Выполняем некоторый код
    for (long i = 1; i < CLKSIZE; i += 1)
        for (long j = 1; j < CLKSIZE; j += 1)
            for (long k = 1; k < CLKSIZE; k += 1) {
                double f = double(k) / double(j);
                double s = sqrt(double(i) / f);
                cout << s << "   ";
            }
    cout << endl;
    string s;
    //  cin >> s;

    // Получаем момент времени_2
    t.stop();

    cout << endl;

    t.spent_second();
    t.spent_msec();
    t.spent_us();

    return 0;
}
