dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.69)

m4_define([soli_major_version], [0])
m4_define([soli_minor_version], [0])
m4_define([soli_micro_version], [1])
m4_define([soli_version], [soli_major_version.soli_minor_version.soli_micro_version])

AC_INIT([Soli], [soli_version], [dluco11@gmail.com])

# Dependencies
GLIB_REQUIRED=2.44
GTK_REQUIRED=3.4.0
GTKSOURCEVIEW_REQUIRED=3.19.4
LIBPEAS_REQUIRED=1.0.0
GSPELL_REQUIRED=0.2.5
PYGOBJECT_REQUIRED=3.0.0	

AC_CONFIG_AUX_DIR([build])
AC_CONFIG_MACRO_DIR([build/autotools])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11 -Wall -Werror foreign -Wno-portability])
AM_SILENT_RULES([yes])

# Checks for programs
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_SYS_LARGEFILE
PKG_PROG_PKG_CONFIG

# Checks for typedefs, structures, and compiler characteristics
AC_PROG_CC_STDC

AC_CHECK_LIBM
AC_SUBST(LIBM)

# Checks for library dependencies
PKG_CHECK_MODULES([SOLI], [
	glib-2.0 >= $GLIB_REQUIRED
	gio-2.0 >= $GLIB_REQUIRED
	gmodule-2.0 >= 2.0.0
	gtk+-3.0 >= $GTK_REQUIRED
	gtksourceview-3.0 >= $GTKSOURCEVIEW_REQUIRED
	libpeas-1.0 >= $LIBPEAS_REQUIRED
	libpeas-gtk-1.0 >= $LIBPEAS_REQUIRED
])

SOLI_CFLAGS="$SOLI_CFLAGS"
SOLI_LIBS="$SOLI_LIBS $LIBM"

AC_SUBST(SOLI_CFLAGS)
AC_SUBST(SOLI_LIBS)

# Initialize libtool
LT_PREREQ([2.2.6])
LT_INIT(disable-static)

# Versioning
SOLI_MAJOR_VERSION=soli_major_version
SOLI_MINOR_VERSION=soli_minor_version
SOLI_MICRO_VERSION=soli_micro_version
SOLI_VERSION=soli_version
AC_SUBST(SOLI_MAJOR_VERSION)
AC_SUBST(SOLI_MINOR_VERSION)
AC_SUBST(SOLI_MICRO_VERSION)
AC_SUBST(SOLI_VERSION)

# Introspection
GOBJECT_INTROSPECTION_CHECK([0.9.3])

if test "$found_introspection" = "yes"; then
	enable_introspection=yes
	AC_DEFINE([ENABLE_INTROSPECTION], [1], [Define to enable GObject Introspection])
else
	enable_introspection=no
fi

# Misc
GLIB_GSETTINGS

AC_PATH_PROG(GLIB_COMPILE_RESOURCES, glib-compile-resources)
AC_PATH_PROG(GLIB_MKENUMS, glib-mkenums)

AC_ARG_ENABLE([spell],
			  AS_HELP_STRING([--disable-spell], \
							 [Disable spell plugin (default: enabled)]),
			  [enable_spell=$enableval],
			  [enable_spell=yes])

if test "x$enable_spell" = "xyes" ; then

	PKG_CHECK_MODULES(GSPELL, gspell-1 >= $GSPELL_REQUIRED, \
					  have_gspell=yes, have_gspell=no)

	if test "x$have_gspell" = "xno"; then
		AC_MSG_ERROR([gspell library not found or too old. Use --disable-spell to build without spell plugin.])
	fi
fi

AM_CONDITIONAL(ENABLE_SPELL, test x"$enable_spell" = "xyes")

AC_ARG_ENABLE([python],
			  AS_HELP_STRING([--enable-python[=@<:@no/auto/yes@:>@]], \
							 [Build with python support]),
			  [enable_python=$enableval],
			  [enable_python="auto"])

if test "x$enable_python" = "xauto"; then
	PKG_CHECK_EXISTS([pygobject-3.0 >= $PYGOBJECT_REQUIRED],
					 [enable_python=yes],[enable_python=no])
	fi

	if test "x$enable_python" = "xyes"; then
		AM_PATH_PYTHON(3.2.3)
		PKG_CHECK_MODULES(PYTHON, [pygobject-3.0 >= $PYGOBJECT_REQUIRED])

		pyoverridesdir="\$(pyexecdir)/gi/overrides"
		AC_SUBST(pyoverridesdir)
	fi

AM_CONDITIONAL(ENABLE_PYTHON, test x"$enable_python" = "xyes")

PLUGIN_LIBTOOL_FLAGS="-module -avoid-version"
AC_SUBST(PLUGIN_LIBTOOL_FLAGS)

PLUGINS_LIB_DIR="$libdir/soli/plugins"
AC_SUBST(PLUGINS_LIB_DIR)

PLUGINS_DATA_DIR="$datadir/soli/plugins"
AC_SUBST(PLUGINS_DATA_DIR)

# Output
AC_CONFIG_FILES([
	Makefile
	src/Makefile
	data/Makefile
	plugins/Makefile
	plugins/bottom-padding/Makefile
	plugins/codecomment/Makefile
	plugins/highlight-selected/Makefile
	plugins/modelines/Makefile
	plugins/quickopen/Makefile
	plugins/restoresession/Makefile
	plugins/sort/Makefile
	plugins/spell/Makefile
	plugins/wordcompletion/Makefile
])

AC_OUTPUT
