#ifndef TASKBAR_SHORTCUT_H_
#define TASKBAR_SHORTCUT_H_

#include "lib/ImageButton.h"
#include "wm/Function.h"

class TaskbarShortcut : public ImageButton {
private:
  Bool m_focus;

  FuncNumber m_fn;
  char* m_exec;

public:
  static const int SC_SIZE = 22;

public:
  TaskbarShortcut(Component* parent, QvImage* img, char* exec, char* name);
  TaskbarShortcut(Component* parent, QvImage* img, FuncNumber fn, char* name);

  virtual void performAction(ButtonState bs);
  virtual void paint();

  virtual void onMouseEnter(const XCrossingEvent& ev);
  virtual void onMouseLeave(const XCrossingEvent& ev);
};

#endif // TASKBAR_SHORTCUT_H_
