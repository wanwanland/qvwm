/*
 * menu.cc
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
#include <sys/time.h>
#include <X11/Xlib.h>
#include <X11/xpm.h>
#include "main.h"
#include "key.h"
#include "window/qvwm.h"
#include "taskbar/taskbar.h"
#include "taskbar/start/StartMenu.h"
#include "parser/qvwmrc.h"
#include "desktop/icon.h"
#include "desktop/paging.h"
#include "image/PixmapImage.h"
#include "lib/MenuItem.h"
#include "lib/callback.h"
#include "lib/timer.h"
#include "lib/Menu.h"
#include "lib/util.h"
#include "lib/misc.h"

Menu::MenuDir	Menu::m_fDir = Menu::RIGHT;
QvImage 	*Menu::m_imgMenuBack, *Menu::m_imgActiveMenuBack;

Menu::Menu(MenuItem** mi, Menu* parent, XFontSet fs, Qvwm* qvWm,
	   int leftMargin, int nameMargin, int hiMargin)
  : m_mi(mi), m_parent(parent), m_leftMargin(leftMargin),
    m_nameMargin(nameMargin), m_hiMargin(hiMargin),
    m_fs(fs), m_qvWm(qvWm)
{
  int maxWidth = 0;
  Dim maxImgSize(0, 0);
  int i, sep = 0;
  XSetWindowAttributes attributes;
  unsigned long valueMask;

  m_mapped = False;
  m_child = NULL;
  m_focusMenuItem = NULL;
  m_delayedMenuItem = NULL;
  m_extractedMenuItem = NULL;

  for (i = 0; mi[i] != NULL; i++) {
    int width = GetRealWidth(fs, mi[i]->getName());

    if (width > maxWidth)
      maxWidth = width;

    QvImage* img = mi[i]->getImage();

    if (mi[i]->getFunction() != Q_SEPARATOR && img) {
      Dim size = img->GetSize();
      if (size.width > maxImgSize.width)
	maxImgSize.width = size.width;
      if (size.height > maxImgSize.height)
	maxImgSize.height = size.height;
    }

    mi[i]->setMenu(this);
  }

  m_nitems = i;

  if (maxImgSize.width == 0)
    m_imageArea = leftMargin + 9 + nameMargin;  // no images
  else {
    m_imageArea = leftMargin + maxImgSize.width + nameMargin;
    if (m_imageArea < 14)
      m_imageArea = 14;
  }

  if (maxImgSize.height == 0)
    m_itemHeight = 16;  // no images
  else {
    m_itemHeight = maxImgSize.height + hiMargin * 2;
    if (m_itemHeight < 16)
      m_itemHeight = 16;
  }

  /*
   * Create frame window.
   */
  attributes.save_under = DoesSaveUnders(ScreenOfDisplay(display, screen));
  attributes.background_pixel = MenuColor.pixel;
  attributes.event_mask = ExposureMask | KeyPressMask | LeaveWindowMask |
                          EnterWindowMask;
  valueMask = CWSaveUnder | CWBackPixel | CWEventMask;
  
  m_frame = XCreateWindow(display, root,
			  0, 0, 1, 1,
			  0, CopyFromParent, InputOutput, CopyFromParent,
			  valueMask, &attributes);

  addWindow(m_frame);

  if (MenuImage)
    m_imgMenuBack->SetBackground(m_frame);

  m_rc.width = m_imageArea + maxWidth + 24 + MenuFrameWidth * 2;

  /*
   * Create item windows for menu.
   */
  for (i = 0; i < m_nitems; i++) {
    Rect rect;

    rect.x = MenuFrameWidth;
    rect.y = MenuFrameWidth + (i - sep) * m_itemHeight + sep * SeparatorHeight;
    rect.width = m_rc.width - MenuFrameWidth * 2;
    rect.height = m_itemHeight;

    mi[i]->createFrame(rect);

    if (mi[i]->getFunction() == Q_SEPARATOR)
      sep++;
  }

  m_rc.height = (m_nitems - sep) * m_itemHeight + sep * SeparatorHeight
    + MenuFrameWidth * 2;

  resize(Dim(m_rc.width, m_rc.height));
}

