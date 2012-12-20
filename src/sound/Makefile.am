## Process this file with automake to produce Makefile.in

noinst_LIBRARIES = libsound.a

libsound_a_SOURCES = \
Audio.cc Audio.h \
AudioWav.cc AudioWav.h AudioAu.cc AudioAu.h \
AudioDev.cc AudioDev.h \
AudioDevAlsa.cc AudioDevAlsa.h AudioDevEsd.cc AudioDevEsd.h \
AudioDevOss.cc AudioDevOss.h AudioDevSun.cc AudioDevSun.h

DEFS = \
-DQVWMDIR=\"@QVWMDIR@\" -DIMGDIR=\"@IMGDIR@\" -DSNDDIR=\"@SNDDIR@\" \
-DPATH_RM=\"@PATH_RM@\" \
-DHAVE_CONFIG_H -I$(top_builddir) -I$(top_builddir)/src \
@DEFS@

EXTRA_DIST = 
