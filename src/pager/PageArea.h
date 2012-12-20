#ifndef PAGE_AREA_H_
#define PAGE_AREA_H_

#include "lib/Component.h"

class Pager;
class ActivePage;
class QvImage;

class PageArea : public Component {
private:
  Pager* m_pager;
  ActivePage* m_activePage;

  Dim m_pageSize;             // size of one page in pager
  QvImage* m_imgPager;

public:
  PageArea(Pager* pager, const Rect& rc);
  ~PageArea();

  Pager* getPager() const { return m_pager; }

  void moveActivePage();

  Point ConvertToPagerPos(const Point& pt);
  Rect ConvertToPagerSize(const Rect& rect);
  Point ConvertToRealPos(const Point& pt);

  virtual void paint();
  virtual void onButtonPress(const XButtonEvent& ev);
};

#endif // PAGE_AREA_H_
