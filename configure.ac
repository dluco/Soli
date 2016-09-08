dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.69)

m4_define([soli_major_version], [0])
m4_define([soli_minor_version], [0])
m4_define([soli_micro_version], [1])
m4_define([soli_version], [soli_major_version.soli_minor_version.soli_micro_version])

AC_INIT([Soli], [soli_version], [dluco11@gmail.com])

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

SOLI_CFLAGS="-Wall -g"
SOLI_LIBS=""

# Checks for library dependencies
PKG_CHECK_MODULES([SOLI], [
	gtk+-3.0 >= 2.90.3
	gtksourceview-3.0 >= 3.20.3
	libpeas-1.0 >= 1.14-1
	libpeas-gtk-1.0 >= 1.14-1
])

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
GOBJECT_INTROSPECTION_CHECK([1.38.0])

# Misc
AC_PATH_PROG(GLIB_COMPILE_RESOURCES, glib-compile-resources)

# Output
AC_SUBST(SOLI_CFLAGS)
AC_SUBST(SOLI_LIBS)

AC_CONFIG_FILES([
	Makefile
	src/Makefile
])

AC_OUTPUT
