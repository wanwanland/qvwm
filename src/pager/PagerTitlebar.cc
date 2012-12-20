#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "parser/qvwmrc.h"
#include "pager/PagerTitlebar.h"
#include "pager/Pager.h"
#include "desktop/paging.h"
#include "lib/Titlebar.h"

PagerTitlebar::PagerTitlebar(Pager* pager, const Rect& rc)
  : m_pager(pager)
{
  XSetWindowAttributes attributes;
  unsigned long valueMask;

  m_rc = rc;

  attributes.background_pixel = TitlebarActiveColor.pixel;
  attributes.event_mask = ButtonPressMask | ButtonReleaseMask |
                          Button1MotionMask;
  valueMask = CWBackPixel | CWEventMask;
  
  m_frame = XCreateWindow(display, pager->getFrame(),
			  rc.x, rc.y, rc.width, rc.height,
			  0, CopyFromParent, InputOutput, CopyFromParent,
			  valueMask, &attributes);
  
  addWindow(m_frame);

  if (GradTitlebar) {
    Pixmap pixGrad = Titlebar::createGradPixmap(Titlebar::m_gradActivePattern,
						rc.width, m_frame);
    XSetWindowBackgroundPixmap(display, m_frame, pixGrad);
  }

  show();
}

PagerTitlebar::~PagerTitlebar()
{
  delWindow(m_frame);

  XDestroyWindow(display, m_frame);
}

void PagerTitlebar::onButtonPress(const XButtonEvent& ev)
{
  if (ev.button != Button1)
    return;

  m_pager->raise();

  if (UseTaskbar) {
    if (OnTopTaskbar && !OnTopPager)
      taskBar->raise();
    if (!taskBar->IsHiding())
      paging->RaisePagingBelt();
  }

  if (DisableDesktopChange)
    return;

  /*
   * Move the pager.
   */
  XEvent xev;
  Point ptRoot(ev.x_root, ev.y_root);
  Point ptOld(ptRoot), ptNew, ptMove;
  Rect rcPager = m_pager->getRect();
  Rect rcOrig = m_pager->getOrigRect();
  Bool pointer = False;

  while (1) {
    XMaskEvent(display,
	       Button1MotionMask | ButtonReleaseMask | ExposureMask |
	       ButtonPressMask | PointerMotionMask,
	       &xev);
    switch (xev.type) {
    case MotionNotify:
      ptNew = Point(xev.xmotion.x_root, xev.xmotion.y_root);
      ptMove = ptNew - ptOld;
      ptOld = ptNew;

      rcPager.x += ptMove.x;
      rcPager.y += ptMove.y;
      rcOrig.x += ptMove.x;
      rcOrig.y += ptMove.y;

      m_pager->move(Point(rcPager.x, rcPager.y));
      m_pager->setOrigRect(rcOrig);
      break;

    case ButtonRelease:
      if (LockDragState) {
	pointer = True;
	XGrabPointer(display, root, True,
		     ButtonPressMask | PointerMotionMask,
		     GrabModeAsync, GrabModeAsync, root, None, CurrentTime);
	break;
      }
      else
	return;

    case ButtonPress:
      if (pointer) {
	XUngrabPointer(display, CurrentTime);
	return;
      }
      break;

    case Expose:
      event.ExposeProc((const XExposeEvent &)xev);
      break;
    }
  }
}
