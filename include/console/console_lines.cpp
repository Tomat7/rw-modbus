
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
  if (update_size() || update_scroll_pos()) {
    clear();
//    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ max: %d start: %d\n",
//      scroll.max_row, scroll.start_row);
    lines_trim(scroll.max_row - scroll.start_row);
    lines_reprint();
  } else {
//    printf("------------------------------------------------------------------------------ max: %d start: %d \n",
//      scroll.max_row, scroll.start_row);
    if (lines_trim(scroll.max_row - scroll.start_row))
      lines_reprint();
    else
      set_cursor(scroll.work_row, scroll.work_col);
    //printf(lines.back().c_str());
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
    lines_reprint();
  else {
    printf("%s", lines.back().c_str());
    get_cursor(&scroll.work_row, &scroll.work_col);
  }
}

void Console::lines_reprint()
{
  set_cursor(scroll.start_row, scroll.start_col);
  for (size_t i=0; i < lines.size(); i++)
    printf("%s", lines[i].c_str());
  //printf(": %li %s", lines.size(), lines[i].c_str());
  get_cursor(&scroll.work_row, &scroll.work_col);
}

bool Console::update_size()
{
  int _row = scroll.max_row;
  int _col = scroll.max_col;
  get_size(&scroll.max_row, &scroll.max_col);

  if ((_row != scroll.max_row) || (_col != scroll.max_col))
    return true;  // Windows size changed!
  else
    return false;
}

bool Console::update_scroll_pos()
{
  int _row = scroll.start_row;
  int _col = scroll.start_col;
  get_cursor(&scroll.start_row, &scroll.start_col);

  if ((_row != scroll.start_row) || (_col != scroll.start_col))
    return true;  // Scrolling start position changed!
  else
    return false;
}


// eof
