#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "window/qvwm.h"
#include "wm/Event.h"
#include "lib/Component.h"
#include "lib/misc.h"

Component::Component(Event::ClassId classId)
  : m_classId(classId)
{
  m_mapped = False;
}

Component::~Component()
{
}

void Component::show()
{
  m_mapped = True;
  XMapWindow(display, m_frame);
}

void Component::hide()
{
  m_mapped = False;
  XUnmapWindow(display, m_frame);
}

void Component::raise()
{
  XRaiseWindow(display, m_frame);
}

void Component::move(const Point& pt)
{
  XMoveWindow(display, m_frame, pt.x, pt.y);

  m_rc.x = pt.x;
  m_rc.y = pt.y;
}

void Component::resize(const Dim& size)
{
  XResizeWindow(display, m_frame, size.width, size.height);

  m_rc.width = size.width;
  m_rc.height = size.height;
}

void Component::reshape(const Rect& rc)
{
  XMoveResizeWindow(display, m_frame, rc.x, rc.y, rc.width, rc.height);

  m_rc = rc;
}

void Component::addWindow(Window win)
{
  XSaveContext(display, win, event.m_context, (caddr_t)this);
}

void Component::delWindow(Window win)
{
  XDeleteContext(display, win, event.m_context);
}

Bool Component::isDoubleClick(const XButtonEvent& ev)
{
  Point ptRoot(ev.x_root, ev.y_root);

  if (m_buttonPrev == ev.button && m_statePrev == ev.state &&
      IsDoubleClick(m_tmPrev, ev.time, m_ptRootPrev, ptRoot)) {
    // prevent triple clicks from judging two double clicks
    m_tmPrev = 0;
    return True;
  }

  m_tmPrev = ev.time;
  m_ptRootPrev = ptRoot;
  m_buttonPrev = ev.button;
  m_statePrev = ev.state;

  return False;
}

void Component::paint()
{
}

/*
 * Event handlers
 */
void Component::onExpose(const XExposeEvent& ev)
{
  paint();
}

void Component::onMouseEnter(const XCrossingEvent& ev)
{
}

void Component::onMouseLeave(const XCrossingEvent& ev)
{
}

void Component::onMouseMotion(const XMotionEvent& ev)
{
}

void Component::onButtonPress(const XButtonEvent& ev)
{
}

void Component::onDoubleClick(const XButtonEvent& ev)
{
  onButtonPress(ev);
}

void Component::onButtonRelease(const XButtonEvent& ev)
{
}

void Component::onKeyPress(const XKeyEvent& ev)
{
}

void Component::onFocusIn(const XFocusChangeEvent& ev)
{
}
