#pragma once

#include <stdlib.h>
#include <iostream>
#include <deque>

struct saved_t {
  int row;
  int col;
  int work_row;
  int work_col;
  int max_row;
  int max_col;
  int start_row;
  int start_col;
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
  static int max_row();
  static int max_col();

  static void moveRW(int _row, int _col);
  static void gotoXY(int _x, int _y);

  static void scrolling_start();
  static void lines_add(std::string _str);

  //static void lines_flush();
  static void lines_reprint();
  static bool update_size();
  static bool update_scroll_pos();

private:
  static int read_char(time_t _sec, suseconds_t _usec);
  static bool lines_trim(size_t _scrolling_size);

  static struct termios saved_termios;

  static saved_t scroll;
  static std::deque<std::string> lines;

};

// eof
