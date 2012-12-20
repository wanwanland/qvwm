#ifndef ACTIVE_PAGE_H_
#define ACTIVE_PAGE_H_

#include "lib/Component.h"

class PageArea;

class ActivePage : public Component {
private:
  PageArea* m_pageArea;

public:
  ActivePage(PageArea* pageArea, const Rect& rc);
  ~ActivePage();

  virtual void onButtonPress(const XButtonEvent& ev);
};

#endif // ACTIVE_PAGE_H_
