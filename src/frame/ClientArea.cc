#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include "main.h"
#include "frame/ClientArea.h"
#include "window/qvwm.h"
#include "parser/qvwmrc.h"
#include "lib/util.h"
#include "desktop/paging.h"

ClientArea::ClientArea(Qvwm* qvWm, Window win)
  : Component(Event::CLASS_CLIENT_AREA), m_qvWm(qvWm)
{
  XSetWindowAttributes attributes;
  unsigned long valueMask;

  attributes.event_mask = SubstructureRedirectMask | ButtonPressMask |
                          ButtonReleaseMask | EnterWindowMask |
			  LeaveWindowMask | ExposureMask;
  attributes.background_pixel = FrameColor.pixel;
  valueMask = CWBackPixel | CWEventMask;

  m_frame = XCreateWindow(display, qvWm->GetFrameWin(),
			  0, 0, 1, 1,
			  0, CopyFromParent, InputOutput, CopyFromParent,
			  valueMask, &attributes);
  addWindow(m_frame);

  // client window
  setupClientWindow(win);
}

ClientArea::~ClientArea()
{
  delWindow(m_frame);
  XDestroyWindow(display, m_frame);
}

void ClientArea::setupClientWindow(Window win)
{
  XWindowAttributes attr;
  XSetWindowAttributes attributes;
  unsigned long valueMask;

  m_win = win;
  addWindow(win);

  XGetWindowAttributes(display, win, &attr);
  m_rcWin = Rect(attr.x, attr.y, attr.width, attr.height);

  m_bwWin = attr.border_width;
  XSetWindowBorderWidth(display, win, 0);

  // reparent window
  XReparentWindow(display, win, m_frame, 0, 0);

  // set new attributes
  attributes.event_mask = StructureNotifyMask | PropertyChangeMask |
    EnterWindowMask | LeaveWindowMask | ColormapChangeMask;

#if defined(USE_SS) && !defined(USE_SSEXT)
  attributes.event_mask |= attr.your_event_mask;
#endif

  attributes.do_not_propagate_mask = ButtonPressMask | ButtonReleaseMask;
  valueMask = CWEventMask | CWDontPropagate;

  XChangeWindowAttributes(display, win, valueMask, &attributes);

  XAddToSaveSet(display, win);

#if defined(USE_SS) && !defined(USE_SSEXT)
  // check KeyPressMask of subwindows of wOrig again
  scrSaver->SelectEvents(win);
#endif
}

void ClientArea::reshapeFast(const Rect& rc)
{
  Component::reshape(rc);
}

void ClientArea::reshape(const Rect& rc)
{
  Component::reshape(rc);

  XResizeWindow(display, m_win, rc.width, rc.height);
}

void ClientArea::show()
{
  Component::show();

  /*
   * Don't notify MapNotify to qvwm.
   */
  XWindowAttributes attr;
  long tmpMask;

  XGetWindowAttributes(display, m_win, &attr);

  tmpMask = attr.your_event_mask &
    ~(StructureNotifyMask | SubstructureNotifyMask);
  XSelectInput(display, m_win, tmpMask);

  XMapWindow(display, m_win);

  XSelectInput(display, m_win, attr.your_event_mask);
}

void ClientArea::hide()
{
  /*
   * Don't notify UnmapNotify to qvwm.
   */
  XWindowAttributes attr;
  long tmpMask;

  XGetWindowAttributes(display, m_win, &attr);

  tmpMask = attr.your_event_mask &
    ~(StructureNotifyMask | SubstructureNotifyMask);
  XSelectInput(display, m_win, tmpMask);

  XUnmapWindow(display, m_win);

  XSelectInput(display, m_win, attr.your_event_mask);

  Component::hide();
}

// return True (non-zero) if success
Bool ClientArea::getClassHint(XClassHint* classHint)
{
  return XGetClassHint(display, m_win, classHint);
}

Bool ClientArea::getCommand(char*** argv, int* argc)
{
  return XGetCommand(display, m_win, argv, argc);
}

Bool ClientArea::getWMNormalHints(XSizeHints *hints)
{
  long supplied;

  return XGetWMNormalHints(display, m_win, hints, &supplied);
}

Bool ClientArea::getWMProtocols(Atom** protocols, int* n)
{
  Atom atype;
  int aformat;
  unsigned long nitems, bytes_remain;

  if (XGetWMProtocols(display, m_win, protocols, n) != 0)
    return True;

  if (XGetWindowProperty(display, m_win, _XA_WM_PROTOCOLS, 0L, 10L, False,
			 _XA_WM_PROTOCOLS, &atype, &aformat,
			 &nitems, &bytes_remain,
			 (unsigned char **)protocols) == Success) {
    *n = nitems;
    return True;
  }

  *protocols = NULL;

  return False;
}

Bool ClientArea::getTransientForHint(Window* tranWin)
{
  return XGetTransientForHint(display, m_win, tranWin);
}