Menu::~Menu()
{
  delWindow(m_frame);

  XDestroyWindow(display, m_frame);

  for (int i = 0; i < m_nitems; i++)
    delete m_mi[i];

  delete [] m_mi;
}

void Menu::show()
{
  PlaySound(MenuPopupSound);

  // focusMenuItem is kept for Q_ACTION after unmapping a menu
  if (m_focusMenuItem) {
    m_focusMenuItem->resetItemFocus();
    m_focusMenuItem = NULL;
  }

  if (GradMenuMap && m_mapped)
    Component::hide();

  Rect rc = getRect();

  if (GradMenuMap)
    resize(Dim(1, 1));

  Component::show();
  raise();

  // make this menu show gradually
  if (GradMenuMap) {
    // child menus expand only horizontaly
    if (m_parent)
      m_gDir &= ~(GD_DOWN | GD_UP);

    int divide = ((rc.width + rc.height) / 2) * GradMenuMapSpeed / 100;
    if (divide == 0)
      divide = 1;

    for (int i = 1; i <= divide; i++) {
      Dim size(rc.width * i / divide, rc.height * i / divide);

      if (size.width == 0)
	size.width = 1;
      if (size.height == 0)
	size.height = 1;

      if (m_gDir & GD_RIGHT) {
	if (m_gDir & GD_DOWN) {
	  // right & down
	  resize(size);
	}
	else if (m_gDir & GD_UP) {
	  // right & up
	  reshape(Rect(rc.x, rc.y + rc.height - size.height,
		       size.width, size.height));
	}
	else {
	  // right
	  resize(size);
	}
      }
      else if (m_gDir & GD_LEFT) {
	if (m_gDir & GD_DOWN) {
	  // left & down
	  reshape(Rect(rc.x + rc.width - size.width, rc.y,
		       size.width, size.height));
	}
	else if (m_gDir & GD_UP) {
	  // left & up
	  reshape(Rect(rc.x + rc.width - size.width,
		       rc.y + rc.height - size.height,
		       size.width, size.height));
	}
	else {
	  // left
	  reshape(Rect(rc.x + rc.width - size.width, rc.y,
		       size.width, rc.height));
	}
      }
      else if (m_gDir & GD_DOWN) {
	// down
	resize(size);
      }
      else {
	// up
	reshape(Rect(rc.x, rc.y + rc.height - size.height,
		     rc.width, size.height));
      }

      XFlush(display);
      usleep(10000);
    }
  }

  m_mapped = True;
  m_fDir = RIGHT;

  /*
   * Take off the focus from the window when any menus are shown.
   */
  if (m_qvWm->CheckMapped()) {
    if (m_qvWm != rootQvwm)
      for (int i = 1; i < 4; i++)
	XGrabButton(display, i, 0, m_qvWm->GetWin(), True, ButtonPressMask,
		    GrabModeAsync, GrabModeAsync, None, cursor[0]);
  }

  XSetInputFocus(display, m_frame, RevertToParent, CurrentTime);

  menuKey->GrabKeys(m_frame);
}

/*
 * UnmapMenu --
 *   Unmap menu.  Keep focusMenuItem until the next MapMenu().
 */
