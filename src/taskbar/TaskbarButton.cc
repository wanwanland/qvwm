/*
 * tbutton.cc
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
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/xpm.h>
#include "main.h"
#include "window/qvwm.h"
#include "taskbar/taskbar.h"
#include "taskbar/TaskbarButton.h"
#include "taskbar/ButtonToolbar.h"
#include "taskbar/start/StartButton.h"
#include "taskbar/start/StartMenu.h"
#include "desktop/paging.h"
#include "desktop/desktop.h"
#include "parser/qvwmrc.h"
#include "image/PixmapImage.h"
#include "lib/Tooltip.h"
#include "lib/misc.h"
#include "lib/util.h"

#include "bitmaps/tile0.xpm"

QvImage* TaskbarButton::imgTile;

TaskbarButton::TaskbarButton(Taskbar* taskBar, const Rect& rc, QvImage* image,
			     Qvwm* qvwm)
  : Button(taskBar->getButtonToolbar()->getFrame(), rc),
    m_toolBar(taskBar->getButtonToolbar()),
    img(image), focus(False), qvWm(qvwm)
{
  name = shortname = NULL;
  ChangeName();

  m_toolTip->SetString(name, &fsTaskbar);
}

TaskbarButton::~TaskbarButton()
{
  delete [] shortname;
  delete [] name;

  QvImage::Destroy(img);
}

// image must be passed as a duplicated object
void TaskbarButton::SetImage(QvImage* image)
{
  if (img)
    QvImage::Destroy(img);

  img = image;
}

void TaskbarButton::ChangeName()
{
  XTextProperty xtp;
  char** cl;
  int n;

  delete [] name;

  if (XGetWMIconName(display, qvWm->GetWin(), &xtp) != 0) {
    XmbTextPropertyToTextList(display, &xtp, &cl, &n);
    if (cl) {
      name = new char[strlen(cl[0]) + 1];
      strcpy(name, cl[0]);
      XFreeStringList(cl);
    }
    else {
      name = new char[9];
      strcpy(name, "Untitled");
    }
  }
  else {
    name = new char[9];
    strcpy(name, "Untitled");
  }

  CalcShortName();

  m_toolTip->SetString(name, &fsTaskbar);
}

void TaskbarButton::CalcShortName()
{
  delete [] shortname;

  shortname = GetFixName(fsTaskbar, name,
			 m_rc.width - TB_MARGIN * 2 - SYMBOL_SIZE - 2);
}

/*
 * Map taskbar button.
 */
void TaskbarButton::show()
{
  if (!qvWm->CheckFlags(TRANSIENT)) {
    m_toolBar->relocateAllTaskbarButtons();
    Button::show();
  }
}

/*
 * UnmapButton --
 *   Unmap taskbar button.
 */
void TaskbarButton::hide()
{
  if (!qvWm->CheckFlags(TRANSIENT)) {
    Button::hide();
    m_toolBar->relocateAllTaskbarButtons();
  }
}

void TaskbarButton::reshape(const Rect& rc)
{
  Button::reshape(rc);

  CalcShortName();
}

Point TaskbarButton::getAbsLocation() const
{
  Point ptToolbar = m_toolBar->getAbsLocation();

  return Point(ptToolbar.x + m_rc.x, ptToolbar.y + m_rc.y);
}

/*
 * Give the focus to the taskbar button and the corresponding window.
 */
void TaskbarButton::performAction(ButtonState bs)
{
  if (bs == PUSH) {
    ASSERT(qvWm);

    if (focus) {
      if (RestoreMinimize) {
	qvWm->MinimizeWindow();
	return;
      }
    }
    else {
      if (qvWm->CheckStatus(MINIMIZE_WINDOW))
	qvWm->RestoreWindow();
      else {
	if (!qvWm->CheckFocus() && !qvWm->CheckFlags(NO_FOCUS))
	  qvWm->SetFocus();
	qvWm->RaiseWindow(True);
	qvWm->AdjustPage();
      }
    }

    /*
     * When button is released, state is set to NORMAL in default.
     * So state sets to PUSH here.
     */
    if (qvWm->CheckFlags(NO_FOCUS))
      m_state = NORMAL;
    else
      m_state = PUSH;
  }
}
  
/*
 * Redraw function
 */
void TaskbarButton::paint()
{
  /*
   * No button for the transient window.
   */
  if (qvWm->CheckFlags(TRANSIENT))
    return;

  if (CheckState(PUSH) && focus) {
    if (TaskbarImage) {
      m_imgBack->SetBackground(None);
      m_imgActiveBack->SetBackground(m_frame);
      XClearWindow(display, m_frame);
    }
    else {
      imgTile->SetBackground(m_frame);
      XClearWindow(display, m_frame);

      XSetForeground(display, gc, white.pixel);
      XDrawLine(display, m_frame, gc, 2, 2, m_rc.width, 2);
    }
  }
  else {
    if (TaskbarImage) {
      m_imgActiveBack->SetBackground(None);
      m_imgBack->SetBackground(m_frame);
    }
    else {
      if (CheckState(PUSH))
	XSetWindowBackground(display, m_frame, ButtonActiveColor.pixel);
      else
	XSetWindowBackground(display, m_frame, ButtonColor.pixel);
    }

    XClearWindow(display, m_frame);
  }

  Point offset;

  offset.x = TB_MARGIN;
  offset.y = TB_MARGIN
    + (TaskbarButtonHeight - TB_MARGIN * 2 - SYMBOL_SIZE) / 2;

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
}

