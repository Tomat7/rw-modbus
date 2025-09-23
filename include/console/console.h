#pragma once

#include <stdlib.h>
#include <iostream>
#include <deque>

struct saved_t {
  int row;
  int col;
  int max_row;
  int max_col;
  int scroll_row;
  int scroll_col;
};

class Console
{
public:
  static void save();
  static void restore();
  static void clear();
  static void home();

  static int read_us(int _us = 1);
  static int read_ms(int _ms = 1);
  static int read_sec(int _s = 1);

  static int save_cursor();
  static int get_size(int* maxrow, int* maxcol);
  static int get_cursor(int* row, int* col);
  static void set_cursor(int row, int col);

  static void moveRW(int _row, int _col);
  static void gotoXY(int _x, int _y);

  void scrolling_refresh();
  void lines_add(std::string _str);
  void lines_flush();

private:
  static int read_char(time_t _sec, suseconds_t _usec);
  static bool lines_trim(int _scrolling_size);

  static struct termios saved_termios;

  static saved_t saved, scroll;
  static std::deque<std::string> lines;

};

// eof
