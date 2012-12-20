#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "parser/qvwmrc.h"
#include "pager/PageArea.h"
#include "pager/ActivePage.h"
#include "pager/Pager.h"
#include "desktop/paging.h"

PageArea::PageArea(Pager* pager, const Rect& rc)
  : m_pager(pager)
{
  XSetWindowAttributes attributes;
  unsigned long valueMask;

  m_rc = rc;

  attributes.background_pixel = PagerColor.pixel;
  attributes.event_mask = ButtonPressMask | ExposureMask;
  valueMask = CWBackPixel | CWEventMask;
  
  m_frame = XCreateWindow(display, pager->getFrame(),
			  rc.x, rc.y, rc.width, rc.height,
			  0, CopyFromParent, InputOutput, CopyFromParent,
			  valueMask, &attributes);
  
  addWindow(m_frame);

  if (PagerImage) {
    m_imgPager = CreateImageFromFile(PagerImage, timer);
    if (m_imgPager)
      m_imgPager->SetBackground(m_frame);
  }

  show();

  m_pageSize = Dim(rc.width / paging->rcVirt.width,
		   rc.height / paging->rcVirt.height);

  Rect rcActivePage(0, 0, m_pageSize.width - 2, m_pageSize.height - 2);
  m_activePage = new ActivePage(this, rcActivePage);
}

PageArea::~PageArea()
{
  delWindow(m_frame);

  XDestroyWindow(display, m_frame);

  if (PagerImage)
    QvImage::Destroy(m_imgPager);

  delete m_activePage;
}

void PageArea::moveActivePage()
{
  Rect rcRoot = rootQvwm->GetRect();
  Point pt;

  pt.x = (paging->origin.x - paging->rcVirt.x * rcRoot.width)
    * m_rc.width / (rcRoot.width * paging->rcVirt.width) + 1;
  pt.y = (paging->origin.y - paging->rcVirt.y * rcRoot.height)
    * m_rc.height / (rcRoot.height * paging->rcVirt.height) + 1;

  m_activePage->move(pt);
} 

/*
 * ConvertToPagerPos
 *   Convert real position to position in pager.
 */
Point PageArea::ConvertToPagerPos(const Point& pt)
{
  Rect rcRoot = rootQvwm->GetRect();
  Point ptMini;

  ptMini.x = pt.x * m_pageSize.width / rcRoot.width -
    paging->rcVirt.x * m_pageSize.width;
  ptMini.y = pt.y * m_pageSize.height / rcRoot.height -
    paging->rcVirt.y * m_pageSize.height;

  return (ptMini);
}

/*
 * ConvertToPagerSize
 *   Convert real size to size in pager.
 */
Rect PageArea::ConvertToPagerSize(const Rect& rect)
{
  Rect rcRoot = rootQvwm->GetRect();
  Rect rcMini;

  rcMini.x = rect.x * m_pageSize.width / rcRoot.width -
    paging->rcVirt.x * m_pageSize.width;
  rcMini.y = rect.y * m_pageSize.height / rcRoot.height -
    paging->rcVirt.y * m_pageSize.height;
  rcMini.width = rect.width * m_pageSize.width / rcRoot.width;
  if (rcMini.width == 0)
    rcMini.width = 1;
  rcMini.height = rect.height * m_pageSize.height / rcRoot.height;
  if (rcMini.height == 0)
    rcMini.height = 1;

  return (rcMini);
}

/*
 * ConvertToRealPos
 *   Convert position in pager to real position.
 */
Point PageArea::ConvertToRealPos(const Point& pt)
{
  Rect rcRoot = rootQvwm->GetRect();
  Point ptReal;

  ptReal.x = pt.x * rcRoot.width / m_pageSize.width +
    paging->rcVirt.x * rcRoot.width;
  ptReal.y = pt.y * rcRoot.height / m_pageSize.height +
    paging->rcVirt.y * rcRoot.height;

  return (ptReal);
}

void PageArea::paint()
{
  int i;

  for (i = 1; i < paging->rcVirt.width; i++) {
    XSetForeground(display, gc, darkGray.pixel);
    XDrawLine(display, m_frame, gc,
	      m_pageSize.width * i - 1, 1,
	      m_pageSize.width * i - 1, m_rc.height - 2);
    XSetForeground(display, gc, white.pixel);
    XDrawLine(display, m_frame, gc,
	      m_pageSize.width * i, 1,
	      m_pageSize.width * i, m_rc.height - 2);
  }

  for (i = 1; i < paging->rcVirt.height; i++) {
    XSetForeground(display, gc, darkGray.pixel);
    XDrawLine(display, m_frame, gc,
	      1, m_pageSize.height * i - 1,
	      m_rc.width - 2, m_pageSize.height * i - 1);
    XSetForeground(display, gc, white.pixel);
    XDrawLine(display, m_frame, gc,
	      1, m_pageSize.height * i,
	      m_rc.width - 2, m_pageSize.height * i);
  }
}

/*
 * Move the current page to the clicked area.
 */
void PageArea::onButtonPress(const XButtonEvent& ev)
{
  if (ev.button != Button1)
    return;

  m_pager->raise();

  if (UseTaskbar) {
    if (OnTopTaskbar && !OnTopPager)
      taskBar->raise();
    if (!taskBar->IsHiding())
      paging->RaisePagingBelt();
  }

  Point ptPage;
  Point ptWin(ev.x, ev.y);
  Rect rcRoot = rootQvwm->GetRect();
  
  ptPage.x = ptWin.x * paging->rcVirt.width / m_rc.width;
  ptPage.y = ptWin.y * paging->rcVirt.height / m_rc.height;
  
  Point oldOrigin = paging->origin;
  paging->origin.x = (ptPage.x + paging->rcVirt.x) * rcRoot.width;
  paging->origin.y = (ptPage.y + paging->rcVirt.y) * rcRoot.height;
  
  if (paging->origin.x != oldOrigin.x || paging->origin.y != oldOrigin.y)
    PlaySound(PagerSound);

  paging->PagingAllWindows(oldOrigin);
}
