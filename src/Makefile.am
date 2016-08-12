## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(SOLI_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = soli

UI_FILES = \
	window.ui \
	menu-bar.ui \
	$(NULL)

soli_SOURCES = \
	main.c \
	soli-app.h \
	soli-app.c \
	soli-window.c \
	soli-window.h \
	$(NULL)

soli_LDFLAGS = \
	-Wl,--export-dynamic

soli_LDADD = $(SOLI_LIBS)

soli-resources.c: soli.gresource.xml $(UI_FILES)
	glib-compile-resources --target=$@ --sourcedir=$(srcdir) --generate-source --c-name soli $(srcdir)/soli.gresource.xml

soli-resources.h: soli.gresource.xml
	glib-compile-resources --target=$@ --sourcedir=$(srcdir) --generate-header --c-name soli $(srcdir)/soli.gresource.xml

BUILT_SOURCES = \
	soli-resources.c \
	soli-resources.h \
	$(NULL)

nodist_soli_SOURCES = \
	$(BUILT_SOURCES) \
	$(NULL)

CLEANFILES = \
	$(BUILT_SOURCES) \
	$(NULL)

EXTRA_DIST = \
	$(UI_FILES) \
	$(NULL)

dist-hook:
	cd $(distdir); rm -f $(CLEANFILES)
