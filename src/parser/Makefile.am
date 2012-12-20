## Process this file with automake to produce Makefile.in

noinst_LIBRARIES = libparser.a

libparser_a_SOURCES = \
parse.cc parse.h yaccsrc.yy yaccsrc.h lexsrc.ll \
qvwmrc.cc qvwmrc.h

DEFS = \
-DQVWMDIR=\"@QVWMDIR@\" -DIMGDIR=\"@IMGDIR@\" -DSNDDIR=\"@SNDDIR@\" \
-DPATH_RM=\"@PATH_RM@\" \
-DHAVE_CONFIG_H -I$(top_builddir) -I$(top_builddir)/src \
@DEFS@

YFLAGS = -d

EXTRA_DIST = 
