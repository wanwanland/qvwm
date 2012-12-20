/*
 * event.cc
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
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#ifdef __EMX__
#include <sys/select.h>
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#ifdef USE_SHAPE
#include <X11/extensions/shape.h>
#endif
#ifdef USE_XSMP
#include <X11/ICE/ICElib.h>
#endif
#include "main.h"
#include "window/qvwm.h"
#include "window/focus_mgr.h"
#include "wm/Event.h"
#include "wm/gnome.h"
#include "wm/session.h"
#include "wm/RemoteCommand.h"
#include "frame/FrameButton.h"
#include "frame/ClientArea.h"
#include "taskbar/TaskbarButton.h"
#include "taskbar/tray/TaskTray.h"
#include "taskbar/tray/Indicator.h"
#include "desktop/desktop.h"
#include "desktop/paging.h"
#include "desktop/ScreenSaver.h"
#include "parser/qvwmrc.h"
#include "lib/Component.h"
#include "lib/misc.h"
#include "lib/timer.h"
#include "lib/util.h"

#if defined(sun) && !defined(__SVR4)
extern "C" int select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
# if defined(__GNUC__) && __GNUC__ >= 2 && __GNUC_MINOR__ >= 8
# else
extern "C" void bzero(char *, int);
# endif
#endif

Qvwm*	Qvwm::focusQvwm;

/*
 * MainLoop --
 *   Wait event and timeout.
 */
