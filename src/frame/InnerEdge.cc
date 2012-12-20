#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "frame/InnerEdge.h"
#include "parser/qvwmrc.h"

InnerEdge::InnerEdge(Qvwm* qvWm, EdgePos pos)
  : m_qvWm(qvWm), m_pos(pos)
{
  XSetWindowAttributes attributes;
  unsigned long valueMask;

  attributes.event_mask = ExposureMask;
  attributes.background_pixel = FrameColor.pixel;
  valueMask = CWBackPixel | CWEventMask;

  m_frame = XCreateWindow(display, qvWm->GetFrameWin(),
			  0, 0, 1, 1,
			  0, CopyFromParent, InputOutput, CopyFromParent,
			  valueMask, &attributes);

  addWindow(m_frame);
}

InnerEdge::~InnerEdge()
{
  delWindow(m_frame);
  XDestroyWindow(display, m_frame);
}

/*
 * Redraw function
 */
void InnerEdge::paint()
{
  XPoint xp[3];
  int yBase;

  switch (m_pos) {
  case TOP:
    yBase = m_qvWm->CheckFlags(TITLE) ? 1 : 0;

    xp[0].x = m_rc.width - 1;
    xp[0].y = yBase;
    xp[1].x = 0;
    xp[1].y = yBase;
    xp[2].x = 0;
    xp[2].y = yBase + 1;
    XSetForeground(display, gc, darkGray.pixel);
    XDrawLines(display, m_frame, gc, xp, 3, CoordModeOrigin);
    
    XSetForeground(display, gc, darkGrey.pixel);
    XDrawLine(display, m_frame, gc, 1, yBase + 1, m_rc.width - 2, yBase + 1);
    
    XSetForeground(display, gc, white.pixel);
    XDrawPoint(display, m_frame, gc, m_rc.width - 1, yBase + 1);
    break;

  case BOTTOM:
    xp[0].x = 0;
    xp[0].y = 1;
    xp[1].x = m_rc.width - 1;
    xp[1].y = 1;
    xp[2].x = m_rc.width - 1;
    xp[2].y = 0;
    XSetForeground(display, gc, white.pixel);
    XDrawLines(display, m_frame, gc, xp, 3, CoordModeOrigin);

    XSetForeground(display, gc, gray.pixel);
    XDrawLine(display, m_frame, gc, 1, 0, m_rc.width - 2, 0);
    
    XSetForeground(display, gc, darkGray.pixel);
    XDrawPoint(display, m_frame, gc, 0, 0);
    break;

  case LEFT:
    XSetForeground(display, gc, darkGray.pixel);
    XDrawLine(display, m_frame, gc, 0, 0, 0, m_rc.height - 1);
    XSetForeground(display, gc, darkGrey.pixel);
    XDrawLine(display, m_frame, gc, 1, 0, 1, m_rc.height - 1);
    break;

  case RIGHT:
    XSetForeground(display, gc, gray.pixel);
    XDrawLine(display, m_frame, gc, 0, 0, 0, m_rc.height - 1);
    XSetForeground(display, gc, white.pixel);
    XDrawLine(display, m_frame, gc, 1, 0, 1, m_rc.height - 1);
    break;
  }
}
