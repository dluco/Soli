## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I build/autotools

DISTCHECK_CONFIGURE_FLAGS = --enable-introspection

SUBDIRS = src data plugins

dist_doc_DATA = \
	README \
	COPYING \
	AUTHORS \
	ChangeLog \
	INSTALL \
	NEWS

# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(docdir)