/*
 * DrawName --
 *   Draw the text taskbar button.
 */
void TaskbarButton::DrawName()
{
  ASSERT(qvWm);

  if (qvWm->CheckFlags(TRANSIENT))
    return;

  XRectangle ink, log;
  int y;

  XmbTextExtents(fsTaskbar, shortname, strlen(shortname), &ink, &log);
  y = (TaskbarButtonHeight - log.height) / 2 - log.y;

  if (CheckState(PUSH) & focus) {
    if (UseBoldFont) {
      XSetForeground(display, gc, ButtonStringActiveColor.pixel);
      XmbDrawString(display, m_frame, fsBoldTaskbar, gc,
		    TB_MARGIN + SYMBOL_SIZE + 2, y + 1,
		    shortname, strlen(shortname));
    }
    else {
      XSetForeground(display, gc, ButtonStringActiveColor.pixel);
      XmbDrawString(display, m_frame, fsTaskbar, gc,
		    TB_MARGIN + SYMBOL_SIZE + 2, y + 1,
		    shortname, strlen(shortname));
      XmbDrawString(display, m_frame, fsTaskbar, gc,
		    TB_MARGIN + SYMBOL_SIZE + 3, y + 1,
		    shortname, strlen(shortname));
    }
    if (strlen(name) > strlen(shortname)) {
      XRectangle xr[3] =
	{{TB_MARGIN + SYMBOL_SIZE + 2 + log.width + 2, y - 1, 2, 2},
         {TB_MARGIN + SYMBOL_SIZE + 2 + log.width + 5, y - 1, 2, 2},
         {TB_MARGIN + SYMBOL_SIZE + 2 + log.width + 8, y - 1, 2, 2}};

      XFillRectangles(display, m_frame, gc, xr, 3);
    }
  }
  else {
    XSetForeground(display, gc, ButtonStringColor.pixel);
    XmbDrawString(display, m_frame, fsTaskbar, gc,
		  TB_MARGIN + SYMBOL_SIZE + 2, y,
		  shortname, strlen(shortname));

    if (strlen(name) > strlen(shortname)) {
      XRectangle xr[3] =
	{{TB_MARGIN + SYMBOL_SIZE + 2 + log.width + 2, y - 2, 1, 2},
	 {TB_MARGIN + SYMBOL_SIZE + 2 + log.width + 5, y - 2, 1, 2},
	 {TB_MARGIN + SYMBOL_SIZE + 2 + log.width + 8, y - 2, 1, 2}};

      XFillRectangles(display, m_frame, gc, xr, 3);
    }
  }
}

/*
 * Event handlers
 */
void TaskbarButton::onMouseEnter(const XCrossingEvent& ev)
{
  // if abbreviated by '...'
  if (strcmp(name, shortname) != 0)
    Button::onMouseEnter(ev);
}

void TaskbarButton::onButtonPress(const XButtonEvent & ev)
{
  if (ev.button != Button1)
    return;

  Menu::UnmapAllMenus(False);
  
  Button::onButtonPress(ev);
}

void TaskbarButton::onButtonRelease(const XButtonEvent& ev)
{
  Point ptRoot(ev.x_root, ev.y_root);
  Bool noFocus = False;

  if (!(ev.state & Button3Mask))
    return;

  /*
   * When button3 is released, map menu if start menu isn't mapped.
   */
  if (startMenu == NULL || !startMenu->CheckMapped()) {
    ASSERT(Qvwm::focusQvwm);

    if (Qvwm::focusQvwm->CheckMenuMapped())
      Qvwm::focusQvwm->UnmapMenu();

    ASSERT(qvWm);
    /*
     * Temporarily, give the focus.
     */
    if (qvWm->CheckFlags(NO_FOCUS)) {
      qvWm->ResetFlags(NO_FOCUS);
      noFocus = True;
    }
    
    if (!qvWm->CheckFocus())
      qvWm->SetFocus();
    qvWm->RaiseWindow(True);
    qvWm->AdjustPage();
    
    ASSERT(qvWm->ctrlMenu);

    qvWm->ctrlMenu->SetQvwm(qvWm);
    qvWm->ctrlMenu->adjustMenuPos(ptRoot);
    qvWm->ctrlMenu->show();

    if (noFocus)
      qvWm->SetFlags(NO_FOCUS);
  }
}

/*
 * Static functions
 */
void TaskbarButton::Initialize()
{
  imgTile = new PixmapImage(tile0);
}
