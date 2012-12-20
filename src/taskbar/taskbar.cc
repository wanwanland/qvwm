/*
 * taskbar.cc
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
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
// #include <X11/Xlocale.h>
#include "xlocale.h"
#include "main.h"
#include "window/qvwm.h"
#include "taskbar/taskbar.h"
#include "taskbar/TaskbarButton.h"
#include "taskbar/ButtonToolbar.h"
#include "taskbar/TaskbarBorder.h"
#include "taskbar/start/StartButton.h"
#include "taskbar/start/StartMenu.h"
#include "taskbar/tray/Indicator.h"
#include "taskbar/tray/TaskTray.h"
#include "taskbar/shortcut/ShortcutToolbar.h"
#include "taskbar/shortcut/TaskbarShortcut.h"
#include "wm/Event.h"
#include "desktop/icon.h"
#include "desktop/paging.h"
#include "desktop/desktop.h"
#include "pager/Pager.h"
#include "parser/qvwmrc.h"
#include "lib/callback.h"
#include "lib/timer.h"
#include "lib/MenuItem.h"
#include "lib/misc.h"

/*
 * XXX create internal class for each pos, which has rc and methods.
 * (not have frame)
 */

int Taskbar::BASE_HEIGHT;
int Taskbar::INC_HEIGHT;

Taskbar::Taskbar(unsigned int rows)
  : Component(Event::CLASS_TASKBAR), hiding(False)
{
  XSetWindowAttributes attributes;
  unsigned long valueMask;

  // create taskbar
  attributes.background_pixel = TaskbarColor.pixel;
  attributes.override_redirect = True;
  attributes.event_mask = ExposureMask | ButtonPressMask | ButtonReleaseMask | 
                          EnterWindowMask | LeaveWindowMask;
  valueMask = CWBackPixel | CWOverrideRedirect | CWEventMask;

  m_frame = XCreateWindow(display, root,
			  -10, -10, 10, 10,
			  0, CopyFromParent, InputOutput, CopyFromParent,
			  valueMask, &attributes);

  addWindow(m_frame);

  m_border = new TaskbarBorder(this);
  m_border->show();

  // create the start button
  m_sButton = new StartButton(this);
  m_sButton->show();

  // create a toolbar for taskbar buttons
  m_bToolBar = new ButtonToolbar(this);
  m_bToolBar->show();

  // create a toolbar for shortcut
  m_sToolBar = new ShortcutToolbar(this);

  if (createShortcutItems(imgLogo) > 0)
    m_sToolBar->show();

  // create the task tray
  m_taskTray = new TaskTray(this);
  m_taskTray->show();

  Dim dRoot(DisplayWidth(display, screen), DisplayHeight(display, screen));
  Dim base;
  int clockWidth = m_taskTray->getClockWidth();
  int width = m_sButton->getRect().width;

  BASE_HEIGHT = TaskbarButtonHeight + 6;
  INC_HEIGHT = TaskbarButtonHeight + ButtonToolbar::BETWEEN_SPACE;

  base.width = (width > clockWidth + 6) ? width : clockWidth + 6;
  base.height = (rows - 1) * INC_HEIGHT + BASE_HEIGHT;
  if (base.height > dRoot.height / 2)
    base.height = (dRoot.height / 2 - BASE_HEIGHT) / INC_HEIGHT * INC_HEIGHT
      + BASE_HEIGHT;

  rc[BOTTOM] = Rect(0, dRoot.height - base.height, dRoot.width, base.height);
  rc[TOP] = Rect(0, 0, dRoot.width, base.height);
  rc[LEFT] = Rect(0, 0, base.width, dRoot.height);
  rc[RIGHT] = Rect(dRoot.width - base.width, 0, base.width, dRoot.height);

  if (TaskbarImage) {
    imgTaskbar = CreateImageFromFile(TaskbarImage, timer);
    if (imgTaskbar) {
      imgTaskbar->SetBackground(m_frame);
    }
    else {
      delete [] TaskbarImage;
      TaskbarImage = NULL;
    }
  }

  ctrlMenu = NULL;
}

