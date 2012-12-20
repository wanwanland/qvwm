#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "taskbar/start/SubMenu.h"
#include "lib/callback.h"
#include "lib/timer.h"
#include "lib/MenuItem.h"

StartSubMenu::StartSubMenu(MenuItem** mItem, Menu* parent, XFontSet fs)
: Menu(mItem, parent, fs, rootQvwm, LeftMargin, NameMargin, HiMargin)
{
}

/*
 * Event handlers
 */
void StartSubMenu::onMouseLeave(const XCrossingEvent& ev)
{
  // cancel the popup of the child menu
  timer->ResetTimeout(new Callback<Menu>(this, &Menu::PopupMenu));

  if (m_child) {
    if (checkEnterEvent(m_child))
      return;
  }
}
