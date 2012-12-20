/*
 * button.cc
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
#include "lib/misc.h"
#include "lib/Button.h"
#include "wm/Event.h"
#include "parser/qvwmrc.h"
#include "image/Image.h"
#include "lib/Tooltip.h"
#include "lib/ActionListener.h"

Button::Button(Window parent, const Rect& rc)
  : m_parent(parent)
{
  XSetWindowAttributes attributes;
  unsigned long valueMask;

  m_rc = rc;

  // initialize state
  m_state = NORMAL;

  attributes.background_pixel = ButtonColor.pixel;
  attributes.event_mask = ExposureMask | ButtonPressMask | ButtonReleaseMask |
                          EnterWindowMask | LeaveWindowMask |
                          PointerMotionMask ;
  valueMask = CWBackPixel | CWEventMask;

  m_frame = XCreateWindow(display, parent,
			  rc.x, rc.y, rc.width, rc.height,
			  0, CopyFromParent, InputOutput, CopyFromParent,
			  valueMask, &attributes);

  addWindow(m_frame);

  m_toolTip = new Tooltip();

  m_imgBack = NULL;
  m_imgActiveBack = NULL;

  m_listener = NULL;
}

Button::~Button()
{
  delWindow(m_frame);

  XDestroyWindow(display, m_frame);

  QvImage::Destroy(m_imgBack);
  QvImage::Destroy(m_imgActiveBack);

  delete m_toolTip;

  if (m_listener)
    delete m_listener;
}  

void Button::setActionListener(ActionListener* listener)
{
  if (m_listener)
    delete m_listener;

  m_listener = listener;
}

// img must be passed as only a pointer
void Button::SetBgImage(QvImage* img, const Point& off)
{
  if (img) {
    QvImage::Destroy(m_imgBack);
    m_imgBack = img->GetOffsetImage(off);
  }
}  

// image must be passed as only a pointer
void Button::SetBgActiveImage(QvImage* img, const Point& off)
{
  if (img) {
    QvImage::Destroy(m_imgActiveBack);
    m_imgActiveBack = img->GetOffsetImage(off);
  }
}  

/*
 * Redraw handler
 */
void Button::paint()
{
  XPoint xp[3];

  xp[0].x = m_rc.width - 2;
  xp[0].y = 0;
  xp[1].x = 0;
  xp[1].y = 0;
  xp[2].x = 0;
  xp[2].y = m_rc.height - 2;

  if (m_state == PUSH)
    XSetForeground(display, gc, darkGrey.pixel);
  else
    XSetForeground(display, gc, white.pixel);
  XDrawLines(display, m_frame, gc, xp, 3, CoordModeOrigin);

  xp[0].x = m_rc.width - 1;
  xp[0].y = 0;
  xp[1].x = m_rc.width - 1;
  xp[1].y = m_rc.height - 1;
  xp[2].x = 0;
  xp[2].y = m_rc.height - 1;

  if (m_state == PUSH)
    XSetForeground(display, gc, white.pixel);
  else
    XSetForeground(display, gc, darkGrey.pixel);
  XDrawLines(display, m_frame, gc, xp, 3, CoordModeOrigin);

  xp[0].x = m_rc.width - 3;
  xp[0].y = 1;
  xp[1].x = 1;
  xp[1].y = 1;
  xp[2].x = 1;
  xp[2].y = m_rc.height - 3;

  if (m_state == PUSH)
    XSetForeground(display, gc, darkGray.pixel);
  else
    XSetForeground(display, gc, gray.pixel);
  XDrawLines(display, m_frame, gc, xp, 3, CoordModeOrigin);

  xp[0].x = m_rc.width - 2;
  xp[0].y = 1;
  xp[1].x = m_rc.width - 2;
  xp[1].y = m_rc.height - 2;
  xp[2].x = 1;
  xp[2].y = m_rc.height - 2;
  
  if (m_state == PUSH)
    XSetForeground(display, gc, gray.pixel);
  else
    XSetForeground(display, gc, darkGray.pixel);
  XDrawLines(display, m_frame, gc, xp, 3, CoordModeOrigin);
}

void Button::performAction(ButtonState state)
{
  if (state == PUSH)
    if (m_listener)
      m_listener->perform();
}

/*
 * Event handlers
 */
void Button::onMouseEnter(const XCrossingEvent& ev)
{
  m_toolTip->SetTimer();
}

void Button::onMouseLeave(const XCrossingEvent& ev)
{
  m_toolTip->Disable();
}

void Button::onMouseMotion(const XMotionEvent& ev)
{
  if (!m_toolTip->IsMapped())
    m_toolTip->ResetTimer();
}

void Button::onButtonPress(const XButtonEvent& ev)
{
  XEvent xev;
  ButtonState bs;

  if (ev.button != Button1)
    return;

  m_toolTip->Disable();

  m_state = PUSH;
  paint();

  while (1) {
    XMaskEvent(display, EnterWindowMask | LeaveWindowMask | ButtonReleaseMask |
	       ExposureMask, &xev);
    switch (xev.type) {
    case EnterNotify:
      if (xev.xcrossing.window == m_frame) {
	m_state = PUSH;
	paint();
      }
      break;

    case LeaveNotify:
      if (xev.xcrossing.window == m_frame) {
	m_state = NORMAL;
	paint();
      }
      break;

    case ButtonRelease:
      if (xev.xbutton.state & Button1Mask) {
	bs = m_state;
	m_state = NORMAL;

	performAction(bs);
	paint();
	return;
      }
      break;

    case Expose:
      event.ExposeProc((const XExposeEvent &)xev);
      break;
    }
  }
}
