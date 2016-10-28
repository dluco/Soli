## Process this file with automake to produce Makefile.in

plugin_module_LTLIBRARIES = libwordcompletion.la

libwordcompletion_la_CPPFLAGS = -I$(top_srcdir)/src

libwordcompletion_la_CFLAGS =		\
	$(SOLI_CFLAGS) 					\
	$(WARN_CFLAGS)					\
	$(DISABLE_DEPRECATED_CFLAGS)

libwordcompletion_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS)

libwordcompletion_la_LIBADD =		\
	$(top_builddir)/src/libsoli.la	\
	$(SOLI_LIBS)

BUILT_SOURCES =						\
	soli-word-completion-resources.c

libwordcompletion_la_SOURCES =		\
	soli-word-completion-plugin.h	\
	soli-word-completion-plugin.c	\
	$(BUILT_SOURCES)

wordcompletion_resource_deps =								\
	$(srcdir)/resources/soli-word-completion.gresource.xml	\
	$(shell $(GLIB_COMPILE_RESOURCES) 						\
		--generate-dependencies								\
		--sourcedir=$(srcdir)/resources						\
		$(srcdir)/resources/soli-word-completion.gresource.xml)

soli-word-completion-resources.c: $(wordcompletion_resource_deps)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES)	\
		--target=$@							\
		--sourcedir=$(srcdir)/resources		\
		--generate-source $(srcdir)/resources/soli-word-completion.gresource.xml

plugin_moduledir = $(PLUGINS_LIB_DIR)/sort

plugin_module_DATA = \
	wordcompletion.plugin

gsettings_SCHEMAS = ca.dluco.soli.plugins.wordcompletion.gschema.xml

EXTRA_DIST =				\
	$(plugin_module_DATA)	\
	$(gsettings_SCHEMAS)

CLEANFILES = \
	$(BUILT_SOURCES)

@GSETTINGS_RULES@
