#ifndef MENU_ITEM_H_
#define MENU_ITEM_H_

#include "lib/Component.h"
#include "lib/misc.h"
#include "lib/util.h"

class QvImage;

class MenuItem : public Component {
protected:
  QvImage* m_img;
  char* m_name;

  FuncNumber m_func;          // function number
  char* m_exec;               // exec command
  char m_scKey;               // short cut key for menu item
  Bool m_check;               // check for toggle function

  Menu* m_next;               // child menu
  MenuElem* m_childItem;      // keep child menu element for delayed creation

  Menu* m_menu;
  int m_index;

  QvImage *m_imgBack, *m_imgActiveBack;
  QvImage *m_imgWhiteNext, *m_imgBlackNext;
  QvImage *m_imgWhiteSelector, *m_imgBlackSelector;
  QvImage *m_imgWhiteCheck, *m_imgBlackCheck;

protected:
  static QvImage* m_imgMenu[2];
  static QvImage* m_imgNext[2];
  static QvImage* m_imgSelector[2];
  static QvImage* m_imgCheck[2];

private:
  void drawSeparator();
  void drawItemField(Bool focus);

public:
  MenuItem(MenuElem* mElem, int index,
	   QvImage* imgDefFolder = NULL, QvImage* imgDefIcon = NULL);
  virtual ~MenuItem();

  FuncNumber getFunction() const { return m_func; }
  int getIndex() const { return m_index; }
  char* getName() const { return m_name; }
  QvImage* getImage() const { return m_img; }
  Rect getRect() const { return m_rc; }

  Menu* getMenu() const { return m_menu; }
  void setMenu(Menu* menu) { m_menu = menu; }

  void createFrame(const Rect& rect);

  void setItemFocus();
  void resetItemFocus();
  Bool changeFocus();

  Bool execShortcut(char key);
  void execMenuItem();

  virtual Menu* getNextMenu();
  void extractChildMenu();

  virtual void paint();
  virtual void onMouseEnter(const XCrossingEvent& ev);
  virtual void onMouseMotion(const XMotionEvent& ev);
  virtual void onButtonPress(const XButtonEvent& ev);
  virtual void onButtonRelease(const XButtonEvent& ev);

public:
  static void Initialize();
  static MenuItem** createMenuItems(MenuElem* mElem);
};

#endif // MENU_ITEM_H_
