#pragma once

#include <stdlib.h>

struct saved_t {
  int row;
  int col;
  int max_row;
  int max_col;
};

class Console
{
public:
  static void save();
  static void restore();

  static int read_us(int _us = 1);
  static int read_ms(int _ms = 1);
  static int read_sec(int _s = 1);

  static int save_cursor();
  static int get_size(int* maxrow, int* maxcol);
  static int get_cursor(int* row, int* col);
  static void set_cursor(int row, int col);

  static void moveRW(int _row, int _col);
  static void gotoXY(int _x, int _y);

private:
  static int read_char(time_t _sec, suseconds_t _usec);
  static struct termios saved_termios;

  static saved_t saved;


};

// eof
