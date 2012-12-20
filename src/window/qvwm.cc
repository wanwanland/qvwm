/*
 * qvwm.cc
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
#ifdef USE_SHAPE
#include <X11/extensions/shape.h>
#endif
#include <X11/Xatom.h>
#include "main.h"
#include "key.h"
#include "window/qvwm.h"
#include "window/focus_mgr.h"
#include "window/exec.h"
#include "frame/WindowTitlebar.h"
#include "taskbar/taskbar.h"
#include "taskbar/TaskbarButton.h"
#include "taskbar/start/StartButton.h"
#include "taskbar/start/StartMenu.h"
#include "taskbar/tray/Indicator.h"
#include "desktop/icon.h"
#include "desktop/desktop.h"
#include "desktop/switcher.h"
#include "desktop/paging.h"
#include "desktop/ScreenSaver.h"
#include "pager/Pager.h"
#include "pager/Miniature.h"
#include "frame/FrameButton.h"
#include "frame/CtrlButton.h"
#include "frame/AllBorders.h"
#include "frame/AllEdges.h"
#include "frame/ClientArea.h"
#include "parser/qvwmrc.h"
#include "wm/Event.h"
#include "wm/session.h"
#include "wm/mwm.h"
#include "wm/gnome.h"
#include "image/Image.h"
#include "lib/Tooltip.h"
#include "lib/MenuItem.h"
#include "lib/misc.h"
#include "lib/util.h"
#include "lib/Menu.h"

int Qvwm::TITLE_HEIGHT;
int Qvwm::BORDER_WIDTH;
int Qvwm::TOP_BORDER;

/*
 * Qvwm class constructor --
 *   Position the window at that point if usrPos is True;
 *   otherwise, the position of the window obeys a given rule.
 */
Qvwm::Qvwm(Window w, Bool usrPos)
{
  flags = 0;
  status = 0;
  mapped = False;
  imgLarge = NULL;
  imgSmall = NULL;
  nCmapWins = 0;
  name = NULL;
  isFirstMapped = True;
#ifdef USE_XSMP
  smClientId = NULL;
  wmRole = NULL;
#endif

  XGrabServer(display);

  CreateFrame(Rect(0, 0, 1, 1));
  m_clientArea = new ClientArea(this, w);

  /*
   * Get some hints.
   */
  wmHints = XGetWMHints(display, w);
  GetWindowSizeHints();
  Gnome::GetHints(this);
  Mwm::GetHints(this);
  GetWindowClassHints();
  FetchWMProtocols();
  GetTransientForHint();

  Gnome::SetInitStates(this);

  GetWindowAttrs(usrPos);

  if (Intersect(rc, rootQvwm->GetRect()))
    Gnome::ResetState(this, WIN_STATE_HID_WORKSPACE);
  else
    Gnome::SetState(this, WIN_STATE_HID_WORKSPACE);

  Rect rcParent, rcTitle, rcBorder, rcInner;

  CalcFramePos(rc, &rcParent, &rcTitle, &rcBorder, &rcInner);

  XMoveResizeWindow(display, frame, rc.x, rc.y, rc.width, rc.height);
  m_clientArea->reshape(rcParent);

  // borders
  m_allBorders = new AllBorders(this, rcBorder);
  
  // inner edges
  m_allInnerEdges = new AllInnerEdges(this, rcInner);

  // titlebar
  m_titleBar = new WindowTitlebar(this, rcTitle);

  if (imgLarge == NULL)
    imgLarge = GetFixedIcon(32);
  if (imgSmall == NULL)
    imgSmall = GetFixedIcon(16);

  if (m_titleBar) {
    m_titleBar->setTitle(name);
    m_titleBar->setImage(imgSmall);
  }

  Rect rect(-50, -50, 50, 50);
  QvImage* img = imgSmall->Duplicate();
  tButton = new TaskbarButton(taskBar, rect, img, this);

  ctrlMenu = ::ctrlMenu;

#ifdef USE_SHAPE
  int bShaped, cShaped;
  int xbs, ybs, xcs, ycs;
  unsigned int wbs, hbs, wcs, hcs;

  if (shapeSupport) {
    XShapeSelectInput(display, w, ShapeNotifyMask);
    XShapeQueryExtents(display, w, &bShaped, &xbs, &ybs, &wbs, &hbs,
		       &cShaped, &xcs, &ycs, &wcs, &hcs);
    if (bShaped) {
      SetFlags(SHAPED);
      SetShape();
    }
  }
#endif // USE_SHAPE

  if (UsePager)
    mini = new Miniature(pager, rc, this);

  scKey->GrabKeys(frame);

  SetTransient();

  XUngrabServer(display);
}

