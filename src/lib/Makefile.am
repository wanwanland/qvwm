## Process this file with automake to produce Makefile.in

noinst_LIBRARIES = liblib.a

liblib_a_SOURCES = \
Titlebar.cc Titlebar.h \
timer.cc timer.h debug.cc debug.h \
hash.h list.h misc.h callback.h ActionListener.h \
Component.cc Component.h Toolbar.cc Toolbar.h \
Button.cc Button.h Tooltip.cc Tooltip.h \
Menu.cc Menu.h menu_func.cc MenuItem.cc MenuItem.h \
StringButton.cc StringButton.h \
util.cc util.h \
ImageButton.cc ImageButton.h

DEFS = \
-DQVWMDIR=\"@QVWMDIR@\" -DIMGDIR=\"@IMGDIR@\" -DSNDDIR=\"@SNDDIR@\" \
-DPATH_RM=\"@PATH_RM@\" \
-DHAVE_CONFIG_H -I$(top_builddir) -I$(top_builddir)/src \
@DEFS@

EXTRA_DIST = 
