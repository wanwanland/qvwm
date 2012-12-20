#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <unistd.h>
#include "frame/WindowTitlebar.h"
#include "window/qvwm.h"
#include "frame/FrameButton.h"
#include "frame/CtrlButton.h"
#include "parser/qvwmrc.h"
#include "desktop/icon.h"

#define SYMBOL_WIDTH 16
#define SYMBOL_HEIGHT 16

WindowTitlebar::WindowTitlebar(Qvwm* qvWm, const Rect& rc)
  : Titlebar(qvWm, rc), m_qvWm(qvWm)
{
  // create a control button
  if (m_qvWm->CheckFlags(CTRL_MENU)) {
    Rect rcCtrl(2, (rc.height - SYMBOL_HEIGHT) / 2,
		SYMBOL_WIDTH, SYMBOL_HEIGHT);

    m_ctrlButton = new CtrlButton(this, rcCtrl, m_qvWm);
  }
  else
    m_ctrlButton = NULL;

  // create three kinds of frame button
  Rect rcFButton[3];

#define BUTTON_WIDTH 16
#define BUTTON_HEIGHT 14

  rcFButton[0].x = rc.width - (BUTTON_WIDTH * 3 + 4);
  rcFButton[1].x = rc.width - (BUTTON_WIDTH * 2 + 4);
  rcFButton[2].x = rc.width - (BUTTON_WIDTH + 2);
  if (!m_qvWm->CheckFlags(BUTTON3)) {
    rcFButton[0].x = rcFButton[1].x;
    rcFButton[1].x = rcFButton[2].x;
  }
  if (!m_qvWm->CheckFlags(BUTTON2))
    rcFButton[0].x = rcFButton[1].x;
  for (int i = 0; i < 3; i++) {
    rcFButton[i].y = (rc.height - BUTTON_HEIGHT) / 2;
    rcFButton[i].width = BUTTON_WIDTH;
    rcFButton[i].height = BUTTON_HEIGHT;
  }

  m_fButton[0] = new FrameButton(this, rcFButton[0], m_qvWm);
  m_fButton[0]->ChangeImage(FrameButton::MINIMIZE);
  m_fButton[0]->setActionListener(new MinimizeAction(m_qvWm));

  m_fButton[1] = new FrameButton(this, rcFButton[1], m_qvWm);
  if (m_qvWm->CheckFlags(INIT_MAXIMIZE))
    m_fButton[1]->ChangeImage(FrameButton::RESTORE);
  else
    m_fButton[1]->ChangeImage(FrameButton::MAXIMIZE);
  m_fButton[1]->setActionListener(new MaximizeRestoreAction(m_fButton[1],
							    m_qvWm));

  m_fButton[2] = new FrameButton(this, rcFButton[2], m_qvWm);
  m_fButton[2]->ChangeImage(FrameButton::CLOSE);
  m_fButton[2]->setActionListener(new CloseAction(m_qvWm));

  // BUTTON3 may be dynamically set by WM_DELETE_WINDOW
  if (m_qvWm->CheckFlags(BUTTON1))
    m_fButton[0]->show();
  if (m_qvWm->CheckFlags(BUTTON2))
    m_fButton[1]->show();
  if (m_qvWm->CheckFlags(BUTTON3))
    m_fButton[2]->show();
}

WindowTitlebar::~WindowTitlebar()
{
  delete m_ctrlButton;
  for (int i = 0; i < 3; i++)
    delete m_fButton[i];
}

void WindowTitlebar::setImage(QvImage* img)
{
  if (m_ctrlButton)
    m_ctrlButton->setImage(img);
}

