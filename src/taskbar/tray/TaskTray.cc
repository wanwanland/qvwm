#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
// #include <X11/Xlocale.h>
#include "xlocale.h"
#include <stddef.h>
#include "main.h"
#include "taskbar/taskbar.h"
#include "taskbar/tray/TaskTray.h"
#include "taskbar/tray/Indicator.h"
#include "parser/qvwmrc.h"
#include "lib/Tooltip.h"
#include "lib/callback.h"
#include "lib/timer.h"

TaskTray::TaskTray(Taskbar* taskBar)
  : m_taskBar(taskBar)
{
  XSetWindowAttributes attributes;
  unsigned long valueMask;

  attributes.background_pixel = TaskbarColor.pixel;
  attributes.event_mask = ExposureMask | EnterWindowMask | LeaveWindowMask |
                          PointerMotionMask;
  valueMask = CWBackPixel | CWEventMask;

  m_frame = XCreateWindow(display, taskBar->getFrame(),
			  0, 0, 1, 1,
			  0, CopyFromParent, InputOutput, CopyFromParent,
			  valueMask, &attributes);
  
  addWindow(m_frame);

  m_imgTBox = NULL;

  // calculate clock width
  char str[128];
  int len;
  XRectangle ink, log;

  if (UseClock) {
    time_t t;
    struct tm* tm;
    
    time(&t);
    tm = localtime(&t);
    
    // set values with max font width
    tm->tm_sec = 59;
    tm->tm_min = 59;
    tm->tm_hour = 23;
    tm->tm_mday = 31;
    tm->tm_yday = 365;
    
    // if ClockLocaleName is not specified in the configuration files
    if (ClockLocaleName == NULL)
      ClockLocaleName = LocaleName;

    setlocale(LC_TIME, ClockLocaleName);

    strftime(str, sizeof(str), ClockFormat, tm);

    len = strlen(str);
    XmbTextExtents(fsTaskbar, str, len, &ink, &log);
    m_clockWidth = log.width;

    Callback<TaskTray>* cb;
    cb = new Callback<TaskTray>(this, &TaskTray::advanceClock);
    timer->SetTimeout(1000, cb);
  }
  else
    m_clockWidth = 0;

  m_toolTip = new Tooltip();
}

TaskTray::~TaskTray()
{
  List<Indicator>::Iterator i(&m_indList);

  for (Indicator* ind = i.GetHead(); ind; ind = i.Remove())
    delete ind;

  delWindow(m_frame);

  XDestroyWindow(display, m_frame);

  QvImage::Destroy(m_imgTBox);

  delete m_toolTip;
}

void TaskTray::addIndicator(Indicator* ind, Window win)
{
  ind->CreateIndicator(this, win);

  m_indList.InsertTail(ind);

  m_taskBar->relocateComponents();
}

void TaskTray::setBgImage(const Point& offset)
{
  QvImage* imgTaskbar = m_taskBar->getBgImage();

  QvImage::Destroy(m_imgTBox);
  m_imgTBox = imgTaskbar->GetOffsetImage(offset);
  m_imgTBox->SetBackground(m_frame);
}

/*
 * Relocate all indicators.
 */
