/*
 * sbutton.h
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

#ifndef _SBUTTON_H_
#define _SBUTTON_H_

#include "lib/Button.h"

/*
 * StartButton class
 */
class StartButton : public Button {
private:
  QvImage* img;                    // image of the left side
  Bool focus;
  int maxWidth;

private:
  static const int SYMBOL_SIZE = 16;	// pixmap size
  static const int SB_MARGIN = 4;

public:
  static Rect rcTButton;           // the size of taskbar button at that time
  static XFontSet* fsb;            // ref to bold font set

private:
  void DrawName();
  void DrawActive();

public:
  StartButton(Taskbar* taskBar);
  ~StartButton();
  
  Bool CheckFocus() const { return focus; }
  void SetFocus() { focus = True; }
  void ResetFocus() { focus = False; }
  int getMaxWidth() const { return maxWidth; }

  virtual void paint();

  virtual void onButtonPress(const XButtonEvent& ev);
};

#endif // _SBUTTON_H_