Taskbar::~Taskbar()
{
  delWindow(m_frame);

  XDestroyWindow(display, m_frame);

  if (TaskbarImage)
    QvImage::Destroy(imgTaskbar);

  delete m_border;
  delete m_sButton;
  delete m_bToolBar;
  delete m_sToolBar;
  delete m_taskTray;
  delete ctrlMenu;
}

int Taskbar::createShortcutItems(QvImage* imgDefIcon)
{
  MenuElem* mElem = TaskbarShortcutItem;
  QvImage* img;
  TaskbarShortcut* sc;
  int n = 0;

  while (mElem) {
    if (strcmp(mElem->file, "") != 0) {
      img = CreateImageFromFile(mElem->file, timer);
      if (img == NULL && imgDefIcon)
	img = imgDefIcon->Duplicate();
    }
    else {
      if (imgDefIcon)
	img = imgDefIcon->Duplicate();
    }

    if (mElem->func == Q_EXEC)
      sc = new TaskbarShortcut(m_sToolBar, img, mElem->exec, mElem->name);
    else
      sc = new TaskbarShortcut(m_sToolBar, img, mElem->func, mElem->name);

    m_sToolBar->addShortcut(sc);
    n++;

    mElem = mElem->next;
  }

  return n;
}

/*
 * MapTaskbar --
 *   Map the taskbar.
 */
void Taskbar::show()
{
  Component::show();

  raise();

  if (TaskbarAutoHide) {
    BasicCallback* cb;
    cb = new Callback<Taskbar>(this, &Taskbar::HideTaskbar);
    timer->SetTimeout(TaskbarHideDelay, cb);
  }
    
  if (!hiding)
    paging->RaisePagingBelt();
}

/*
 * Raise the taskbar.
 */
void Taskbar::raise()
{
  Window win[2];
  int nwindows = 0;
  Window menuFrame = Menu::GetMappedMenuFrame();

  if (OnTopTaskbar) {
    if (menuFrame != None)
      win[nwindows++] = menuFrame;
  }
  else
    win[nwindows++] = desktop.GetTopWindow();
  win[nwindows++] = m_frame;
  
  if (hiding) {
    paging->RaisePagingBelt();
    if (OnTopTaskbar)
      Component::raise();
    XRestackWindows(display, win, nwindows);
  }
  else {
    if (OnTopTaskbar)
      Component::raise();
    XRestackWindows(display, win, nwindows);
    paging->RaisePagingBelt();
  }
}

/*
 * Move the taskbar to the tp position.
 */
void Taskbar::placeTaskbar(TaskbarPos tp)
{
  pos = tp;

  reshape(rc[pos]);

  if (TaskbarImage) {
    switch (pos) {
    case BOTTOM:
      m_bToolBar->setBgImage(Point(0, 4));
      m_sToolBar->setBgImage(Point(0, 4));
      break;

    case TOP:
    case LEFT:
      m_bToolBar->setBgImage(Point(0, 0));
      m_sToolBar->setBgImage(Point(0, 0));
      break;

    case RIGHT:
      m_bToolBar->setBgImage(Point(4, 0));
      m_sToolBar->setBgImage(Point(4, 0));
      break;
    }
  }

  relocateComponents();

  if (TaskbarAutoHide)
    rcScreen = GetScreenRectOnHiding();
  else
    rcScreen = GetScreenRectOnShowing();

  desktop.RecalcAllWindows();
  desktop.RedrawAllIcons();

  if (UsePager)
    pager->RecalcPager();

  if (!hiding)
    paging->RaisePagingBelt();
}

/*
 * Drag the taskbar to move.
 */
