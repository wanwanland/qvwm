#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "parser/qvwmrc.h"
#include "taskbar/taskbar.h"
#include "lib/Toolbar.h"

Toolbar::Toolbar(Taskbar* taskBar)
  : m_taskBar(taskBar)
{
  XSetWindowAttributes attributes;
  unsigned long valueMask;

  attributes.event_mask = ExposureMask | ButtonPressMask | ButtonReleaseMask |
                          Button1MotionMask;
  attributes.background_pixel = TaskbarColor.pixel;
  valueMask = CWBackPixel | CWEventMask;

  m_frame = XCreateWindow(display, taskBar->getFrame(),
			  0, 0, 1, 1,
			  0, CopyFromParent, InputOutput, CopyFromParent,
			  valueMask, &attributes);

  addWindow(m_frame);

  m_imgBg = NULL;
}

Toolbar::~Toolbar()
{
  delWindow(m_frame);

  XDestroyWindow(display, m_frame);

  QvImage::Destroy(m_imgBg);
}

Point Toolbar::getAbsLocation() const
{
  Point ptTaskbar = m_taskBar->getLocation();

  return Point(ptTaskbar.x + m_rc.x, ptTaskbar.y + m_rc.y);
}

void Toolbar::setBgImage(const Point& offset)
{
  QvImage* imgTaskbar = m_taskBar->getBgImage();

  QvImage::Destroy(m_imgBg);
  m_imgBg = imgTaskbar->GetOffsetImage(offset);
  m_imgBg->SetBackground(m_frame);
}

void Toolbar::paint()
{
  XPoint xp[3];

  XClearWindow(display, m_frame);

  switch (m_taskBar->GetPos()) {
  case Taskbar::BOTTOM:
  case Taskbar::TOP:
    XSetForeground(display, gc, darkGray.pixel);
    XDrawLine(display, m_frame, gc, 1, 0, 1, m_rc.height - 1);
    XSetForeground(display, gc, white.pixel);
    XDrawLine(display, m_frame, gc, 2, 0, 2, m_rc.height - 1);

    xp[0].x = 6;
    xp[0].y = 2;
    xp[1].x = 5;
    xp[1].y = 2;
    xp[2].x = 5;
    xp[2].y = m_rc.height - 3;
    XSetForeground(display, gc, white.pixel);
    XDrawLines(display, m_frame, gc, xp, 3, CoordModeOrigin);

    xp[0].x = 7;
    xp[0].y = 2;
    xp[1].x = 7;
    xp[1].y = m_rc.height - 3;
    xp[2].x = 5;
    xp[2].y = m_rc.height - 3;
    XSetForeground(display, gc, darkGray.pixel);
    XDrawLines(display, m_frame, gc, xp, 3, CoordModeOrigin);
    break;

  case Taskbar::LEFT:
  case Taskbar::RIGHT:
    XSetForeground(display, gc, darkGray.pixel);
    XDrawLine(display, m_frame, gc, 0, 1, m_rc.width - 1, 1);
    XSetForeground(display, gc, white.pixel);
    XDrawLine(display, m_frame, gc, 0, 2, m_rc.width - 1, 2);
    
    xp[0].x = m_rc.width - 3;
    xp[0].y = 5;
    xp[1].x = 2;
    xp[1].y = 5;
    xp[2].x = 2;
    xp[2].y = 6;
    XSetForeground(display, gc, white.pixel);
    XDrawLines(display, m_frame, gc, xp, 3, CoordModeOrigin);

    xp[0].x = 2;
    xp[0].y = 7;
    xp[1].x = m_rc.width - 3;
    xp[1].y = 7;
    xp[2].x = m_rc.width - 3;
    xp[2].y = 5;
    XSetForeground(display, gc, darkGray.pixel);
    XDrawLines(display, m_frame, gc, xp, 3, CoordModeOrigin);
    break;
  }
}

/*
 * Event handlers
 */
void Toolbar::onMouseMotion(const XMotionEvent& ev)
{
  if (ev.state & Button1Mask)
    m_taskBar->dragTaskbar(Point(ev.x_root, ev.y_root));
}

void Toolbar::onButtonPress(const XButtonEvent& ev)
{
  m_taskBar->onButtonPress(ev);
}

void Toolbar::onButtonRelease(const XButtonEvent& ev)
{
  m_taskBar->onButtonRelease(ev);
}
