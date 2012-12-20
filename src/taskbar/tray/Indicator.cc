/*
 * indicator.cc
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "taskbar/taskbar.h"
#include "taskbar/tray/TaskTray.h"
#include "taskbar/tray/Indicator.h"
#include "window/qvwm.h"
#include "parser/qvwmrc.h"
#include "lib/misc.h"
#include "lib/util.h"

List<Indicator> Indicator::pendingList;

Indicator::Indicator(char* exec, char* comp)
: m_win(None), name(comp)
{
  pendingList.InsertTail(this);

  pid = ExecCommand(exec);
}  

Indicator::~Indicator()
{
  if (m_win != None) {
    delWindow(m_frame);
    XDestroyWindow(display, m_frame);
  }

  if (TaskbarImage)
    QvImage::Destroy(imgIndicator);

  kill(pid, SIGKILL);
}

void Indicator::CreateIndicator(TaskTray* taskTray, Window win)
{
  m_win = win;
  pendingList.Remove(this);

  XSetWindowAttributes attributes;
  unsigned long valueMask;
  
  attributes.background_pixel = gray.pixel;
  valueMask = CWBackPixel;

  m_frame = XCreateWindow(display, taskTray->getFrame(),
			  -IndicatorSize, -IndicatorSize,
			  IndicatorSize, IndicatorSize,
			  0, CopyFromParent, InputOutput, CopyFromParent,
			  valueMask, &attributes);

  addWindow(m_frame);

  if (TaskbarImage) {
    QvImage* img = taskTray->getBgImage();
    imgIndicator = img->Duplicate();
    imgIndicator->SetBackground(m_frame);
  }

  /*
   * Change window property for indicator.
   */
  XSetWindowBorderWidth(display, win, 0);
  XResizeWindow(display, win, IndicatorSize, IndicatorSize);

  XReparentWindow(display, win, m_frame, 0, 0);

  XMapWindow(display, win);
  show();
}

/*
 * LookInList --
 *   Look for a pending indicator with name in pending list.
 */
Indicator* Indicator::LookInList(char* name, XClassHint& classHints)
{
  List<Indicator>::Iterator i(&pendingList);

  for (Indicator* ind = i.GetHead(); ind; ind = i.GetNext())
    if ((name && strcmp(ind->name, name) == 0) ||
	(classHints.res_name && strcmp(ind->name, classHints.res_name) == 0) ||
	(classHints.res_class && strcmp(ind->name, classHints.res_class) == 0))
      return (ind);

  return NULL;
}
