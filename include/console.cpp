// Thanx to eddy from unixforum.org
// https://unixforum.org/memberlist.php?mode=viewprofile&u=31191&sid=bb9fec4ccd505e1fa135cbf5c0e9af52
// Stolen from here https://unixforum.org/viewtopic.php?t=113242
//
#include "./console.h"

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static struct termios old_term;

int read_console(time_t _sec, suseconds_t _usec)  // считываем с консоли
{
  int rb = -1;
  int retval;
  struct termios new_term;
  struct timeval tv;

  // открываем терминал для реакции на клавиши без эха
  tcgetattr(STDIN_FILENO, &old_term);
  new_term = old_term;
  new_term.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(STDIN_FILENO, &rfds);  // 0 - стандартный вход
  tv.tv_sec = _sec;  // задаём время ожидания в секундах
  tv.tv_usec = _usec;  // ... в микросекундах
  retval = select(1, &rfds, NULL, NULL, &tv);

  if (retval && FD_ISSET(STDIN_FILENO, &rfds))
    rb = getchar();

  // возвращаем эхо в терминале
  tcsetattr(STDIN_FILENO, TCSANOW, &old_term);

  return rb;
}

void wait_console(int _s, int _us)
{
  read_console((time_t)_s, (suseconds_t)_us);
}

void restore_console() { tcsetattr(STDIN_FILENO, TCSANOW, &old_term); }

// eof
