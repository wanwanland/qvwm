/*
 * menu.h
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

#ifndef _MENU_H_
#define _MENU_H_

#include "lib/Component.h"
#include "lib/misc.h"
#include "lib/util.h"

class Qvwm;
class QvImage;
class MenuItem;

#define GD_RIGHT (1 << 0)
#define GD_LEFT  (1 << 1)
#define GD_DOWN  (1 << 2)
#define GD_UP    (1 << 3)

/*
 * Menu class
 */
class Menu : public Component {
protected:
  Bool m_mapped;               // flag if this menu is mapped
  int m_nitems;                // item number
  Menu* m_parent;              // parent of this menu
  Menu* m_child;               // menu extracted from this menu
  int m_gDir;

  int m_imageArea;             // leftMargin + image width + midMargin
  int m_itemHeight;            // image height + hiMargin * 2
  int m_leftMargin;            // left margin of image
  int m_nameMargin;            // margin between image and name
  int m_hiMargin;              // top and bottom margin of image

  XFontSet m_fs;
  Qvwm* m_qvWm;
  QvImage* m_imgDefFolder;
  QvImage* m_imgDefIcon;

  MenuItem** m_mi;

  MenuItem* m_focusMenuItem;      // focused one
  MenuItem* m_delayedMenuItem;    // delayed one for popup
  MenuItem* m_extractedMenuItem;  // one whose chile menu is extracted

public:
  // direction of menu extraction
  enum MenuDir { RIGHT, LEFT };

  static MenuDir m_fDir;          // orientation of menu extraction

  static QvImage *m_imgMenuBack, *m_imgActiveMenuBack;

  static const int SeparatorHeight = 9;
  static const int MenuFrameWidth = 3;

protected:
  Bool checkEnterEvent(Menu* child);

public:
  Menu(MenuItem** mi, Menu* parent, XFontSet fs, Qvwm* qvWm,
       int leftMargin = 1, int nameMargin = 4, int hiMargin = 1);
  virtual ~Menu();

  Bool CheckMapped() const { return m_mapped; }
  int GetItemNum() const { return m_nitems; }
  Menu* GetChildMenu() const { return m_child; }
  XFontSet GetFontSet() const { return m_fs; }

  void SetRect(const Rect& rect) { m_rc = rect; }

  void SetQvwm(Qvwm* qvwm) { m_qvWm = qvwm; }
  Qvwm* GetQvwm() const { return m_qvWm; }

  int GetImageAreaWidth() const { return m_imageArea; }
  int GetLeftMargin() const { return m_leftMargin; }
  int GetNameMargin() const { return m_nameMargin; }
  int GetHeightMargin() const { return m_hiMargin; }

  MenuItem* GetFocusItem() const { return m_focusMenuItem; }
  void SetFocusItem(MenuItem* mi) { m_focusMenuItem = mi; }
  
  virtual void show();
  virtual void hide();

  void adjustMenuPos(const Point& pt);
  void setDirection(int gDir) { m_gDir = gDir; }
  int getDirection() const { return m_gDir; }

  void PopupMenu();
  void PopdownMenu();
  void SetDelayedItem(MenuItem* mi);

  void ExecShortCutKey(char key);
  Bool IsSelectable(FuncNumber fn);
  Bool IsChecked(FuncNumber fn);
  Bool IsSelected(FuncNumber fn);

  virtual void ExecFunction(MenuItem* item);
  void ExtractChildMenu(MenuItem* item, Menu* next);
  void MoveFocusUp();
  void MoveFocusDown();
  void MoveFocusRight();
  void MoveFocusLeft();
  void ExecFocusItem();

  virtual void paint();
  virtual void onMouseLeave(const XCrossingEvent& ev);
  virtual void onKeyPress(const XKeyEvent& ev);

public:
  static void Initialize();
  static void UnmapAllMenus(Bool hideTaskbar = True);
  static Bool CheckAnyMenusMapped();
  static Window GetMappedMenuFrame();
};

#endif // _MENU_H_
