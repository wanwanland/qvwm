#ifndef SUBMENU_ITEM_
#define SUBMENU_ITEM_

#include "lib/MenuItem.h"

class StartSubMenuItem : public MenuItem {
public:
  StartSubMenuItem(MenuElem* mElem, int index);

  virtual Menu* getNextMenu();

  virtual void onMouseEnter(const XCrossingEvent& ev);

public:
  static MenuItem** createMenuItems(MenuElem* mElem);
};

#endif // SUBMENU_ITEM_