void Taskbar::dragTaskbar(const Point& ptRoot)
{
  XEvent ev;
  Point ptNew;
  Rect rcRoot = rootQvwm->GetRect();
  Bool pointer = False;
  TaskbarPos posNew, posOld;

  if (DisableDesktopChange)
    return;

  /*
   * There is parts that cannot let taskbar to move.
   */
  switch (pos) {
  case BOTTOM:
    if (ptRoot.y == rcRoot.height-1 || ptRoot.y == rcRoot.height-2)
      return;
    break;

  case TOP:
    if (ptRoot.y == 0 || ptRoot.y == 1)
      return;
    break;

  case LEFT:
    if (ptRoot.x == 0 || ptRoot.x == 1)
      return;
    break;

  case RIGHT:
    if (ptRoot.x == rcRoot.width-1 || ptRoot.x == rcRoot.width-2)
      return;
    break;
  }
  
  if (!OpaqueMove) {
    XGrabServer(display);

    XDrawRectangle(display, root, gcXor, rc[pos].x, rc[pos].y,
		   rc[pos].width, rc[pos].height);
    posOld = pos;
  }

  while (1) {
    XMaskEvent(display,
	       Button1MotionMask | ButtonReleaseMask | ExposureMask |
	       ButtonPressMask | PointerMotionMask,
	       &ev);
    switch (ev.type) {
    case MotionNotify:
      ptNew = Point(ev.xbutton.x_root, ev.xbutton.y_root);
      posNew = desktop.GetDesktopArea(ptNew);

      if (OpaqueMove) {
	if (posNew != pos)
	  placeTaskbar(posNew);
      }
      else {
	if (posNew != posOld) {
	  XDrawRectangle(display, root, gcXor, rc[posOld].x, rc[posOld].y,
			 rc[posOld].width, rc[posOld].height);
	  XDrawRectangle(display, root, gcXor, rc[posNew].x, rc[posNew].y,
			 rc[posNew].width, rc[posNew].height);
	  posOld = posNew;
	}
      }
      break;

    case ButtonRelease:
      if (LockDragState) {
	pointer = True;
	XGrabPointer(display, root, True, ButtonPressMask | PointerMotionMask,
		     GrabModeAsync, GrabModeAsync, root, None, CurrentTime);
	break;
      }
      else
	goto decide;
      
    case ButtonPress:
      if (pointer) {
	XUngrabPointer(display, CurrentTime);
	goto decide;
      }
      break;

    case Expose:
      event.ExposeProc((const XExposeEvent &)ev);
      break;
    }
  }

decide:
  if (!OpaqueMove) {
    XDrawRectangle(display, root, gcXor, rc[posOld].x, rc[posOld].y,
		   rc[posOld].width, rc[posOld].height);

    ptNew = Point(ev.xbutton.x_root, ev.xbutton.y_root);
    posNew = desktop.GetDesktopArea(ptNew);
    placeTaskbar(posNew);

    XUngrabServer(display);
  }
}  

/*
 * Resize the taskbar.
 */
