#ifndef CLIENT_AREA_H_
#define CLIENT_AREA_H_

#include "lib/Component.h"

class Qvwm;

class ClientArea : public Component {
private:
  Window m_win;
  Rect m_rcWin;
  unsigned int m_bwWin;

  Qvwm* m_qvWm;

private:
  void setupClientWindow(Window win);

public:
  ClientArea(Qvwm* qvWm, Window win);
  ~ClientArea();

  Window getWindow() const { return m_win; }
  Rect getWinBounds() const { return m_rcWin; }
  void setWinBounds(const Rect& rcWin) { m_rcWin = rcWin; }

  Qvwm* getQvwm() const { return m_qvWm; }

  void reshapeFast(const Rect& rc);
  virtual void reshape(const Rect& rc);

  virtual void show();
  virtual void hide();

  Bool getClassHint(XClassHint* classHints);
  Bool getCommand(char*** argv, int* argc);
  Bool getWMNormalHints(XSizeHints *hints);
  Bool getWMProtocols(Atom** protocols, int* n);
  char* getWMName();
  Bool getTransientForHint(Window* tranWin);

#ifdef USE_XSMP
  unsigned char* getSMClientId();
  unsigned char* getWMRole();
#endif

  void changeState(unsigned long state, Window iconWin);
  void setInputFocus();

  void sendMessage(Atom atom);
  void sendConfigureEvent(const Rect& rc);

  virtual void onMouseEnter(const XCrossingEvent& ev);
  virtual void onButtonPress(const XButtonEvent& ev);
  virtual void onButtonRelease(const XButtonEvent& ev);
};

#endif // CLIENT_AREA_H_
