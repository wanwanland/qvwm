## Process this file with automake to produce Makefile.in

noinst_LIBRARIES = libpager.a

libpager_a_SOURCES = \
Pager.cc Pager.h PagerTitlebar.cc PagerTitlebar.h \
PageArea.cc PageArea.h ActivePage.cc ActivePage.h Miniature.cc Miniature.h

DEFS = \
-DQVWMDIR=\"@QVWMDIR@\" -DIMGDIR=\"@IMGDIR@\" -DSNDDIR=\"@SNDDIR@\" \
-DPATH_RM=\"@PATH_RM@\" \
-DHAVE_CONFIG_H -I$(top_builddir) -I$(top_builddir)/src \
@DEFS@

EXTRA_DIST = 