void Taskbar::dragResizeTaskbar(const Point& ptRoot)
{
  RectPt resize;
  XEvent ev;
  Rect rcNew = rc[pos], rcOrig = rc[pos], rcOld;
  Point ptNew;
  Rect rcRoot = rootQvwm->GetRect();
  Bool pointer = False;
  int ctype;

  if (DisableDesktopChange)
    return;

  if (!OpaqueResize) {
    XGrabServer(display);

    XDrawRectangle(display, root, gcXor,
		   rc[pos].x, rc[pos].y, rc[pos].width, rc[pos].height);
    rcOld = rc[pos];
  }

  while (1) {
    XMaskEvent(display,
	       Button1MotionMask | ButtonReleaseMask | ExposureMask |
	       ButtonPressMask | PointerMotionMask,
	       &ev);
    switch (ev.type) {
    case MotionNotify:
      ptNew = Point(ev.xbutton.x_root, ev.xbutton.y_root);
      resize = RectPt(0, 0, 0, 0);

      switch (pos) {
      case BOTTOM:
	resize.top = ptNew.y - ptRoot.y;
	break;

      case TOP:
	resize.bottom = ptNew.y - ptRoot.y;
	break;

      case LEFT:
	resize.right = ptNew.x - ptRoot.x;
	break;

      case RIGHT:
	resize.left = ptNew.x - ptRoot.x;
	break;
      }
      
      rcNew.x = rcOrig.x + resize.left;
      rcNew.y = rcOrig.y + resize.top;
      rcNew.width = rcOrig.width - resize.left + resize.right;
      rcNew.height = rcOrig.height - resize.top + resize.bottom;
      
      /*
       * Adjust.
       */
      switch (pos) {
      case BOTTOM:
      case TOP:
	if (rcNew.height > rcRoot.height/2)
	  rcNew.height = rcRoot.height / 2;
	rcNew.height = rcNew.height / INC_HEIGHT * INC_HEIGHT + 3;
	if (rcNew.height < 6)
	  rcNew.height = 6;
	break;

      case LEFT:
      case RIGHT:
	if (rcNew.width > rcRoot.width/2)
	  rcNew.width = rcRoot.width / 2;
	if (rcNew.width < 6)
	  rcNew.width = 6;
	break;
      }
      
      if (pos == BOTTOM)
	rcNew.y = rcRoot.height - rcNew.height;
      if (pos == RIGHT)
	rcNew.x = rcRoot.width - rcNew.width;
      
      if (OpaqueResize) {
	if (rcNew.width != rc[pos].width || rcNew.height != rc[pos].height) {
	  rc[pos] = rcNew;
	  placeTaskbar(pos);
	}
      }
      else {
	if (rcNew.width != rcOld.width || rcNew.height != rcOld.height) {
	  XDrawRectangle(display, root, gcXor,
			 rcOld.x, rcOld.y, rcOld.width, rcOld.height);
	  XDrawRectangle(display, root, gcXor,
			 rcNew.x, rcNew.y, rcNew.width, rcNew.height);
	  rcOld = rcNew;
	}
      }
      break;
      
    case ButtonRelease:
      if (LockDragState) {
	pointer = True;
	if (pos == BOTTOM || pos == TOP)
	  ctype = Y_RESIZE;
	else
	  ctype = X_RESIZE;
	XGrabPointer(display, root, True, ButtonPressMask | PointerMotionMask,
		     GrabModeAsync, GrabModeAsync, root, cursor[ctype],
		     CurrentTime);
	break;
      }
      else
	goto decide;

    case ButtonPress:
      if (pointer) {
	XUngrabPointer(display, CurrentTime);
	goto decide;
      }
      break;

    case Expose:
      event.ExposeProc((const XExposeEvent &)ev);
      break;
    }
  }

decide:
  if (!OpaqueResize) {
    XDrawRectangle(display, root, gcXor,
		   rcNew.x, rcNew.y, rcNew.width, rcNew.height);
    XUngrabServer(display);
  }

  rc[pos] = rcNew;
  placeTaskbar(pos);
}  

#define SB_LEFT  2  // start button
#define SB_RIGHT 1
#define SB_UP    2
#define SB_DOWN  0
#define IA_LEFT  5  // indicator area
#define IA_RIGHT 2
#define IA_UP    4
#define IA_DOWN  6
#define TB_UP    2  // taskbar button
#define TB_DOWN  2
#define TB_LEFT_L   2  // in left taskbar
#define TB_RIGHT_L  0
#define TB_LEFT_R   0  // in right taskbar
#define TB_RIGHT_R  2
#define T_EDGE    4  // taskbar

/*
 * Move and resize indicator area.
 */
void Taskbar::relocateComponents()
{
  TaskbarBorder::BorderPos transPos[4] = {
    TaskbarBorder::TOP, TaskbarBorder::BOTTOM,
    TaskbarBorder::RIGHT, TaskbarBorder::LEFT
  };

  m_border->setPosition(transPos[pos]);

  relocateStartButton();
  relocateTaskTray();
  if (m_sToolBar->isMapped())
    relocateShortcutToolbar();
  relocateButtonToolbar();

}