/*
 * Qvwm class constructor --
 *   For only root window.
 */
Qvwm::Qvwm(Window root)
  : frame(root)
{
  rc = Rect(0, 0,
	    DisplayWidth(display, screen), DisplayHeight(display, screen));

  mapped = True;
  flags = 0L;
  status = 0;
  nCmapWins = 0;
  name = new char[10];
  strcpy(name, "qvwm root");
  tButton = NULL;

  m_clientArea = NULL;
  m_titleBar = NULL;
  m_allBorders = NULL;

#ifdef USE_XSMP
  smClientId = NULL;
  wmRole = NULL;
#endif

  taskBar = new Taskbar(TaskbarRows);

  MenuItem** mi = MenuItem::createMenuItems(DesktopMenuItem);
  ctrlMenu = new Menu(mi, NULL, fsCtrlMenu, this);

  addWindow(root);

  focusQvwm = this;

  scKey->GrabKeys(root);
}

Qvwm::~Qvwm()
{
  int i;

  if (this != rootQvwm) {
    delWindow(frame);

    delete m_clientArea;
    delete m_titleBar;
    delete m_allBorders;

    QvImage::Destroy(imgLarge);
    QvImage::Destroy(imgSmall);

    if (wmHints)
      XFree(wmHints);

    if (classHints.res_name)
      XFree(classHints.res_name);
    if (classHints.res_class)
      XFree(classHints.res_class);

#ifdef USE_XSMP
    if (wmRole)
      XFree(wmRole);
    if (smClientId)
      XFree(smClientId);
#endif

    delete tButton;

    /*
     * When this is a transient window.
     */
    if (CheckFlags(TRANSIENT) && qvMain) {
      List<Qvwm>::Iterator li(&qvMain->trList);
      Qvwm* qvWm = li.GetHead();

      while (qvWm) {
	if (qvWm == this) {
	  qvMain->trList.Remove(this);
	  break;
	}
	qvWm = li.GetNext();
      }
    }
    else {
      /*
       * When this is a main window which has some transient windows.
       */
      List<Qvwm>::Iterator li(&trList);
      Qvwm* qvTrans = li.GetHead();

      while (qvTrans) {
	qvTrans->qvMain = NULL;
	qvTrans = li.GetNext();
      }
    }

    if (CheckFlags(ONTOP))
      desktop.GetOnTopList().Remove(this);

    if (UsePager)
      delete mini;

    XDestroyWindow(display, frame);
  }
  else
    delete ctrlMenu;

  delete [] name;
}

void Qvwm::GetWindowAttrs(Bool usrPos)
{
  Rect rcWin;

  bwOrig = 1; // temp

  /*
   * Preserve original border width and set border width to 0.
   */
#ifdef USE_XSMP
  /*
   * Restore properties and attributes from the previous session
   */
  GetSMClientId(); // initialize smClientId
  GetWMRole(); // initialize wmRole
  int ax, ay, aw, ah;
  if (session->SearchProto(smClientId, wmRole,
			   classHints.res_class,
			   classHints.res_name,
			   name, &ax, &ay, &aw, &ah)) {
    usrPos = True; // force place
    rcWin.x = ax;
    rcWin.y = ay;
    rcWin.width = aw;
    rcWin.height = ah;
  }
  else
#endif // USE_XSMP
    rcWin = m_clientArea->getWinBounds();

  Point pageoff;
  ExecPending* pending = ExecPending::LookInList(name, classHints);

  if (pending) {
    pageoff = pending->GetPageOff();
    ExecPending::m_pendingList.Remove(pending);
    delete pending;
  }
  else
    pageoff = paging->origin;

  /*
   * Calculate the window position.
   */
  if (usrPos || (flags & (TRANSIENT | FIXED_POS)) ||
      (hints.flags & (USPosition | PPosition))) {
    if (CheckFlags(GEOMETRY)) {
      if (geom->bitmask & WidthValue)
	rcWin.width = geom->rc.width;
      if (geom->bitmask & HeightValue)
	rcWin.height = geom->rc.height;
    }

    if (!CheckFlags(STICKY)) {
      rcWin.x += pageoff.x;
      rcWin.y += pageoff.y;
    }

    CalcWindowPos(rcWin);
  }
  else {
    if (CheckFlags(GEOMETRY)) {
      if (geom->bitmask & WidthValue)
	rcWin.width = geom->rc.width;
      if (geom->bitmask & HeightValue)
	rcWin.height = geom->rc.height;

      if (geom->bitmask & XNegative)
	rcWin.x = DisplayWidth(display, screen) + geom->rc.x - rcWin.width;
      else if (geom->bitmask & XValue)
	rcWin.x = geom->rc.x;

      if (geom->bitmask & YNegative)
	rcWin.y = DisplayHeight(display, screen) + geom->rc.y - rcWin.height;
      else if (geom->bitmask & YValue)
	rcWin.y = geom->rc.y;

      if (!CheckFlags(STICKY)) {
	rcWin.x += pageoff.x;
	rcWin.y += pageoff.y;
      }
      
      CalcWindowPos(rcWin);
    }
    else {
      int borderWidth, topBorder, titleHeight, titleEdge;

      GetBorderAndTitle(borderWidth, topBorder, titleHeight, titleEdge);

      rc.width = rcWin.width + borderWidth * 2;
      rc.height = rcWin.height + borderWidth + topBorder + titleHeight
	+ titleEdge;

      Point pt = desktop.GetNextPlace(Dim(rc.width, rc.height), pageoff);

      rc.x = pt.x;
      rc.y = pt.y;

      if (CheckFlags(STICKY)) {
	rcWin.x = rc.x - rcScreen.x - pageoff.x;
	rcWin.y = rc.y - rcScreen.y - pageoff.y;
      }
      else {
	rcWin.x = rc.x - rcScreen.x;
	rcWin.y = rc.y - rcScreen.y;
      }
    }
  }

  m_clientArea->setWinBounds(rcWin);
  rcOrig = rcWin;  // temp
}

