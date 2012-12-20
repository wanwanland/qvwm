/*
 * fbutton.cc
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
#include <X11/xpm.h>
#include "main.h"
#include "window/qvwm.h"
#include "frame/FrameButton.h"
#include "frame/WindowTitlebar.h"
#include "parser/qvwmrc.h"
#include "image/PixmapImage.h"
#include "lib/Tooltip.h"
#include "lib/misc.h"
#include "bitmaps/button_min.xpm"
#include "bitmaps/button_max.xpm"
#include "bitmaps/button_close.xpm"
#include "bitmaps/button_restore.xpm"

QvImage* FrameButton::imgButton[4];
char* FrameButton::desc[4];

FrameButton::FrameButton(WindowTitlebar* titleBar, const Rect& rc, Qvwm* qvWm)
  : ImageButton(titleBar, NULL, rc), m_qvWm(qvWm)
{
  if (TitlebarImage) {
    SetBgImage(Titlebar::m_imgTitlebar, Point(rc.x, rc.y));
    SetBgActiveImage(Titlebar::m_imgActiveTitlebar, Point(rc.x, rc.y));
  }
}

void FrameButton::ChangeImage(ButtonName bn)
{
  QvImage::Destroy(m_img);
  m_img = imgButton[bn]->Duplicate();

  m_toolTip->SetString(desc[bn], &fsTitle);
}

/*
 * Redraw funtion
 */
void FrameButton::paintWithFocusChange()
{
  if (m_qvWm->CheckFocus()) {
    if (TitlebarImage)
      m_imgBack->SetBackground(None);

    if (TitlebarActiveImage)
      m_imgActiveBack->SetBackground(m_frame);
    else
      XSetWindowBackground(display, m_frame, ButtonActiveColor.pixel);
  }
  else {
    if (TitlebarActiveImage)
      m_imgActiveBack->SetBackground(None);

    if (TitlebarImage)
      m_imgBack->SetBackground(m_frame);
    else
      XSetWindowBackground(display, m_frame, ButtonColor.pixel);
  }

  XClearWindow(display, m_frame);

  paint();
}

/*
 * Event handlers
 */
void FrameButton::onButtonPress(const XButtonEvent& ev)
{
  if (ev.button != Button1)
    return;

  ASSERT(qvWm);

  if (!m_qvWm->CheckFocus())
    m_qvWm->SetFocus();

  m_qvWm->RaiseWindow(True);

  ImageButton::onButtonPress(ev);
}

/*
 * Static functions
 */
void FrameButton::Initialize()
{
  imgButton[MINIMIZE] = new PixmapImage(button_min);
  imgButton[MAXIMIZE] = new PixmapImage(button_max);
  imgButton[CLOSE] = new PixmapImage(button_close);
  imgButton[RESTORE] = new PixmapImage(button_restore);

  desc[MINIMIZE] = MinimizeButtonMessage;
  desc[MAXIMIZE] = MaximizeButtonMessage;
  desc[CLOSE] = CloseButtonMessage;
  desc[RESTORE] = RestoreButtonMessage;
}

/*
 * FrameButton actions
 */
void MinimizeAction::perform()
{
  m_qvWm->MinimizeWindow();
}

void MaximizeRestoreAction::perform()
{
  if (m_qvWm->CheckStatus(MAXIMIZE_WINDOW)) {
    m_fb->ChangeImage(FrameButton::MAXIMIZE);
    m_qvWm->RestoreWindow();
  }
  else {
    m_fb->ChangeImage(FrameButton::RESTORE);
    m_qvWm->MaximizeWindow();
  }
}

void CloseAction::perform()
{
  QvFunction::execFunction(Q_CLOSE, m_qvWm);
}
