## Process this file with automake to produce Makefile.in

noinst_LIBRARIES = libdesktop.a

libdesktop_a_SOURCES = \
desktop.cc desktop.h paging.cc paging.h \
icon.cc icon.h iconmenu.cc iconmenu.h \
switcher.cc switcher.h \
info_display.cc info_display.h \
Accessory.cc Accessory.h \
rearrange.cc \
ScreenSaver.cc ScreenSaver.h 

DEFS = \
-DQVWMDIR=\"@QVWMDIR@\" -DIMGDIR=\"@IMGDIR@\" -DSNDDIR=\"@SNDDIR@\" \
-DPATH_RM=\"@PATH_RM@\" \
-DHAVE_CONFIG_H -I$(top_builddir) -I$(top_builddir)/src \
@DEFS@

EXTRA_DIST = 