void Menu::hide()
{
  int i;

  timer->ResetTimeout(new Callback<Menu>(this, &Menu::PopupMenu));

  m_mapped = False;

  if (m_child != NULL)
    m_child->hide();

  menuKey->UngrabKeys(m_frame);

  /*
   * Yield the focus.
   */
  if (m_qvWm->CheckMapped()) {
    if (m_qvWm != rootQvwm)
      for (i = 1; i < 4; i++)
	XUngrabButton(display, i, 0, m_qvWm->GetWin());

    /*
     * Yield the focus to the parent, if any.
     */
    if (m_parent != NULL)
      XSetInputFocus(display, m_parent->getFrame(), RevertToParent,
		     CurrentTime);
    else
      Qvwm::SetFocusToActiveWindow();
  }
  
  Component::hide();

  if (m_parent != NULL) {
    m_parent->m_child = NULL;
    m_parent->m_extractedMenuItem = NULL;
  }
}

/*
 * Adjust the menu position.
 */
void Menu::adjustMenuPos(const Point& pt)
{
  Rect rcRoot = rootQvwm->GetRect();

  if (pt.x + m_rc.width > rcRoot.width - 1) {
    m_rc.x = pt.x - m_rc.width;
    m_gDir = GD_LEFT;
  }
  else {
    m_rc.x = pt.x;
    m_gDir = GD_RIGHT;
  }

  if (pt.y + m_rc.height > rcRoot.height - 1) {
    m_rc.y = rcRoot.height - m_rc.height;
    m_gDir |= GD_UP;
  }
  else {
    m_rc.y = pt.y;
    m_gDir |= GD_DOWN;
  }

  move(Point(m_rc.x, m_rc.y));
}

/*
 * PopupMenu --
 *   Pop down previous child menu, if any, and pop up new child menu.
 */
void Menu::PopupMenu()
{
  if (m_delayedMenuItem == NULL)
    return;

  if (m_delayedMenuItem == m_extractedMenuItem)
    return;

  if (m_child)
    m_child->hide();
  
  m_delayedMenuItem->extractChildMenu();
  m_delayedMenuItem = NULL;
}

/*
 * PopdownMenu --
 *   Pop down previous child menu.
 */
void Menu::PopdownMenu()
{
  hide();
}  

/*
 * Delay that child menu popup/popdown.
 */
void Menu::SetDelayedItem(MenuItem* mi)
{
  if (mi->getFunction() == Q_DIR) {
    m_delayedMenuItem = mi;

    timer->ResetTimeout(new Callback<Menu>(this, &Menu::PopupMenu));

    if (m_child)
      timer->SetTimeout(MenuDelayTime2,
			new Callback<Menu>(this, &Menu::PopupMenu));
    else
      timer->SetTimeout(MenuDelayTime,
			new Callback<Menu>(this, &Menu::PopupMenu));
  }
  else {
    if (m_child)
      timer->SetTimeout(MenuDelayTime,
			new Callback<Menu>(m_child, &Menu::PopdownMenu));
  }
}

Bool Menu::IsChecked(FuncNumber fn)
{
  switch (fn) {
  case Q_TOGGLE_ONTOP:
    if (m_qvWm->CheckFlags(ONTOP))
      return True;
    break;
    
  case Q_TOGGLE_STICKY:
    if (m_qvWm->CheckFlags(STICKY))
      return True;
    break;

  case Q_TOGGLE_AUTOHIDE:
    if (TaskbarAutoHide)
      return True;
    break;
    
  case Q_TOGGLE_TASKBAR:
    if (UseTaskbar && enableTaskbar)
      return True;
    break;

  case Q_TOGGLE_PAGER:
    if (UsePager && enablePager)
      return True;
    break;

  default:
    break;
  }

  return False;
}

Bool Menu::IsSelected(FuncNumber fn)
{
  switch (fn) {
  case Q_BOTTOM:
    if (UseTaskbar)
      if (taskBar->GetPos() == Taskbar::BOTTOM)
	return True;
    break;

  case Q_TOP:
    if (UseTaskbar)
      if (taskBar->GetPos() == Taskbar::TOP)
	return True;
    break;

  case Q_LEFT:
    if (UseTaskbar)
      if (taskBar->GetPos() == Taskbar::LEFT)
	return True;
    break;

  case Q_RIGHT:
    if (UseTaskbar)
      if (taskBar->GetPos() == Taskbar::RIGHT)
	return True;
    break;

  default:
    break;
  }

  return False;
}

