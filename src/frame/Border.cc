#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "frame/Border.h"
#include "parser/qvwmrc.h"
#include "image/Image.h"

QvImage* Border::m_imgFrame;
QvImage* Border::m_imgActiveFrame;

Border::Border(Qvwm* qvWm, BorderPos pos)
  : m_qvWm(qvWm), m_pos(pos)
{
  XSetWindowAttributes attributes;
  unsigned long valueMask;

  attributes.event_mask = ButtonPressMask | ButtonReleaseMask | ExposureMask |
                          EnterWindowMask | LeaveWindowMask |
			  Button1MotionMask | Button2MotionMask;
  attributes.background_pixel = FrameColor.pixel;
  valueMask = CWBackPixel | CWEventMask;

  m_frame = XCreateWindow(display, qvWm->GetFrameWin(),
			  0, 0, 1, 1,
			  0, CopyFromParent, InputOutput, CopyFromParent,
			  valueMask, &attributes);
  addWindow(m_frame);

  if (FrameImage) {
    m_imgBorder = m_imgFrame->Duplicate();
    m_imgBorder->SetBackground(m_frame);
  }
  if (FrameActiveImage)
    m_imgActiveBorder = m_imgActiveFrame->Duplicate();
  
  switch (pos) {
  case TOP:
  case BOTTOM:
    XDefineCursor(display, m_frame, cursor[Y_RESIZE]);
    break;

  case LEFT:
  case RIGHT:
    XDefineCursor(display, m_frame, cursor[X_RESIZE]);
    break;

  case TOP_LEFT:
  case BOTTOM_RIGHT:
    XDefineCursor(display, m_frame, cursor[RD_RESIZE]);
    break;

  case TOP_RIGHT:
  case BOTTOM_LEFT:
    XDefineCursor(display, m_frame, cursor[LD_RESIZE]);
    break;
  }
}

Border::~Border()
{
  delWindow(m_frame);
  XDestroyWindow(display, m_frame);

  if (FrameImage)
    QvImage::Destroy(m_imgBorder);
  if (FrameActiveImage)
    QvImage::Destroy(m_imgActiveBorder);
}

/*
 * Redraw function
 */
void Border::paintWithFocusChange()
{
  if (m_qvWm->CheckFocus()) {
    if (FrameImage)
      m_imgBorder->SetBackground(None);

    if (FrameActiveImage) {
      m_imgActiveBorder->SetBackground(m_frame);
      XClearWindow(display, m_frame);
      paint();
    }
    else {
      if (FrameActiveColor.pixel != FrameColor.pixel) {
	XSetWindowBackground(display, m_frame, FrameActiveColor.pixel);
	XClearWindow(display, m_frame);
	paint();
      }
    }
  }
  else {
    if (FrameActiveImage)
      m_imgActiveBorder->SetBackground(None);
    
    if (FrameImage) {
      m_imgBorder->SetBackground(m_frame);
      XClearWindow(display, m_frame);
      paint();
    }
    else {
      if (FrameColor.pixel != FrameActiveColor.pixel) {
	XSetWindowBackground(display, m_frame, FrameColor.pixel);
	XClearWindow(display, m_frame);
	paint();
      }
    }
  }
}

