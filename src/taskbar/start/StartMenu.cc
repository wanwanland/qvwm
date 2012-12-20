/*
 * startmenu.cc
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
#include "taskbar/taskbar.h"
#include "taskbar/TaskbarButton.h"
#include "taskbar/start/StartMenu.h"
#include "taskbar/start/StartMenuItem.h"
#include "taskbar/start/StartButton.h"
#include "parser/qvwmrc.h"
#include "image/PixmapImage.h"
#include "lib/callback.h"
#include "lib/timer.h"
#include "lib/misc.h"
#include "lib/util.h"
#include "lib/Menu.h"

#include "bitmaps/logo_qvwm11.xpm"

QvImage* StartMenu::imgLogoMark;

StartMenu::StartMenu(MenuItem** mItem)
: Menu(mItem, NULL, fsStartMenu, rootQvwm,
       imgLogoMark->GetSize().width + LeftMargin, NameMargin, HiMargin)
{
  XSetWindowAttributes attributes;
  unsigned long valueMask;
  Dim szLogo = imgLogoMark->GetSize();

  attributes.background_pixel = StartMenuLogoColor.pixel;
  attributes.event_mask = ButtonPressMask | ExposureMask;
  valueMask = CWBackPixel | CWEventMask;
  
  logo = XCreateWindow(display, m_frame,
		       MenuFrameWidth, MenuFrameWidth,
		       szLogo.width, m_rc.height - MenuFrameWidth * 2,
		       0, CopyFromParent, InputOutput,
		       CopyFromParent, valueMask, &attributes);

  addWindow(logo);

  XMapWindow(display, logo);
}

StartMenu::~StartMenu()
{
  delWindow(logo);

  XDestroyWindow(display, logo);
}

/*
 * Map start menu according to taskbar position.
 */
void StartMenu::show()
{
  if (UseTaskbar)
    if (TaskbarAutoHide && taskBar->IsHiding())
      taskBar->ShowTaskbar();

  StartButton* sButton = taskBar->getStartButton();
  Rect rcSButton, rcTaskbar, rcRoot;
  Point pt;
  int gDir;

  sButton->SetState(Button::PUSH);
  sButton->paint();

  rootQvwm->SetFocus();

  if (UseTaskbar) {
    switch (taskBar->GetPos()) {
    case Taskbar::BOTTOM:
      rcTaskbar = taskBar->GetRect();

      pt.x = 2;
      pt.y = rcTaskbar.y + 2 - m_rc.height;
      m_fDir = RIGHT;
      gDir = GD_UP;
      break;
      
    case Taskbar::TOP:
      rcSButton = sButton->getRect();

      pt.x = 2;
      pt.y = rcSButton.height + 2;
      m_fDir = RIGHT;
      gDir = GD_DOWN;
      break;
      
    case Taskbar::LEFT:
      rcSButton = sButton->getRect();

      pt.x = 2;
      pt.y = rcSButton.height + 2;
      m_fDir = RIGHT;
      gDir = GD_RIGHT;
      break;
      
    case Taskbar::RIGHT:
      rcRoot = rootQvwm->GetRect();
      rcSButton = sButton->getRect();
	
      pt.x = rcRoot.width - m_rc.width - 2;
      pt.y = rcSButton.height + 2;
      m_fDir = LEFT;
      gDir = GD_LEFT;
      break;

    default:
      return;
    }
  }
  else {
    rcRoot = rootQvwm->GetRect();

    pt.x = 2;
    pt.y = rcRoot.height - m_rc.height - 2;
    gDir = GD_UP;
  }
    
  move(pt);
  setDirection(gDir);
  Menu::show();

  XSelectInput(display, sButton->getFrame(), 
	       ExposureMask | ButtonPressMask | ButtonReleaseMask |
	       Button1MotionMask);
}

/*
 * Unmap start menu.
 */
void StartMenu::hide()
{
  Menu::hide();

  StartButton* sButton = taskBar->getStartButton();
  sButton->SetState(Button::NORMAL);
  sButton->paint();

  XSelectInput(display, sButton->getFrame(),
	       ExposureMask | ButtonPressMask | ButtonReleaseMask |
	       Button1MotionMask | OwnerGrabButtonMask);

  if (UseTaskbar && TaskbarAutoHide) {
    Window junkRoot, junkChild;
    Point ptRoot, ptChild;
    unsigned int mask;

    XQueryPointer(display, root, &junkRoot, &junkChild,
		  &ptRoot.x, &ptRoot.y, &ptChild.x, &ptChild.y, &mask);
    if (InRect(ptRoot, taskBar->GetScreenRectOnShowing())) {
      BasicCallback* cb;
      cb = new Callback<Taskbar>(taskBar, &Taskbar::HideTaskbar);
      timer->SetTimeout(TaskbarHideDelay, cb);
    }
  }
}

void StartMenu::ExecFunction(MenuItem* item)
{
  if (item->getFunction() == Q_DIR)
    item->extractChildMenu();
  else
    Menu::ExecFunction(item);
}

/*
 * Redraw function
 */
void StartMenu::paint()
{
  Menu::paint();

  Dim szLogo = imgLogoMark->GetSize();
  Point offset(0, m_rc.height - szLogo.height - MenuFrameWidth * 2);
  
  imgLogoMark->Display(logo, offset);
}

/*
 * Event handlers
 */
void StartMenu::onMouseLeave(const XCrossingEvent& ev)
{
  // cancel the popup of the child menu
  timer->ResetTimeout(new Callback<Menu>(this, &Menu::PopupMenu));

  if (m_child) {
    if (checkEnterEvent(m_child))
      return;
  }
}

/*
 * Static functions
 */
void StartMenu::Initialize()
{
  if (StartMenuLogoImage) {
    imgLogoMark = CreateImageFromFile(StartMenuLogoImage, timer);
    if (imgLogoMark)
      return;
  }

  imgLogoMark = new PixmapImage(logo_qvwm11);

  StartMenuItem::Initialize();
}
