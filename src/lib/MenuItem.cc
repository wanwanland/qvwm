#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "parser/qvwmrc.h"
#include "wm/Function.h"
#include "image/PixmapImage.h"
#include "lib/MenuItem.h"
#include "lib/util.h"
#include "lib/callback.h"
#include "lib/timer.h"
#include "lib/Menu.h"

#include "bitmaps/folder16.xpm"
#include "bitmaps/icon16.xpm"
#include "bitmaps/next_black.xpm"
#include "bitmaps/next_white.xpm"
#include "bitmaps/check_black.xpm"
#include "bitmaps/check_white.xpm"
#include "bitmaps/selector_black.xpm"
#include "bitmaps/selector_white.xpm"

QvImage* MenuItem::m_imgMenu[2];
QvImage* MenuItem::m_imgNext[2];
QvImage* MenuItem::m_imgCheck[2];
QvImage* MenuItem::m_imgSelector[2];

MenuItem::MenuItem(MenuElem* mElem, int index,
		   QvImage* imgDefFolder, QvImage* imgDefIcon)
{
  char* keyStr;

  m_frame = None;
  m_index = index;

  m_func = mElem->func;
  m_exec = mElem->exec;
  m_name = mElem->name;
  
  keyStr = strstr(m_name, "\\&");
  if (keyStr) {
    m_scKey = *(keyStr + 2);
    if (m_scKey >= 'a' && m_scKey <= 'z')
      m_scKey -= 0x20;
  }
  else
    m_scKey = 0;
  
  if (mElem->func == Q_DIR) {
    m_childItem = mElem->child;
    m_next = NULL;  // menu creation is delayed
  }
  else {
    m_childItem = NULL;
    m_next = NULL;
  }
  
  if (mElem->func == Q_DIR) {
    if (strcmp(mElem->file, "") != 0) {
      m_img = CreateImageFromFile(mElem->file, timer);
      if (m_img == NULL && imgDefFolder)
	m_img = imgDefFolder->Duplicate();
    }
    else {
      if (imgDefFolder)
	m_img = imgDefFolder->Duplicate();
      else
	m_img = NULL;
    }
  }
  else {
    if (strcmp(mElem->file, "") != 0) {
      m_img = CreateImageFromFile(mElem->file, timer);
      if (m_img == NULL && imgDefIcon)
	m_img = imgDefIcon->Duplicate();
    }
    else {
      if (imgDefIcon)
	m_img = imgDefIcon->Duplicate();
      else
	m_img = NULL;
    }
  }
  
  m_imgWhiteNext = m_imgNext[0]->Duplicate();
  m_imgBlackNext = m_imgNext[1]->Duplicate();
  m_imgWhiteSelector = m_imgSelector[0]->Duplicate();
  m_imgBlackSelector = m_imgSelector[1]->Duplicate();
  m_imgWhiteCheck = m_imgCheck[0]->Duplicate();
  m_imgBlackCheck = m_imgCheck[1]->Duplicate();
}

MenuItem::~MenuItem()
{
  delWindow(m_frame);

  XDestroyWindow(display, m_frame);

  QvImage::Destroy(m_img);

  if (m_next)
    delete m_next;

  if (MenuImage) {
    if (m_imgBack)
      QvImage::Destroy(m_imgBack);
  }
  if (MenuActiveImage) {
    if (m_imgActiveBack)
      QvImage::Destroy(m_imgActiveBack);
  }

  QvImage::Destroy(m_imgWhiteNext);
  QvImage::Destroy(m_imgBlackNext);
  QvImage::Destroy(m_imgWhiteSelector);
  QvImage::Destroy(m_imgBlackSelector);
  QvImage::Destroy(m_imgWhiteCheck);
  QvImage::Destroy(m_imgBlackCheck);
}

void MenuItem::createFrame(const Rect& rect)
{
  XSetWindowAttributes attributes;
  unsigned long valueMask;

  m_rc = rect;

  if (m_func != Q_SEPARATOR)
    attributes.event_mask = ButtonPressMask | ButtonReleaseMask |
      EnterWindowMask | OwnerGrabButtonMask | ExposureMask | PointerMotionMask;
  else {
    attributes.event_mask = ButtonPressMask | EnterWindowMask |
      OwnerGrabButtonMask | ExposureMask;
    m_rc.height = Menu::SeparatorHeight;
  }
  
  attributes.background_pixel = MenuColor.pixel;
  valueMask = CWBackPixel | CWEventMask;

  m_frame = XCreateWindow(display, m_menu->getFrame(),
			  m_rc.x, m_rc.y, m_rc.width, m_rc.height,
			  0, CopyFromParent, InputOutput, CopyFromParent,
			  valueMask, &attributes);
  
  if (MenuImage) {
    m_imgBack = Menu::m_imgMenuBack->GetOffsetImage(Point(m_rc.x, m_rc.y));
    
    if (m_func == Q_SEPARATOR)
      m_imgBack->SetBackground(m_frame);
  }

  if (MenuActiveImage) {
    if (m_func == Q_SEPARATOR)
      m_imgActiveBack = NULL;
    else {
      Point pt(m_rc.x, m_rc.y);
      m_imgActiveBack = Menu::m_imgActiveMenuBack->GetOffsetImage(pt);
    }
  }

  addWindow(m_frame);

  show();
}