/*
 * IsSelectable --
 *   Return True if the menu item is selectable.
 */
Bool Menu::IsSelectable(FuncNumber fn)
{
  ASSERT(m_qvWm);

  switch (fn) {
  case Q_NONE:
    return False;
    
  case Q_RESTORE:
    if (m_qvWm->CheckStatus(MINIMIZE_WINDOW | MAXIMIZE_WINDOW))
      return True;
    return False;

  case Q_MOVE:
  case Q_RESIZE:
    if (m_qvWm->CheckStatus(MINIMIZE_WINDOW | MAXIMIZE_WINDOW))
      return False;
    return True;

  case Q_MINIMIZE:
    if (m_qvWm->CheckStatus(MINIMIZE_WINDOW) || m_qvWm->CheckFlags(NO_TBUTTON))
      return False;
    return True;

  case Q_MAXIMIZE:
    if (m_qvWm->CheckStatus(MAXIMIZE_WINDOW) &&
	!m_qvWm->CheckStatus(MINIMIZE_WINDOW))
      return False;
    return True;

  case Q_SEPARATOR:
    return False;

  case Q_BOTTOM:
  case Q_TOP:
  case Q_LEFT:
  case Q_RIGHT:
    if (UseTaskbar)
      return True;
    else
      return False;

  case Q_LEFT_PAGING:
    {
      Rect rcRoot = rootQvwm->GetRect();
      Rect rcVirt = paging->GetVirtRect();
      if (paging->origin.x > rcVirt.x * rcRoot.width)
	return True;
      else
	return False;
    }

  case Q_RIGHT_PAGING:
    {
      Rect rcRoot = rootQvwm->GetRect();
      Rect rcVirt = paging->GetVirtRect();
      if (paging->origin.x < (rcVirt.x + rcVirt.width - 1) * rcRoot.width)
	return True;
      else
	return False;
    }

  case Q_UP_PAGING:
    {
      Rect rcRoot = rootQvwm->GetRect();
      Rect rcVirt = paging->GetVirtRect();
      if (paging->origin.y > rcVirt.y * rcRoot.height)
	return True;
      else
	return False;
    }
    
  case Q_DOWN_PAGING:
    {
      Rect rcRoot = rootQvwm->GetRect();
      Rect rcVirt = paging->GetVirtRect();
      if (paging->origin.y < (rcVirt.y + rcVirt.height - 1) * rcRoot.height)
	return True;
      else
	return False;
    }

  default:
    return True;
  }
}

Bool Menu::checkEnterEvent(Menu* child)
{
  XEvent ev;

  // leave focus in this menu if moving to the submenu
  if (XCheckTypedWindowEvent(display, child->getFrame(), EnterNotify, &ev)) {
    // restore the focus item when moving to the submenu, if necessary
    if (m_focusMenuItem && m_extractedMenuItem &&
	m_focusMenuItem != m_extractedMenuItem) {
      m_focusMenuItem->resetItemFocus();
      m_extractedMenuItem->setItemFocus();
      
      timer->ResetTimeout(new Callback<Menu>(this, &Menu::PopupMenu));
      m_focusMenuItem = m_extractedMenuItem;
    }      
    return True;
  }

  return False;
}

/*
 * Redraw function
 */
