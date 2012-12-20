#include "ComboBox.h"
#include "TextField.h"
//#include "ImageButton.h"
/#include "ListBox.h"

ComboBox::ComboBox()
{
  m_textField = new TextField(this);
  m_textField->show();

  //  m_dropButton = new ImageButton(this);
  //  m_dropButton->show();

  //  m_listBox = new ListBox();
}

ComboBox::~ComboBox()
{
  delete m_textField;
  //  delete m_dropButton;
  //  delete m_listBox;
}