/*
 * Adjust the position and the size of the start button.
 */
void Taskbar::relocateStartButton()
{
  Rect rcSButton = m_sButton->getRect();

  if (pos == RIGHT)
    rcSButton.x = 4;
  else
    rcSButton.x = 2;

  if (pos == BOTTOM)
    rcSButton.y = 4;
  else
    rcSButton.y = 2;

  if (rc[pos].width > m_sButton->getMaxWidth() + 6)
    rcSButton.width = m_sButton->getMaxWidth();
  else {
    rcSButton.width = rc[pos].width - 6;
    if (rcSButton.width == 0)
      rcSButton.width = 1;
  }

  m_sButton->reshape(rcSButton);

  if (TaskbarImage) {
    m_sButton->SetBgImage(imgTaskbar, Point(rcSButton.x, rcSButton.y));
    m_sButton->SetBgActiveImage(imgTaskbar,
			      Point(rcSButton.x, rcSButton.y - 1));
    m_sButton->paint();
  }
}

/*
 * adjust task tray
 */
void Taskbar::relocateTaskTray()
{
  Rect rcSButton = m_sButton->getRect();
  Rect rcTaskTray;
  Point pt;

  m_taskTray->relocateAllIndicators();
  rcTaskTray = m_taskTray->getRect();

  switch (pos) {
  case BOTTOM:
    pt = Point(rc[BOTTOM].width - rcTaskTray.width - 2, 4);
    m_taskTray->move(pt);

    if (TaskbarImage)
      m_taskTray->setBgImage(Point(pt.x, pt.y + 4));
    break;

  case TOP:
    pt = Point(rc[TOP].width - rcTaskTray.width - 2, 2);
    m_taskTray->move(pt);

    if (TaskbarImage)
      m_taskTray->setBgImage(pt);
    break;

  case LEFT:
    if (rc[LEFT].width < rcSButton.width + rcTaskTray.width + 19) {
      if (rc[LEFT].width - 9 > rcTaskTray.width) {
	pt = Point((rc[LEFT].width - rcTaskTray.width - 4) / 2 + 1,
		   rc[LEFT].height - rcTaskTray.height - 6);
        m_taskTray->move(pt);
      }
      else {
	rcTaskTray.x = 3;
	rcTaskTray.y = rc[LEFT].height - rcTaskTray.height - 6;
	rcTaskTray.width = rc[LEFT].width - 9;
	if (rcTaskTray.width <= 0)
	  rcTaskTray.width = 1;
	m_taskTray->reshape(rcTaskTray);
      }
    }
    else {
      pt = Point(rc[LEFT].width - rcTaskTray.width - 8, SB_UP);
      m_taskTray->move(pt);
    }

    if (TaskbarImage)
      m_taskTray->setBgImage(pt);
    break;
    
  case RIGHT:
    if (rc[RIGHT].width < rcSButton.width + rcTaskTray.width + 19) {
      if (rc[RIGHT].width - 9 > rcTaskTray.width) {
	pt = Point((rc[RIGHT].width - rcTaskTray.width - 4) / 2 + 3,
		   rc[RIGHT].height - rcTaskTray.height - 6);
	m_taskTray->move(pt);
      }
      else {
	rcTaskTray.x = 4;
	rcTaskTray.y = rc[RIGHT].height - rcTaskTray.height - 6;
	rcTaskTray.width = rc[RIGHT].width - 9;
	if (rcTaskTray.width <= 0)
	  rcTaskTray.width = 1;
	m_taskTray->reshape(rcTaskTray);
      }
    }
    else {
      pt = Point(rc[RIGHT].width - rcTaskTray.width - 8, SB_UP);
      m_taskTray->move(pt);
    }

    if (TaskbarImage)
      m_taskTray->setBgImage(Point(pt.x + 4, pt.y));
    break;
  }
}

/*
 * relocate toolbar for taskbar button
 */
