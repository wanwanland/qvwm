#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "frame/AllBorders.h"
#include "frame/Border.h"
#include "window/qvwm.h"

AllBorders::AllBorders(Qvwm* qvWm, const Rect& rc)
{
  for (int i = 0; i < 8; i++)
    m_border[i] = new Border(qvWm, (Border::BorderPos)i);

  // call RedrawWindow() or so in the first time
  reshape(rc);
}

AllBorders::~AllBorders()
{
  for (int i = 0; i < 8; i++)
    delete m_border[i];
}

void AllBorders::show()
{
  for (int i = 0; i < 8; i++)
    m_border[i]->show();
}

void AllBorders::hide()
{
  for (int i = 0; i < 8; i++)
    m_border[i]->hide();
}

void AllBorders::reshape(const Rect& rc)
{
  int szCorner = Qvwm::TITLE_HEIGHT + Qvwm::TOP_BORDER;
  int thick = Qvwm::TOP_BORDER;
  Rect rcBorder[8] = {
    // TOP
    Rect(szCorner, 0, rc.width - szCorner * 2, thick),
    // BOTTOM
    Rect(szCorner, rc.height - thick, rc.width - szCorner * 2, thick),
    // LEFT
    Rect(0, szCorner, thick, rc.height - szCorner * 2),
    // RIGHT
    Rect(rc.width - thick, szCorner, thick, rc.height - szCorner * 2),
    // TOP_LEFT
    Rect(0, 0, szCorner, szCorner),
    // TOP_RIGHT
    Rect(rc.width - szCorner, 0, szCorner, szCorner),
    // BOTTOM_LEFT
    Rect(0, rc.height - szCorner, szCorner, szCorner),
    // BOTTOM_RIGHT
    Rect(rc.width - szCorner, rc.height - szCorner, szCorner, szCorner)
  };

  if (rcBorder[Border::TOP].width <= 0)
    rcBorder[Border::TOP].width = rcBorder[Border::BOTTOM].width = 1;
  if (rcBorder[Border::LEFT].height <= 0)
    rcBorder[Border::LEFT].height = rcBorder[Border::RIGHT].height = 1;

  for (int i = 0; i < 8; i++)
    m_border[i]->reshape(rcBorder[i]);
}

void AllBorders::paintWithFocusChange()
{
  for (int i = 0; i < 8; i++)
    m_border[i]->paintWithFocusChange();
}
