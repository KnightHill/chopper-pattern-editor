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

std::vector<Element> pattern;

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
    const int pattern_row = 7;
    const int pattern_left = 5;
    int col = 0;

    mvaddch(pattern_row, pattern_left - 1, '|');

    for (size_t i = 0; i < pattern.size(); i++) {
      Element e = pattern[i];
      if (e.type == Note) {
        switch (e.duration) {
        case D16:
          mvaddch(pattern_row, pattern_left + col, 'x');
          mvaddch(pattern_row, pattern_left + col + 1, '|');
          col += 2;
          break;
        case D8:
          mvaddch(pattern_row, pattern_left + col, 'x');
          mvaddch(pattern_row, pattern_left + col + 1, 'x');
          mvaddch(pattern_row, pattern_left + col + 2, '|');
          col += 3;
          break;
        case D4:
          mvaddch(pattern_row, pattern_left + col, 'x');
          mvaddch(pattern_row, pattern_left + col + 1, 'x');
          mvaddch(pattern_row, pattern_left + col + 2, 'x');
          mvaddch(pattern_row, pattern_left + col + 3, 'x');
          mvaddch(pattern_row, pattern_left + col + 4, '|');
          col += 5;
          break;
        }
      } else {
        switch (e.duration) {
        case D16:
          mvaddch(pattern_row, pattern_left + col, '.');
          mvaddch(pattern_row, pattern_left + col + 1, '|');
          col += 2;
          break;
        case D8:
          mvaddch(pattern_row, pattern_left + col, '.');
          mvaddch(pattern_row, pattern_left + col + 1, '.');
          mvaddch(pattern_row, pattern_left + col + 2, '|');
          col += 3;
          break;
        case D4:
          mvaddch(pattern_row, pattern_left + col, '.');
          mvaddch(pattern_row, pattern_left + col + 1, '.');
          mvaddch(pattern_row, pattern_left + col + 2, '.');
          mvaddch(pattern_row, pattern_left + col + 3, '.');
          mvaddch(pattern_row, pattern_left + col + 4, '|');
          col += 5;
          break;
        }
      }
    }

    refresh();

  } while (ch != 'x');

  endwin();

  return 0;
}