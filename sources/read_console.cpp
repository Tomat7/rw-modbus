// Thanx to eddy from unixforum.org
// https://unixforum.org/memberlist.php?mode=viewprofile&u=31191&sid=bb9fec4ccd505e1fa135cbf5c0e9af52
// Stolen from here https://unixforum.org/viewtopic.php?t=113242
//

#include <termios.h>
#include <unistd.h>

#include "./libs.h"

struct termios oldt, newt;

int read_console(time_t _sec, suseconds_t _usec) // считываем данные с консоли
{
  int rb = -1;
  struct timeval tv;
  int retval;

  // открываем терминал для реакции на клавиши без эха

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(STDIN_FILENO, &rfds); // 0 - стандартный вход
  tv.tv_sec = _sec;
  tv.tv_usec = _usec; // ждем 0.01с
  retval = select(1, &rfds, NULL, NULL, &tv);

  if (retval)
    if (FD_ISSET(STDIN_FILENO, &rfds))
      rb = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return rb;
}

void wait_console(int _s, int _us) {
  read_console((time_t)_s, (suseconds_t)_us);
}