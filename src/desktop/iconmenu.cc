#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "desktop/iconmenu.h"
#include "desktop/icon.h"
#include "desktop/desktop.h"
#include "lib/util.h"
#include "lib/MenuItem.h"

void IconMenu::ExecFunction(MenuItem* item)
{
  switch (item->getFunction()) {
  case Q_EXEC_ICON:
    if (icon) {
      if (icon->IsBuiltin())
	QvFunction::execFunction(icon->GetFunc());
      else
	ExecCommand(icon->GetExec());
    }
    break;

  case Q_DELETE_ICON:
    desktop.GetIconList().Remove(icon);
    delete icon;
    icon = NULL;
    break;

  default:
    QvwmError("cannot execute a specified function");
    break;
  }
}
