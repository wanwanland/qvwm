## Process this file with automake to produce Makefile.in

noinst_LIBRARIES = libwindow.a

libwindow_a_SOURCES = \
window.cc move.cc resize.cc resize_sp.cc \
focus.cc focus_mgr.cc focus_mgr.h colormap.cc configure.cc property.cc \
qvwm.cc qvwm.h exec.cc exec.h

DEFS = \
-DQVWMDIR=\"@QVWMDIR@\" -DIMGDIR=\"@IMGDIR@\" -DSNDDIR=\"@SNDDIR@\" \
-DPATH_RM=\"@PATH_RM@\" \
-DHAVE_CONFIG_H -I$(top_builddir) -I$(top_builddir)/src \
@DEFS@

EXTRA_DIST = 
