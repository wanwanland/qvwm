/*
 * indicator.h
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

#ifndef INDICATOR_H_
#define INDICATOR_H_

#include "lib/Component.h"
#include "lib/list.h"

class QvImage;
class TaskTray;

class Indicator : public Component {
private:
  Window m_win;				// original window
  char* name;				// window indentical name
  pid_t pid;				// application pid
  QvImage* imgIndicator;

  static List<Indicator> pendingList;

public:
  Indicator(char* exec, char* comp);
  ~Indicator();

  pid_t getPid() const { return pid; }

  void CreateIndicator(TaskTray* taskTray, Window win);

public:
  static Indicator* LookInList(char* name, XClassHint& classHints);
};

#endif // INDICATOR_H_
