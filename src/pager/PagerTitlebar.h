#ifndef PAGER_TITLEBAR_H_
#define PAGER_TITLEBAR_H_

#include "lib/Component.h"

class Pager;

class PagerTitlebar : public Component {
private:
  Pager* m_pager;

public:
  PagerTitlebar(Pager* pager, const Rect& rc);
  ~PagerTitlebar();

  void onButtonPress(const XButtonEvent& ev);
};

#endif // PAGER_TITLEBAR_H_
