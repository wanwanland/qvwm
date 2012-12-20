#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "parser/qvwmrc.h"
#include "pager/ActivePage.h"
#include "pager/PageArea.h"
#include "pager/Pager.h"
#include "desktop/paging.h"

ActivePage::ActivePage(PageArea* pageArea, const Rect& rc)
  : m_pageArea(pageArea)
{
  XSetWindowAttributes attributes;
  unsigned long valueMask;

  m_rc = rc;

  attributes.background_pixel = PagerActiveColor.pixel;
  attributes.event_mask = ButtonPressMask | ButtonReleaseMask |
                          Button3MotionMask;
  valueMask = CWBackPixel | CWEventMask;
  
  m_frame = XCreateWindow(display, pageArea->getFrame(),
			  rc.x, rc.y, rc.width, rc.height,
			  0, CopyFromParent, InputOutput, CopyFromParent,
			  valueMask, &attributes);
  
  addWindow(m_frame);

  show();
}

ActivePage::~ActivePage()
{
  delWindow(m_frame);

  XDestroyWindow(display, m_frame);
}

/*
 * Process the press of button3(mouse right button).
 * You can move the visual page, or the current screen freely.
 */
void ActivePage::onButtonPress(const XButtonEvent& ev)
{
  if (ev.button != Button3)
    return;

  XEvent xev;
  Point ptOld, ptNew;
  Rect rcRoot = rootQvwm->GetRect();
  Bool pointer = False;
  
  Pager* pager = m_pageArea->getPager();
  pager->raise();

  if (UseTaskbar && !taskBar->IsHiding())
    paging->RaisePagingBelt();

  XGrabPointer(display, m_frame, True, Button3MotionMask | ButtonReleaseMask,
	       GrabModeAsync, GrabModeAsync, m_pageArea->getFrame(),
	       None, CurrentTime);

  ptOld = Point(ev.x_root, ev.y_root);

  while (1) {
    XMaskEvent(display,
	       Button3MotionMask | ButtonReleaseMask | ExposureMask |
	       ButtonPressMask | PointerMotionMask,
	       &xev);
    switch (xev.type) {
    case MotionNotify:
      {
	Point oldOrigin = paging->origin;
	Rect rcPageArea = m_pageArea->getRect();

	ptNew = Point(xev.xbutton.x_root, xev.xbutton.y_root);
	paging->origin.x += (ptNew.x - ptOld.x) *
	  (rcRoot.width * paging->rcVirt.width / rcPageArea.width);
	paging->origin.y += (ptNew.y - ptOld.y) *
	  (rcRoot.height * paging->rcVirt.height / rcPageArea.height);
	ptOld = ptNew;
	paging->PagingAllWindows(oldOrigin);
	break;
      }

    case ButtonRelease:
      if (LockDragState) {
	pointer = True;
	XChangeActivePointerGrab(display, ButtonPressMask | PointerMotionMask,
				 None, CurrentTime);
	break;
      }
      else {
	XUngrabPointer(display, CurrentTime);
	return;
      }

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
