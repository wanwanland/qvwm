#ifndef WINDOW_TITLEBAR_H_
#define WINDOW_TITLEBAR_H_

#include "lib/Titlebar.h"

class CtrlButton;
class FrameButton;
class Qvwm;

class WindowTitlebar : public Titlebar {
private:
  Qvwm *m_qvWm;

  CtrlButton* m_ctrlButton;
  FrameButton* m_fButton[3];

protected:
  int getTitleWidth();
  int getTitleX();
  const XColor& getForeColor();

public:
  WindowTitlebar(Qvwm* qvWm, const Rect& rc);
  ~WindowTitlebar();

  void reshape(const Rect& rc);
  void animate(const Rect& rcSrc, const Rect& rcDest);

  void setImage(QvImage* img);
  FrameButton* getFrameButton1() const { return m_fButton[1]; }

  void paintWithFocusChange();

  void onMouseEnter(const XCrossingEvent& ev);
  void onMouseMotion(const XMotionEvent& ev);
  void onButtonPress(const XButtonEvent& ev);
  void onButtonRelease(const XButtonEvent& ev);
  void onDoubleClick(const XButtonEvent& ev);
};

#endif // WINDOW_TITLEBAR_H_