int WindowTitlebar::getTitleWidth()
{
  int titleWidth;

  if (m_qvWm->CheckFlags(BUTTON1)) {
    Rect rcFButton = m_fButton[0]->getRect();
    titleWidth = rcFButton.x - 2;
  }
  else if (m_qvWm->CheckFlags(BUTTON2)) {
    Rect rcFButton = m_fButton[1]->getRect();
    titleWidth = rcFButton.x - 2;
  }
  else if (m_qvWm->CheckFlags(BUTTON3)) {
    Rect rcFButton = m_fButton[2]->getRect();
    titleWidth = rcFButton.x - 2;
  }
  else
    titleWidth = m_rc.width - 2;

  if (m_ctrlButton) {
    Rect rcCtrlButton = m_ctrlButton->getRect();
    titleWidth -= rcCtrlButton.y + rcCtrlButton.width + 4;
  }
  else
    titleWidth -= 4;

  return titleWidth;
}

void WindowTitlebar::reshape(const Rect& rc)
{
  Titlebar::reshape(rc);

  if (m_ctrlButton) {
    Point ptCtrl(2, (m_rc.height - SYMBOL_HEIGHT) / 2);

    m_ctrlButton->move(ptCtrl);
  }

  for (int i = 0; i < 3; i++) {
    Point ptFButton[3];

    ptFButton[0].x = m_rc.width - (BUTTON_WIDTH * 3 + 4);
    ptFButton[1].x = m_rc.width - (BUTTON_WIDTH * 2 + 4);
    ptFButton[2].x = m_rc.width - (BUTTON_WIDTH + 2);
    if (!m_qvWm->CheckFlags(BUTTON3)) {
      ptFButton[0].x = ptFButton[1].x;
      ptFButton[1].x = ptFButton[2].x;
    }
    if (!m_qvWm->CheckFlags(BUTTON2))
      ptFButton[0].x = ptFButton[1].x;
    for (int j = 0; j < 3; j++)
      ptFButton[j].y = (m_rc.height - BUTTON_HEIGHT) / 2;

    m_fButton[i]->move(ptFButton[i]);

    if (TitlebarImage) {
      m_fButton[i]->SetBgImage(m_imgTitlebar, ptFButton[i]);
      m_fButton[i]->SetBgActiveImage(m_imgTitlebar, ptFButton[i]);
      m_fButton[i]->paintWithFocusChange();
    }
  }
}

/*
 * Move the titlebar to dest slowly.
 */
void WindowTitlebar::animate(const Rect& rcSrc, const Rect& rcDest)
{
  Window motionBarWin;
  Rect rcNew;
  XSetWindowAttributes attributes;
  unsigned long valueMask;
  QvImage* imgIcon = m_qvWm->GetIconImage();

  if (!TitlebarMotion)
    return;

  XSync(display, 0);

  attributes.save_under = DoesSaveUnders(ScreenOfDisplay(display, screen));
  attributes.background_pixel = TitlebarActiveColor.pixel;
  valueMask = CWSaveUnder | CWBackPixel;

  /*
   * Create a motion title bar.
   */
  motionBarWin = XCreateWindow(display, root, rcSrc.x, rcSrc.y,
			       rcSrc.width, m_rc.height,
			       0, CopyFromParent, InputOutput, CopyFromParent, 
			       valueMask, &attributes);

  XMapRaised(display, motionBarWin);

  /*
   * Move the titlebar slowly.
   */
  XSetForeground(display, gc, white.pixel);

  for (int i = 1; i < TitlebarMotionSpeed; i++) {
    rcNew.x = rcSrc.x + (rcDest.x - rcSrc.x) * i / TitlebarMotionSpeed;
    rcNew.y = rcSrc.y + (rcDest.y - rcSrc.y) * i / TitlebarMotionSpeed;
    rcNew.width = rcSrc.width + (rcDest.width - rcSrc.width) * i
      / TitlebarMotionSpeed;

    XMoveResizeWindow(display, motionBarWin,
		      rcNew.x, rcNew.y, rcNew.width, m_rc.height);
    imgIcon->Display(motionBarWin, Point(2, 1));

    /*
     * If you don't have this function, motion title bar may be underneath
     * main window.
     */
    XRaiseWindow(display, motionBarWin);

    XFlush(display);
    usleep(10000);
  }

  XDestroyWindow(display, motionBarWin);
}

