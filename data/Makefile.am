## Process this file with automake to produce Makefile.in

gsettings_SCHEMAS = ca.dluco.soli.gschema.xml

gsettings_ENUM_NAMESPACE = ca.dluco.soli
gsettings_ENUM_FILES = $(top_srcdir)/src/*.h

##gsettings_ENUMS = ca.dluco.soli.enums.xml

##ca.dluco.soli.enums.xml: $(top_srcdir)/src/*.h

EXTRA_DIST = $(gsettings_SCHEMAS)

##CLEANFILES = $(gsettings_ENUMS)

@GSETTINGS_RULES@
