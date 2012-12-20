#ifndef TITLEBAR_H_
#define TITLEBAR_H_

#include "lib/Component.h"

class QvImage;
class Tooltip;

class Titlebar : public Component {
protected:
  Component *m_comp;

  char* m_title;
  char* m_shortTitle;

  Pixmap m_pixGrad;                // titlebar gradation pixmap
  Pixmap m_pixActiveGrad;          // active titlebar gradation pixmap

  QvImage* m_imgTitle;
  QvImage* m_imgActiveTitle;

  Tooltip* m_toolTip;

public:
  static QvImage* m_imgTitlebar;
  static QvImage* m_imgActiveTitlebar;
  static unsigned long* m_gradPattern;
  static unsigned long* m_gradActivePattern;

protected:
  virtual int getTitleWidth();
  virtual int getTitleX();
  virtual const XColor& getForeColor();

public:
  Titlebar(Component* comp, const Rect& rc);
  virtual ~Titlebar();

  void setTitle(char* title);
  void calcShortTitle();

  virtual void reshape(const Rect& rc);

  virtual void paint();
  virtual void paintWithFocusChange();
  
  virtual void onMouseEnter(const XCrossingEvent& ev);
  virtual void onMouseLeave(const XCrossingEvent& ev);
  virtual void onButtonPress(const XButtonEvent& ev);
  virtual void onMouseMotion(const XMotionEvent& ev);

private:
  static unsigned long* createPattern(XColor base1, XColor base2);
  static void setPattern(unsigned long* pat, int offset, unsigned long p[]);

public:
  static void initialize();
  static void createGradPattern();

  static Pixmap createGradPixmap(unsigned long* pat, int width, Window win);
};

#endif // TITLEBAR_H_
