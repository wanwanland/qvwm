/*
 * titlebar.cc
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
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "parser/qvwmrc.h"
#include "wm/Event.h"
#include "lib/Tooltip.h"
#include "lib/Titlebar.h"

QvImage* Titlebar::m_imgTitlebar;
QvImage* Titlebar::m_imgActiveTitlebar;
unsigned long* Titlebar::m_gradPattern;
unsigned long* Titlebar::m_gradActivePattern;

Titlebar::Titlebar(Component* comp, const Rect& rc)
  : m_comp(comp)
{
  XSetWindowAttributes attributes;
  unsigned long valueMask;

  m_rc = rc;
  m_title = NULL;
  m_shortTitle = NULL;

  attributes.event_mask = ButtonPressMask | ButtonReleaseMask | ExposureMask |
                          EnterWindowMask | LeaveWindowMask |
			  Button1MotionMask | PointerMotionMask;
  attributes.background_pixel = darkGray.pixel;
  valueMask = CWBackPixel | CWEventMask;

  m_frame = XCreateWindow(display, m_comp->getFrame(),
			  rc.x, rc.y, rc.width, rc.height,
			  0, CopyFromParent, InputOutput, CopyFromParent,
			  valueMask, &attributes);
  addWindow(m_frame);

  if (GradTitlebar) {
    m_pixGrad = None;
    m_pixActiveGrad = None;
  }

  if (TitlebarImage) {
    m_imgTitle = m_imgTitlebar->Duplicate();
    m_imgTitle->SetBackground(m_frame);
  }
  if (TitlebarActiveImage)
    m_imgActiveTitle = m_imgActiveTitlebar->Duplicate();

  m_toolTip = new Tooltip();
}

Titlebar::~Titlebar()
{
  delWindow(m_frame);
  XDestroyWindow(display, m_frame);

  if (TitlebarImage)
    QvImage::Destroy(m_imgTitle);
  if (TitlebarActiveImage)
    QvImage::Destroy(m_imgActiveTitle);

  if (GradTitlebar) {
    if (m_pixGrad)
      XFreePixmap(display, m_pixGrad);
    if (m_pixActiveGrad)
      XFreePixmap(display, m_pixActiveGrad);
  }
  
  if (m_shortTitle)
    delete [] m_shortTitle;

  delete m_toolTip;
}

void Titlebar::setTitle(char* title)
{
  m_title = title;

  calcShortTitle();
  m_toolTip->SetString(title);
}

void Titlebar::calcShortTitle()
{
  delete [] m_shortTitle;

  m_shortTitle = GetFixName(fsTitle, m_title, getTitleWidth());
}

int Titlebar::getTitleWidth()
{
  return m_rc.width - 2;
}

void Titlebar::reshape(const Rect& rc)
{
  int oldWidth = m_rc.width;

  Component::reshape(rc);

  if (rc.width != oldWidth) {
    if (GradTitlebar) {
      if (m_pixGrad != None)
	XFreePixmap(display, m_pixGrad);
      m_pixGrad = None;
      
      if (m_pixActiveGrad != None)
	XFreePixmap(display, m_pixActiveGrad);
      m_pixActiveGrad = None;
    }
    
    calcShortTitle();
    paintWithFocusChange();
  }
}

/*
 * Redraw function
 */
void Titlebar::paint()
{
  Point ptTitle;
  XRectangle ink, log;
  int len = strlen(m_shortTitle);

  XmbTextExtents(fsTitle, m_shortTitle, len, &ink, &log);

  ptTitle.x = getTitleX();
  ptTitle.y = (m_rc.height - log.height)/2 - log.y;
  
  XColor color = getForeColor();

  XSetForeground(display, gc, color.pixel);
  
  XmbDrawString(display, m_frame, fsTitle, gc, ptTitle.x, ptTitle.y,
		m_shortTitle, len);

  if (strlen(m_title) > len) {
    XRectangle xr[3] =
    {{ptTitle.x + log.width + 2, ptTitle.y - 2, 1, 2},
     {ptTitle.x + log.width + 5, ptTitle.y - 2, 1, 2},
     {ptTitle.x + log.width + 8, ptTitle.y - 2, 1, 2}};
    
    XFillRectangles(display, m_frame, gc, xr, 3);
  }
}

void Titlebar::paintWithFocusChange()
{
  paint();
}

int Titlebar::getTitleX()
{
  return 4;
}

const XColor& Titlebar::getForeColor()
{
  return TitleStringActiveColor;
}

/*
 * Event handlers
 */
void Titlebar::onMouseEnter(const XCrossingEvent& ev)
{
  if (strcmp(m_title, m_shortTitle) != 0)
    m_toolTip->SetTimer();
}

void Titlebar::onMouseLeave(const XCrossingEvent& ev)
{
  m_toolTip->Disable();
}

void Titlebar::onMouseMotion(const XMotionEvent& ev)
{
  if (!m_toolTip->IsMapped())
    m_toolTip->ResetTimer();
}

void Titlebar::onButtonPress(const XButtonEvent& ev)
{
  m_toolTip->Disable();
}

