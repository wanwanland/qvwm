/*
 * startmenu.h
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

#ifndef _STARTMENU_H_
#define _STARTMENU_H_

#include "lib/Menu.h"

class QvImage;

class StartMenu : public Menu {
private:
  Window logo;                      // window for logo mark in the left side

private:
  static QvImage* imgLogoMark;           // logo mark
  
  static const int LeftMargin = 5;
  static const int NameMargin = 2;
  static const int HiMargin = 2;
  static const int LOGO_WIDTH = 21;

public:
  StartMenu(MenuItem** mItem);
  ~StartMenu();

  virtual void show();
  virtual void hide();

  virtual void ExecFunction(MenuItem* item);

  virtual void paint();
  virtual void onMouseLeave(const XCrossingEvent& ev);

public:
  static void Initialize();
};

#endif // _STARTMENU_H_
