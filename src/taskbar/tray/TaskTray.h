#ifndef TASK_TRAY_H_
#define TASK_TRAY_H_

#include "lib/Component.h"
#include "lib/list.h"
#include "taskbar/taskbar.h"

class Tooltip;
class QvImage;
class Indicator;

class TaskTray : public Component {
private:
  List<Indicator> m_indList;

  int m_clockWidth;		// clock width
  char m_strClk[128];

  Tooltip* m_toolTip;
  char m_strTip[128];

  Taskbar* m_taskBar;
  QvImage* m_imgTBox;

  static const int IA_MARGIN = 10;  // space between clock and right edge
  static const int IC_MARGIN = 6;   // space between indicators and clock
  static const int IND_MARGIN = 2;

public:
  TaskTray(Taskbar* taskBar);
  ~TaskTray();

  int getClockWidth() const { return m_clockWidth; }

  void addIndicator(Indicator* ind, Window win);

  void relocateAllIndicators();
  Bool notifyDeadPid(pid_t pid);

  QvImage* getBgImage() const { return m_taskBar->getBgImage(); }
  void setBgImage(const Point& pt);

  virtual void paint();

  void drawClock();
  void advanceClock();

  virtual void onMouseEnter(const XCrossingEvent& ev);
  virtual void onMouseLeave(const XCrossingEvent& ev);
  virtual void onMouseMotion(const XMotionEvent& ev);
  virtual void onButtonPress(const XButtonEvent& ev);
  virtual void onButtonRelease(const XButtonEvent& ev);
};

#endif // INDICATOR_AREA_H_
