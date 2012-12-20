#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "taskbar/taskbar.h"
#include "taskbar/shortcut/ShortcutToolbar.h"
#include "taskbar/shortcut/TaskbarShortcut.h"
#include "parser/qvwmrc.h"

ShortcutToolbar::ShortcutToolbar(Taskbar* taskBar)
  : Toolbar(taskBar)
{
}

void ShortcutToolbar::addShortcut(TaskbarShortcut* sc)
{
  m_scList.InsertTail(sc);

  sc->show();
  relocateAllShortcuts();
}

#define NOB_WIDTH 12
#define NOB_HEIGHT 12
#define TB_RIGHT 7
#define TB_DOWN 7

void ShortcutToolbar::relocateAllShortcuts()
{
  List<TaskbarShortcut>::Iterator i(&m_scList);
  int num = m_scList.GetSize();
  int row, col;
  int count = 0;
  
  if (num == 0)
    return;

  switch (m_taskBar->GetPos()) {
  case Taskbar::BOTTOM:
  case Taskbar::TOP:
    for (TaskbarShortcut* sc = i.GetHead(); sc; sc = i.GetNext()) {
      sc->move(Point(NOB_WIDTH + ShortcutSize * count, 0));
      count++;
    }

    resize(Dim(NOB_WIDTH + ShortcutSize * num + TB_RIGHT, m_rc.height));
    break;

  case Taskbar::LEFT:
  case Taskbar::RIGHT:
    row = (ShortcutSize * num + m_rc.width - 1) / m_rc.width;
    col = (num + row - 1) / row;

    for (TaskbarShortcut* sc = i.GetHead(); sc; sc = i.GetNext()) {
      sc->move(Point(ShortcutSize * (count % col),
		     NOB_HEIGHT + ShortcutSize * (count / col)));
      count++;
    }

    resize(Dim(m_rc.width, NOB_HEIGHT + ShortcutSize * row + TB_DOWN));
    break;
  }
}
