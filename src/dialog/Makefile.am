## Process this file with automake to produce Makefile.in

noinst_LIBRARIES = libdialog.a

libdialog_a_SOURCES = \
dialog.cc dialog.h system_dialog.cc system_dialog.h \
radio_button.cc radio_button.h radio_set.cc radio_set.h resource.h \
exit_dialog.cc exit_dialog.h confirm_dialog.cc confirm_dialog.h

DEFS = \
-DQVWMDIR=\"@QVWMDIR@\" -DIMGDIR=\"@IMGDIR@\" -DSNDDIR=\"@SNDDIR@\" \
-DPATH_RM=\"@PATH_RM@\" \
-DHAVE_CONFIG_H -I$(top_builddir) -I$(top_builddir)/src \
@DEFS@

EXTRA_DIST = 