void Event::MainLoop()
{  
  XEvent ev;
  int fd = ConnectionNumber(display);
  fd_set fds;
  struct timeval tm, *timep;
  int fdMax;
#ifdef USE_XSMP
//  int s;
  int iceConnFd;
#endif
#ifdef ALLOW_RMTCMD
  int rmtCmdFd = -1;
#endif

  while (1) {
    while (XPending(display) != 0) {
      XNextEvent(display, &ev);
      DispatchEvent(ev);

      timer->CheckTimeout(&tm);
    }

    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    fdMax = fd;

#ifdef USE_XSMP
    iceConnFd = session->GetIceConnFd();
    if (iceConnFd >= 0) {
      FD_SET(iceConnFd, &fds);
      if (iceConnFd > fdMax) {
	fdMax = iceConnFd;
      }
    }
#endif // USE_XSMP

#ifdef ALLOW_RMTCMD
    if (AllowRemoteCmd) {
      rmtCmdFd = remoteCmd->getRmtCmdFd();
      if (rmtCmdFd >= 0) {
	FD_SET(rmtCmdFd, &fds);
	if (rmtCmdFd > fdMax)
	  fdMax = rmtCmdFd;
      }
    }
#endif // ALLOW_RMTCMD

/* ???
#ifdef USE_XSMP
    // Why we should waitpid here, ...
    waitpid(-1, &s, WNOHANG);
#endif
*/

    /*
     * If next timeout does not exist, select blocks until next events.
     */
    if (!timer->CheckTimeout(&tm))
      timep = NULL;
    else
      timep = &tm;

    // Flush all requests before select, or some requests is not
    // flushed until select is returned.
    XFlush(display);

    if (XPending(display) > 0)
      continue;

#if defined(__hpux) && !defined(_XPG4_EXTENDED)
    if (select(fdMax + 1, (int *)&fds, 0, 0, timep) == 0) {    // timeout
#else
    if (select(fdMax + 1, &fds, 0, 0, timep) == 0) {    // timeout
#endif
      timer->CheckTimeout(&tm);
    }
#ifdef USE_XSMP
    else if (iceConnFd >=0 && FD_ISSET(iceConnFd, &fds)) {
      Bool res;
      IceProcessMessagesStatus rc;
      rc = IceProcessMessages(session->GetIceConn(), NULL, &res);
      if (rc == IceProcessMessagesIOError) {
	session->CloseConnection();
      }
      //else if (rc == IceProcessMessagesConnectionClosed) {
      //}
    }
#endif // USE_XSMP
#ifdef ALLOW_RMTCMD
    else if (rmtCmdFd >= 0 && FD_ISSET(rmtCmdFd, &fds)) {
      remoteCmd->process();
    }
#endif
  }
}

/*
 * DispatchEvent --
 *   Call the event procedure according to a event type.
 */
void Event::DispatchEvent(const XEvent& ev)
{
  switch (ev.type) {
  case Expose: {
    const XExposeEvent& xev = (const XExposeEvent &)ev;
    ExposeProc(xev);
    break;
  }

  case EnterNotify: {
    const XCrossingEvent& xev = (const XCrossingEvent &)ev;
    EnterNotifyProc(xev);
    break;
  }

  case LeaveNotify: {
    const XCrossingEvent& xev = (const XCrossingEvent &)ev;
    LeaveNotifyProc(xev);
    break;
  }

  case MotionNotify: {
    const XMotionEvent& xev = (const XMotionEvent &)ev;
    MotionNotifyProc(xev);
    break;
  }

  case ButtonPress: {
    const XButtonEvent& xev = (const XButtonEvent &)ev;
    ButtonPressProc(xev);
    ptPrevRoot = Point(xev.x_root, xev.y_root);
    break;
  }

  case ButtonRelease: {
    const XButtonEvent& xev = (const XButtonEvent &)ev;
    ButtonReleaseProc(xev);
    break;
  }

  case KeyPress: {
    const XKeyEvent& xev = (const XKeyEvent &)ev;
    KeyPressProc(xev);
    break;
  }

  case FocusIn: {
    const XFocusChangeEvent& xev = (const XFocusChangeEvent &)ev;
    FocusInProc(xev);
    break;
  }

  case FocusOut:
    break;

  case MapRequest: {
    const XMapRequestEvent& xev = (const XMapRequestEvent &)ev;
    MapRequestProc(xev);
    break;
  }

  case MapNotify: {
    const XMapEvent& xev = (const XMapEvent &)ev;
    MapNotifyProc(xev);
    break;
  }

  case UnmapNotify: {
    if (ev.xany.window == root)
      return;
    const XUnmapEvent& xev = (const XUnmapEvent &)ev;
    UnmapNotifyProc(xev);
    break;
  }

  case DestroyNotify: {
    const XDestroyWindowEvent& xev = (const XDestroyWindowEvent &)ev;
    DestroyNotifyProc(xev);
    break;
  }

  case ClientMessage: {
    const XClientMessageEvent& xev = (const XClientMessageEvent &)ev;
    ClientMessageProc(xev);
    break;
  }

  case ColormapNotify: {
    const XColormapEvent& xev = (const XColormapEvent &)ev;
    ColormapNotifyProc(xev);
    break;
  }

  case ConfigureRequest: {
    const XConfigureRequestEvent& xev = (const XConfigureRequestEvent &)ev;
    ConfigureRequestProc(xev);
    break;
  }

  case CirculateRequest: {
    const XCirculateRequestEvent& xev = (const XCirculateRequestEvent &)ev;
    CirculateRequestProc(xev);
    break;
  }

  case PropertyNotify: {
    const XPropertyEvent& xev = (const XPropertyEvent &)ev;
    PropertyNotifyProc(xev);
    break;
  }

#if defined(USE_SS) && !defined(USE_SSEXT)
  case CreateNotify:
    scrSaver->SelectEvents(ev.xcreatewindow.window);
    break;
#endif

#ifdef USE_SHAPE
  default:
    if (shapeSupport) {
      if (ev.type == shapeEventBase + ShapeNotify) {
	const XShapeEvent &xev = (const XShapeEvent &)ev;
	ShapeNotifyProc(xev);
      }
    }
    break;
#endif // USE_SHAPE
  }
}

/*
 * ExposeProc --
 *   Process Expose event.
 */
void Event::ExposeProc(const XExposeEvent& ev)
{
  Component* comp = findComponent(ev.window);
  if (comp) {
    comp->onExpose(ev);

    XEvent xev;
    while (XCheckWindowEvent(display, ev.window, ExposureMask, &xev))
      ;
  }
}

/*
 * EnterNotifyProc --
 *   Process EnterNotify event.
 */
void Event::EnterNotifyProc(const XCrossingEvent& ev)
{
  Component* comp = findComponent(ev.window);
  if (comp) {
    if (!comp->equalClass(CLASS_TASKBAR)) {
      XEvent xev;

      // check if there is LeaveNotify event for the window
      if (XCheckTypedWindowEvent(display, ev.window, LeaveNotify, &xev)) {
	if (xev.xcrossing.mode == NotifyNormal &&
	    xev.xcrossing.detail != NotifyInferior)
	  return;
      }
    }

    comp->onMouseEnter(ev);
    return;
  }

  ASSERT(paging);

  if (paging->IsPagingBelt(ev.window)) {
    paging->HandlePaging(Point(ev.x_root, ev.y_root));
  }
}

/*
 * LeaveNotifyProc --
 *   Process LeaveNotify event.
 */
void Event::LeaveNotifyProc(const XCrossingEvent& ev)
{
  Component* comp = findComponent(ev.window);
  if (comp)
    comp->onMouseLeave(ev);
}

/*
 * MotionNotifyProc --
 *   Process MotionNotify event.
 */
void Event::MotionNotifyProc(const XMotionEvent& ev)
{
  Component* comp = findComponent(ev.window);
  if (comp)
    comp->onMouseMotion(ev);
}

/*
 * ButtonPressProc --
 *   Process ButtonPress event.
 */
void Event::ButtonPressProc(const XButtonEvent& ev)
{
#if defined(USE_SS) && !defined(USE_SSEXT)
  scrSaver->ResetTimer();
#endif

  Component* comp = findComponent(ev.window);
  if (comp) {
    if (comp->isDoubleClick(ev))
      comp->onDoubleClick(ev);
    else
      comp->onButtonPress(ev);
  }

  XAllowEvents(display, ReplayPointer, CurrentTime);
}

/*
 * ButtonReleaseProc --
 *   Process ButtonRelease event.
 */
void Event::ButtonReleaseProc(const XButtonEvent& ev)
{
  Component* comp = findComponent(ev.window);
  if (comp)
    comp->onButtonRelease(ev);
}

/*
 * KeyPressProc --
 *   Process KeyPress event.
 */
void Event::KeyPressProc(const XKeyEvent& ev)
{
#if defined(USE_SS) && !defined(USE_SSEXT)
  scrSaver->ResetTimer();
#endif

  Component* comp = findComponent(ev.window);
  if (comp)
    comp->onKeyPress(ev);
}

void Event::FocusInProc(const XFocusChangeEvent& ev)
{
  Component* comp = findComponent(ev.window);
  if (comp)
    comp->onFocusIn(ev);
}

/*
 * MapRequestProc --
 *   Process MapRequest event.
 */
void Event::MapRequestProc(const XMapRequestEvent& ev)
{
  /*
   * Check the window is valid.
   */
  Window junkRoot;
  Rect rcJunk;
  unsigned int junkBW, junkDepth;

  if (XGetGeometry(display, ev.window, &junkRoot, &rcJunk.x, &rcJunk.y,
		   (unsigned int *)&rcJunk.width,
		   (unsigned int *)&rcJunk.height,
		   &junkBW, &junkDepth) == 0)
    return;

  // Qvwm deals with a window whose size is larger than 32768 as an invalid
  // window.  (This may be incorrect in some cases...)
  if ((short)rcJunk.width < 0 || (short)rcJunk.height < 0) {
    QvwmError("window size is probably invalid: %d(%d)x%d(%d)",
	      rcJunk.width, (short)rcJunk.width,
	      rcJunk.height, (short)rcJunk.height);
    return;
  }

  /*
   * If the name of the window is requested by Indicator, the window becomes
   * an indicator.
   */
  XTextProperty xtp;
  char** cl = NULL;
  int n;
  XClassHint classHints;
  Indicator* ind;
  char* title = "";

  if (XGetWMName(display, ev.window, &xtp) != 0) {
    XmbTextPropertyToTextList(display, &xtp, &cl, &n);
    if (cl)
      title = cl[0];
  }

  classHints.res_name = NULL;
  classHints.res_class = NULL;
  XGetClassHint(display, ev.window, &classHints);

  if ((ind = Indicator::LookInList(title, classHints))) {
    XFreeStringList(cl);
    if (classHints.res_name)
      XFree(classHints.res_name);
    if (classHints.res_class)
      XFree(classHints.res_class);
      
    if (UseTaskbar && taskBar) {
      TaskTray* taskTray = taskBar->getTaskTray();
      taskTray->addIndicator(ind, ev.window);
      return;
    }
  }
  else {
    XFreeStringList(cl);
    if (classHints.res_name)
      XFree(classHints.res_name);
    if (classHints.res_class)
      XFree(classHints.res_class);
  }

  /*
   * Frame setup.
   */
  Qvwm* qvWm;
  XWMHints *wmHints;

  /*
   * If qvWm has already created and is unmapped, need not create new qvWm.
   */
  if ((qvWm = desktop.LookInList(ev.window)) == NULL) {
    qvWm = new Qvwm(ev.window, False);
    desktop.GetQvwmList().InsertTail(qvWm);
  }

  if (qvWm->CheckFlags(CLOSE_SOON)) {
#ifdef DEBUG
    QvwmError("force to close a window: %s", qvWm->GetName());
#endif
    XReparentWindow(display, ev.window, root,
		    rcScreen.x + rcScreen.width, rcScreen.y + rcScreen.height);
    qvWm->CloseWindow();
    desktop.GetQvwmList().Remove(qvWm);
    delete qvWm;
    return;
  }

  wmHints = qvWm->GetWMHints();

  if ((wmHints && (wmHints->flags & StateHint) && 
       wmHints->initial_state == IconicState) ||
      qvWm->CheckFlags(INIT_MINIMIZE))
    qvWm->MinimizeWindow(False);
  else if (qvWm->CheckFlags(INIT_MAXIMIZE))
    qvWm->MaximizeWindow(False, False);
  else if (qvWm->CheckStatus(MINIMIZE_WINDOW))
    qvWm->RestoreWindow(False, False);
  else
    qvWm->MapWindows();

  qvWm->tButton->show();

  if (!qvWm->CheckStatus(MINIMIZE_WINDOW)) {
    if (ClickToFocus || FocusOnMap)
      qvWm->SetFocus();
  }
  else
    focusMgr.InsertUnmapList(qvWm);

  Gnome::ChangeClientList();

  XSync(display, 0);
}

/*
 * MapNotifyProc --
 *   Process MapNotify event.
 */
void Event::MapNotifyProc(const XMapEvent& ev)
{
  Qvwm* qvWm;

  qvWm = findQvwm(ev.window);
  if (qvWm == NULL)
    return;

  // WM should not interfere if override_redirect is True, but does
  // interfere when override_redirect is changed from False to True.
  // So reparent the window and return if override_redirect is True.
  if (ev.override_redirect) {
#if defined(USE_SS) && !defined(USE_SSEXT)
    XWindowAttributes attr;
    int keyMask;
    
    XGetWindowAttributes(display, ev.window, &attr);
    keyMask = (attr.all_event_masks | attr.do_not_propagate_mask)
      & KeyPressMask;
    XSelectInput(display, ev.window, SubstructureNotifyMask | keyMask);
#else
    XSelectInput(display, ev.window, 0);
#endif

    Window junkRoot;
    Rect rect;
    unsigned int junkBW, junkDepth;
    
    XGetGeometry(display, ev.window, &junkRoot, &rect.x, &rect.y,
		 (unsigned int *)&rect.width,
		 (unsigned int *)&rect.height,
		 &junkBW, &junkDepth);
    XReparentWindow(display, ev.window, root, rect.x, rect.y);
    desktop.GetQvwmList().Remove(qvWm);
    delete qvWm;
    return;
  }
  
  if (!qvWm->CheckMapped()) {
    Menu::UnmapAllMenus();
    
    XGrabServer(display);
    
    if (qvWm->CheckStatus(MINIMIZE_WINDOW))
      qvWm->RestoreWindow(False, False);
    else {
      qvWm->MapWindows();
      qvWm->tButton->show();
      XAddToSaveSet(display, qvWm->GetWin());
    }
    
    if (ClickToFocus || FocusOnMap)
      qvWm->SetFocus();
    
    XSync(display, 0);
    XUngrabServer(display);
  }
}

/*
 * UnmapNotifyProc --
 *   Process UnmapNotify event.
 */
void Event::UnmapNotifyProc(const XUnmapEvent& ev)
{
  Qvwm* qvWm;

  qvWm = findQvwm(ev.window);
  if (qvWm == NULL)
    return;

  XGrabServer(display);

  qvWm->SetStateProperty(WithdrawnState);
  
  Rect rcRoot = rootQvwm->GetRect();
  Rect rcOrig = qvWm->GetOrigRect();
  Point pt;

  pt.x = rcOrig.x % rcRoot.width;
  if (pt.x < 0)
    pt.x += rcRoot.width;
  pt.y = rcOrig.y % rcRoot.height;
  if (pt.y < 0)
    pt.y += rcRoot.height;

  XReparentWindow(display, qvWm->GetWin(), root, pt.x, pt.y);
  XUnmapWindow(display, qvWm->GetWin());
  
  DestroyQvwm(qvWm);
  
  XSync(display, 0);
  XUngrabServer(display);
}

/*
 * DestroyNotifyProc --
 *   Process DestroyNotify event.
 */
void Event::DestroyNotifyProc(const XDestroyWindowEvent& ev)
{
  Qvwm* qvWm;

  qvWm = findQvwm(ev.window);
  if (qvWm == NULL)
    return;

  XGrabServer(display);
  DestroyQvwm(qvWm);
  XUngrabServer(display);
}

void Event::DestroyQvwm(Qvwm* qvWm)
{
  Bool yield = False;

  if (qvWm == Qvwm::focusQvwm) {
    yield = True;
    qvWm->YieldFocus();
  }

  if (qvWm->CheckMapped())
    qvWm->UnmapWindows();

  focusMgr.RemoveUnmapList(qvWm);
  focusMgr.Remove(qvWm);

  XRemoveFromSaveSet(display, qvWm->GetWin());
  desktop.GetQvwmList().Remove(qvWm);
  qvWm->tButton->hide();

  delete qvWm;

  Gnome::ChangeClientList();

  if (yield && !ClickToFocus)
    desktop.ChangeFocusToCursor();
}

/*
 * ClientMessageProc --
 *   Process ClientMessageNotify event.
 */
void Event::ClientMessageProc(const XClientMessageEvent& ev)
{
  Qvwm* qvWm;

  qvWm = findQvwm(ev.window);
  if (qvWm) {
    if (ev.message_type == _XA_WM_CHANGE_STATE && ev.data.l[0] == IconicState)
      qvWm->MinimizeWindow();
    else
      Gnome::SetProperty(qvWm, ev.message_type, (long *)ev.data.l);
  }
}

/*
 * ColormapNotifyProc --
 *   Process ColormapNotify event.
 */
void Event::ColormapNotifyProc(const XColormapEvent& ev)
{
  Qvwm* qvWm;

  qvWm = findQvwm(ev.window);
  if (qvWm)
    qvWm->ChangeColormap(ev);
}

/*
 * ConfigureRequestProc --
 *   Process ConfigureRequest event.
 */
void Event::ConfigureRequestProc(const XConfigureRequestEvent& ev)
{
  Qvwm* qvWm;

  qvWm = findQvwm(ev.window);
  if (qvWm)
    qvWm->Configure(&ev);
  else {
    XWindowChanges xwc;
    unsigned long xwcm;

    xwcm = ev.value_mask & (CWX | CWY | CWWidth | CWHeight | CWBorderWidth);
    xwc.x = ev.x;
    xwc.y = ev.y;
    xwc.width = ev.width;
    xwc.height = ev.height;
    xwc.border_width = ev.border_width;

    XConfigureWindow(display, ev.window, xwcm, &xwc);
  }
}

void Event::CirculateRequestProc(const XCirculateRequestEvent& ev)
{
  Qvwm* qvWm;

  qvWm = findQvwm(ev.window);
  if (qvWm)
    qvWm->Circulate(ev.place);
}

/*
 * PropertyNotifyProc --
 *   Process PropertyNotify event.
 */
void Event::PropertyNotifyProc(const XPropertyEvent& ev)
{
  Qvwm* qvWm;

  qvWm = findQvwm(ev.window);
  if (qvWm)
    qvWm->SetProperty(ev.atom);
}    

#ifdef USE_SHAPE
/*
 * ShapeNotifyProc --
 *   Process ShapeNotify event.
 */
void Event::ShapeNotifyProc(const XShapeEvent& ev)
{
  Qvwm* qvWm;

  if (ev.kind != ShapeBounding)
    return;

  qvWm = findQvwm(ev.window);
  if (qvWm)
    qvWm->SetShape();
}
#endif // USE_SHAPE


Component* Event::findComponent(Window win)
{
  Component* comp;

  if (XFindContext(display, win, m_context, (caddr_t *)&comp) == XCSUCCESS)
    return comp;
  else
    return NULL;
}

Qvwm* Event::findQvwm(Window win)
{
  Component* comp;
  ClientArea* clientArea;

  comp = findComponent(win);
  if (comp) {
    if (comp->equalClass(Event::CLASS_CLIENT_AREA)) {
      clientArea = (ClientArea *)comp;
      return clientArea->getQvwm();
    }
  }

  return NULL;
}