void Taskbar::relocateShortcutToolbar()
{
  Rect rcSButton = m_sButton->getRect();
  Rect rcToolbar;

  m_sToolBar->relocateAllShortcuts();
  rcToolbar = m_sToolBar->getRect();

  switch (pos) {
  case BOTTOM:
  case TOP:
    rcToolbar.x = rcSButton.width + SB_LEFT + SB_RIGHT;
    rcToolbar.y = T_EDGE;
    //    rcToolbar.width = ;
    rcToolbar.height = rc[pos].height - T_EDGE - TB_UP;
    break;

  case LEFT:
  case RIGHT:
    rcToolbar.x = T_EDGE + TB_LEFT_R;
    rcToolbar.y = rcSButton.height + SB_UP + SB_DOWN;
    rcToolbar.width = rc[pos].width - rcToolbar.x - TB_RIGHT_R;
    //    rcToolbar.height = ;
    break;
  }

  m_sToolBar->reshape(rcToolbar);
}

/*
 * relocate toolbar for taskbar button
 */
void Taskbar::relocateButtonToolbar()
{
  Rect rcSButton = m_sButton->getRect();
  Rect rcTaskTray = m_taskTray->getRect();
  Rect rcSToolbar, rcToolbar;

  if (m_sToolBar->isMapped())
    rcSToolbar = m_sToolBar->getRect();
  else
    rcSToolbar = Rect(0, 0, 0, 0);

  switch (pos) {
  case BOTTOM:
    rcToolbar.x = rcSButton.width + SB_LEFT + SB_RIGHT + rcSToolbar.width;
    rcToolbar.y = T_EDGE;

    rcToolbar.width = rc[BOTTOM].width - rcToolbar.x
      - (rcTaskTray.width + IA_LEFT + IA_RIGHT);
    rcToolbar.height = rc[BOTTOM].height - T_EDGE - TB_UP;
    break;

  case TOP:
    rcToolbar.x = rcSButton.width + SB_LEFT + SB_RIGHT + rcSToolbar.width;
    rcToolbar.y = TB_UP;

    rcToolbar.width =
      rc[TOP].width - rcToolbar.x - (rcTaskTray.width + IA_LEFT + IA_RIGHT);
    rcToolbar.height = rc[TOP].height - TB_DOWN - T_EDGE;
    break;

  case LEFT:
    rcToolbar.x = TB_LEFT_L;
    rcToolbar.y = rcSButton.height + SB_UP + SB_DOWN + rcSToolbar.height;

    rcToolbar.width = rc[LEFT].width - rcToolbar.x - TB_RIGHT_L - T_EDGE;
    rcToolbar.height = rc[LEFT].height - rcToolbar.y;
    if (rc[LEFT].width < rcSButton.width + rcTaskTray.width + 19)
      rcToolbar.height -= rcTaskTray.height + IA_UP + IA_DOWN;
    break;

  case RIGHT:
    rcToolbar.x = T_EDGE + TB_LEFT_R;
    rcToolbar.y = rcSButton.height + SB_UP + SB_DOWN + rcSToolbar.height;

    rcToolbar.width = rc[RIGHT].width - rcToolbar.x - TB_RIGHT_R;
    rcToolbar.height = rc[RIGHT].height - rcToolbar.y;
    if (rc[RIGHT].width < rcSButton.width + rcTaskTray.width + 19)
      rcToolbar.height -= rcTaskTray.height + IA_UP + IA_DOWN;
    break;
  }
    
  m_bToolBar->reshape(rcToolbar);
  m_bToolBar->relocateAllTaskbarButtons();
}

Bool Taskbar::CheckMenuMapped() const
{
  if (ctrlMenu && ctrlMenu->CheckMapped())
    return True;
  return False;
}

void Taskbar::UnmapMenu()
{
  if (ctrlMenu)
    ctrlMenu->hide();
} 

Menu* Taskbar::GetMenu()
{
  if (ctrlMenu == NULL) {
    MenuItem** mi = MenuItem::createMenuItems(TaskbarMenuItem);
    ctrlMenu = new Menu(mi, NULL, fsCtrlMenu, rootQvwm);
  }

  return ctrlMenu;
}


