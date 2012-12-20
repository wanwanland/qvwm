/*
 * mini.h
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

#ifndef MINI_H_
#define MINI_H_

#include "lib/Component.h"

class Qvwm;
class Tooltip;

/*
 * Miniature class
 *   Miniture window in pager according to real window.
 */
class Miniature : public Component {
private:
  Pager* m_pager; 
  Qvwm* qvWm;
  Bool focus;
  Tooltip* toolTip;

private:
  void selectMiniature(const XButtonEvent& ev);
  void dragMiniature(const Point& ptRoot);

public:
  Miniature(Pager* pager, const Rect& rect, Qvwm* qvWm);
  ~Miniature();

  void SetFocus();
  void ResetFocus();

  virtual void paint();
  virtual void onMouseEnter(const XCrossingEvent& ev);
  virtual void onMouseLeave(const XCrossingEvent& ev);
  virtual void onMouseMotion(const XMotionEvent& ev);
  virtual void onButtonPress(const XButtonEvent& ev);

public:
  static void RestackMiniatures(Miniature* minis[], int nminis);
};

#endif // MINI_H_
