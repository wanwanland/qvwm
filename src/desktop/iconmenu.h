#ifndef ICONMENU_H_
#define ICONMENU_H_

#include "lib/Menu.h"

class Icon;

/*
 * IconMenu class
 */
class IconMenu : public Menu {
private:
  Icon* icon;
  
public:
  IconMenu(MenuItem** mItem, Menu* par, XFontSet menufs, Icon* owner)
  : Menu(mItem, par, menufs, rootQvwm), icon(owner) {}

  void SetIcon(Icon* owner) { icon = owner; }

  virtual void ExecFunction(MenuItem* item);
};

#endif // ICONMENU_H_
