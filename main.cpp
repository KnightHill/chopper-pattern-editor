#include <ncurses.h>

int main() {
  initscr();
  raw();                /* Line buffering disabled	*/
  keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
  noecho();             /* Don't echo() while we do getch */

  mvprintw(0, 0, "-- Pattern Editor --");
  mvprintw(2, 0, "  Note: 1/4 - q, 1/8 - w, 1/16 - e");
  mvprintw(3, 0, " Pause: 1/4 - a, 1/8 - s, 1/16 - d");
  mvprintw(5, 0, " z - delete last, r - render, x - exit");
  refresh();

  int ch;

  do {
    ch = getch();
    switch (ch) {
    case 'q':
      break;
    case 'w':
      break;
    case 'e':
      break;
    }
  } while (ch != 'x');

  endwin();

  return 0;
}