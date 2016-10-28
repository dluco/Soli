## Process this file with automake to produce Makefile.in

plugin_moduledir = $(PLUGINS_LIB_DIR)/restoresession

plugin_module_PYTHON = \
	restoresession.py

plugin_module_DATA = \
	restoresession.plugin

gsettings_SCHEMAS = ca.dluco.soli.plugins.restoresession.gschema.xml

EXTRA_DIST =				\
	$(plugin_module_DATA)	\
	$(gsettings_SCHEMAS)

@GSETTINGS_RULES@