/*
 * SetTransient
 *   Search pre-mapped transient windows for this window to handle
 *   a transient window properly even if the main window is mapped
 *   after the transient window is mapped first.
 */
void Qvwm::SetTransient()
{
  List<Qvwm>::Iterator i(&desktop.GetQvwmList());
  Window tWin;

  for (Qvwm* qvWm = i.GetHead(); qvWm; qvWm = i.GetNext()) {
    if (qvWm->CheckFlags(TRANSIENT) && qvWm->qvMain == NULL)
      if (qvWm->m_clientArea->getTransientForHint(&tWin))
	if (tWin == GetWin()) {
	  qvWm->qvMain = this;  // qvWm is a transient window of this
	  if (CheckFlags(ONTOP) && !qvWm->CheckFlags(ONTOP)) {
	    qvWm->SetFlags(ONTOP);
	    desktop.GetOnTopList().InsertTail(qvWm);
	    Gnome::ChangeLayer(qvWm, WIN_LAYER_ONTOP);
	  }
	  trList.InsertTail(qvWm);
	}
  }
}


void Qvwm::SendConfigureEvent()
{
  m_clientArea->sendConfigureEvent(rc);
}

/*
 * SendMessage --
 *   Send the messeage to this window.
 */
void Qvwm::SendMessage(Atom atom)
{
  m_clientArea->sendMessage(atom);
}

/*
 * AdjustPage --
 *   Switch to the page where the window center point exists.
 */
void Qvwm::AdjustPage()
{
  if (this == rootQvwm)
    return;

  if (CheckFlags(STICKY))
    return;

  Rect rect = GetRect();
  Point ptCenter(rect.x+rect.width/2, rect.y+rect.height/2);
  Rect rcRoot = rootQvwm->GetRect();
  
  if (ptCenter.x >= 0)
    ptCenter.x =  ptCenter.x / rcRoot.width * rcRoot.width;
  else
    ptCenter.x = (ptCenter.x / rcRoot.width - 1) * rcRoot.width;

  if (ptCenter.y >= 0)
    ptCenter.y = ptCenter.y / rcRoot.height * rcRoot.height;
  else 
    ptCenter.y = (ptCenter.y / rcRoot.height - 1) * rcRoot.height;
  
  ASSERT(paging);

  paging->PagingProc(ptCenter, False);
}

/*
 * Event handler
 */
void Qvwm::onMouseEnter(const XCrossingEvent& ev)
{
  if ((ev.state & NoFocusChangeMask) == NoFocusChangeMask)
    return;

  if (!ClickToFocus && !(NoDesktopFocus && this == rootQvwm) &&
      !CheckStatus(MINIMIZE_WINDOW) && !CheckFlags(FOCUS_ON_CLICK)) {
    if (!CheckFocus() && !Menu::CheckAnyMenusMapped()) {
      if (AutoRaise)
	RaiseWindow(False);
      SetFocus();
    }
  }
}

