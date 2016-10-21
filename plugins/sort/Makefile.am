## Process this file with automake to produce Makefile.in

plugin_module_LTLIBRARIES = libsort.la

libsort_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS)

libsort_la_LIBADD =					\
	$(top_builddir)/src/libsoli.la	\
	$(SOLI_LIBS)

libsort_la_CPPFLAGS = -I$(top_srcdir)/src

libsort_la_CFLAGS =				\
	$(SOLI_CFLAGS) 				\
	$(WARN_CFLAGS)				\
	$(DISABLE_DEPRECATED_CFLAGS)

BUILT_SOURCES = \
	soli-sort-resources.c

libsort_la_SOURCES =		\
	soli-sort-plugin.h		\
	soli-sort-plugin.c		\
	soli-sort-resources.c

sort_resource_deps = $(shell $(GLIB_COMPILE_RESOURCES) --generate-dependencies --sourcedir $(srcdir)/resources $(srcdir)/resources/soli-sort.gresource.xml)

soli-sort-resources.c: $(sort_resource_deps)
	$(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir)/resources --generate-source $(srcdir)/resources/soli-sort.gresource.xml

plugin_moduledir = $(PLUGINS_LIB_DIR)/sort

plugin_module_DATA = \
	sort.plugin

EXTRA_DIST = \
	$(plugin_module_DATA)

CLEANFILES = \
	$(BUILT_SOURCES)
