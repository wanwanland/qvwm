/*
 * taskbar.h
 *
 * Copyright (C) 1995-2001 Kenichi Kourai
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with qvwm; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _TASKBAR_H_
#define _TASKBAR_H_

#include "lib/Component.h"
#include "taskbar/TaskbarButton.h"
#include "lib/Menu.h"

class TaskbarButton;
class Qvwm;
class Tooltip;
class QvImage;
class ButtonToolbar;
class ShortcutToolbar;
class TaskTray;
class StartButton;
class TaskbarBorder;

/*
 * Taskbar class
 */
class Taskbar : public Component {
friend class TaskbarButton;
friend class Indicator;
public:
  // Taskbar position
  enum TaskbarPos { BOTTOM = 0, TOP, LEFT, RIGHT };

private:
  Window m_win;
  Rect rc[4];			// 4 positions where taskbar can exist
  int buttonArea;		// area size for taskbar button
  TaskbarPos pos;		// taskbar position
  Menu* ctrlMenu;		// popup menu on right-click  
  Bool hiding;
  QvImage* imgTaskbar;

  TaskbarBorder* m_border;
  StartButton* m_sButton;
  ButtonToolbar* m_bToolBar;
  ShortcutToolbar* m_sToolBar;
  TaskTray* m_taskTray;

public:
  static int BASE_HEIGHT;	// base height when TOP or BOTTOM
  static int INC_HEIGHT;	// gain

private:
  int createShortcutItems(QvImage* imgDefIcon);

public:
  Taskbar(unsigned int rows);
  ~Taskbar();

  Rect GetRect() const { return rc[pos]; }
  TaskbarPos GetPos() const { return pos; }
  Bool IsTaskbarWindows(Window win);
  Bool CheckMenuMapped() const;
  void UnmapMenu();
  Menu* GetMenu();
  Bool IsHiding() const { return hiding; }
  QvImage* GetTaskbarImage() const { return imgTaskbar; }

  StartButton* getStartButton() const { return m_sButton; }
  ButtonToolbar* getButtonToolbar() const { return m_bToolBar; }
  ShortcutToolbar* getShortcutToolbar() const { return m_sToolBar; }
  TaskTray* getTaskTray() const { return m_taskTray; }

  QvImage* getBgImage() const { return imgTaskbar; }
  int getWidth() const { return rc[pos].width; }

  virtual void show();
  virtual void raise();

  void placeTaskbar(TaskbarPos tp);
  void dragTaskbar(const Point& ptRoot);
  void dragResizeTaskbar(const Point& ptRoot);

  void MoveResizeTaskbarBox();
  void DrawTaskbarFrame();
  void DrawTaskbar();
  void DrawTaskbarBox();
  void DrawClock();
  void AdvanceClock();
  void ShowTaskbar();
  void HideTaskbar();
  Rect GetScreenRectOnShowing() const;
  Rect GetScreenRectOnHiding() const;

  void relocateComponents();
  void relocateStartButton();
  void relocateTaskTray();
  void relocateButtonToolbar();
  void relocateShortcutToolbar();

  virtual void paint();
  virtual void onMouseEnter(const XCrossingEvent& ev);
  virtual void onMouseLeave(const XCrossingEvent& ev);
  virtual void onButtonPress(const XButtonEvent& ev);
  virtual void onButtonRelease(const XButtonEvent& ev);
};

#endif // _TASKBAR_H_
