## Process this file with automake to produce Makefile.in

noinst_LIBRARIES = libstart.a

libstart_a_SOURCES = \
StartButton.cc StartButton.h \
StartMenu.cc StartMenu.h StartMenuItem.cc StartMenuItem.h \
SubMenu.cc SubMenu.h SubMenuItem.cc SubMenuItem.h

DEFS = \
-DQVWMDIR=\"@QVWMDIR@\" -DIMGDIR=\"@IMGDIR@\" -DSNDDIR=\"@SNDDIR@\" \
-DPATH_RM=\"@PATH_RM@\" \
-DHAVE_CONFIG_H -I$(top_builddir) -I$(top_builddir)/src \
@DEFS@

EXTRA_DIST = 
