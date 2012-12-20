#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include "main.h"
#include "image/Image.h"
#include "lib/ImageButton.h"

ImageButton::ImageButton(Component* parent, QvImage* img, const Rect& rc)
  : Button(parent->getFrame(), rc), m_img(img)
{
}

ImageButton::~ImageButton()
{
  QvImage::Destroy(m_img);
}

void ImageButton::paint()
{

  XClearWindow(display, m_frame);

  if (m_img) {
    if (CheckState(NORMAL))
      m_img->Display(m_frame, Point(2, 2));
    else
      m_img->Display(m_frame, Point(3, 3));
  }

  Button::paint();
}
