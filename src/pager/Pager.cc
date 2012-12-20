/*
 * pager.cc
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "window/qvwm.h"
#include "pager/Pager.h"
#include "pager/PagerTitlebar.h"
#include "pager/PageArea.h"
#include "pager/Miniature.h"
#include "wm/Event.h"
#include "parser/qvwmrc.h"
#include "desktop/paging.h"
#include "taskbar/taskbar.h"
#include "lib/util.h"
#include "lib/callback.h"
#include "lib/misc.h"

Pager::Pager(InternGeom geom)
  : m_rcOrig(geom.rc), m_gravity(geom.gravity)
{
  XSetWindowAttributes attributes;
  unsigned long valueMask;

  /*
   * If width or height is 0, set default value.
   */
  if (m_rcOrig.width == 0)
    m_rcOrig.width = 48;
  if (m_rcOrig.height == 0)
    m_rcOrig.height = 48;

  /*
   * Adjust pager size and position.
   */
  m_rcOrig.width = RoundDown(m_rcOrig.width, paging->rcVirt.width);
  m_rcOrig.height = RoundDown(m_rcOrig.height, paging->rcVirt.height);
  if (m_gravity.x == EAST)
    m_rcOrig.x = DisplayWidth(display, screen) + m_rcOrig.x - m_rcOrig.width;
  if (m_gravity.y == SOUTH)
    m_rcOrig.y = DisplayHeight(display, screen) + m_rcOrig.y - m_rcOrig.height;

  CalcPagerPos();

  /*
   * Create the frame window for pager.
   */
  attributes.background_pixel = PagerColor.pixel;
  attributes.override_redirect = True;
  attributes.event_mask = ExposureMask;
  valueMask = CWBackPixel | CWEventMask | CWOverrideRedirect;
  
  m_frame = XCreateWindow(display, root,
			  m_rc.x, m_rc.y, m_rc.width, m_rc.height,
			  0, CopyFromParent, InputOutput, CopyFromParent,
			  valueMask, &attributes);
  
  addWindow(m_frame);

  /*
   * Create the title window.
   */
  Rect rcTitle(BORDER_WIDTH, BORDER_WIDTH, m_rcOrig.width, TITLE_HEIGHT);
  m_titleBar = new PagerTitlebar(this, rcTitle);

  /*
   * Create the window for the miniature of whole virtual screen.
   */
  Rect rcPageArea(BORDER_WIDTH, TITLE_HEIGHT + BORDER_WIDTH + 1,
		  m_rcOrig.width, m_rcOrig.height);
  m_pageArea = new PageArea(this, rcPageArea);
}

Pager::~Pager()
{
  delWindow(m_frame);

  XDestroyWindow(display, m_frame);

  delete m_pageArea;
  delete m_titleBar;
}

void Pager::show()
{
  m_pageArea->moveActivePage();

  Component::show();
  raise();
}

void Pager::raise()
{
  if (OnTopPager)
    Component::raise();
  else {
    Window wins[2];

    wins[0] = desktop.GetTopWindow();
    wins[1] = m_frame;
    XRestackWindows(display, wins, 2);
  }
}

void Pager::CalcPagerPos()
{
  Rect rect;

  if (UseTaskbar) {
    if (TaskbarAutoHide)
      rect = taskBar->GetScreenRectOnHiding();
    else
      rect = taskBar->GetScreenRectOnShowing();
  }

  m_rc.x = m_rcOrig.x + rect.x + BORDER_WIDTH * m_gravity.x;
  m_rc.y = m_rcOrig.y + rect.y;
  if (m_gravity.y == CENTER)
    m_rc.y -= BORDER_WIDTH + TITLE_HEIGHT + 1;
  else if (m_gravity.y == SOUTH)
    m_rc.y -= BORDER_WIDTH * 2 + TITLE_HEIGHT + 1;

  Rect rcRoot = rootQvwm->GetRect();

  if (UseTaskbar) {
    switch (taskBar->GetPos()) {
    case Taskbar::BOTTOM:
    case Taskbar::TOP:
      // South??Gravity
      if (m_gravity.y == SOUTH)
	m_rc.y -= rcRoot.height - rect.height;
      break;
      
    case Taskbar::LEFT:
    case Taskbar::RIGHT:
      // ??EastGravity
      if (m_gravity.x == EAST) {
	m_rc.x -= rcRoot.width - rect.width;
      }
      break;
    }
  }

  m_rc.width = m_rcOrig.width + BORDER_WIDTH * 2;
  m_rc.height = m_rcOrig.height + BORDER_WIDTH * 2 + TITLE_HEIGHT + 1;
}

/*
 * RecalcPager --
 *   Recalc the position of pager according to rcScreen and move the pager.
 */
void Pager::RecalcPager()
{
  CalcPagerPos();
  move(Point(m_rc.x, m_rc.y));
}

void Pager::lowerMiniature(Miniature* mini)
{
  Window junkRoot, junkParent, *children;
  unsigned int nChildren;
  Window wins[2];

  // find lowest pager mini window to place this mini window below it
  XQueryTree(display, m_pageArea->getFrame(), &junkRoot, &junkParent,
	     &children, &nChildren);

  ASSERT(nChildren >= 2);
  ASSERT(children[0] == pager->GetVisualWin()); // lowest should be visual

  wins[0] = children[1];
  wins[1] = mini->getFrame();

  XRestackWindows(display, wins, 2);

  XFree(children);
}

void Pager::moveActivePage()
{
  m_pageArea->moveActivePage();
}

/*
 * Event handler
 */
void Pager::paint()
{
  XPoint xp[3];

  xp[0].x = m_rc.width - 2;
  xp[0].y = 0;
  xp[1].x = 0;
  xp[1].y = 0;
  xp[2].x = 0;
  xp[2].y = m_rc.height - 2;
  
  XSetForeground(display, gc, gray.pixel);
  XDrawLines(display, m_frame, gc, xp, 3, CoordModeOrigin);

  xp[0].x = m_rc.width - 1;
  xp[0].y = 0;
  xp[1].x = m_rc.width - 1;
  xp[1].y = m_rc.height - 1;
  xp[2].x = 0;
  xp[2].y = m_rc.height - 1;

  XSetForeground(display, ::gc, darkGrey.pixel);
  XDrawLines(display, m_frame, ::gc, xp, 3, CoordModeOrigin);

  xp[0].x = m_rc.width - 3;
  xp[0].y = 1;
  xp[1].x = 1;
  xp[1].y = 1;
  xp[2].x = 1;
  xp[2].y = m_rc.height - 3;

  XSetForeground(display, ::gc, white.pixel);
  XDrawLines(display, m_frame, ::gc, xp, 3, CoordModeOrigin);
  
  xp[0].x = m_rc.width - 2;
  xp[0].y = 1;
  xp[1].x = m_rc.width - 2;
  xp[1].y = m_rc.height - 2;
  xp[2].x = 1;
  xp[2].y = m_rc.height - 2;

  XSetForeground(display, ::gc, darkGray.pixel);
  XDrawLines(display, m_frame, ::gc, xp, 3, CoordModeOrigin);
}

void Pager::onButtonPress(const XButtonEvent& ev)
{
  if (ev.button != Button1)
    return;

  raise();

  if (UseTaskbar) {
    if (OnTopTaskbar && !OnTopPager)
      taskBar->raise();
    if (!taskBar->IsHiding())
      paging->RaisePagingBelt();
  }
}
