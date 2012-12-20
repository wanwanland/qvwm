#ifndef COMBO_BOX_H_
#define COMBO_BOX_H_

#include "Component.h"

class ComboBox : public Component {
private:
  TextField* m_textField;
  //  ImageButton* m_dropButton;
  //  ListBox* m_listBox;

public:
  ComboBox();
  virtual ~ComboBox();

  void addString();

  virtual void paint();

  virtual void onButtonMotion(const XMotionEvent& ev);
  virtual void onButtonPress(const XButtonEvent& ev);
  virtual void onButtonRelease(const XButtonEvent& ev);
};

#endif // COMBOBOX_H_
