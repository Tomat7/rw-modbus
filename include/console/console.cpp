// Thanx to eddy from unixforum.org
// https://unixforum.org/memberlist.php?mode=viewprofile&u=31191&sid=bb9fec4ccd505e1fa135cbf5c0e9af52
// Stolen from here https://unixforum.org/viewtopic.php?t=113242
//
#include "console.h"

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>    // For STDOUT_FILENO

#include <iostream>
#include <sys/ioctl.h> // For ioctl and winsize

#include <cstdint>

struct termios Console::saved_termios;
saved_t Console::saved;

void Console::save() { tcgetattr(STDIN_FILENO, &saved_termios); }
void Console::restore() { tcsetattr(STDIN_FILENO, TCSANOW, &saved_termios); }

int Console::read_us(int _us) { return read_char(0, (suseconds_t)_us); }
int Console::read_ms(int _ms) { return read_char(0, (suseconds_t)(_ms * 1000)); }
int Console::read_sec(int _s) { return read_char((time_t)_s, 0); }

void Console::moveRW(int _row, int _col) { printf("\033[%d;%dH", _row, _col); }
void Console::gotoXY(int _x, int _y) { printf("\033[%d;%dH", _y, _x); }

int Console::read_char(time_t _sec, suseconds_t _usec)  // считываем с консоли
{
  int rb = -1;
  int retval;
  struct termios current_termios, new_termios;
  struct timeval tv;

  // открываем терминал для реакции на клавиши без эха
  tcgetattr(STDIN_FILENO, &current_termios);
  new_termios = current_termios;
  new_termios.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(STDIN_FILENO, &rfds);  // 0 - стандартный вход
  tv.tv_sec = _sec;  // задаём время ожидания в секундах
  tv.tv_usec = _usec;  // ... в микросекундах
  retval = select(1, &rfds, NULL, NULL, &tv);

  if (retval && FD_ISSET(STDIN_FILENO, &rfds))
    rb = getchar();

  // возвращаем эхо в терминале
  tcsetattr(STDIN_FILENO, TCSANOW, &current_termios);

  return rb;
}

int Console::save_cursor()
{
  return get_cursor(&saved.row, &saved.col);
}

void Console::set_cursor(int row, int col)
{
  moveRW(row, col);
}

int Console::get_cursor(int* row, int* col)
{
  struct termios current_termios, new_termios;
  char buffer[32];
  uint64_t i = 0;

  // Save current terminal settings
  tcgetattr(STDIN_FILENO, &current_termios);
  new_termios = current_termios;

  // Set terminal to raw mode (disable canonical mode and echo)
  new_termios.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

  // Send ANSI escape sequence to query cursor position ESC[6n
  write(STDOUT_FILENO, "\033[6n", 4);

  // Read response from terminal
  while (i < sizeof(buffer) - 1) {
    if (read(STDIN_FILENO, &buffer[i], 1) != 1)
      break;
    if (buffer[i] == 'R')
      break; // 'R' indicates end of response
    i++;
  }

  buffer[i] = '\0'; // Null-terminate the string

  // Restore original terminal settings
  tcsetattr(STDIN_FILENO, TCSANOW, &current_termios);

  // Parse the response: ESC[row;colR
  if (sscanf(buffer, "\033[%d;%dR", row, col) == 2) {
    return 1; // Success
  }

  return 0; // Failure
}

int Console::get_size(int* maxrow, int* maxcol)
{
  struct winsize win_size;
  // Call ioctl to get the window size information
  // STDOUT_FILENO refers to standard output
  // TIOCGWINSZ is the command to get window size
  // &w is a pointer to the winsize struct where the information will be stored
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &win_size) == 0) {
    saved.max_row = win_size.ws_row;
    saved.max_col = win_size.ws_col;
    *maxrow = win_size.ws_row;
    *maxcol = win_size.ws_col;
  } else
    return 0;

  return 1;
}

// eof
