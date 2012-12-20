/*
 * mini.cc
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
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include "main.h"
#include "window/qvwm.h"
#include "pager/Miniature.h"
#include "pager/Pager.h"
#include "taskbar/taskbar.h"
#include "parser/qvwmrc.h"
#include "desktop/paging.h"
#include "wm/Event.h"
#include "lib/Tooltip.h"
#include "lib/misc.h"

Miniature::Miniature(Pager* pager, const Rect& rc, Qvwm* qvWm)
  : m_pager(pager), focus(False), qvWm(qvWm)
{
  XSetWindowAttributes attributes;
  unsigned long valueMask;
  PageArea* pageArea = pager->getPageArea();

  m_rc = pager->ConvertToPagerSize(rc);

  attributes.border_pixel = darkGray.pixel;
  attributes.event_mask = ExposureMask | ButtonPressMask | ButtonReleaseMask |
                          Button2MotionMask | EnterWindowMask |
                          LeaveWindowMask | PointerMotionMask;
  valueMask = CWBorderPixel | CWEventMask;

  m_frame = XCreateWindow(display, pageArea->getFrame(),
			  m_rc.x, m_rc.y, m_rc.width, m_rc.height,
			  1, CopyFromParent, InputOutput, CopyFromParent,
			  valueMask, &attributes);

  addWindow(m_frame);

  toolTip = new Tooltip();
}

Miniature::~Miniature()
{
  delWindow(m_frame);

  XDestroyWindow(display, m_frame);

  delete toolTip;
}

void Miniature::SetFocus()
{
  focus = True;
  paint();
}

void Miniature::ResetFocus()
{
  focus = False;
  paint();
}

/*
 * Redraw function
 */
void Miniature::paint()
{
  if (focus)
    XSetWindowBackground(display, m_frame, MiniatureActiveColor.pixel);
  else
    XSetWindowBackground(display, m_frame, MiniatureColor.pixel);
  
  XClearWindow(display, m_frame);
}

/*
 * Event handlers
 */
void Miniature::onMouseEnter(const XCrossingEvent& ev)
{
  toolTip->SetString(qvWm->GetName(), &fsTitle);
  toolTip->SetTimer();
}

void Miniature::onMouseLeave(const XCrossingEvent& ev)
{
  toolTip->Disable();
}

void Miniature::onMouseMotion(const XMotionEvent& ev)
{
  if (!toolTip->IsMapped())
    toolTip->ResetTimer();
}

void Miniature::onButtonPress(const XButtonEvent& ev)
{
  Point ptRoot(ev.x_root, ev.y_root);

  if (ev.button == Button1)
    selectMiniature(ev);
  else if (ev.button == Button2)
    dragMiniature(ptRoot);
}

/*
 * Switch to the page and give focus to the corresponding window.
 */
void Miniature::selectMiniature(const XButtonEvent& ev)
{
  PageArea* pageArea = m_pager->getPageArea();

  toolTip->Disable();
  m_pager->raise();

  if (UseTaskbar && !taskBar->IsHiding())
    paging->RaisePagingBelt();

  // send this event to the below window
  XButtonEvent xev = ev;
  Window junkChild;

  XTranslateCoordinates(display, root, pageArea->getFrame(),
			ev.x_root, ev.y_root, &xev.x, &xev.y, &junkChild);
  pageArea->onButtonPress(xev);

  if (ClickToFocus) {
    qvWm->SetFocus();
    qvWm->RaiseWindow(True);
  }
}

/*
 * Drag miniature window.
 */
void Miniature::dragMiniature(const Point& ptRoot)
{
  XEvent ev;
  Point ptOld, ptNew, ptMini, ptReal;
  Rect rcReal;
  Bool pointer = False;
  PageArea* pageArea = m_pager->getPageArea();
  
  toolTip->Disable();
  m_pager->raise();

  if (UseTaskbar && !taskBar->IsHiding())
    paging->RaisePagingBelt();

  if (ClickToFocus) {
    qvWm->SetFocus();
    qvWm->RaiseWindow(True);
    qvWm->AdjustPage();
  }

  XGrabPointer(display, m_frame, True, Button2MotionMask | ButtonReleaseMask,
	       GrabModeAsync, GrabModeAsync, pageArea->getFrame(), None,
	       CurrentTime);

  ptOld = ptRoot;
  ptMini = Point(m_rc.x, m_rc.y);

  if (!qvWm->CheckStatus(MAXIMIZE_WINDOW)) {
    while (1) {
      XMaskEvent(display,
		 Button2MotionMask | ButtonReleaseMask | ExposureMask |
		 ButtonPressMask | PointerMotionMask,
		 &ev);
      switch (ev.type) {
      case MotionNotify:
	ptNew = Point(ev.xmotion.x_root, ev.xmotion.y_root);
	ptMini.x += ptNew.x - ptOld.x;
	ptMini.y += ptNew.y - ptOld.y;
	ptOld = ptNew;
	
	move(ptMini);

	if (OpaqueMove) {
	  ptReal = m_pager->ConvertToRealPos(ptMini);
	  XMoveWindow(display, qvWm->GetFrameWin(),
		      ptReal.x - paging->origin.x,
		      ptReal.y - paging->origin.y);
	  
	  rcReal = qvWm->GetRect();
	  rcReal.x = ptReal.x;
	  rcReal.y = ptReal.y;
	  qvWm->ConfigureNewRect(rcReal);
      }
      break;
      
      case ButtonRelease:
	if (LockDragState) {
	  pointer = True;
	  XChangeActivePointerGrab(display,
				   ButtonPressMask | PointerMotionMask,
				   None, CurrentTime);
	  break;
	}
	else
	  goto decide;

      case ButtonPress:
	if (pointer)
	  goto decide;
	break;

      case Expose:
	event.ExposeProc((const XExposeEvent &)ev);
	break;
      }
    }
  }

decide:
  if (!OpaqueMove) {
    ptReal = m_pager->ConvertToRealPos(ptMini);
    XMoveWindow(display, qvWm->GetFrameWin(),
		ptReal.x - paging->origin.x, ptReal.y - paging->origin.y);
	  
    rcReal = qvWm->GetRect();
    rcReal.x = ptReal.x;
    rcReal.y = ptReal.y;
    qvWm->ConfigureNewRect(rcReal);
  }
  XUngrabPointer(display, CurrentTime);
}

/*
 * RestackMiniatures --
 *   Restack miniatures according to minis's order.
 */
void Miniature::RestackMiniatures(Miniature* minis[], int nminis)
{
  Window* wins = new Window[nminis];

  for (int i = 0; i < nminis; i++)
    wins[i] = minis[i]->getFrame();

  XRestackWindows(display, wins, nminis);

  delete [] wins;
}
