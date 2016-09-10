## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I build/autotools

DISTCHECK_CONFIGURE_FLAGS = --enable-introspection

SUBDIRS = data src 

dist_doc_DATA = \
	README \
	COPYING \
	AUTHORS \
	ChangeLog \
	INSTALL \
	NEWS

plugindir = $(pkglibdir)/plugins
plugin_DATA = \
	plugins/demo-plugin.plugin \
	plugins/demo-plugin.py

EXTRA_DIST = \
	$(plugin_DATA)

# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(docdir)
