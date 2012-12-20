#ifndef TOOLBAR_H_
#define TOOLBAR_H_

#include "lib/Component.h"

class Taskbar;

class Toolbar : public Component {
protected:
  Taskbar* m_taskBar;
  QvImage* m_imgBg;

public:
  Toolbar(Taskbar* taskBar);
  virtual ~Toolbar();

  Point getAbsLocation() const;

  void setBgImage(const Point& offset);

  virtual void paint();

  virtual void onMouseMotion(const XMotionEvent& ev);
  virtual void onButtonPress(const XButtonEvent& ev);
  virtual void onButtonRelease(const XButtonEvent& ev);
};

#endif // TOOLBAR_H_
