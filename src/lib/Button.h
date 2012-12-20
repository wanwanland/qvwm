/*
 * button.h
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

#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "lib/Component.h"
#include "lib/misc.h"

class Tooltip;
class QvImage;
class ActionListener;

/*
 * Button class
 */
class Button : public Component {
public:
  // Button state
  enum ButtonState { NORMAL, PUSH };

protected:
  Window m_parent;
  ButtonState m_state;

  ActionListener* m_listener;

  QvImage* m_imgBack;
  QvImage* m_imgActiveBack;

  Tooltip* m_toolTip;

public:
  Button(Window parent, const Rect& rc);
  virtual ~Button();

  void SetRect(const Rect& rect) { m_rc = rect; }
  Window GetParent() const { return m_parent; }
  void SetState(ButtonState bs) { m_state = bs; }
  Bool CheckState(ButtonState bs) const { return m_state == bs; }

  void setActionListener(ActionListener* listener);
  
  virtual void SetBgImage(QvImage* img, const Point& off);
  virtual void SetBgActiveImage(QvImage* img, const Point& off);

  virtual void performAction(ButtonState state);

  virtual void paint();
  virtual void onMouseEnter(const XCrossingEvent& ev);
  virtual void onMouseLeave(const XCrossingEvent& ev);
  virtual void onMouseMotion(const XMotionEvent& ev);
  virtual void onButtonPress(const XButtonEvent& ev);
};

#endif // _BUTTON_H_
