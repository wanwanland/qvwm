#ifndef STARTMENU_ITEM_H_
#define STARTMENU_ITEM_H_

#include "lib/MenuItem.h"

class StartMenuItem : public MenuItem {
private:
  Window m_logoArea;
  
  static QvImage* m_imgStart[2];

public:
  StartMenuItem(MenuElem* mElem, int index);

  virtual Menu* getNextMenu();

  virtual void onMouseEnter(const XCrossingEvent& ev);

public:
  static void Initialize();
  static MenuItem** createMenuItems(MenuElem* mElem);
};

#endif // STARTMENU_ITEM_H_
