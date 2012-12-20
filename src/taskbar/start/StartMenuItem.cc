#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "taskbar/start/StartMenuItem.h"
#include "taskbar/start/SubMenuItem.h"
#include "taskbar/start/SubMenu.h"
#include "parser/qvwmrc.h"
#include "image/PixmapImage.h"

#include "bitmaps/folder32.xpm"
#include "bitmaps/icon32.xpm"

QvImage* StartMenuItem::m_imgStart[2];

StartMenuItem::StartMenuItem(MenuElem* mElem, int index)
: MenuItem(mElem, index, m_imgStart[0], m_imgStart[1])
{
}

Menu* StartMenuItem::getNextMenu()
{
  // delayed menu creation
  if (m_next == NULL) {
    ASSERT(m_childItem);

    MenuItem** mi = StartSubMenuItem::createMenuItems(m_childItem);
    XFontSet fs = fsCascadeMenu;

    m_next = new StartSubMenu(mi, m_menu, fs);
  }

  return m_next;
}

/*
 * Event handlers
 */
void StartMenuItem::onMouseEnter(const XCrossingEvent& ev)
{
  if (m_func == Q_SEPARATOR)
    return;

  MenuItem::onMouseEnter(ev);
}

/*
 * Static functions
 */
void StartMenuItem::Initialize()
{
  m_imgStart[0] = new PixmapImage(folder32);
  m_imgStart[1] = new PixmapImage(icon32);
}

MenuItem** StartMenuItem::createMenuItems(MenuElem* mElem)
{
  MenuItem** mi;
  int nitems;
  int i;

  nitems = GetMenuItemNum(mElem);
  mi = new MenuItem*[nitems + 1];

  // create child menus
  for (i = 0; i < nitems; i++) {
    mi[i] = new StartMenuItem(mElem, i);

    mElem = mElem->next;
  }

  mi[nitems] = NULL;

  return mi;
}