void Menu::paint()
{
  XPoint xp[3];

  xp[0].x = m_rc.width - 2;
  xp[0].y = 0;
  xp[1].x = 0;
  xp[1].y = 0;
  xp[2].x = 0;
  xp[2].y = m_rc.height - 2;

  XSetForeground(display, ::gc, gray.pixel);
  XDrawLines(display, m_frame, ::gc, xp, 3, CoordModeOrigin);
  
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

/*
 * Event handlers
 */
void Menu::onMouseLeave(const XCrossingEvent& ev)
{
  // cancel the popup of the child menu
  timer->ResetTimeout(new Callback<Menu>(this, &Menu::PopupMenu));

  if (m_child) {
    if (checkEnterEvent(m_child))
      return;
    else
      m_child->hide();
  }

  if (m_focusMenuItem) {
    m_focusMenuItem->resetItemFocus();
    m_focusMenuItem = NULL;
  }
}

void Menu::onKeyPress(const XKeyEvent& ev)
{
  char key;
  KeySym sym;

  if (!menuKey->ExecShortCutKey(ev.keycode, ev.state, this)) {
    if (XLookupString((XKeyEvent *)&ev, &key, 1, &sym, NULL) == 1) {
      if (key >= 'a' && key <= 'z')
	key -= 0x20;

      for (int i = 0; i < m_nitems; i++)
	if (m_mi[i]->execShortcut(key) == True)
	  return;
    }
  }
}

/*
 * Static functions
 */
void Menu::Initialize()
{
  if (MenuImage) {
    m_imgMenuBack = CreateImageFromFile(MenuImage, timer);
    if (m_imgMenuBack == NULL) {
      delete [] MenuImage;
      MenuImage = NULL;
    }
  }
  if (MenuActiveImage) {
    m_imgActiveMenuBack = CreateImageFromFile(MenuActiveImage, timer);
    if (m_imgActiveMenuBack == NULL) {
      delete [] MenuActiveImage;
      MenuActiveImage = NULL;
    }
  }

  MenuItem::Initialize();
}

/*
 * UnmapAllMenus --
 *   Unmap all menus (qvwm ctrl menu, startmenu, icon menu and taskbar menu).
 *   Only a menu in these menus is mapped.
 */
void Menu::UnmapAllMenus(Bool hideTaskbar)
{
  if (Qvwm::focusQvwm && Qvwm::focusQvwm->CheckMenuMapped())
    Qvwm::focusQvwm->UnmapMenu();
  else if (::ctrlMenu->CheckMapped())
    ::ctrlMenu->hide(); // for NOFOCUS window
  else if (startMenu && startMenu->CheckMapped())
    startMenu->hide();
  else if (Icon::focusIcon && Icon::focusIcon->CheckMenuMapped())
    Icon::focusIcon->UnmapMenu();
  else if (UseTaskbar && taskBar->CheckMenuMapped())
    taskBar->UnmapMenu();

  if (UseTaskbar)
    if (hideTaskbar && TaskbarAutoHide && !taskBar->IsHiding())
      taskBar->HideTaskbar();
}

/*
 * CheckAnyMenusMapped --
 *   check whether any menus are mapped.
 */
Bool Menu::CheckAnyMenusMapped()
{
  if ((Qvwm::focusQvwm && Qvwm::focusQvwm->CheckMenuMapped()) ||
      ::ctrlMenu->CheckMapped() ||
      (startMenu && startMenu->CheckMapped()) ||
      (Icon::focusIcon && Icon::focusIcon->CheckMenuMapped()) ||
      (UseTaskbar && taskBar->CheckMenuMapped()))
    return True;
  
  return False;
}

/*
 * GetMappedMenuFrame --
 *   get the frame of a mapped menu.
 */
Window Menu::GetMappedMenuFrame()
{
  if (Qvwm::focusQvwm && Qvwm::focusQvwm->CheckMenuMapped())
    return Qvwm::focusQvwm->ctrlMenu->getFrame();
  else if (::ctrlMenu->CheckMapped())
    return ::ctrlMenu->getFrame();
  else if (startMenu && startMenu->CheckMapped())
    return startMenu->getFrame();
  else if (Icon::focusIcon && Icon::focusIcon->CheckMenuMapped())
    return Icon::ctrlMenu->getFrame();
  else if (UseTaskbar && taskBar->CheckMenuMapped())
    return taskBar->GetMenu()->getFrame();

  return None;
}