void Border::paint()
{
  XPoint xp[3];

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
    XSetForeground(display, gc, darkGrey.pixel);
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
    XSetForeground(display, gc, darkGrey.pixel);
    XDrawLine(display, m_frame, gc,
	      m_rc.width - 1, 0, m_rc.width - 1, m_rc.height - 1);
    break;

  case TOP_LEFT:
    xp[0].x = m_rc.width - 1;
    xp[0].y = 0;
    xp[1].x = 0;
    xp[1].y = 0;
    xp[2].x = 0;
    xp[2].y = m_rc.height - 1;
    XSetForeground(display, gc, gray.pixel);
    XDrawLines(display, m_frame, gc, xp, 3, CoordModeOrigin);
    
    xp[0].x = m_rc.width - 1;
    xp[0].y = 1;
    xp[1].x = 1;
    xp[1].y = 1;
    xp[2].x = 1;
    xp[2].y = m_rc.height - 1;
    XSetForeground(display, gc, white.pixel);
    XDrawLines(display, m_frame, gc, xp, 3, CoordModeOrigin);
    break;

  case TOP_RIGHT:
    XSetForeground(display, gc, gray.pixel);
    XDrawLine(display, m_frame, gc, 0, 0, m_rc.width - 2, 0);
    XSetForeground(display, gc, white.pixel);
    XDrawLine(display, m_frame, gc, 0, 1, m_rc.width - 3, 1);
    XSetForeground(display, gc, darkGray.pixel);
    XDrawLine(display, m_frame, gc,
	      m_rc.width - 2, 1, m_rc.width - 2, m_rc.height - 1);
    XSetForeground(display, gc, darkGrey.pixel);
    XDrawLine(display, m_frame, gc,
	      m_rc.width - 1, 0, m_rc.width - 1, m_rc.height - 1);
    break;

  case BOTTOM_LEFT:
    XSetForeground(display, gc, gray.pixel);
    XDrawLine(display, m_frame, gc, 0, 0, 0, m_rc.height - 2);
    XSetForeground(display, gc, white.pixel);
    XDrawLine(display, m_frame, gc, 1, 0, 1, m_rc.height - 3);
    XSetForeground(display, gc, darkGray.pixel);
    XDrawLine(display, m_frame, gc,
	      1, m_rc.width - 2, m_rc.width - 1, m_rc.height - 2);
    XSetForeground(display, gc, darkGrey.pixel);
    XDrawLine(display, m_frame, gc,
	      0, m_rc.width - 1, m_rc.width - 1, m_rc.height - 1);
    break;

  case BOTTOM_RIGHT:
    xp[0].x = m_rc.width - 2;
    xp[0].y = 0;
    xp[1].x = m_rc.width - 2;
    xp[1].y = m_rc.height - 2;
    xp[2].x = 0;
    xp[2].y = m_rc.height - 2;
    XSetForeground(display, gc, darkGray.pixel);
    XDrawLines(display, m_frame, gc, xp, 3, CoordModeOrigin);
    
    xp[0].x = m_rc.width - 1;
    xp[0].y = 0;
    xp[1].x = m_rc.width - 1;
    xp[1].y = m_rc.height - 1;
    xp[2].x = 0;
    xp[2].y = m_rc.height - 1;
    XSetForeground(display, gc, darkGrey.pixel);
    XDrawLines(display, m_frame, gc, xp, 3, CoordModeOrigin);
    break;
  }
}

void Border::onMouseEnter(const XCrossingEvent& ev)
{
  m_qvWm->onMouseEnter(ev);
}

void Border::onMouseMotion(const XMotionEvent& ev)
{
  static unsigned int transTable[8] = {
    P_TOP, P_BOTTOM, P_LEFT, P_RIGHT,
    P_TOP | P_LEFT, P_TOP | P_RIGHT, P_BOTTOM | P_LEFT, P_BOTTOM | P_RIGHT 
  };

  if (ev.state & Button1Mask)
    m_qvWm->ResizeWindow(transTable[m_pos], True);
  else if (ev.state & Button2Mask) {
    if (!m_qvWm->CheckStatus(MAXIMIZE_WINDOW))
      m_qvWm->MoveWindow(Point(ev.x_root, ev.y_root), True);
  }
}

void Border::onButtonPress(const XButtonEvent& ev)
{
  if (ev.button == Button2)
    m_qvWm->SetStatus(PRESS_FRAME);

  m_qvWm->onButtonPress(ev);
}

void Border::onButtonRelease(const XButtonEvent& ev)
{
  m_qvWm->onButtonRelease(ev);

  if (ev.state & Button2Mask)
    m_qvWm->LowerWindow();
}

/*
 * Static functions
 */
void Border::initialize()
{
  if (FrameImage) {
    m_imgFrame = CreateImageFromFile(FrameImage, timer);
    if (m_imgFrame == NULL) {
      delete [] FrameImage;
      FrameImage = NULL;
    }
  }
  if (FrameActiveImage) {
    m_imgActiveFrame = CreateImageFromFile(FrameActiveImage, timer);
    if (m_imgActiveFrame == NULL) {
      delete [] FrameActiveImage;
      FrameActiveImage = NULL;
    }
  }
}
