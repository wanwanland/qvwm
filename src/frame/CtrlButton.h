#ifndef CTRL_BUTTON_H_
#define CTRL_BUTTON_H_

#include "lib/Component.h"

class QvImage;
class Qvwm;
class WindowTitlebar;

class CtrlButton : public Component {
private:
  QvImage* m_img;
  Bool m_unmapFlag;

  Qvwm* m_qvWm;

public:
  CtrlButton(WindowTitlebar* titleBar, const Rect& rc, Qvwm* qvWm);
  ~CtrlButton();

  void setImage(QvImage* img);

  void paintWithFocusChange();
  virtual void paint();

  virtual void onButtonPress(const XButtonEvent& ev);
  virtual void onDoubleClick(const XButtonEvent& ev);
  virtual void onButtonRelease(const XButtonEvent& ev);
};

#endif // CTRL_BUTTON_H_
