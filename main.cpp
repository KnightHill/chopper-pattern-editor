#include <cstdlib>
#include <string>
#include <vector>
#include <format>
#include <fstream>
#include <ncurses.h>
#include "pattern.h"

// http://ld2009.scusa.lsu.edu/php/ref.ncurses.html

using namespace std;
/*

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

// prototypes
int get_element_len(Element el);
int get_pattern_len();


vector<Element> pattern;
string buffer;
*/

Pattern pattern;

// pattern display coordinates
const int pattern_top = 7;
const int pattern_left = 5;

#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

void clr_line()
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

/*
int get_pattern_len()
{
  int len = 0;
  for (size_t i = 0; i < pattern.size(); i++) {
    Element el = pattern[i];
    len += get_element_len(el);
  }
  return len;
}

int get_element_len(Element el)
{
  switch (el.duration) {
  case D16:
  default:
    return 1;
  case D8:
    return 2;
  case D4:
    return 4;
  }
}

void generate()
{
  buffer.clear();
  buffer.append("{");
  buffer.append(format("{},{{", static_cast<int>(pattern.size())));

  for (size_t i = 0; i < pattern.size(); i++) {
    Element el = pattern[i];
    buffer.append(format("{{{},", el.type == Note ? 1 : 0));

    switch (el.duration) {
    case D4:
      buffer.append("D4}");
      break;
    case D8:
      buffer.append("D8}");
      break;
    case D16:
      buffer.append("D16}");
      break;
    }
    if (i < pattern.size() - 1)
      buffer.append(",");
  }

  buffer.append("}},");
}
*/

void draw_element(Element el, int &col, int &pos)
{
  int ch = el.type == Note ? '=' : '.';
  int len = pattern.GetElementLen(el);

  for (int j = 0; j < len; j++) {
    int beat = (pos + j) % 8;
    int pair_index = beat < 4 ? 1 : 2;
    attron(COLOR_PAIR(pair_index));
    mvaddch(pattern_top, pattern_left + col + j, ch);
    attroff(COLOR_PAIR(pair_index));
  }

  mvaddch(pattern_top, pattern_left + col + len, '|');
  col += len + 1;
  pos += len;
}

void generatePattern()
{
  auto buffer = pattern.Generate();
  move(9, 5); // move to begining of line
  clr_line();
  mvprintw(9, 5, buffer.c_str());
  refresh();

  fstream fs;
  fs.open("pattern.txt", std::fstream::out | std::fstream::app);
  fs << buffer << endl;
  fs.close();
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
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_BLACK, COLOR_GREEN);

  border(0, 0, 0, 0, 0, 0, 0, 0);
  mvprintw(0, 2, " Pattern Editor ");
  mvprintw(2, 2, " Add Note: 1/4 - q, 1/8 - w, 1/16 - e");
  mvprintw(3, 2, "Add Pause: 1/4 - a, 1/8 - s, 1/16 - d");
  mvprintw(4, 2, " Commands: z - delete last, r - render, v - clear pattern, x - exit");
  refresh();

  int ch;

  do {
    ch = getch();
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
      // if (pattern.size() > 0)
      //   generatePattern();
      break;
    }

    // Draw the pattern
    move(pattern_top, 4); // move to begining of line
    clr_line();

    int pattern_len = pattern.GetPatternLen();
    attron(A_UNDERLINE);
    mvprintw(5, 2, "Pattern length: %2d", pattern_len);
    attroff(A_UNDERLINE);

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

  } while (ch != 'x');

  endwin();

  return 0;
}
