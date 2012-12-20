#ifndef SHORTCUT_TOOLBAR_H_
#define SHORTCUT_TOOLBAR_H_

#include "lib/Toolbar.h"
#include "lib/list.h"

class TaskbarShortcut;

class ShortcutToolbar : public Toolbar {
private:
  List<TaskbarShortcut> m_scList;

public:
  static const int ShortcutSize = 22;

public:
  ShortcutToolbar(Taskbar* taskBar);

  void addShortcut(TaskbarShortcut* sc);

  void relocateAllShortcuts();
};

#endif // SHORTCUT_TOOLBAR_H_
