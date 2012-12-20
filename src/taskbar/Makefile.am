## Process this file with automake to produce Makefile.in

SUBDIRS = start tray shortcut

noinst_LIBRARIES = libtaskbar.a

libtaskbar_a_SOURCES = \
taskbar.cc taskbar.h \
TaskbarBorder.cc TaskbarBorder.h \
ButtonToolbar.cc ButtonToolbar.h \
TaskbarButton.cc TaskbarButton.h

DEFS = \
-DQVWMDIR=\"@QVWMDIR@\" -DIMGDIR=\"@IMGDIR@\" -DSNDDIR=\"@SNDDIR@\" \
-DPATH_RM=\"@PATH_RM@\" \
-DHAVE_CONFIG_H -I$(top_builddir) -I$(top_builddir)/src \
@DEFS@

EXTRA_DIST = 
