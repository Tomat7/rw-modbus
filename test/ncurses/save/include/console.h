#pragma once

//#include <stdio.h>
#include <stdlib.h>
// #include <termios.h>
// #include <unistd.h>
//#include <time.h>
//#include <sys/types.h>

// int read_console(time_t _sec, suseconds_t _usec);
// void wait_console(int _s, int _us);
int read_console(time_t _sec = 0, suseconds_t _usec = 10000);
void wait_console(int _s = 0, int _us = 10000);
void restore_console();

// eof
