/*
 * frame.cc
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
#include "frame/WindowTitlebar.h"
#include "taskbar/TaskbarButton.h"
#include "taskbar/taskbar.h"
#include "frame/FrameButton.h"
#include "frame/CtrlButton.h"
#include "frame/AllBorders.h"
#include "frame/AllEdges.h"
#include "frame/ClientArea.h"
#include "pager/Miniature.h"
#include "pager/Pager.h"
#include "desktop/paging.h"
#include "parser/qvwmrc.h"
#include "lib/timer.h"
#include "lib/callback.h"
#include "lib/misc.h"

/*
 * CreateFrame --
 *   Create a frame (most lower base window).
 */
void Qvwm::CreateFrame(const Rect& rect)
{
  XSetWindowAttributes attributes;
  unsigned long valueMask;

  attributes.event_mask = SubstructureRedirectMask | ButtonPressMask |
                          ButtonReleaseMask | EnterWindowMask |
			  LeaveWindowMask | ExposureMask | FocusChangeMask;
  attributes.background_pixel = FrameColor.pixel;
  valueMask = CWBackPixel | CWEventMask;

  ASSERT(paging);

  frame = XCreateWindow(display, root,
			rect.x - paging->origin.x, rect.y - paging->origin.y,
			rect.width, rect.height,
			0, CopyFromParent, InputOutput, CopyFromParent,
			valueMask, &attributes);
  addWindow(frame);

  if (CheckFlags(THIN_BORDER))
    XSetWindowBorderWidth(display, frame, 1);
}

/*
 * CalcFramePos --
 *   Calculate the size of parts from frame window size.
 */
void Qvwm::CalcFramePos(const Rect& rcFrame, Rect* rcParent, Rect* rcTitle,
			Rect* rcBorder, Rect* rcInner)
{
  int borderWidth, topBorder, titleHeight, titleEdge;

  GetBorderAndTitle(borderWidth, topBorder, titleHeight, titleEdge);

  ASSERT(rcParent);

  /*
   * Parent window pos & size.
   */
  rcParent->x = borderWidth;
  rcParent->y = topBorder + titleHeight + titleEdge;
  rcParent->width = rcFrame.width - borderWidth * 2;
  rcParent->height = rcFrame.height - (borderWidth + topBorder + titleHeight
				       + titleEdge);

  ASSERT(rcTitle);

  /*
   * Title window pos & size.
   */
  rcTitle->x = topBorder;
  rcTitle->y = topBorder;
  rcTitle->width = rcFrame.width - topBorder * 2;
  rcTitle->height = TITLE_HEIGHT;

  ASSERT(rcBorder);

  rcBorder->x = 0;
  rcBorder->y = 0;
  rcBorder->width = rcFrame.width;
  rcBorder->height = rcFrame.height;

  ASSERT(rcInner);

  rcInner->x = topBorder;
  rcInner->y = topBorder + titleHeight;
  rcInner->width = rcFrame.width - topBorder * 2;
  rcInner->height = rcFrame.height - topBorder * 2 - titleHeight;
}    

/*
 * CalcWindowPos --  rcOrig -> rc
 *   Calculate the correct window position.
 */
void Qvwm::CalcWindowPos(const Rect& base)
{
  Point gravity;
  Rect rcTaskbar;
  int borderWidth, topBorder, titleHeight, titleEdge;

  GetBorderAndTitle(borderWidth, topBorder, titleHeight, titleEdge);
  gravity = GetGravityOffset();

  rc.x = base.x + rcScreen.x + (borderWidth - bwOrig) * gravity.x;
  rc.y = base.y + rcScreen.y;
  if (CheckFlags(STICKY)) {
    rc.x += paging->origin.x;
    rc.y += paging->origin.y;
  }
  if (gravity.y == CENTER)
    rc.y -= topBorder + titleHeight + titleEdge - bwOrig;
  else if (gravity.y == SOUTH)
    rc.y -= borderWidth + topBorder + titleHeight + titleEdge - bwOrig*2;

  if (UseTaskbar) {
    switch (taskBar->GetPos()) {
    case Taskbar::BOTTOM:
    case Taskbar::TOP:
      // South??Gravity
      if (gravity.y == SOUTH) {
	rcTaskbar = taskBar->GetRect();
	rc.y -= rcTaskbar.height;
      }
      break;
    case Taskbar::LEFT:
    case Taskbar::RIGHT:
      // ??EastGravity
      if (gravity.x == EAST) {
	rcTaskbar = taskBar->GetRect();
	rc.x -= rcTaskbar.width;
      }
      break;
    }
  }

  rc.width = base.width + borderWidth * 2;
  rc.height = base.height + borderWidth + topBorder + titleHeight + titleEdge;
}

/*
 * RedrawWindow --
 *   Redraw the window decoration.
 */
void Qvwm::RedrawWindow(Bool delay)
{
  Rect rcParent, rcTitle, rcBorder, rcInner;

  CalcFramePos(rc, &rcParent, &rcTitle, &rcBorder, &rcInner);

  ASSERT(paging);

  XMoveResizeWindow(display, frame,
		    rc.x - paging->origin.x, rc.y - paging->origin.y,
		    rc.width, rc.height);

  if (delay)
    m_clientArea->reshapeFast(rcParent);
  else
    m_clientArea->reshape(rcParent);

  if (CheckFlags(BORDER))
    m_allBorders->reshape(rcBorder);

  if (CheckFlags(BORDER_EDGE))
    m_allInnerEdges->reshape(rcInner);

  if (CheckFlags(TITLE))
    m_titleBar->reshape(rcTitle);

  if (CheckFlags(SHAPED))
    SetShape();

  if (UsePager)
    mini->reshape(pager->ConvertToPagerSize(rc));
}

void Qvwm::RecalcWindow()
{
  ASSERT(paging);

  if (CheckStatus(MAXIMIZE_WINDOW)) {
    rc = GetFixSize(rcScreen, hints.max_width, hints.max_height,
		    hints.width_inc, hints.height_inc,
		    hints.base_width, hints.base_height);
    ConfigureNewRect(Rect(rc.x + paging->origin.x, rc.y + paging->origin.y,
			  rc.width, rc.height));
    RedrawWindow();
  }
  else {
    CalcWindowPos(rcOrig);
    SendConfigureEvent();
    XMoveWindow(display, frame,
		rc.x - paging->origin.x, rc.y - paging->origin.y);
  }
}
