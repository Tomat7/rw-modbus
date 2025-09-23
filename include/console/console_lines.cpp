
#include "console.h"

#include <stdio.h>
//#include <cstdint>
#include <iostream>
#include <deque>

std::deque<std::string> lines;

void Console::scrolling_refresh()
{
  get_cursor(&saved.scroll_row, &saved.scroll_col);
  get_size(&saved.max_row, &saved.max_col);

  if (lines_trim(saved.max_row - saved.scroll_row))
    for (int i=0; i < lines.size(); i++)
      printf(lines[i].c_str());
  else
    printf(lines.back().c_str());
}

bool Console::lines_trim(int _scrolling_size)
{
  bool ret = true;
  if (lines.size() > _scrolling_size)
    while (!lines.empty() && (lines.size() > _scrolling_size))
      lines.pop_front();
  else
    ret = false;

  return ret;
}

void Console::lines_add(std::string _str)
{
  lines.push_back(_str);
}

// eof
