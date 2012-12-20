## Process this file with automake to produce Makefile.in

noinst_LIBRARIES = libshortcut.a

libshortcut_a_SOURCES = \
ShortcutToolbar.cc ShortcutToolbar.h \
TaskbarShortcut.cc TaskbarShortcut.h

DEFS = \
-DQVWMDIR=\"@QVWMDIR@\" -DIMGDIR=\"@IMGDIR@\" -DSNDDIR=\"@SNDDIR@\" \
-DPATH_RM=\"@PATH_RM@\" \
-DHAVE_CONFIG_H -I$(top_builddir) -I$(top_builddir)/src \
@DEFS@

EXTRA_DIST = 
