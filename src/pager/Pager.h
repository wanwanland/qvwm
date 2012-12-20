/*
 * pager.h
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

#ifndef _PAGER_H_
#define _PAGER_H_

#include "lib/Component.h"
#include "lib/misc.h"
#include "lib/util.h"
#include "pager/PageArea.h"

class PageArea;
class PagerTitlebar;
class Miniature;

/*
 * Pager class
 */
class Pager : public Component {
private:
  Rect m_rcOrig;              // size and position of pages
  Point m_gravity;            // gravity offset (see util.cc)

  PageArea* m_pageArea;
  PagerTitlebar* m_titleBar;

  static const int BORDER_WIDTH = 3;
  static const int TITLE_HEIGHT = 7;

public:
  Pager(InternGeom geom);
  ~Pager();

  PageArea* getPageArea() const { return m_pageArea; }

  Rect getOrigRect() const { return m_rcOrig; }
  void setOrigRect(const Rect& rc) { m_rcOrig = rc; }

  virtual void show();
  virtual void raise();

  Bool IsPagerWindows(Window win);
  void CalcPagerPos();
  void RecalcPager();

  Point ConvertToPagerPos(const Point& pt) {
    return m_pageArea->ConvertToPagerPos(pt);
  }
  Rect ConvertToPagerSize(const Rect& rect) {
    return m_pageArea->ConvertToPagerSize(rect);
  }
  Point ConvertToRealPos(const Point& pt) {
    return m_pageArea->ConvertToRealPos(pt);
  }

  void lowerMiniature(Miniature* mini);
  void moveActivePage();

  virtual void paint();
  virtual void onButtonPress(const XButtonEvent& ev);
};

#endif // _PAGER_H_
