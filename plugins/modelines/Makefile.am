## Process this file with automake to produce Makefile.in

plugin_module_LTLIBRARIES = libmodelines.la

libmodelines_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS)

libmodelines_la_LIBADD = \
	$(top_builddir)/src/libsoli.la \
	$(SOLI_LIBS)

libmodelines_la_CPPFLAGS = -I$(top_srcdir)/src

libmodelines_la_CFLAGS = \
	$(SOLI_CFLAGS) \
	$(WARN_CFLAGS) \
	$(DISABLE_DEPRECATED_CFLAGS)

libmodelines_la_SOURCES = \
	modeline-parser.c \
	modeline-parser.h \
	soli-modeline-plugin.c \
	soli-modeline-plugin.h

plugin_moduledir = $(PLUGINS_LIB_DIR)/modelines

plugin_module_DATA = \
	modelines.plugin

plugin_datadir = $(PLUGINS_DATA_DIR)/modelines

plugin_data_DATA = \
	language-mappings

EXTRA_DIST = \
	$(plugin_module_DATA) \
	$(plugin_data_DATA)
