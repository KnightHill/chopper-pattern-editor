#pragma once
#ifndef _POPUP_H
#define _POPUP_H

#ifdef __cplusplus

enum PopupType { Info, Error };

class Popup
{
public:
  Popup(PopupType type);
  ~Popup() {}

  void Show(const char *message);

private:
  WINDOW *m_win;
  int m_color_pair;
};

#endif
#endif