char* ClientArea::getWMName()
{
  XTextProperty xtp;
  char** cl;
  int n;
  char* name;

  if (XGetWMName(display, m_win, &xtp)) {
    XmbTextPropertyToTextList(display, &xtp, &cl, &n);
    if (cl) {
      name = new char[strlen(cl[0]) + 1];
      strcpy(name, cl[0]);
      XFreeStringList(cl);
      return name;
    }
  }

  return NULL;
}

#ifdef USE_XSMP
unsigned char* ClientArea::getSMClientId()
{
  Atom atype;
  int aformat;
  unsigned long nitems, bytes_remain;
  unsigned char* prop = NULL;
  Window leader;

  // WM_CLIENT_LEADER
  //  type: WINDOW
  //  format: 8
  //  -- the window ID of the client leader window
  if (XGetWindowProperty(display, m_win,
			 _XA_WM_CLIENT_LEADER, 0, 1, False,
			 XA_WINDOW, &atype, &aformat,
			 &nitems, &bytes_remain, &prop) != Success)
    return NULL;

  if (atype == XA_WINDOW && aformat == 32 && nitems == 1 && bytes_remain == 0)
    leader = ((Window *)prop)[0];
  else
    leader = None;

  if (prop)
    XFree(prop);

  if (leader == None)
    return NULL;

  XTextProperty text_prop;

  // SM_CLIENT_ID
  //   type: STRING
  //   format: 8
  //   XPCS, ISO 8859-1
  //   -- the client leader window must have this prop.
  if (XGetTextProperty(display, leader, &text_prop, _XA_SM_CLIENT_ID)) {
    if (text_prop.encoding == XA_STRING &&
	text_prop.format == 8 && text_prop.nitems != 0) {
      return text_prop.value;
    }
  }

  return NULL;
}

unsigned char* ClientArea::getWMRole()
{
  Atom atype;
  int aformat;
  unsigned long nitems, bytes_remain;
  unsigned char* wmRole = NULL;

  // WM_WINDOW_ROLE
  //  type: STRING
  //  format: 8
  //  XPCS, ISO 8859-1
  //  -- a string that uniquely identifies that window amaong
  //     all windows that have the same client leader window.
  if (XGetWindowProperty(display, m_win,
			 _XA_WM_WINDOW_ROLE, 0, 256, False,
			 XA_STRING, &atype, &aformat,
			 &nitems, &bytes_remain, &wmRole) != Success)
    wmRole = NULL;

  return wmRole;
}
#endif // USE_XSMP

void ClientArea::changeState(unsigned long state, Window iconWin)
{
  unsigned long data[2];

  data[0] = state;
  data[1] = iconWin;
  
  XChangeProperty(display, m_win, _XA_WM_STATE, _XA_WM_STATE, 32, 
		  PropModeReplace, (unsigned char *)data, 2);
}

void ClientArea::setInputFocus()
{
  XSetInputFocus(display, m_win, RevertToParent, CurrentTime);
}

void ClientArea::sendMessage(Atom atom)
{
  XClientMessageEvent cev;

  cev.type = ClientMessage;
  cev.window = m_win;
  cev.message_type = _XA_WM_PROTOCOLS;
  cev.format = 32;
  cev.data.l[0] = atom;
  cev.data.l[1] = CurrentTime;

#ifdef DEBUG
  if (atom == _XA_WM_DELETE_WINDOW)
    printf("Send delete msg to '%s'\n", name);
#endif

  XSendEvent(display, m_win, False, 0L, (XEvent *)&cev);
}

/*
 * Send event to the window whose position and size changed.
 * rc is a virtual rectangle value.
 */
void ClientArea::sendConfigureEvent(const Rect& rc)
{
  XConfigureEvent cev;
  int borderWidth, topBorder, titleHeight, titleEdge;
  
  cev.type = ConfigureNotify;
  cev.display = display;
  cev.event = m_win;
  cev.window = m_win;
      
  m_qvWm->GetBorderAndTitle(borderWidth, topBorder, titleHeight, titleEdge);

  cev.x = rc.x - paging->origin.x + borderWidth;
  cev.y = rc.y - paging->origin.y + topBorder + titleHeight + titleEdge;
  cev.width = rc.width - borderWidth * 2;
  cev.height = rc.height - (borderWidth + topBorder
				       + titleHeight + titleEdge);

  cev.border_width = 0;
  cev.above = m_qvWm->GetFrameWin();
  cev.override_redirect = False;
  
  XSendEvent(display, m_win, False, StructureNotifyMask, (XEvent *)&cev);
}

/*
 * Event handlers
 */
void ClientArea::onMouseEnter(const XCrossingEvent& ev)
{
  m_qvWm->onMouseEnter(ev);
}

void ClientArea::onButtonPress(const XButtonEvent& ev)
{
  m_qvWm->onButtonPress(ev);
}

void ClientArea::onButtonRelease(const XButtonEvent& ev)
{
  m_qvWm->onButtonRelease(ev);
}