void TaskTray::relocateAllIndicators()
{
  int num, needWidth, row;
  int tWidth = taskBar->getWidth();

  num = m_indList.GetSize();
  if (num == 0) {
    resize(Dim(IA_MARGIN + m_clockWidth + IA_MARGIN, TaskbarButtonHeight));
    paint();
    return;
  }

  /*
   * necessary width.
   */
  needWidth = num * (IndicatorSize + 1) + IND_MARGIN * 2 + IC_MARGIN
    + m_clockWidth + IA_MARGIN;

  row = (tWidth - 15) / (IndicatorSize + 1);
  if (row == 0)
    row = 1;

  List<Indicator>::Iterator i(&m_indList);
  Indicator* ind = i.GetHead();
  int j;

  for (j = 0; j < num; j++) {
    Point pt;

    pt.x = 1 + IND_MARGIN + (IndicatorSize + 1) * (j % row);
    if (needWidth <= tWidth - 9)
      pt.y = (TaskbarButtonHeight - IndicatorSize) / 2;
    else
      pt.y = TaskbarButtonHeight + (IndicatorSize + 2) * (j / row);

    ind->move(pt);

    ind = i.GetNext();
  }

  Dim size;

  if (needWidth <= tWidth - 9) {
    size.width = (IndicatorSize + 1) * num + IND_MARGIN * 2;
    size.height = TaskbarButtonHeight;
  }
  else {
    size.width = IA_MARGIN - IC_MARGIN;
    size.height = TaskbarButtonHeight
      + (IndicatorSize + 2) * ((num + row - 1) / row) + 1;
  }

  size.width += IC_MARGIN + m_clockWidth + IA_MARGIN;

  resize(size);
  paint();
}

Bool TaskTray::notifyDeadPid(pid_t pid)
{
  List<Indicator>::Iterator i(&m_indList);

  for (Indicator* ind = i.GetHead(); ind; ind = i.GetNext()) {
    if (pid == ind->getPid()) {
      i.Remove();
      delete ind;
      taskBar->relocateComponents();
      return True;
    }
  }

  return False;
}

/*
 * Redraw function
 */
void TaskTray::paint()
{
  XPoint xp[3];

  drawClock();

  xp[0].x = m_rc.width - 2;
  xp[0].y = 0;
  xp[1].x = 0;
  xp[1].y = 0;
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

  XSetForeground(display, gc, white.pixel);
  XDrawLines(display, m_frame, gc, xp, 3, CoordModeOrigin);
}

/*
 * Draw the clock in the right or bottom side.
 */
void TaskTray::drawClock()
{
  time_t t;
  struct tm *tm;
  int len;
  Point pt;
  XRectangle ink, log;

  XClearArea(display, m_frame, 1, 1, m_rc.width - 2, m_rc.height - 2, False);

  time(&t);
  tm = localtime(&t);

  strftime(m_strClk, sizeof(m_strClk), ClockFormat, tm);
  len = strlen(m_strClk);

  XmbTextExtents(fsTaskbar, m_strClk, len, &ink, &log);
  pt.x = m_rc.width - IA_MARGIN - log.width
    + (m_clockWidth - log.width) / 2 - log.x;
  pt.y = (TaskbarButtonHeight - log.height) / 2 - log.y;

  XSetForeground(display, gc, ClockStringColor.pixel);
  XmbDrawString(display, m_frame, fsTaskbar, gc, pt.x, pt.y, m_strClk, len);
}

void TaskTray::advanceClock()
{
  time_t t;
  struct tm *tm;
  char str[128];

  time(&t);
  tm = localtime(&t);
  strftime(str, sizeof(str), ClockFormat, tm);
  
  if (strcmp(str, m_strClk) != 0)
    drawClock();

  Callback<TaskTray>* cb;
  cb = new Callback<TaskTray>(this, &TaskTray::advanceClock);
  timer->SetTimeout(1000, cb);
}

/*
 * Event handlers
 */
void TaskTray::onMouseEnter(const XCrossingEvent& ev)
{
  time_t t;
  struct tm *tm;
    
  time(&t);
  tm = localtime(&t);

  strftime(m_strTip, sizeof(m_strTip), ClockMessageFormat, tm);
  m_toolTip->SetString(m_strTip, &fsTaskbar);
  
  m_toolTip->SetTimer();
}

void TaskTray::onMouseLeave(const XCrossingEvent& ev)
{
  m_toolTip->Disable();
}

void TaskTray::onMouseMotion(const XMotionEvent& ev)
{
  if (!m_toolTip->IsMapped())
    m_toolTip->ResetTimer();
}

void TaskTray::onButtonPress(const XButtonEvent& ev)
{
  m_taskBar->onButtonPress(ev);
}

void TaskTray::onButtonRelease(const XButtonEvent& ev)
{
  m_taskBar->onButtonRelease(ev);
}
