dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(soli, 0.1)

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AC_PROG_CC

LT_INIT

PKG_CHECK_MODULES(SOLI, [
	gtk+-3.0 >= 2.90.3
	gtksourceview-3.0 >= 3.20.3
])

AC_PATH_PROG(GLIB_COMPILE_RESOURCES, glib-compile-resources)

AC_OUTPUT([
Makefile
src/Makefile
])
