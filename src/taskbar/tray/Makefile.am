## Process this file with automake to produce Makefile.in

noinst_LIBRARIES = libtray.a

libtray_a_SOURCES = TaskTray.cc TaskTray.h Indicator.cc Indicator.h

DEFS = \
-DQVWMDIR=\"@QVWMDIR@\" -DIMGDIR=\"@IMGDIR@\" -DSNDDIR=\"@SNDDIR@\" \
-DPATH_RM=\"@PATH_RM@\" \
-DHAVE_CONFIG_H -I$(top_builddir) -I$(top_builddir)/src \
@DEFS@

EXTRA_DIST = 
