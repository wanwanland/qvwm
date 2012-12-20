#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "parser/qvwmrc.h"
#include "taskbar/ButtonToolbar.h"
#include "taskbar/taskbar.h"
#include "desktop/paging.h"
#include "frame/FrameButton.h"

ButtonToolbar::ButtonToolbar(Taskbar* taskBar)
  : Toolbar(taskBar)
{
}

/*
 * Relocate all taskbar buttons.
 */
void ButtonToolbar::relocateAllTaskbarButtons()
{
  Rect rcTaskbar = m_taskBar->getRect();
  Rect rcCurPage(paging->origin.x, paging->origin.y,
		 rcScreen.width, rcScreen.height);
  Point pt;
  List<Qvwm>::Iterator i(&desktop.GetQvwmList());
  Qvwm* tmpQvwm;
  int buttonWidth;
  int row, col;
  int buttonNum = 0, count = 0;

  // Count the number of taskbar buttons.
  for (tmpQvwm = i.GetHead(); tmpQvwm; tmpQvwm = i.GetNext()) {
    if (!tmpQvwm->CheckFlags(TRANSIENT) && !tmpQvwm->CheckFlags(NO_TBUTTON) &&
	(tmpQvwm->CheckMapped() || tmpQvwm->CheckStatus(MINIMIZE_WINDOW))) {
      if (TaskbarButtonInScr) {
	// count only taskbar buttons in the current page
	if (!Intersect(tmpQvwm->GetRect(), rcCurPage))
	  continue;
      }

      buttonNum++;
    }
  }
     
  if (buttonNum == 0)
    return;

  /*
   * According to taskbar position, determine the way arranging buttons.
   */
  switch (m_taskBar->GetPos()) {
  case Taskbar::BOTTOM:
  case Taskbar::TOP:
    row = rcTaskbar.height / (TaskbarButtonHeight + BETWEEN_SPACE);
    if (row == 0)
      row = 1;  // quick hack

    buttonWidth = m_rc.width * row / RoundUp(buttonNum, row) - BETWEEN_SPACE;
    if (buttonWidth > TaskbarButtonWidth)
      buttonWidth = TaskbarButtonWidth;
    if (buttonWidth == 0)
      buttonWidth = 1;
    col = m_rc.width / buttonWidth;
    break;

  case Taskbar::LEFT:
  case Taskbar::RIGHT:
    row = m_rc.height / (TaskbarButtonHeight + BETWEEN_SPACE);
    if (row == 0)
      row = 1;
    col = (buttonNum + row - 1) / row;

    buttonWidth = (rcTaskbar.width - 5) / col - BETWEEN_SPACE;
    if (buttonWidth == 0)
      buttonWidth = 1;
    break;

  default:
    return;
  }

  /*
   * Reposition all taskbar buttons.
   */
  for (tmpQvwm = i.GetHead(); tmpQvwm; tmpQvwm = i.GetNext()) {
    if (tmpQvwm->CheckFlags(TRANSIENT) || tmpQvwm->CheckFlags(NO_TBUTTON) ||
	(!tmpQvwm->CheckMapped() && !tmpQvwm->CheckStatus(MINIMIZE_WINDOW)))
      continue;

    TaskbarButton* tb = tmpQvwm->tButton;

    if (TaskbarButtonInScr) {
      // show only taskbar buttons in the current page
      if (!Intersect(tmpQvwm->GetRect(), rcCurPage)) {
	tb->reshape(Rect(-1, -1, 1, 1));  // hide
	continue;
      }
    }

#define NOB_WIDTH  12
#define NOB_HEIGHT 12

    switch (m_taskBar->GetPos()) {
    case Taskbar::BOTTOM:
    case Taskbar::TOP:
      pt.x = NOB_WIDTH + (buttonWidth + BETWEEN_SPACE) * (count % col);
      pt.y = (count / col) * (TaskbarButtonHeight + BETWEEN_SPACE);
      break;

    case Taskbar::LEFT:
    case Taskbar::RIGHT:
      pt.x = (count % col) * (buttonWidth + BETWEEN_SPACE);
      pt.y = NOB_HEIGHT
	+ (TaskbarButtonHeight + BETWEEN_SPACE) * (count / col);
      break;

    default:
      return;
    }
    count++;
    
    tb->reshape(Rect(pt.x, pt.y, buttonWidth, TaskbarButtonHeight));

    if (TaskbarImage) {
      QvImage* imgTaskbar = m_taskBar->getBgImage();

      tb->SetBgImage(imgTaskbar, Point(pt.x + m_rc.x, pt.y + m_rc.y));
      tb->SetBgActiveImage(imgTaskbar,
			   Point(pt.x + m_rc.x, pt.y + m_rc.y - 1));
      tb->paint();
    }
  }
}                         