Menu* MenuItem::getNextMenu()
{
  // delayed menu creation
  if (m_next == NULL) {
    XFontSet fs = m_menu->GetFontSet();
    Qvwm* qvWm = m_menu->GetQvwm();
    int leftMargin = m_menu->GetLeftMargin();
    int nameMargin = m_menu->GetNameMargin();
    int hiMargin = m_menu->GetHeightMargin();

    ASSERT(m_childItem);

    MenuItem** mi = MenuItem::createMenuItems(m_childItem);
    m_next = new Menu(mi, m_menu, fs, qvWm, leftMargin, nameMargin, hiMargin);
  }

  return m_next;
}

/*
 * SetMenuFocus --
 *   Give the focus to the menu item, and redraw.
 */
void MenuItem::setItemFocus()
{
  if (MenuImage)
    m_imgBack->SetBackground(None);
  
  if (MenuActiveImage)
    m_imgActiveBack->SetBackground(m_frame);
  else
    XSetWindowBackground(display, m_frame, MenuActiveColor.pixel);

  XClearWindow(display, m_frame);

  drawItemField(True);
}

void MenuItem::resetItemFocus()
{
  if (MenuActiveImage)
    m_imgActiveBack->SetBackground(None);
  
  if (MenuImage)
    m_imgBack->SetBackground(m_frame);
  else
    XSetWindowBackground(display, m_frame, MenuColor.pixel);

  XClearWindow(display, m_frame);

  drawItemField(False);
}

Bool MenuItem::changeFocus()
{
  MenuItem* focusMenuItem = m_menu->GetFocusItem();
  
  if (this != focusMenuItem) {
    if (focusMenuItem)
      focusMenuItem->resetItemFocus();
  
    if (m_func != Q_SEPARATOR) {
      setItemFocus();
      m_menu->SetFocusItem(this);
    }
    else
      m_menu->SetFocusItem(NULL);

    return True;
  }

  return False;
}

Bool MenuItem::execShortcut(char key)
{
  if (m_scKey != key)
    return False;

  if (m_menu->IsSelectable(m_func)) {
    if (m_func != Q_DIR)
      Menu::UnmapAllMenus();
    else {
      MenuItem* focusMenuItem = m_menu->GetFocusItem();

      if (this != focusMenuItem) {
	if (focusMenuItem)
	  focusMenuItem->resetItemFocus();
	setItemFocus();
	m_menu->SetFocusItem(this);
      }
    }

    m_menu->ExecFunction(this);
  }

  return True;
}
  
void MenuItem::execMenuItem()
{
  ExecCommand(m_exec);
}

void MenuItem::extractChildMenu()
{
  Menu* child;

  child = getNextMenu();

  m_menu->ExtractChildMenu(this, child);
}

/*
 * Redraw function
 */
void MenuItem::paint()
{
  if (m_func == Q_SEPARATOR)
    drawSeparator();
  else {
    Bool focus = (this == m_menu->GetFocusItem());
    drawItemField(focus);
  }
}

/*
 * DrawSeparator --
 *   Draw the separator.
 */
void MenuItem::drawSeparator()
{
  XSetForeground(display, ::gc, darkGray.pixel);
  XDrawLine(display, m_frame, ::gc, 0, 2, m_rc.width - 1, 2);

  XSetForeground(display, ::gc, white.pixel);
  XDrawLine(display, m_frame, ::gc, 0, 3, m_rc.width - 1, 3);
}

