#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "taskbar/taskbar.h"
#include "taskbar/TaskbarBorder.h"
#include "parser/qvwmrc.h"
#include "image/Image.h"

TaskbarBorder::TaskbarBorder(Taskbar* taskBar)
  : m_taskBar(taskBar)
{
  XSetWindowAttributes attributes;
  unsigned long valueMask;

  attributes.event_mask = ButtonPressMask | ButtonReleaseMask | ExposureMask |
                          EnterWindowMask | LeaveWindowMask |
			  Button1MotionMask | Button2MotionMask;
  attributes.background_pixel = TaskbarColor.pixel;
  valueMask = CWBackPixel | CWEventMask;

  m_frame = XCreateWindow(display, taskBar->getFrame(),
			  0, 0, 1, 1,
			  0, CopyFromParent, InputOutput, CopyFromParent,
			  valueMask, &attributes);
  addWindow(m_frame);
}

TaskbarBorder::~TaskbarBorder()
{
  delWindow(m_frame);
  XDestroyWindow(display, m_frame);
}

void TaskbarBorder::setPosition(BorderPos pos)
{
  Rect rcTaskbar = m_taskBar->getRect();

  m_pos = pos;

  switch (pos) {
  case TOP:
    reshape(Rect(0, 0, rcTaskbar.width, 4));
    XDefineCursor(display, m_frame, cursor[Y_RESIZE]);
    break;

  case BOTTOM:
    reshape(Rect(0, rcTaskbar.height - 4, rcTaskbar.width, 4));
    XDefineCursor(display, m_frame, cursor[Y_RESIZE]);
    break;

  case LEFT:
    reshape(Rect(0, 0, 4, rcTaskbar.height));
    XDefineCursor(display, m_frame, cursor[X_RESIZE]);
    break;

  case RIGHT:
    reshape(Rect(rcTaskbar.width - 4, 0, 4, rcTaskbar.height));
    XDefineCursor(display, m_frame, cursor[X_RESIZE]);
    break;
  }
}

/*
 * Redraw funtion
 */
void TaskbarBorder::paint()
{
  switch (m_pos) {
  case TOP:
    XSetForeground(display, gc, gray.pixel);
    XDrawLine(display, m_frame, gc, 0, 0, m_rc.width - 1, 0);
    XSetForeground(display, gc, white.pixel);
    XDrawLine(display, m_frame, gc, 0, 1, m_rc.width - 1, 1);
    break;

  case BOTTOM:
    XSetForeground(display, gc, darkGray.pixel);
    XDrawLine(display, m_frame, gc,
	      0, m_rc.height - 2, m_rc.width - 1, m_rc.height - 2);
    XSetForeground(display, gc, black.pixel);
    XDrawLine(display, m_frame, gc,
	      0, m_rc.height - 1, m_rc.width - 1, m_rc.height - 1);
    break;

  case LEFT:
    XSetForeground(display, gc, gray.pixel);
    XDrawLine(display, m_frame, gc, 0, 0, 0, m_rc.height - 1);
    XSetForeground(display, gc, white.pixel);
    XDrawLine(display, m_frame, gc, 1, 0, 1, m_rc.height - 1);
    break;

  case RIGHT:
    XSetForeground(display, gc, darkGray.pixel);
    XDrawLine(display, m_frame, gc,
	      m_rc.width - 2, 0, m_rc.width - 2, m_rc.height - 1);
    XSetForeground(display, gc, black.pixel);
    XDrawLine(display, m_frame, gc,
	      m_rc.width - 1, 0, m_rc.width - 1, m_rc.height - 1);
    break;
  }
}

void TaskbarBorder::onMouseEnter(const XCrossingEvent& ev)
{
  m_taskBar->onMouseEnter(ev);
}

void TaskbarBorder::onMouseLeave(const XCrossingEvent& ev)
{
  m_taskBar->onMouseLeave(ev);
}

void TaskbarBorder::onMouseMotion(const XMotionEvent& ev)
{
  if (ev.state & Button1Mask)
    m_taskBar->dragResizeTaskbar(Point(ev.x_root, ev.y_root));
}

void TaskbarBorder::onButtonPress(const XButtonEvent& ev)
{
  m_taskBar->onButtonPress(ev);
}

void TaskbarBorder::onButtonRelease(const XButtonEvent& ev)
{
  m_taskBar->onButtonRelease(ev);
}
