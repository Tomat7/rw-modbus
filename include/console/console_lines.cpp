
#include "console.h"

#include <stdio.h>
//#include <cstdint>
#include <iostream>
#include <deque>

saved_t Console::scroll;
std::deque<std::string> Console::lines;

int Console::max_row() { return scroll.max_row; }
int Console::max_col() { return scroll.max_col; }

void Console::scrolling_start()
{
  if (update_size() || update_scroll_position()) {
    clear();
    lines_trim(scroll.max_row - scroll.start_row);
    lines_print();
  } else {
    if (lines_trim(scroll.max_row - scroll.start_row))
      lines_print();
    else
      set_cursor(scroll.work_row, scroll.work_col);
  }
}

bool Console::lines_trim(size_t _scrolling_rows)
{
  bool ret = true;
  if (lines.size() >= _scrolling_rows)
    while (!lines.empty() && (lines.size() >= _scrolling_rows))
      lines.pop_front();
  else
    ret = false;

  return ret;
}

void Console::lines_add(std::string _str)
{
  lines.push_back(_str);
  if (lines_trim(scroll.max_row - scroll.start_row))
    lines_print();
  else {
    printf("%s", lines.back().c_str());
    get_cursor(&scroll.work_row, &scroll.work_col);
  }
}

void Console::lines_print()
{
  set_cursor(scroll.start_row, scroll.start_col);
  for (size_t i=0; i < lines.size(); i++)
    printf("%s", lines[i].c_str());
  //printf(": %li %s", lines.size(), lines[i].c_str());
  get_cursor(&scroll.work_row, &scroll.work_col);
}

bool Console::update_size()
{
  int old_max_row = scroll.max_row;
  int old_max_col = scroll.max_col;
  get_size(&scroll.max_row, &scroll.max_col);

  if ((old_max_row != scroll.max_row) || (old_max_col != scroll.max_col))
    return true;  // Windows size changed!
  else
    return false;
}

bool Console::update_scroll_position()
{
  int old_row = scroll.start_row;
  int old_col = scroll.start_col;
  get_cursor(&scroll.start_row, &scroll.start_col);

  if ((old_row != scroll.start_row) || (old_col != scroll.start_col))
    return true;  // Scrolling start position changed!
  else
    return false;
}

void Console::refresh()
{
  clear();
  scroll.start_row = 0;
  scroll.start_col = 0;
  scroll.max_row = 0;
  scroll.max_col = 0;
}

// eof
