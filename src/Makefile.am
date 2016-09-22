# Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-DPACKAGE_LIB_DIR=\""$(pkglibdir)"\" \
	-DDATADIR=\""$(datadir)"\" \
	-DLIBDIR=\""$(libdir)"\" \
	$(SOLI_CFLAGS) \
	$(NULL)

AM_CFLAGS = \
	$(SOLI_CFLAGS) \
	$(NULL)

bin_PROGRAMS = soli

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
	soli-commands.c \
	soli-dirs.h \
	soli-dirs.c \
	soli-settings.h \
	soli-plugins-engine.h \
	soli-plugins-engine.c \
	soli-tag-manager.h \
	soli-tag-manager.c \
	$(NULL)

nodist_soli_SOURCES = \
	soli-resources.c \
	soli-resources.h \
	$(NULL)

soli_LDADD = \
	$(SOLI_LIBS) \
	$(NULL)
	
UI_FILES = \
	soli-window.ui \
	soli-view-frame.ui \
	menu-bar.ui \
	$(NULL)

BUILT_SOURCES = \
	soli-resources.c \
	soli-resources.h \
	$(NULL)

CLEANFILES = \
	$(BUILT_SOURCES) \
	$(NULL)

EXTRA_DIST = \
	$(UI_FILES) \
	$(NULL)

#lib_LTLIBRARIES += libsoli-1.0.la
#libsoli_1_0_la_LIBADD = $(SOLI_LIBS)
#libsoli_1_0_la_SOURCES = $(soli_SOURCES)

soli_resource_deps = $(shell $(GLIB_COMPILE_RESOURCES) --generate-dependencies --sourcedir $(srcdir)/resources --sourcedir $(builddir)/resources $(builddir)/resources/soli.gresource.xml)

soli_resource_c_deps = $(builddir)/resources/soli.gresource.xml $(soli_resource_deps)

soli-resources.c: $(soli_resource_c_deps)
	$(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir)/resources --sourcedir=$(builddir)/resources --generate-source --c-name soli $(srcdir)/resources/soli.gresource.xml

soli-resources.h: $(soli_resource_c_deps)
	$(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir)/resources --sourcedir=$(builddir)/resources --generate-header --c-name soli $(srcdir)/resources/soli.gresource.xml

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS = Soli-1.0.gir
INTROSPECTION_SCANNER_ARGS = --add-include-path=$(srcdir) --warn-all --accept-unprefixed --identifier-prefix=Soli --verbose
#INTROSPECTION_COMPILER_ARGS = --include-dir=$(srcdir)

if HAVE_INTROSPECTION
introspection_sources = $(soli_SOURCES)

Soli_1_0_gir_NAMESPACE = Soli
Soli_1_0_gir_VERSION = 1.0
Soli_1_0_gir_PROGRAM = $(builddir)/soli

Soli-1.0.gir: soli
Soli_1_0_gir_INCLUDES = Gtk-3.0 GtkSource-3.0
#Soli_1_0_gir_CFLAGS = $(AM_CPPFLAGS)
#Soli_1_0_gir_LIBS = libsoli-1.0.la
#Soli_1_0_gir_FILES = $(introspection_sources)
Soli_1_0_gir_FILES = \
	soli-view.h \
	soli-view.c \
	soli-view-activatable.h \
	soli-view-activatable.c \
	soli-window.h \
	soli-window.c \
	soli-window-activatable.h \
	soli-window-activatable.c \
	soli-tab.h \
	soli-tab.c \
	soli-document.h \
	soli-document.c

girdir = $(datadir)/soli/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/soli/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelib_DATA)
endif
