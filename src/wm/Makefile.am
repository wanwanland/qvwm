## Process this file with automake to produce Makefile.in

noinst_LIBRARIES = libwm.a

libwm_a_SOURCES = \
Event.cc Event.h Function.cc Function.h \
mwm.cc mwm.h gnome.cc gnome.h session.cc session.h \
RemoteCommand.cc RemoteCommand.h

DEFS = \
-DQVWMDIR=\"@QVWMDIR@\" -DIMGDIR=\"@IMGDIR@\" -DSNDDIR=\"@SNDDIR@\" \
-DPATH_RM=\"@PATH_RM@\" \
-DHAVE_CONFIG_H -I$(top_builddir) -I$(top_builddir)/src \
@DEFS@

EXTRA_DIST = 
