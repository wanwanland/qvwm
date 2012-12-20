#ifndef BUTTON_TOOLBAR_H_
#define BUTTON_TOOLBAR_H_

#include "lib/Toolbar.h"

class ButtonToolbar : public Toolbar {
public:
  static const int BETWEEN_SPACE = 3;	// space between tbuttons

public:
  ButtonToolbar(Taskbar* taskBar);

  void relocateAllTaskbarButtons();
};

#endif // BUTTON_TOOLBAR_H_
