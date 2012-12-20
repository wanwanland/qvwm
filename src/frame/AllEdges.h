#ifndef ALL_EDGES_H_
#define ALL_EDGES_H_

class InnerEdge;
class Qvwm;

class AllInnerEdges {
private:
  InnerEdge* m_edge[4];
  Qvwm* m_qvWm;

public:
  AllInnerEdges(Qvwm* qvWm, const Rect& rc);
  ~AllInnerEdges();

  void show();
  void hide();
  void reshape(const Rect& rc);
};

#endif // ALL_EDGES_H_
