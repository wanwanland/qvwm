## Process this file with automake to produce Makefile.in

noinst_LIBRARIES = libimage.a

libimage_a_SOURCES = \
Image.cc Image.h PixmapImage.cc PixmapImage.h \
ExtraImage.cc ExtraImage.h AnimImage.cc AnimImage.h

DEFS = \
-DQVWMDIR=\"@QVWMDIR@\" -DIMGDIR=\"@IMGDIR@\" -DSNDDIR=\"@SNDDIR@\" \
-DPATH_RM=\"@PATH_RM@\" \
-DHAVE_CONFIG_H -I$(top_builddir) -I$(top_builddir)/src \
@DEFS@

EXTRA_DIST = 
