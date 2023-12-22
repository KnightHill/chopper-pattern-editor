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

// symbols
const int NOTE_SYMBOL = '=';
const int PAUSE_SYMBOL = '-';

// custom colors
const int COLOR_BRIGHT_WHITE = 8;
const int COLOR_BRIGHT_RED = 9;
const int COLOR_BRIGHT_GREEN = 10;

// color pairs
const int ODD_COLOR_PAIR = 1;
const int EVEN_COLOR_PAIR = 2;

// pattern display coordinates
const int PATTERN_TOP = 8;
const int PATTERN_LEFT = 5;
const int CODE_TOP = 11;
const int CODE_LEFT = 5;

// screen dimentions
int SCREEN_ROWS, SCREEN_COLS;

void clear_line()
{
  int col, row;
  getyx(stdscr, row, col);
  move(row, 1);
  for (int c = 0; c < SCREEN_COLS - 2; c++) {
    addch(' ');
  }
}

void draw_element(Element el, int &col, int &pos)
{
  int ch = el.type == Note ? NOTE_SYMBOL : PAUSE_SYMBOL;
  int len = pattern.GetElementLen(el);

  for (int j = 0; j < len; j++) {
    int beat = (pos + j) % 8;
    int pair_index = beat < 4 ? ODD_COLOR_PAIR : EVEN_COLOR_PAIR;
    attron(COLOR_PAIR(pair_index));
    mvaddch(PATTERN_TOP, PATTERN_LEFT + col + j, ch);
    attroff(COLOR_PAIR(pair_index));
  }

  mvaddch(PATTERN_TOP, PATTERN_LEFT + col + len, '|');
  col += len + 1;
  pos += len;
}

void generate_pattern()
{
  if (pattern.Size() > 0) {
    auto buffer = pattern.Generate();
    move(CODE_TOP, CODE_LEFT); // move to begining of line
    clear_line();
    mvprintw(CODE_TOP, CODE_LEFT, buffer.c_str());
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
  move(PATTERN_TOP, 4); // move to begining of line
  clear_line();

  // attron(A_UNDERLINE);
  mvprintw(5, 2, "Pattern length: %2d", pattern.GetPatternLen());
  // attroff(A_UNDERLINE);

  if (pattern.Size() > 0) {
    int col = 0;
    int pos = 0;

    mvaddch(PATTERN_TOP, PATTERN_LEFT - 1, '|');

    for (size_t i = 0; i < pattern.Size(); i++) {
      Element e = pattern.Get(i);
      draw_element(e, col, pos);
    }
  }
  refresh();
}

void init_colors()
{
  start_color();
  init_color(COLOR_BRIGHT_WHITE, 1000, 1000, 1000);
  init_color(COLOR_BRIGHT_RED, 1000, 0, 0);
  init_color(COLOR_BRIGHT_GREEN, 0, 1000, 0);

  init_pair(ODD_COLOR_PAIR, COLOR_BRIGHT_WHITE, COLOR_BLACK);
  init_pair(EVEN_COLOR_PAIR, COLOR_BRIGHT_GREEN, COLOR_BLACK);
}

int main()
{
  initscr();
  raw();                                      // Line buffering disabled
  keypad(stdscr, TRUE);                       // We get F1, F2 etc..
  noecho();                                   // Don't echo() while we do getch
  curs_set(0);                                // Hide cursor
  getmaxyx(stdscr, SCREEN_ROWS, SCREEN_COLS); // get screen dimentions

  if (has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }

  init_colors();

  border(0, 0, 0, 0, 0, 0, 0, 0);
  mvprintw(0, 2, " Pattern Editor ");
  mvprintw(2, 2, "      Add Note: 1/4 - q, 1/8 - w, 1/16 - e");
  mvprintw(3, 2, "     Add Pause: 1/4 - a, 1/8 - s, 1/16 - d");
  mvprintw(4, 2, "      Commands: z - delete last, r - render, v - clear pattern, x - exit");
  mvhline(PATTERN_TOP - 1, 1, ACS_HLINE, SCREEN_COLS - 2);
  mvhline(PATTERN_TOP + 1, 1, ACS_HLINE, SCREEN_COLS - 2);
  refresh();

  bool exit;

  do {
    exit = process_keys();
    draw_pattern();
  } while (!exit);

  endwin();

  return 0;
}
