#ifndef ALL_BORDERS_H_
#define ALL_BORDERS_H_

#include "lib/misc.h"

class Border;

class AllBorders {
private:
  Border* m_border[8];

public:
  AllBorders(Qvwm* qvWm, const Rect& rc);
  ~AllBorders();

  void show();
  void hide();
  void reshape(const Rect& rc);

  void paintWithFocusChange();
};

#endif // ALL_BORDERS_H_
