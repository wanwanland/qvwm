# Makefile.in generated automatically by automake 1.4 from Makefile.am

# Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.


SHELL = /bin/sh

srcdir = .
top_srcdir = ../..

prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/qvwm
pkglibdir = $(libdir)/qvwm
pkgincludedir = $(includedir)/qvwm

top_builddir = ../..

ACLOCAL = aclocal
AUTOCONF = autoconf
AUTOMAKE = automake
AUTOHEADER = autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL} $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
host_alias = 
host_triplet = @host@
AS = @AS@
CC = gcc
CPP = gcc -E
CXX = g++
DLLTOOL = @DLLTOOL@
ESD_CFLAGS = 
ESD_CONFIG = no
ESD_LIBS = 
GDK_IMLIB_CFLAGS = @GDK_IMLIB_CFLAGS@
GDK_IMLIB_LIBS = @GDK_IMLIB_LIBS@
IMGDIR = /usr/local/share/qvwm/images
IMLIB_CFLAGS = 
IMLIB_CONFIG = no
IMLIB_LIBS = 
LEX = flex
LIBTOOL = @LIBTOOL@
LN_S = ln -s
MAINT = #
MAKEINFO = makeinfo
OBJDUMP = @OBJDUMP@
PACKAGE = qvwm
PATH_RM = /bin/rm
QVWMDIR = /usr/local/share/qvwm
RANLIB = ranlib
SNDDIR = /usr/local/share/qvwm/sounds
VERSION = 1.2.0-pre
YACC = bison -y

EXTRA_DIST =  logo_qvwm11.xpm button_min.xpm button_max.xpm button_close.xpm button_restore.xpm cursor_arrow.xbm csmask_arrow.xbm cursor_size_h.xbm csmask_size_h.xbm cursor_size_v.xbm csmask_size_v.xbm cursor_size_l.xbm csmask_size_l.xbm cursor_size_r.xbm csmask_size_r.xbm cursor_busy.xbm csmask_busy.xbm cursor_move.xbm csmask_move.xbm cursor_wait.xbm csmask_wait.xbm cursor_no.xbm csmask_no.xbm folder16.xpm folder32.xpm icon16.xpm icon32.xpm logo16.xpm logo32.xpm next_black.xpm next_white.xpm check_black.xpm check_white.xpm selector_black.xpm selector_white.xpm radiobtn_normal.xpm radiobtn_push.xpm radiobtn_select.xpm tile0.xpm

mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = ../../config.h
CONFIG_CLEAN_FILES = 
DIST_COMMON =  Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = gtar
GZIP_ENV = --best
all: all-redirect
.SUFFIXES:
$(srcdir)/Makefile.in: # Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnu src/bitmaps/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status $(BUILT_SOURCES)
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status

tags: TAGS
TAGS:


distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)

subdir = src/bitmaps

distdir: $(DISTFILES)
	here=`cd $(top_builddir) && pwd`; \
	top_distdir=`cd $(top_distdir) && pwd`; \
	distdir=`cd $(distdir) && pwd`; \
	cd $(top_srcdir) \
	  && $(AUTOMAKE) --include-deps --build-dir=$$here --srcdir-name=$(top_srcdir) --output-dir=$$top_distdir --gnu src/bitmaps/Makefile
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$d/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done
info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
check: check-am
installcheck-am:
installcheck: installcheck-am
install-exec-am:
install-exec: install-exec-am

install-data-am:
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am:
uninstall: uninstall-am
all-am: Makefile
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-generic mostlyclean-am

clean: clean-am

distclean-am:  distclean-generic clean-am

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: tags distdir info-am info dvi-am dvi check check-am \
installcheck-am installcheck install-exec-am install-exec \
install-data-am install-data install-am install uninstall-am uninstall \
all-redirect all-am all installdirs mostlyclean-generic \
distclean-generic clean-generic maintainer-clean-generic clean \
mostlyclean distclean maintainer-clean


# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
