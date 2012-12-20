#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "lib/Menu.h"
#include "wm/Function.h"
#include "lib/util.h"
#include "lib/MenuItem.h"

void Menu::ExecFunction(MenuItem* item)
{
  QvFunction::execFunction(item->getFunction(), item);
}

void Menu::ExtractChildMenu(MenuItem* mi, Menu* child)
{
  Point ptFrame;
  int gDir;

  m_extractedMenuItem = mi;
  m_child = child;

  Rect rcChild = child->getRect();

  switch (m_fDir) {
    /*
     * To right side.
     */
  case RIGHT:
    ptFrame.x = m_rc.x + m_rc.width - 7;
    if (ptFrame.x + rcChild.width > rcScreen.width-1) {
      ptFrame.x = m_rc.x - rcChild.width + MenuFrameWidth;
      m_fDir = LEFT;
      gDir = GD_LEFT;
    }
    else
      gDir = GD_RIGHT;
    break;

    /*
     * To left side.
     */
  case LEFT:
    ptFrame.x = m_rc.x - rcChild.width+MenuFrameWidth;
    if (ptFrame.x + rcChild.width < 0) {
      ptFrame.x = m_rc.x + m_rc.width - 7;
      m_fDir = RIGHT;
      gDir = GD_RIGHT;
    }
    else
      gDir = GD_LEFT;
    break;

  default:
    gDir = 0;  // XXX for warning
    ASSERT(False);
  }

  Rect rcItem = mi->getRect();

  ptFrame.y = m_rc.y + rcItem.y - MenuFrameWidth;
  if (ptFrame.y + rcChild.height > rcScreen.height - 1) {
    ptFrame.y = ptFrame.y + m_itemHeight - rcChild.height +
      MenuFrameWidth * 2;
    gDir |= GD_UP;
  }
  else
    gDir |= GD_DOWN;

  m_child->setDirection(gDir);
  m_child->move(ptFrame);
  m_child->show();
}

void Menu::MoveFocusUp()
{
  int index, i;

  if (m_focusMenuItem == NULL)
    index = m_nitems - 1;
  else
    index = m_focusMenuItem->getIndex() - 1;
    
  for (i = 0; i < m_nitems; i++) {
    if (index == -1)
      index = m_nitems - 1;

    if (m_mi[index]->getFunction() != Q_SEPARATOR)
      break;

    index--;
  }

  if (i == m_nitems)
    return;

  if (m_mi[index] != m_focusMenuItem) {
    if (m_focusMenuItem)
      m_focusMenuItem->resetItemFocus();
    m_mi[index]->setItemFocus();
    m_focusMenuItem = m_mi[index];
  }
}

void Menu::MoveFocusDown()
{
  int index, i;

  if (m_focusMenuItem == NULL)
    index = 0;
  else
    index = m_focusMenuItem->getIndex() + 1;

  for (i = 0; i < m_nitems; i++) {
    if (index == m_nitems)
      index = 0;

    if (m_mi[index]->getFunction() != Q_SEPARATOR)
      break;

    index++;
  }

  if (i == m_nitems)
    return;

  if (m_mi[index] != m_focusMenuItem) {
    if (m_focusMenuItem)
      m_focusMenuItem->resetItemFocus();
    m_mi[index]->setItemFocus();
    m_focusMenuItem = m_mi[index];
  }
}

void Menu::MoveFocusRight()
{
  if (m_fDir == RIGHT) {
    if (m_focusMenuItem == NULL)
      return;

    if (m_focusMenuItem->getFunction() == Q_DIR) {
      QvFunction::execFunction(Q_DIR, m_focusMenuItem);
      QvFunction::execFunction(Q_DOWN_FOCUS, m_child);
    }
  }
  else {
    ASSERT(m_fDir == LEFT);
    if (m_parent != NULL)
      hide();
  }
}

void Menu::MoveFocusLeft()
{
  if (m_fDir == LEFT) {
    if (m_focusMenuItem == NULL)
      return;

    if (m_focusMenuItem->getFunction() == Q_DIR) {
      QvFunction::execFunction(Q_DIR, m_focusMenuItem);
      QvFunction::execFunction(Q_DOWN_FOCUS, m_child);
    }
  }
  else {
    ASSERT(m_fDir == RIGHT);
    if (m_parent != NULL)
      hide();
  }
}

// exec a function of the focused menu item
void Menu::ExecFocusItem()
{
  FuncNumber func = m_focusMenuItem->getFunction();

  if (IsSelectable(func)) {
    if (func != Q_DIR)
      UnmapAllMenus();
    
    QvFunction::execFunction(func, m_focusMenuItem);
  }
}
