#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "window/qvwm.h"
#include "taskbar/start/SubMenuItem.h"
#include "taskbar/start/SubMenu.h"

StartSubMenuItem::StartSubMenuItem(MenuElem* mElem, int index)
: MenuItem(mElem, index, MenuItem::m_imgMenu[0], MenuItem::m_imgMenu[1])
{
}

Menu* StartSubMenuItem::getNextMenu()
{
  // delayed menu creation
  if (m_next == NULL) {
    ASSERT(m_childItem);

    MenuItem** mi = StartSubMenuItem::createMenuItems(m_childItem);
    XFontSet fs = m_menu->GetFontSet();

    m_next = new StartSubMenu(mi, m_menu, fs);
  }

  return m_next;
}

/*
 * Event handlers
 */
void StartSubMenuItem::onMouseEnter(const XCrossingEvent& ev)
{
  if (m_func == Q_SEPARATOR)
    return;

  MenuItem::onMouseEnter(ev);
}

/*
 * Static functions
 */
MenuItem** StartSubMenuItem::createMenuItems(MenuElem* mElem)
{
  MenuItem** mi;
  int nitems;
  int i;

  nitems = GetMenuItemNum(mElem);
  mi = new MenuItem*[nitems + 1];

  // create child menus
  for (i = 0; i < nitems; i++) {
    mi[i] = new StartSubMenuItem(mElem, i);

    mElem = mElem->next;
  }

  mi[nitems] = NULL;

  return mi;
}