/*
 * Get the rectangle of screen except taskbar on taskbar showing.
 */
Rect Taskbar::GetScreenRectOnShowing() const
{
  Rect rcRoot = rootQvwm->GetRect();

  switch (pos) {
  case TOP:
    return Rect(0, rc[TOP].height, rcRoot.width,
		rcRoot.height - rc[TOP].height);

  case BOTTOM:
    return Rect(0, 0, rcRoot.width, rcRoot.height - rc[BOTTOM].height);

  case LEFT:
    return Rect(rc[LEFT].width, 0,
		rcRoot.width - rc[LEFT].width, rcRoot.height);

  case RIGHT:
    return Rect(0, 0, rcRoot.width - rc[RIGHT].width, rcRoot.height);
  }

  ASSERT(False);
  return Rect(0, 0, 0, 0);
}

/*
 * Get the rectangle of screen except taskbar on taskbar hiding.
 */
Rect Taskbar::GetScreenRectOnHiding() const
{
  Rect rcRoot = rootQvwm->GetRect();

  switch (pos) {
  case TOP:
    return Rect(0, 2, rcRoot.width, rcRoot.height - 2);

  case BOTTOM:
    return Rect(0, 0, rcRoot.width, rcRoot.height - 2);

  case LEFT:
    return Rect(2, 0, rcRoot.width - 2, rcRoot.height);

  case RIGHT:
    return Rect(0, 0, rcRoot.width - 2, rcRoot.height);
  }

  ASSERT(False);
  return Rect(0, 0, 0, 0);
}

void Taskbar::ShowTaskbar()
{
  if (!hiding)
    return;

  hiding = False;

  Rect rcRoot = rootQvwm->GetRect();
  Point pt;

  if (GradTaskbarMotion) {
    switch (pos) {
    case TOP:
      pt = Point(0, 2 - rc[TOP].height);
      break;
      
    case BOTTOM:
      pt = Point(0, rcRoot.height - 2);
      break;
      
    case LEFT:
      pt = Point(2 - rc[LEFT].width, 0);
      break;
      
    case RIGHT:
      pt = Point(rcRoot.width - 2, 0);
      break;
    }
    
    for (int i = GradTaskbarMotionSpeed - 1; i >= 0; i--) {
      XMoveWindow(display, m_frame,
		  rc[pos].x + (pt.x - rc[pos].x) * i / GradTaskbarMotionSpeed,
		  rc[pos].y + (pt.y - rc[pos].y) * i / GradTaskbarMotionSpeed);

      XFlush(display);
      usleep(10000);
    }
  }
  else
    XMoveWindow(display, m_frame, rc[pos].x, rc[pos].y);

  XClearWindow(display, m_frame);

  raise();

  if (UsePager && OnTopPager)
    pager->raise();
}

void Taskbar::HideTaskbar()
{
  if (hiding)
    return;

  if (startMenu && startMenu->CheckMapped())
    return;

  hiding = True;

  Rect rcRoot = rootQvwm->GetRect();
  Point pt;

  switch (pos) {
  case TOP:
    pt = Point(0, 2 - rc[TOP].height);
    break;

  case BOTTOM:
    pt = Point(0, rcRoot.height - 2);
    break;

  case LEFT:
    pt = Point(2 - rc[LEFT].width, 0);
    break;

  case RIGHT:
    pt = Point(rcRoot.width - 2, 0);
    break;
  }

  if (GradTaskbarMotion) {
    for (int i = GradTaskbarMotionSpeed - 1; i >= 0; i--) {
      XMoveWindow(display, m_frame,
		  pt.x + (rc[pos].x - pt.x) * i / GradTaskbarMotionSpeed,
		  pt.y + (rc[pos].y - pt.y) * i / GradTaskbarMotionSpeed);

      XFlush(display);
      usleep(10000);
    }
  }
  else
    XMoveWindow(display, m_frame, pt.x, pt.y);

  XClearWindow(display, m_frame);

  if (OnTopTaskbar)
    raise();
  if (UsePager && OnTopPager)
    pager->raise();
}

