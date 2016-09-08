# Process this file with automake to produce Makefile.in

#NULL =
AM_CPPFLAGS =
AM_CFLAGS =
BUILT_SOURCES =
CLEANFILES =
EXTRA_DIST =
DISTCLEANFILES =
lib_LTLIBRARIES =
bin_PROGRAMS =

bin_PROGRAMS += soli

AM_CPPFLAGS += \
	$(SOLI_CFLAGS) \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-DDATADIR=\""$(datadir)"\" \
	-DLIBDIR=\""$(libdir)"\" \
	$(NULL)

AM_CFLAGS += \
	$(SOLI_CFLAGS) \
	$(NULL)
	 
#soli_LDFLAGS = \
#	-Wl,--export-dynamic

soli_LDADD = $(SOLI_LIBS)

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
	$(NULL)
	
UI_FILES = \
	soli-window.ui \
	soli-view-frame.ui \
	menu-bar.ui \
	$(NULL)

BUILT_SOURCES += \
	soli-resources.c \
	soli-resources.h \
	$(NULL)

CLEANFILES += \
	$(BUILT_SOURCES) \
	$(NULL)

EXTRA_DIST += \
	$(UI_FILES) \
	$(NULL)

lib_LTLIBRARIES += libsoli-1.0.la
libsoli_1_0_la_LIBADD = $(SOLI_LIBS)
libsoli_1_0_la_SOURCES = $(soli_SOURCES)

soli_resource_deps = $(shell $(GLIB_COMPILE_RESOURCES) --generate-dependencies --sourcedir $(srcdir)/resources --sourcedir $(builddir)/resources $(builddir)/resources/soli.gresource.xml)

soli_resource_c_deps = $(builddir)/resources/soli.gresource.xml $(soli_resource_deps)

soli-resources.c: $(soli_resource_c_deps)
	$(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir)/resources --sourcedir=$(builddir)/resources --generate-source --c-name soli $(srcdir)/resources/soli.gresource.xml

soli-resources.h: $(soli_resource_c_deps)
	$(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir)/resources --sourcedir=$(builddir)/resources --generate-header --c-name soli $(srcdir)/resources/soli.gresource.xml

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = --add-include-path=$(srcdir) --warn-all --verbose
INTROSPECTION_COMPILER_ARGS = --include-dir=$(srcdir)

if HAVE_INTROSPECTION
introspection_sources = $(libsoli_1_0_la_SOURCES)

Soli_1_0_gir_NAMESPACE = Soli
Soli_1_0_gir_VERSION = 1.0
Soli_1_0_gir_PROGRAM = $(builddir)/soli

Soli-1.0.gir: libsoli-1.0.la
Soli_1_0_gir_INCLUDES = GObject-2.0
Soli_1_0_gir_CFLAGS = $(AM_CPPFLAGS)
Soli_1_0_gir_LIBS = libsoli-1.0.la
Soli_1_0_gir_FILES = $(introspection_sources)

INTROSPECTION_GIRS += Soli-1.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelib_DATA)
endif
