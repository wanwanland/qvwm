## Process this file with automake to produce Makefile.in

noinst_LIBRARIES = libframe.a

libframe_a_SOURCES = \
CtrlButton.cc CtrlButton.h \
AllBorders.cc AllBorders.h Border.cc Border.h InnerEdge.cc InnerEdge.h \
AllEdges.cc AllEdges.h ClientArea.cc ClientArea.h \
frame.cc FrameButton.cc FrameButton.h \
WindowTitlebar.cc WindowTitlebar.h

DEFS = \
-DQVWMDIR=\"@QVWMDIR@\" -DIMGDIR=\"@IMGDIR@\" -DSNDDIR=\"@SNDDIR@\" \
-DPATH_RM=\"@PATH_RM@\" \
-DHAVE_CONFIG_H -I$(top_builddir) -I$(top_builddir)/src \
@DEFS@

EXTRA_DIST = 
