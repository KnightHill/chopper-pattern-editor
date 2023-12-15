#include <ncurses.h>
#include <vector>

// g++ -Wall main.cpp -lncurses -o editor

enum Duration {
  D4, // quarter
  D8, // eigth
  D16 // sixteenth
};

enum ElementType { Note, Pause };

struct Element {
  ElementType type;
  Duration duration;
};

const int pattern_row = 7;
const int pattern_left = 5;

std::vector<Element> pattern;

void draw_element(Element el, int &col) {
  char ch = el.type == Note ? 'x' : '.';
  int len;
  switch (el.duration) {
  case D16:
    len = 1;
    break;
  case D8:
    len = 2;
    break;
  case D4:
    len = 4;
    break;
  }

  for (int j = 0; j < len; j++) {
    mvaddch(pattern_row, pattern_left + col + j, ch);
  }

  mvaddch(pattern_row, pattern_left + col + len, '|');
  col += len + 1;
}

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
      pattern.push_back({Note, D4});
      break;
    case 'w':
      pattern.push_back({Note, D8});
      break;
    case 'e':
      pattern.push_back({Note, D16});
      break;
    case 'a':
      pattern.push_back({Pause, D4});
      break;
    case 's':
      pattern.push_back({Pause, D8});
      break;
    case 'd':
      pattern.push_back({Pause, D16});
      break;
    }

    // Draw the pattern
    int col = 0;
    mvaddch(pattern_row, pattern_left - 1, '|');

    for (size_t i = 0; i < pattern.size(); i++) {
      Element e = pattern[i];
      draw_element(e, col);
    }

    refresh();

  } while (ch != 'x');

  endwin();

  return 0;
}