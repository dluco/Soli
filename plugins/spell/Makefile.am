## Process this file with automake to produce Makefile.in

if ENABLE_SPELL

plugin_module_LTLIBRARIES = libspell.la

libspell_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS)

libspell_la_LIBADD  =				\
	$(top_builddir)/src/libsoli.la	\
	$(SOLI_LIBS)					\
	$(GSPELL_LIBS)

libspell_la_CPPFLAGS = -I$(top_srcdir)/src

libspell_la_CFLAGS =				\
	$(SOLI_CFLAGS) 					\
	$(GSPELL_CFLAGS)				\
	$(WARN_CFLAGS)					\
	$(DISABLE_DEPRECATED_CFLAGS)

libspell_la_SOURCES =				\
	soli-spell-plugin.c 			\
	soli-spell-plugin.h 			\
	soli-spell-app-activatable.c	\
	soli-spell-app-activatable.h

plugin_moduledir = $(PLUGINS_LIB_DIR)/spell

plugin_module_DATA = \
	spell.plugin

EXTRA_DIST = \
	$(plugin_module_DATA)

endif
