#ifndef IMAGE_BUTTON_H_
#define IMAGE_BUTTON_H_

#include "lib/Button.h"

class ImageButton : public Button {
protected:
  QvImage* m_img;

public:
  ImageButton(Component* parent, QvImage* img, const Rect& rc);
  virtual ~ImageButton();

  virtual void paint();
};


#endif // IMAGE_BUTTON_H_
