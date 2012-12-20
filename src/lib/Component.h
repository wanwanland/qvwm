#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "wm/Event.h"
#include "lib/misc.h"

class Component {
protected:
  Window m_frame;
  Rect m_rc;
  Bool m_mapped;

protected:
  Event::ClassId m_classId;

  Time m_tmPrev;
  Point m_ptRootPrev;
  unsigned int m_buttonPrev;
  unsigned int m_statePrev;

public:
  Component(Event::ClassId classId = Event::CLASS_ANON);
  virtual ~Component();

  Window getFrame() const { return m_frame; }
  Rect getRect() const { return m_rc; }
  Bool isMapped() const { return m_mapped; }

  Rect getBounds() const { return m_rc; }
  Point getLocation() const { return Point(m_rc.x, m_rc.y); }
  Dim getSize() const { return Dim(m_rc.width, m_rc.height); }

  virtual void show();
  virtual void hide();
  virtual void raise();

  virtual void move(const Point& pt);
  virtual void resize(const Dim& size);
  virtual void reshape(const Rect& rc);

public:
  void addWindow(Window win);
  void delWindow(Window win);

  Bool equalClass(Event::ClassId classId) const {
    return (classId == m_classId);
  }
  Bool isDoubleClick(const XButtonEvent& ev);

public:
  virtual void paint();

  virtual void onExpose(const XExposeEvent& ev);

  virtual void onMouseEnter(const XCrossingEvent& ev);
  virtual void onMouseLeave(const XCrossingEvent& ev);
  virtual void onMouseMotion(const XMotionEvent& ev);

  virtual void onButtonPress(const XButtonEvent& ev);
  virtual void onDoubleClick(const XButtonEvent& ev);
  virtual void onButtonRelease(const XButtonEvent& ev);

  virtual void onKeyPress(const XKeyEvent& ev);

  virtual void onFocusIn(const XFocusChangeEvent& ev);
};

#endif // COMPONENT_H_
