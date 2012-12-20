/*
 * fbutton.h
 *
 * Copyright (C) 1995-2001 Kenichi Kourai
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with qvwm; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef FBUTTON_H_
#define FBUTTON_H_

#include "lib/ImageButton.h"
#include "lib/ActionListener.h"

class Qvwm;
class QvImage;

/*
 * FrameButton class
 */
class FrameButton : public ImageButton {
private:
  Qvwm* m_qvWm;

private:
  static QvImage* imgButton[4];
  static char* desc[4];

  static const int BUTTON_WIDTH = 12;
  static const int BUTTON_HEIGHT = 10;

public:
  // Frame button name
  enum ButtonName { MINIMIZE, MAXIMIZE, CLOSE, RESTORE };

public:
  FrameButton(WindowTitlebar* titleBar, const Rect& rc, Qvwm* qvWm);

  void ChangeImage(ButtonName bn);

  void paintWithFocusChange();

  virtual void onButtonPress(const XButtonEvent& ev);

public:
  static void Initialize();
};


/*
 * FrameButton actions
 */
class MinimizeAction : public ActionListener {
private:
  Qvwm* m_qvWm;

public:
  MinimizeAction(Qvwm* qvWm) : m_qvWm(qvWm) {}

  virtual void perform();
};

class MaximizeRestoreAction : public ActionListener {
private:
  FrameButton* m_fb;
  Qvwm* m_qvWm;

public:
  MaximizeRestoreAction(FrameButton* fb, Qvwm* qvWm)
    : m_fb(fb), m_qvWm(qvWm) {}

  virtual void perform();
};

class CloseAction : public ActionListener {
private:
  Qvwm* m_qvWm;

public:
  CloseAction(Qvwm* qvWm) : m_qvWm(qvWm) {}

  virtual void perform();
};

#endif // FBUTTON_H_