void WindowTitlebar::paintWithFocusChange()
{
  if (m_qvWm->CheckFocus()) {
    if (TitlebarImage)
      m_imgTitle->SetBackground(None);
    
    if (TitlebarActiveImage)
      m_imgActiveTitle->SetBackground(m_frame);
    else if (GradTitlebar) {
      if (m_pixActiveGrad == None)
	m_pixActiveGrad = createGradPixmap(m_gradActivePattern,
					   m_rc.width, m_frame);
      XSetWindowBackgroundPixmap(display, m_frame, m_pixActiveGrad);
    }
    else
      XSetWindowBackground(display, m_frame, TitlebarActiveColor.pixel);
  }
  else {
    if (TitlebarActiveImage)
      m_imgActiveTitle->SetBackground(None);
    
    if (TitlebarImage)
      m_imgTitle->SetBackground(m_frame);
    else if (GradTitlebar) {
      if (m_pixGrad == None)
	m_pixGrad = createGradPixmap(m_gradPattern, m_rc.width, m_frame);
      XSetWindowBackgroundPixmap(display, m_frame, m_pixGrad);
    }
    else
      XSetWindowBackground(display, m_frame, TitlebarColor.pixel);
  }

  XClearWindow(display, m_frame);

  paint();
  
  if (m_ctrlButton)
    m_ctrlButton->paintWithFocusChange();

  for (int i = 0; i < 3; i++)
    m_fButton[i]->paintWithFocusChange();
}

int WindowTitlebar::getTitleX()
{
  int x;

  if (m_ctrlButton) {
    Rect rcCtrlButton = m_ctrlButton->getRect();
    x = rcCtrlButton.y + rcCtrlButton.width + 4;
  }
  else
    x = 4;

  return x;
}

const XColor& WindowTitlebar::getForeColor()
{
  if (m_qvWm->CheckFocus())
    return TitleStringActiveColor;
  else
    return TitleStringColor;
}

/*
 * Event handlers
 */
void WindowTitlebar::onMouseEnter(const XCrossingEvent& ev)
{
  Titlebar::onMouseEnter(ev);

  m_qvWm->onMouseEnter(ev);
}

void WindowTitlebar::onMouseMotion(const XMotionEvent& ev)
{
  Titlebar::onMouseMotion(ev);

  if (ev.state & (Button1Mask | Button2Mask)) {
    if (!m_qvWm->CheckStatus(MAXIMIZE_WINDOW))
      m_qvWm->MoveWindow(Point(ev.x_root, ev.y_root), True);
  }
}

void WindowTitlebar::onButtonPress(const XButtonEvent& ev)
{
  Titlebar::onButtonPress(ev);

  m_qvWm->onButtonPress(ev);
}

void WindowTitlebar::onButtonRelease(const XButtonEvent& ev)
{
  m_qvWm->onButtonRelease(ev);

  if (ev.state & Button2Mask)
    m_qvWm->LowerWindow();
}

void WindowTitlebar::onDoubleClick(const XButtonEvent& ev)
{
  Menu::UnmapAllMenus();
    
  if (Icon::focusIcon != NULL)
    Icon::focusIcon->ResetFocus();

  m_qvWm->SetFocus();

  if (ClickingRaises)
    m_qvWm->RaiseWindow(True);

  /*
   * When double click.
   */
  if (!m_qvWm->CheckFlags(TRANSIENT)) {
    m_fButton[1]->SetState(Button::NORMAL);
    if (m_qvWm->CheckStatus(MAXIMIZE_WINDOW)) {
      /*
       * Restore if maximun window.
       */
      m_fButton[1]->ChangeImage(FrameButton::MAXIMIZE);
      m_qvWm->RestoreWindow();
    }
    else {
      /*
       * Maximize if normal window.
       */
      m_fButton[1]->ChangeImage(FrameButton::RESTORE);
      m_qvWm->MaximizeWindow();
    }
  }

  m_qvWm->SetStatus(PRESS_FRAME);

  if (AutoRaise && Qvwm::focusQvwm == m_qvWm && Qvwm::activeQvwm != m_qvWm)
    m_qvWm->RaiseWindow(True);
}
