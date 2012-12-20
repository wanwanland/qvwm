#ifndef BORDER_H_
#define BORDER_H_

#include "lib/Component.h"

class QvImage;
class Qvwm;

class Border : public Component {
public:
  enum BorderPos {
    TOP = 0, BOTTOM = 1, LEFT = 2, RIGHT = 3,
    TOP_LEFT = 4, TOP_RIGHT = 5, BOTTOM_LEFT = 6, BOTTOM_RIGHT = 7
  };

public:
  BorderPos m_pos;

  QvImage* m_imgBorder;
  QvImage* m_imgActiveBorder;

  Qvwm* m_qvWm;

public:
  static QvImage* m_imgFrame;
  static QvImage* m_imgActiveFrame;

public:
  Border(Qvwm* qvWm, BorderPos pos);
  ~Border();

  void paintWithFocusChange();
  virtual void paint();

  virtual void onMouseEnter(const XCrossingEvent& ev);
  virtual void onMouseMotion(const XMotionEvent& ev);
  virtual void onButtonPress(const XButtonEvent& ev);
  virtual void onButtonRelease(const XButtonEvent& ev);

public:
  static void initialize();
};

#endif // BORDER_H_
