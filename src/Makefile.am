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
	soli-window.ui \
	soli-view-frame.ui \
	menu-bar.ui

soli_SOURCES = \
	main.c \
	soli-app.h \
	soli-app.c \
	soli-window.h \
	soli-window.c \
	soli-notebook.h \
	soli-notebook.c \
	soli-tab.c \
	soli-tab.h \
	soli-view.c \
	soli-view.h \
	soli-document.c \
	soli-document.h \
	soli-view-frame.c \
	soli-view-frame.h

soli_LDFLAGS = \
	-Wl,--export-dynamic

soli_LDADD = $(SOLI_LIBS)

soli-resources.c: soli.gresource.xml $(UI_FILES)
	glib-compile-resources --target=$@ --sourcedir=$(srcdir) --generate-source --c-name soli $(srcdir)/soli.gresource.xml

soli-resources.h: soli.gresource.xml
	glib-compile-resources --target=$@ --sourcedir=$(srcdir) --generate-header --c-name soli $(srcdir)/soli.gresource.xml

BUILT_SOURCES = \
	soli-resources.c \
	soli-resources.h

nodist_soli_SOURCES = \
	$(BUILT_SOURCES)

CLEANFILES = \
	$(BUILT_SOURCES)

EXTRA_DIST = \
	$(UI_FILES)

dist-hook:
	cd $(distdir); rm -f $(CLEANFILES)
