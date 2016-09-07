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
	soli-app-activatable.h \
	soli-app-activatable.c \
	soli-window.h \
	soli-window.c \
	soli-window-activatable.h \
	soli-window-activatable.c \
	soli-notebook.h \
	soli-notebook.c \
	soli-tab.h \
	soli-tab.c \
	soli-view.h \
	soli-view.c \
	soli-view-activatable.h \
	soli-view-activatable.c \
	soli-document.h \
	soli-document.c \
	soli-view-frame.h \
	soli-view-frame.c \
	soli-commands.h \
	soli-commands.c

soli_LDFLAGS = \
	-Wl,--export-dynamic

soli_LDADD = $(SOLI_LIBS)

soli_resource_deps = $(shell $(GLIB_COMPILE_RESOURCES) --generate-dependencies --sourcedir $(srcdir)/resources --sourcedir $(builddir)/resources $(builddir)/resources/soli.gresource.xml)

soli_resource_c_deps = $(builddir)/resources/soli.gresource.xml $(soli_resource_deps)

soli-resources.c: $(soli_resource_c_deps)
	$(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir)/resources --sourcedir=$(builddir)/resources --generate-source --c-name soli $(srcdir)/resources/soli.gresource.xml

soli-resources.h: $(soli_resource_c_deps)
	$(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir)/resources --sourcedir=$(builddir)/resources --generate-header --c-name soli $(srcdir)/resources/soli.gresource.xml

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
