#ifndef INNER_EDGE_H_
#define INNER_EDGE_H_

#include "lib/Component.h"

class Qvwm;

class InnerEdge : public Component {
public:
  enum EdgePos {
    TOP = 0, BOTTOM = 1, LEFT = 2, RIGHT = 3
  };

private:
  EdgePos m_pos;
  Qvwm* m_qvWm;

public:
  InnerEdge(Qvwm* qvWm, EdgePos pos);
  ~InnerEdge();

  virtual void paint();
};

#endif // INNER_EDGE_H_
