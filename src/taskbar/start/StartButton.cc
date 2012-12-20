/*
 * sbutton.cc
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
#include "taskbar/taskbar.h"
#include "taskbar/start/StartMenuItem.h"
#include "taskbar/start/StartButton.h"
#include "taskbar/start/StartMenu.h"
#include "parser/qvwmrc.h"
#include "image/Image.h"
#include "lib/callback.h"
#include "lib/Tooltip.h"

StartButton::StartButton(Taskbar* taskBar)
  : Button(taskBar->getFrame(), Rect(0, 0, 1, 1)), focus(False)
{
  XRectangle ink, log;
  int width;

  XmbTextExtents(fsTaskbar, StartButtonTitle, strlen(StartButtonTitle),
                 &ink, &log);
  maxWidth = log.width + 2 + StartButton::SYMBOL_SIZE + 4 + 2 + 4;

  resize(Dim(maxWidth, TaskbarButtonHeight));

  XSelectInput(display, m_frame,
	       ExposureMask | ButtonPressMask | ButtonReleaseMask |
	       Button1MotionMask | PointerMotionMask | EnterWindowMask |
	       LeaveWindowMask | OwnerGrabButtonMask);

  img = imgLogo;

  img->SetPreCallback(new Callback<StartButton>(this,
						&StartButton::DrawActive));

  m_toolTip->SetString(StartButtonMessage, &fsTaskbar);
}

StartButton::~StartButton()
{
  QvImage::Destroy(img);
}

/*
 * Redraw function
 */
void StartButton::paint()
{
  if (CheckState(PUSH)) {
    if (TaskbarImage) {
      m_imgBack->SetBackground(None);
      m_imgActiveBack->SetBackground(m_frame);
    }
  }
  else {
    if (TaskbarImage) {
      m_imgActiveBack->SetBackground(None);
      m_imgBack->SetBackground(m_frame);
    }
  }

  XClearWindow(display, m_frame);

  Point offset;

  offset.x = SB_MARGIN;
  offset.y = SB_MARGIN
    + (TaskbarButtonHeight - SB_MARGIN * 2 - SYMBOL_SIZE) / 2;

  /*
   * Draw the pixmap of the taskbar button.
   */
  if (CheckState(PUSH) & focus)
    img->Display(m_frame, Point(offset.x, offset.y + 1));
  else
    img->Display(m_frame, offset);

  // Draw the title of the taskbar button.
  DrawName();

  Button::paint();

  DrawActive();
}

/*
 * DrawName --
 *   Don't adjust the text in start button even if it is too long.
 */
void StartButton::DrawName()
{
  int y;
  XRectangle ink, log;
  char* name = StartButtonTitle;
  int len = strlen(name);

  XmbTextExtents(fsTaskbar, name, len, &ink, &log);
  y = (TaskbarButtonHeight - log.height) / 2 - log.y;

  if (CheckState(PUSH)) {
    if (UseBoldFont) {
      XSetForeground(display, ::gc, ButtonStringActiveColor.pixel);
      XmbDrawString(display, m_frame, fsBoldTaskbar, ::gc, 22, y+1, name, len);
    }
    else {
      XSetForeground(display, ::gc, ButtonStringActiveColor.pixel);
      XmbDrawString(display, m_frame, fsTaskbar, ::gc, 22, y+1, name, len);
      XmbDrawString(display, m_frame, fsTaskbar, ::gc, 23, y+1, name, len);
    }
  }
  else {
    if (UseBoldFont) {
      XSetForeground(display, ::gc, ButtonStringColor.pixel);
      XmbDrawString(display, m_frame, fsBoldTaskbar, ::gc, 22, y, name, len);
    }
    else {
      XSetForeground(display, ::gc, ButtonStringColor.pixel);
      XmbDrawString(display, m_frame, fsTaskbar, ::gc, 22, y, name, len);
      XmbDrawString(display, m_frame, fsTaskbar, ::gc, 23, y, name, len);
    }
  }
}

void StartButton::DrawActive()
{
  if (CheckState(PUSH)) {
    XSetForeground(display, gcDash, black.pixel);
    XDrawRectangle(display, m_frame, gcDash,
		   3, 3, m_rc.width - 7, m_rc.height - 7);
  }
}

/*
 * Event handlers
 */
void StartButton::onButtonPress(const XButtonEvent& ev)
{
  if (ev.button != Button1)
    return;

  m_toolTip->Disable();

  /*
   * Create start button if it isn't still created. The creation of start
   * button is delayed until it is needed.
   */
  if (startMenu == NULL) {
    MenuItem** mi = StartMenuItem::createMenuItems(StartMenuItem);
    startMenu = new StartMenu(mi);
  }

  if (startMenu->CheckMapped())
    startMenu->hide();
  else {
    Menu::UnmapAllMenus(False);
    startMenu->show();
  }
}