void MenuItem::drawItemField(Bool focus)
{
  XRectangle ink, log;
  int y;

  int imageArea = m_menu->GetImageAreaWidth();
  XFontSet fs = m_menu->GetFontSet();

  /*
   * Draw the menu text.
   */
  XmbTextExtents(fs, m_name, strlen(m_name), &ink, &log);
  y = (m_rc.height - log.height) / 2 - log.y;

  if (focus) {
    if (m_menu->IsSelectable(m_func)) {
      XSetForeground(display, gc, MenuStringActiveColor.pixel);
      DrawRealString(m_frame, fs, gc, imageArea, y, m_name);
    }
    else {
      XSetForeground(display, gc, darkGray.pixel);
      DrawRealString(m_frame, fs, gc, imageArea, y, m_name);
    }
    
    if (m_menu->IsChecked(m_func) && m_img == NULL)
      m_imgWhiteCheck->Display(m_frame, Point(5, 6));
    
    if (m_menu->IsSelected(m_func) && m_img == NULL)
      m_imgWhiteSelector->Display(m_frame, Point(4, 5));
    
    if (m_func == Q_DIR)
      m_imgWhiteNext->Display(m_frame,
			      Point(m_rc.width - 14, m_rc.height / 2 - 4));
  }
  else {
    if (m_menu->IsSelectable(m_func)) {
      XSetForeground(display, gc, MenuStringColor.pixel);
      DrawRealString(m_frame, fs, gc, imageArea, y, m_name);
    }
    else {
      XSetForeground(display, gc, white.pixel);
      DrawRealString(m_frame, fs, gc, imageArea + 1, y + 1, m_name);
      XSetForeground(display, gc, darkGray.pixel);
      DrawRealString(m_frame, fs, gc, imageArea, y, m_name);
    }

    if (m_menu->IsChecked(m_func) && m_img == NULL)
      m_imgBlackCheck->Display(m_frame, Point(5, 6));

    if (m_menu->IsSelected(m_func) && m_img == NULL)
      m_imgBlackSelector->Display(m_frame, Point(4, 5));
    
    if (m_func == Q_DIR)
      m_imgBlackNext->Display(m_frame,
			      Point(m_rc.width - 14, m_rc.height / 2 - 4));
  }

  /*
   * Draw the menu pixmap, if neccesary.
   */
  if (m_img && m_func != Q_NONE) {
    int leftMargin = m_menu->GetLeftMargin();
    
    y = (m_rc.height - m_img->GetSize().height) / 2;
    
    m_img->Display(m_frame, Point(leftMargin, y));
  }
}

/*
 * Event Handlers
 */
void MenuItem::onMouseEnter(const XCrossingEvent& ev)
{
  if (changeFocus())
    m_menu->SetDelayedItem(this);
}

void MenuItem::onMouseMotion(const XMotionEvent& ev)
{
  MenuItem* focusMenuItem = m_menu->GetFocusItem();

  // the mouse pointer is outside of this menu
  if (focusMenuItem == NULL)
    return;

  if (this != focusMenuItem && m_func != Q_SEPARATOR) {
    if (changeFocus())
      m_menu->SetDelayedItem(this);
  }
}

void MenuItem::onButtonPress(const XButtonEvent& ev) 
{
  if (ev.button != Button1)
    return;

  if (m_func == Q_DIR) {
    timer->ForceTimeout(new Callback<Menu>(m_menu, &Menu::PopupMenu));

    if (m_menu->GetChildMenu() != m_next) {
      if (changeFocus()) {
	timer->ResetTimeout(new Callback<Menu>(m_menu, &Menu::PopupMenu));
	timer->SetTimeout(0, new Callback<Menu>(m_menu, &Menu::PopupMenu));
      }
    }
  }
}

void MenuItem::onButtonRelease(const XButtonEvent& ev)
{
  if (!(ev.state & Button1Mask))
    return;

  if (m_menu->IsSelectable(m_func)) {
    if (m_func != Q_DIR) {
      Menu::UnmapAllMenus();
      m_menu->ExecFunction(this);
    }
  }
}

/*
 * Static functions
 */
void MenuItem::Initialize()
{
  /*
   * create images for default folder
   */
  m_imgMenu[0] = new PixmapImage(folder16);
  m_imgMenu[1] = new PixmapImage(icon16);

  /*
   * create pixmap for next mark
   */
  m_imgNext[0] = new PixmapImage(next_white);
  m_imgNext[1] = new PixmapImage(next_black);

  /*
   * create pixmap for check mark
   */
  m_imgCheck[0] = new PixmapImage(check_white);
  m_imgCheck[1] = new PixmapImage(check_black);

  /*
   * create pixmap for selector
   */
  m_imgSelector[0] = new PixmapImage(selector_white);
  m_imgSelector[1] = new PixmapImage(selector_black);
}

MenuItem** MenuItem::createMenuItems(MenuElem* mElem)
{
  MenuItem** mi;
  int nitems;
  int i;

  nitems = GetMenuItemNum(mElem);
  mi = new MenuItem*[nitems + 1];

  // create child menus
  for (i = 0; i < nitems; i++) {
    mi[i] = new MenuItem(mElem, i);

    mElem = mElem->next;
  }

  mi[nitems] = NULL;

  return mi;
}
