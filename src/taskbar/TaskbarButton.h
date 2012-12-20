/*
 * tbutton.h
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

#ifndef _TBUTTON_H_
#define _TBUTTON_H_

#include "lib/Button.h"

class QvImage;
class Qvwm;
class ButtonToolbar;

/*
 * TaskbarButton
 */
class TaskbarButton : public Button {
private:
  QvImage* img;                    // image of the left side
  char* name;                      // taskbar button title
  char* shortname;                 // short title fitted to button size
  Bool focus;

  ButtonToolbar* m_toolBar;
  Qvwm* qvWm;                      // ptr to corresponding qvwm

private:
  static const int TB_MARGIN = 4;

public:
  static QvImage* imgTile;

  static const int SYMBOL_SIZE = 16;	// pixmap size
  static int BUTTON_HEIGHT;		// tbutton height

private:
  virtual void DrawName();

public:
  TaskbarButton(Taskbar* taskBar, const Rect& rc, QvImage* image, Qvwm* qvwm);
  ~TaskbarButton();

  void ChangeName();
  void CalcShortName();
  Bool CheckFocus() const { return focus; }
  void SetFocus() { focus = True; }
  void ResetFocus() { focus = False; }
  QvImage* GetImage() const { return img; }
  void SetImage(QvImage* image);

  Point getAbsLocation() const;

  virtual void show();
  virtual void hide();
  virtual void reshape(const Rect& rc);

  virtual void performAction(ButtonState bs);

  virtual void paint();
  virtual void onMouseEnter(const XCrossingEvent& ev);
  virtual void onButtonPress(const XButtonEvent& ev);
  virtual void onButtonRelease(const XButtonEvent& ev);

public:
  static void Initialize();
};

#endif // _TBUTTON_H_