void Qvwm::onButtonPress(const XButtonEvent& ev)
{
  if (ev.button == Button1) {
    if (UseTaskbar && TaskbarAutoHide && !taskBar->IsHiding())
      taskBar->HideTaskbar();

    Menu::UnmapAllMenus();
    
    if (Icon::focusIcon != NULL)
      Icon::focusIcon->ResetFocus();

    SetFocus();

    if (ClickingRaises || ev.window != GetWin())
      RaiseWindow(True);

    SetStatus(PRESS_FRAME);
  }

  if (AutoRaise && focusQvwm == this && activeQvwm != this)
    RaiseWindow(True);
}      

void Qvwm::onButtonRelease(const XButtonEvent& ev)
{
  if (ev.state & (Button1Mask | Button2Mask))
    ResetStatus(PRESS_FRAME);
  else if (ev.state & Button3Mask)
    procButton3Release(ev.window, Point(ev.x_root, ev.y_root));
}

/*
 * Button3Release --
 *   Process the release of button3(mouse right button).
 */
void Qvwm::procButton3Release(Window win, const Point& ptRoot)
{
  if (this == rootQvwm) {
    Menu::UnmapAllMenus();
    
    if (!CheckFocus())
      SetFocus();
    
    ctrlMenu->adjustMenuPos(ptRoot);
    ctrlMenu->show();
  }
  else {
    Menu::UnmapAllMenus();
    
    SetFocus();
    RaiseWindow(True);
    
    ctrlMenu->SetQvwm(this);
    ctrlMenu->adjustMenuPos(ptRoot);
    ctrlMenu->show();
  }
}

void Qvwm::onKeyPress(const XKeyEvent& ev)
{
  ctrlMenu->SetQvwm(this);

  scKey->ExecShortCutKey(ev.keycode, ev.state, ctrlMenu);
}

void Qvwm::onFocusIn(const XFocusChangeEvent& ev)
{
  if (ev.mode == NotifyUngrab)
    m_clientArea->setInputFocus();
}

/*
 * Static functions
 */
void Qvwm::Initialize()
{
  TITLE_HEIGHT = FrameTitleHeight;
  BORDER_WIDTH = FrameBorderWidth + 4;
  TOP_BORDER = FrameBorderWidth + 2;
}

#ifdef USE_XSMP
/*
 * SavePropertiesToFile --
 *   Save the state of this window to the FILE for XSMP
 */
Bool Qvwm::SavePropertiesToFile(int idx, FILE* fp)
{
  if (qvMain) {
    // this window is a transient win.
    return True;
  }
  GetSMClientId();
  GetWMRole();
  // WM_CLASS  (if WM_WINDOW_ROLE is not specified)
  XClassHint wm_class;        // STRING
  if (wmRole == NULL) {
    if (!m_clientArea->getClassHint(&wm_class))
      return False;
  }

  // Write out to the 'fp'
  if (smClientId != NULL)
    fprintf(fp, "%d,id=%s\n", idx, smClientId);       // SM_CLIENT_ID

  fprintf(fp, "%d,wOrig=%x\n", idx, (int)GetWin());
  fprintf(fp, "%d,frame=%x\n", idx, (int)frame);
  if (wmRole)
    fprintf(fp, "%d,role=%s\n", idx, wmRole); // WM_WINDOW_ROLE
  else {
    fprintf(fp, "%d,class=%s\n", idx, wm_class.res_class);// WM_CLASS
    fprintf(fp, "%d,rname=%s\n", idx, wm_class.res_name);
    fprintf(fp, "%d,name=%s\n", idx, name);   // WM_NAME
  }
  fprintf(fp, "%d,pos=%d,%d\n", idx, rcOrig.x, rcOrig.y);
  fprintf(fp, "%d,size=%d,%d\n", idx, rcOrig.width, rcOrig.height);

  if (wmRole == NULL) {
    XFree(wm_class.res_class);
    XFree(wm_class.res_name);
  }
  return True;
}
#endif // USE_XSMP

void Qvwm::MapCtrlMenu()
{
  int borderWidth, topBorder, titleHeight, titleEdge;

  GetBorderAndTitle(borderWidth, topBorder, titleHeight, titleEdge);
    
  ctrlMenu->SetQvwm(this);
    
  Point pt(rc.x - paging->origin.x + topBorder,
	   rc.y - paging->origin.y + topBorder + titleHeight);
  ctrlMenu->adjustMenuPos(pt);
  
  if (ctrlMenu->getDirection() & GD_UP) {
    Rect rcMenu = ctrlMenu->getRect();
    
    pt.y = rc.y - paging->origin.y + topBorder - rcMenu.height;
    ctrlMenu->move(pt);
  }
  
  ctrlMenu->show();
}

void Qvwm::KillClient()
{
  XKillClient(display, m_clientArea->getWindow());
}

Window Qvwm::GetWin() const
{
  return m_clientArea->getWindow();
}