/*
 * Redraw function
 */
void Taskbar::paint()
{
  if (hiding)
    return;

  // line next to taskbar box
  Rect rcTaskTray = m_taskTray->getRect();
  Rect rcSButton;
  Point pt1, pt2;

  switch (pos) {
  case BOTTOM:
  case TOP:
    if (pos == BOTTOM) {
      pt1 = Point(rcTaskTray.x - 4, T_EDGE);
      pt2 = Point(rcTaskTray.x - 4, m_rc.height - TB_DOWN - 1);
    }
    else {
      pt1 = Point(rcTaskTray.x - 4, TB_UP);
      pt2 = Point(rcTaskTray.x - 4, m_rc.height - T_EDGE - 1);
    }

    XSetForeground(display, gc, darkGray.pixel);
    XDrawLine(display, m_frame, gc, pt1.x, pt1.y, pt2.x, pt2.y);
    XSetForeground(display, gc, white.pixel); 
    XDrawLine(display, m_frame, gc, pt1.x + 1, pt1.y, pt2.x + 1, pt2.y);
    break;

  case LEFT:
  case RIGHT:
    rcSButton = m_sButton->getRect();

    if (rc[pos].width < rcSButton.width + m_rc.width + 19) {
      if (pos == LEFT) {
	pt1 = Point(TB_LEFT_L, rcTaskTray.y - 3);
	pt2 = Point(m_rc.width - T_EDGE - TB_RIGHT_L, rcTaskTray.y - 3);
      }
      else {
	pt1 = Point(T_EDGE + TB_LEFT_R, rcTaskTray.y - 3);
	pt2 = Point(rc[pos].width - T_EDGE - TB_RIGHT_R, rcTaskTray.y - 3);
      }

      XSetForeground(display, gc, darkGray.pixel);
      XDrawLine(display, m_frame, gc, pt1.x, pt1.y, pt2.x, pt2.y);
      XSetForeground(display, gc, white.pixel); 
      XDrawLine(display, m_frame, gc, pt1.x, pt1.y + 1, pt2.x, pt2.y + 1);
    }
    break;
  }
}

/*
 * Event handlers
 */
void Taskbar::onMouseEnter(const XCrossingEvent& ev)
{
  if (!TaskbarAutoHide)
    return;

  if (ev.detail != NotifyInferior)
    if (!Menu::CheckAnyMenusMapped())
      if (hiding) {
	BasicCallback* cb;
	cb= new Callback<Taskbar>(this, &Taskbar::ShowTaskbar);
	timer->SetTimeout(TaskbarShowDelay, cb);
      }
}

void Taskbar::onMouseLeave(const XCrossingEvent& ev)
{
  if (!TaskbarAutoHide)
    return;

  if (ev.detail != NotifyInferior)
    if (!Menu::CheckAnyMenusMapped())
      if (!hiding)
	if (InRect(Point(ev.x_root, ev.y_root), GetScreenRectOnShowing())) {
	  BasicCallback* cb;
	  cb = new Callback<Taskbar>(this, &Taskbar::HideTaskbar);
	  timer->SetTimeout(TaskbarHideDelay, cb);
	}
}

void Taskbar::onButtonPress(const XButtonEvent& ev)
{
  if (ev.button != Button1)
    return;

  Menu::UnmapAllMenus(False);

  if (!OnTopTaskbar) {
    raise();

    if (UsePager && OnTopPager)
      pager->raise();
  }
}

void Taskbar::onButtonRelease(const XButtonEvent& ev)
{
  if (!(ev.state & Button3Mask))
    return;

  Menu::UnmapAllMenus(False);
  rootQvwm->SetFocus();

  ctrlMenu = GetMenu();

  ctrlMenu->adjustMenuPos(Point(ev.x_root, ev.y_root));
  ctrlMenu->show();
}
