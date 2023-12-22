#include <cstdlib>

#include <string>
#include <vector>
#include <format>
#include <fstream>
#include <ncurses.h>
#include "pattern.h"

// http://ld2009.scusa.lsu.edu/php/ref.ncurses.html

using namespace std;

Pattern pattern;

const int ODD_COLOR_PAIR = 1;
const int EVEN_COLOR_PAIR = 2;

// pattern display coordinates
const int pattern_top = 7;
const int pattern_left = 5;

#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

void clear_line()
{
  int col, row;
  int cols, rows;

  getyx(stdscr, row, col);
  getmaxyx(stdscr, rows, cols);

  move(row, 1);
  for (int c = 0; c < cols - 2; c++) {
    addch(' ');
  }
}

void draw_element(Element el, int &col, int &pos)
{
  int ch = el.type == Note ? '=' : '.';
  int len = pattern.GetElementLen(el);

  for (int j = 0; j < len; j++) {
    int beat = (pos + j) % 8;
    int pair_index = beat < 4 ? ODD_COLOR_PAIR : EVEN_COLOR_PAIR;
    attron(COLOR_PAIR(pair_index));
    mvaddch(pattern_top, pattern_left + col + j, ch);
    attroff(COLOR_PAIR(pair_index));
  }

  mvaddch(pattern_top, pattern_left + col + len, '|');
  col += len + 1;
  pos += len;
}

void generate_pattern()
{
  if (pattern.Size() > 0) {
    auto buffer = pattern.Generate();
    move(9, 5); // move to begining of line
    clear_line();
    mvprintw(9, 5, buffer.c_str());
    refresh();
  }
}

bool process_keys()
{
  bool exit = false;

  int ch = getch();
  switch (ch) {
  case 'q':
    pattern.Add({Note, D4});
    break;
  case 'w':
    pattern.Add({Note, D8});
    break;
  case 'e':
    pattern.Add({Note, D16});
    break;
  case 'a':
    pattern.Add({Pause, D4});
    break;
  case 's':
    pattern.Add({Pause, D8});
    break;
  case 'd':
    pattern.Add({Pause, D16});
    break;
  case 'z':
    pattern.DeleteLast();
    break;
  case 'v':
    pattern.Clear();
    break;
  case 'r':
    generate_pattern();
    break;
  case 'x':
    exit = true;
    break;
  }

  return exit;
}

void draw_pattern()
{
  // Draw the pattern
  move(pattern_top, 4); // move to begining of line
  clear_line();

  // attron(A_UNDERLINE);
  mvprintw(5, 2, "Pattern length: %2d", pattern.GetPatternLen());
  // attroff(A_UNDERLINE);

  if (pattern.Size() > 0) {
    int col = 0;
    int pos = 0;

    mvaddch(pattern_top, pattern_left - 1, '|');

    for (size_t i = 0; i < pattern.Size(); i++) {
      Element e = pattern.Get(i);
      draw_element(e, col, pos);
    }
  }
  refresh();
}

int main()
{
  initscr();
  raw();                // Line buffering disabled
  keypad(stdscr, TRUE); // We get F1, F2 etc..
  noecho();             // Don't echo() while we do getch
  curs_set(0);          // Hide cursor

  if (has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }

  start_color();
  init_pair(ODD_COLOR_PAIR, COLOR_WHITE, COLOR_BLACK);
  init_pair(EVEN_COLOR_PAIR, COLOR_BLACK, COLOR_GREEN);

  border(0, 0, 0, 0, 0, 0, 0, 0);
  mvprintw(0, 2, " Pattern Editor ");
  mvprintw(2, 2, " Add Note: 1/4 - q, 1/8 - w, 1/16 - e");
  mvprintw(3, 2, "Add Pause: 1/4 - a, 1/8 - s, 1/16 - d");
  mvprintw(4, 2, " Commands: z - delete last, r - render, v - clear pattern, x - exit");
  refresh();

  bool exit;

  do {
    exit = process_keys();
    draw_pattern();
  } while (!exit);

  endwin();

  return 0;
}