// titlebar general routines
Pixmap Titlebar::createGradPixmap(unsigned long* pat, int width, Window win)
{
  Pixmap pix = XCreatePixmap(display, win, width, 4, depth);
  GC gc = XCreateGC(display, pix, 0, 0);
  XImage* image = XGetImage(display, pix, 0, 0, width, 4, AllPlanes, XYPixmap);
  int* xIndex = new int[width];
  int level = (GradTitlebarColors - 1) / 3;
  int k = 0, sw;

  // calculate the position of base pattern
  for (int i = 0; i < level; i++) {
    int base = i * 28;
    for (int j = 0; j < 28;) {
      switch (j) {
	// # of pattern is 2
      case 0: case 6: case 8: case 10: case 16: case 18: case 20: case 26:
	sw = 0;
	for (; k < (base + j + 2) * width / (level * 28) - 1; k++) {
	  xIndex[k] = base + j + sw;
	  sw = (sw + 1) & 1;
	}
	j += 2;
	break;

	// # of pattern is 4
      case 2: case 12: case 22:
	sw = 0;
	for (; k < (base + j + 4) * width / (level * 28) - 1; k++) {
	  xIndex[k] = base + j + sw;
	  sw = (sw + 1) & 3;
	}
	j += 4;
	break;
      }
    }
  }
  for (; k < width; k++)
    xIndex[k] = level * 28 - 1;

  for (int x = 0; x < width; x++)
    for (int y = 0; y < 4; y++)
      XPutPixel(image, x, y, pat[xIndex[x] * 4 + y]);

  XPutImage(display, pix, gc, image, 0, 0, 0, 0, width, 4);
  XDestroyImage(image);
  XFreeGC(display, gc);
  delete [] xIndex;

  return pix;
}

/*
 * Static functions
 */
void Titlebar::initialize()
{
  if (TitlebarImage) {
    m_imgTitlebar = CreateImageFromFile(TitlebarImage, timer);
    if (m_imgTitlebar == None) {
      delete [] TitlebarImage;
      TitlebarImage = NULL;
    }
  }
  if (TitlebarActiveImage) {
    m_imgActiveTitlebar = CreateImageFromFile(TitlebarActiveImage, timer);
    if (m_imgActiveTitlebar == None) {
      delete [] TitlebarActiveImage;
      TitlebarActiveImage = NULL;
    }
  }
}

void Titlebar::createGradPattern()
{
  if (GradTitlebarColors == 1) {
    GradTitlebar = False;
    return;
  }

  m_gradPattern = createPattern(TitlebarColor, TitlebarColor2);
  m_gradActivePattern = createPattern(TitlebarActiveColor,
				      TitlebarActiveColor2);
}

unsigned long* Titlebar::createPattern(XColor base1, XColor base2)
{
  int level = (GradTitlebarColors - 1) / 3;

  // need 4 colors at minimum
  if (level == 0) {
    GradTitlebarColors = 4;  // XXX
    level = 1;
  }

  unsigned long* pat = new unsigned long[28 * level * 4];
  XColor* cMajor = new XColor[level + 1];
  int diffRed = base2.red - base1.red;
  int diffGreen = base2.green - base1.green;
  int diffBlue = base2.blue - base1.blue;
  int i, j;

  // major color allocation
  for (i = 0; i < (level + 1) / 2; i++) {
    XColor* sub = (i == 0) ? NULL : &cMajor[i - 1];

    CreateColor(base1.red   + diffRed   * i / level,
		base1.green + diffGreen * i / level,
		base1.blue  + diffBlue  * i / level,
		&cMajor[i], sub, "titlebar gradation");
  }
  for (j = level; j >= i; j--) {
    XColor* sub = (j == level) ? NULL : &cMajor[j + 1];

    CreateColor(base1.red   + diffRed   * j / level,
                base1.green + diffGreen * j / level,
		base1.blue  + diffBlue  * j / level,
		&cMajor[j], sub, "titlebar gradation");
  }

  // minor color allocation
  for (i = 0; i < level; i++) {
    double scale1 = (i + 0.33) / level;
    double scale2 = (i + 0.66) / level;
    XColor cMinor[2];
    unsigned long p[4];

    CreateColor(base1.red   + int(diffRed   * scale1),
		base1.green + int(diffGreen * scale1),
		base1.blue  + int(diffBlue  * scale1),
		&cMinor[0], &cMajor[i]);

    CreateColor(base1.red   + int(diffRed   * scale2),
		base1.green + int(diffGreen * scale2),
		base1.blue  + int(diffBlue  * scale2),
		&cMinor[1], &cMajor[i + 1]);

    p[0] = cMajor[i].pixel;
    p[1] = cMinor[0].pixel;
    p[2] = cMinor[1].pixel;
    p[3] = cMajor[i + 1].pixel;

    setPattern(pat, 28 * i, p);
  }

  delete [] cMajor;
  
  return pat;
}

void Titlebar::setPattern(unsigned long* pat, int offset, unsigned long p[])
{
  static int table[4][28] =
    {{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 1, 1, 2,
       1, 2, 2, 2, 1, 2, 2, 3, 3, 3, 3, 3, 3, 3 },
     { 0, 0, 2, 0, 1, 0, 2, 0, 2, 0, 2, 0, 2, 1,
       2, 1, 3, 1, 3, 1, 3, 1, 3, 2, 3, 1, 3, 3 },
     { 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 1, 1, 2,
       1, 2, 2, 2, 1, 2, 2, 3, 3, 3, 3, 3, 3, 3 },
     { 0, 0, 1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 1,
       2, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 2, 3, 3 }};
  
  for (int x = 0; x < 28; x++)
    for (int y = 0; y < 4; y++)
      pat[(x + offset) * 4 + y] = p[table[y][x]];
}
