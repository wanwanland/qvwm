#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include "main.h"
#include "taskbar/shortcut/TaskbarShortcut.h"
#include "wm/Function.h"
#include "lib/util.h"
#include "lib/Tooltip.h"

TaskbarShortcut::TaskbarShortcut(Component* parent, QvImage* img,
				 char* exec, char* name)
  : ImageButton(parent, img, Rect(0, 0, SC_SIZE, SC_SIZE)), m_exec(exec)
{
  m_fn = Q_EXEC;
  m_focus = False;

  m_toolTip->SetString(name, &fsTaskbar);
}

TaskbarShortcut::TaskbarShortcut(Component* parent, QvImage* img,
				 FuncNumber fn, char* name)
  : ImageButton(parent, img, Rect(0, 0, SC_SIZE, SC_SIZE)), m_fn(fn)
{
  m_focus = False;

  m_toolTip->SetString(name, &fsTaskbar);
}

void TaskbarShortcut::performAction(ButtonState bs)
{
  if (bs == PUSH) {
    if (m_fn == Q_EXEC)
      ExecCommand(m_exec);
    else
      QvFunction::execFunction(m_fn);
  }
}

void TaskbarShortcut::paint()
{
  XPoint xp[3];

  XClearWindow(display, m_frame);

  if (m_img) {
    if (CheckState(NORMAL))
      m_img->Display(m_frame, Point(3, 3));
    else
      m_img->Display(m_frame, Point(4, 4));
  }

  if (m_focus) {
    // draw border
    xp[0].x = m_rc.width - 1;
    xp[0].y = 0;
    xp[1].x = 0;
    xp[1].y = 0;
    xp[2].x = 0;
    xp[2].y = m_rc.height - 1;
    
    if (m_state == PUSH)
      XSetForeground(display, gc, darkGray.pixel);
    else
      XSetForeground(display, gc, white.pixel);
    XDrawLines(display, m_frame, gc, xp, 3, CoordModeOrigin);
    
    xp[0].x = m_rc.width - 1;
    xp[0].y = 0;
    xp[1].x = m_rc.width - 1;
    xp[1].y = m_rc.height - 1;
    xp[2].x = 0;
    xp[2].y = m_rc.height - 1;
    
    if (m_state == PUSH)
      XSetForeground(display, gc, white.pixel);
    else
      XSetForeground(display, gc, darkGray.pixel);
    XDrawLines(display, m_frame, gc, xp, 3, CoordModeOrigin);
  }
}

void TaskbarShortcut::onMouseEnter(const XCrossingEvent& ev)
{
  m_focus = True;
  paint();

  ImageButton::onMouseEnter(ev);
}

void TaskbarShortcut::onMouseLeave(const XCrossingEvent& ev)
{
  m_focus = False;
  paint();

  ImageButton::onMouseLeave(ev);
}
