/*
 * ctrl_button.cc
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
#include "main.h"
#include "frame/CtrlButton.h"
#include "frame/WindowTitlebar.h"
#include "image/Image.h"
#include "window/qvwm.h"
#include "wm/Function.h"
#include "parser/qvwmrc.h"
#include "lib/Menu.h"

CtrlButton::CtrlButton(WindowTitlebar* titleBar, const Rect& rc, Qvwm* qvWm)
  : m_qvWm(qvWm)
{
  XSetWindowAttributes attributes;
  unsigned long valueMask;

  m_rc = rc;
  m_unmapFlag = False;

  attributes.event_mask = ButtonPressMask | ButtonReleaseMask | ExposureMask |
                          EnterWindowMask | LeaveWindowMask |
			  Button1MotionMask | OwnerGrabButtonMask;
  attributes.background_pixel = darkGray.pixel;
  valueMask = CWBackPixel | CWEventMask;

  m_frame = XCreateWindow(display, titleBar->getFrame(),
			  rc.x, rc.y, rc.width, rc.height,
			  0, CopyFromParent, InputOutput, CopyFromParent,
			  valueMask, &attributes);
  addWindow(m_frame);

  show();
}

CtrlButton::~CtrlButton()
{
  delWindow(m_frame);
  XDestroyWindow(display, m_frame);
}

void CtrlButton::setImage(QvImage* img)
{
  m_img = img;

  paint();
}

/*
 * Redraw functions
 */
void CtrlButton::paintWithFocusChange()
{
  if (m_qvWm->CheckFocus()) {
    if(TitlebarImage)
      Titlebar::m_imgTitlebar->SetBackground(None);

    if (TitlebarActiveImage)
      Titlebar::m_imgActiveTitlebar->SetBackground(m_frame);
    else
      XSetWindowBackground(display, m_frame, TitlebarActiveColor.pixel);
  }
  else {
    if (TitlebarActiveImage)
      Titlebar::m_imgActiveTitlebar->SetBackground(None);

    if(TitlebarImage)
      Titlebar::m_imgTitlebar->SetBackground(m_frame);
    else
      XSetWindowBackground(display, m_frame, TitlebarColor.pixel);
  }
  
  XClearWindow(display, m_frame);

  paint();
}

void CtrlButton::paint()
{
  m_img->Display(m_frame, Point(0, 0));
}

void CtrlButton::onButtonPress(const XButtonEvent& ev)
{
  if (ev.button != Button1)
    return;

  // unmap menu at button release if menu is already mapped
  if (m_qvWm->CheckMenuMapped())
    m_unmapFlag = True;
  else {
    Menu::UnmapAllMenus();
    
    m_qvWm->SetFocus();
    m_qvWm->RaiseWindow(True);
    m_qvWm->MapCtrlMenu();
  }
}

void CtrlButton::onButtonRelease(const XButtonEvent& ev)
{
  if (m_unmapFlag) {
    m_qvWm->UnmapMenu();
    m_unmapFlag = False;
  }
}

void CtrlButton::onDoubleClick(const XButtonEvent& ev)
{
  if (ev.button != Button1)
    return;

  if (m_qvWm->CheckMenuMapped())
    m_unmapFlag = True;
    
  if (ev.state & ControlMask)
    QvFunction::execFunction(Q_KILL, m_qvWm);
  else
    QvFunction::execFunction(Q_CLOSE, m_qvWm);
}
