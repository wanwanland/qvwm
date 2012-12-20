#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "frame/AllEdges.h"
#include "frame/InnerEdge.h"
#include "window/qvwm.h"

AllInnerEdges::AllInnerEdges(Qvwm* qvWm, const Rect& rc)
  : m_qvWm(qvWm)
{
  for (int i = 0; i < 4; i++)
    m_edge[i] = new InnerEdge(qvWm, (InnerEdge::EdgePos)i);

  reshape(rc);
}

AllInnerEdges::~AllInnerEdges()
{
  for (int i = 0; i < 4; i++)
    delete m_edge[i];
}

void AllInnerEdges::show()
{
  for (int i = 0; i < 4; i++)
    m_edge[i]->show();
}

void AllInnerEdges::hide()
{
  for (int i = 0; i < 4; i++)
    m_edge[i]->hide();
}

void AllInnerEdges::reshape(const Rect& rc)
{
  int thick = 2;
  int topHeight = m_qvWm->CheckFlags(TITLE) ? (thick + 1) : thick;
  Rect rcEdge[4] = {
    // TOP
    Rect(rc.x, rc.y, rc.width, topHeight),
    // BOTTOM
    Rect(rc.x, rc.y + rc.height - thick, rc.width, thick),
    // LEFT
    Rect(rc.x, rc.y + topHeight, thick, rc.height - topHeight - thick),
    // RIGHT
    Rect(rc.x + rc.width - thick, rc.y + topHeight,
	 thick, rc.height - topHeight - thick)
  };
    
  for (int i = 0; i < 4; i++)
    m_edge[i]->reshape(rcEdge[i]);
}
