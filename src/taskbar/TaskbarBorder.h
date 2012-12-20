#ifndef TASKBAR_BORDER_H_
#define TASKBAR_BORDER_H_

#include "lib/Component.h"

class QvImage;
class Taskbar;

class TaskbarBorder : public Component {
public:
  enum BorderPos { TOP = 0, BOTTOM = 1, LEFT = 2, RIGHT = 3 };

public:
  BorderPos m_pos;

  QvImage* m_imgBorder;

  Taskbar* m_taskBar;

public:
  TaskbarBorder(Taskbar* taskBar);
  ~TaskbarBorder();

  void setPosition(BorderPos pos);

  void paint();

  void onMouseEnter(const XCrossingEvent& ev);
  void onMouseLeave(const XCrossingEvent& ev);
  void onMouseMotion(const XMotionEvent& ev);
  void onButtonPress(const XButtonEvent& ev);
  void onButtonRelease(const XButtonEvent& ev);
};

#endif // TASKBAR_BORDER_H_
