#ifndef SUB_MENU_H_
#define SUB_MENU_H_

#include "lib/Menu.h"

class StartSubMenu : public Menu {
private:
  static const int LeftMargin = 5;
  static const int NameMargin = 2;
  static const int HiMargin = 2;

public:
  StartSubMenu(MenuItem** mItem, Menu* parent, XFontSet fs);

  virtual void onMouseLeave(const XCrossingEvent& ev);
};

#endif // SUB_MENU_H_
