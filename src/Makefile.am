# Process this file with automake to produce Makefile.in

bin_PROGRAMS = soli
lib_LTLIBRARIES = libsoli.la

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

soli_LDADD = \
	libsoli.la \
	$(SOLI_LIBS) \
	$(NULL)

soli_SOURCES = \
	soli.c \
	$(NULL)

libsoli_la_LIBADD = \
	$(SOLI_LIBS) \
	$(NULL)

built_sources = \
	soli-enum-types.c \
	soli-enum-types.h \
	$(NULL)

libsoli_la_INST_H_FILES = \
	soli-app-activatable.h \
	soli-app.h \
	soli-commands.h \
	soli-debug.h \
	soli-document.h \
	soli-encodings-combo-box.h\
	soli-menu-extension.h \
	soli-message-bus.h \
	soli-message.h \
	soli-progress-info-bar.h \
	soli-statusbar.h \
	soli-tab.h \
	soli-utils.h \
	soli-view-activatable.h \
	soli-view.h \
	soli-window-activatable.h \
	soli-window.h \
	$(NULL)

libsoli_la_NOINST_H_FILES = \
	soli-app-private.h \
	soli-app-x11.h \
	soli-close-confirmation-dialog.h \
	soli-commands-private.h \
	soli-dirs.h \
	soli-document-private.h \
	soli-documents-panel.h \
	soli-encoding-items.h \
	soli-encodings-dialog.h \
	soli-file-chooser-dialog-gtk.h \
	soli-file-chooser-dialog.h \
	soli-highlight-mode-dialog.h \
	soli-highlight-mode-selector.h \
	soli-history-entry.h \
	soli-io-error-info-bar.h \
	soli-menu-stack-switcher.h \
	soli-metadata-manager.h \
	soli-multi-notebook.h \
	soli-notebook-popup-menu.h \
	soli-notebook-stack-switcher.h \
	soli-notebook.h \
	soli-open-document-selector-helper.h \
	soli-open-document-selector-store.h \
	soli-open-document-selector.h \
	soli-plugins-engine.h \
	soli-preferences-dialog.h \
	soli-print-job.h \
	soli-print-preview.h \
	soli-recent.h \
	soli-replace-dialog.h \
	soli-settings.h \
	soli-status-menu-button.h \
	soli-tab-label.h \
	soli-tab-private.h \
	soli-view-centering.h \
	soli-view-frame.h \
	soli-window-private.h \
	$(NULL)

libsoli_la_sources_c = \
	soli-app-activatable.c \
	soli-app.c \
	soli-app-x11.c \
	soli-close-confirmation-dialog.c \
	soli-commands-documents.c \
	soli-commands-edit.c \
	soli-commands-file-print.c \
	soli-commands-file.c \
	soli-commands-help.c \
	soli-commands-search.c \
	soli-commands-view.c \
	soli-debug.c \
	soli-dirs.c \
	soli-document.c \
	soli-documents-panel.c \
	soli-encoding-items.c \
	soli-encodings-combo-box.c \
	soli-encodings-dialog.c \
	soli-file-chooser-dialog-gtk.c \
	soli-file-chooser-dialog.c \
	soli-highlight-mode-dialog.c \
	soli-highlight-mode-selector.c \
	soli-history-entry.c \
	soli-io-error-info-bar.c \
	soli-menu-extension.c \
	soli-menu-stack-switcher.c \
	soli-message-bus.c \
	soli-message.c \
	soli-metadata-manager.c \
	soli-multi-notebook.c \
	soli-notebook-popup-menu.c \
	soli-notebook-stack-switcher.c \
	soli-notebook.c \
	soli-open-document-selector-helper.c \
	soli-open-document-selector-store.c \
	soli-open-document-selector.c \
	soli-plugins-engine.c \
	soli-preferences-dialog.c \
	soli-print-job.c \
	soli-print-preview.c \
	soli-progress-info-bar.c \
	soli-recent.c \
	soli-replace-dialog.c \
	soli-resources.c \
	soli-settings.c \
	soli-status-menu-button.c \
	soli-statusbar.c \
	soli-tab-label.c \
	soli-tab.c \
	soli-utils.c \
	soli-view-activatable.c \
	soli-view-centering.c \
	soli-view-frame.c \
	soli-view.c \
	soli-window-activatable.c \
	soli-window.c \
	$(NULL)

libsoli_la_SOURCES = \
	$(built_sources) \
	$(libsoli_la_sources_c) \
	$(libsoli_la_NOINST_H_FILES) \
	$(libsoli_la_INST_H_FILES) \
	$(NULL)

BUILT_SOURCES = \
	soli-resources.c \
	$(built_sources) \
	$(NULL)

include_HEADERS = \
	$(libsoli_la_INST_H_FILES) \
	$(NULL)

ENUM_TYPES = \
	soli-notebook.h \
	$(libsoli_la_INST_H_FILES) \
	$(NULL)

soli-enum-types.h: soli-enum-types.h.template $(ENUM_TYPES) $(GLIB_MKENUMS)
	@echo SOLI_LIBS = $(SOLI_LIBS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template soli-enum-types.h.template $(ENUM_TYPES)) > $@

soli-enum-types.c: soli-enum-types.c.template $(ENUM_TYPES) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template soli-enum-types.c.template $(ENUM_TYPES)) > $@

soli_resource_deps = $(shell $(GLIB_COMPILE_RESOURCES) --generate-dependencies --sourcedir $(srcdir)/resources --sourcedir $(builddir)/resources $(builddir)/resources/soli.gresource.xml)

soli_resource_c_deps = $(builddir)/resources/soli.gresource.xml $(soli_resource_deps)

soli-resources.c: $(soli_resource_c_deps)
	$(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir)/resources --sourcedir=$(builddir)/resources --generate-source --c-name soli $(srcdir)/resources/soli.gresource.xml

soli-resources.h: $(soli_resource_c_deps)
	$(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir)/resources --sourcedir=$(builddir)/resources --generate-header --c-name soli $(srcdir)/resources/soli.gresource.xml

EXTRA_DIST = \
	soli-enum-types.h.template \
	soli-enum-types.c.template \
	$(NULL)

CLEANFILES = \
	$(BUILT_SOURCES) \
	$(NULL)

if HAVE_INTROSPECTION
-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS = Soli-1.0.gir
INTROSPECTION_SCANNER_ARGS = --add-include-path=$(srcdir) --warn-all --accept-unprefixed --identifier-prefix=Soli --verbose
#INTROSPECTION_COMPILER_ARGS = --include-dir=$(srcdir)

introspection_sources = $(soli_SOURCES)

Soli_1_0_gir_NAMESPACE = Soli
Soli_1_0_gir_VERSION = 1.0
Soli_1_0_gir_PROGRAM = $(builddir)/soli$(EXEEXT)

Soli-1.0.gir: soli
Soli_1_0_gir_INCLUDES = Gtk-3.0 GtkSource-3.0
Soli_1_0_gir_FILES = \
	$(libsoli_la_INST_H_FILES) \
	$(libsoli_la_sources_c) \
	$(built_sources) \
	$(NULL)

girdir = $(datadir)/soli/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/soli/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelib_DATA)
endif
