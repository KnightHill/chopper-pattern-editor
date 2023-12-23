#include <cstring>
#include <ncurses.h>
#include "color_pairs.h"
#include "popup.h"

const int delay = 1000;

Popup::Popup(PopupType type) { m_color_pair = type == Info ? INFO_COLOR_PAIR : ERROR_COLOR_PAIR; }

void Popup::Show(const char *message)
{
  int height = 3;
  int width = 40;
  int starty = (LINES - height) / 2;
  int startx = (COLS - width) / 2;
  int center_offset = (width - strlen(message)) / 2;

  m_win = newwin(height, width, starty, startx);
  wbkgd(m_win, COLOR_PAIR(m_color_pair));
  box(m_win, 0, 0);
  mvwprintw(m_win, 1, center_offset, message);
  wrefresh(m_win);
  napms(delay);
  wbkgd(m_win, COLOR_PAIR(MAIN_COLOR_PAIR));
  wclear(m_win);
  wrefresh(m_win);
  delwin(m_win);
}