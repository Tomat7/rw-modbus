#include <ncurses.h>

int main()
{
  /*
      initscr();      // Start curses mode
      printw("Hello World !!!");  // Print Hello World
      refresh();      // Print it on to the real screen
      getch();      // Wait for user input
      endwin();     // End curses mode
  */

  // инициализация (должна быть выполнена
  // перед использованием ncurses)
  initscr();

  // Измеряем размер экрана в рядах и колонках
  int row, col;
  getmaxyx(stdscr, row, col);

  // перемещение курсора в стандартном экране
  move(row / 2, col / 2);

  printw("Hello world\n"); // вывод строки
  printw("Hello world\n"); // вывод строки
  printw("Hello world\n"); // вывод строки
  printw("Hello world\n"); // вывод строки
  printw("Hello world\n"); // вывод строки
  refresh(); // обновить экран
  getch(); // ждём нажатия символа

  endwin(); // завершение работы с ncurses

  return 0;
}
