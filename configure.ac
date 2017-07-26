#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)

# if not 1, append datestamp to the version number.
m4_define([gtkfw_released], [0])
m4_define([gtkfw_major_version], [0])
m4_define([gtkfw_minor_version], [1])
m4_define([gtkfw_micro_version], [0])
m4_define([gtkfw_interface_age], [0])
m4_define([gtkfw_binary_age], [m4_eval(100 * gtkfw_minor_version + gtkfw_micro_version)])

m4_define(gtkfw_maybe_datestamp, m4_esyscmd([if test x]gtkfw_released[ != x1; then date +.%Y%m%d | tr -d '\n\r'; fi]))
m4_define([gtkfw_version], gtkfw_major_version.gtkfw_minor_version.gtkfw_micro_version[]gtkfw_maybe_datestamp)

m4_define([gtkfw_lt_current], [m4_eval(100 * gtkfw_minor_version + gtkfw_micro_version - gtkfw_interface_age)])
m4_define([gtkfw_lt_revision], [gtkfw_interface_age])
m4_define([gtkfw_lt_age], [m4_eval(gtkfw_binary_age - gtkfw_interface_age)])

AC_INIT(gtkfw, [gtkfw_version], http://code.google.com/p/libgtkfw/)

AC_CONFIG_HEADERS(config.h)
AC_CONFIG_SRCDIR(gfw/gfw-window.c)

AM_INIT_AUTOMAKE([1.9 dist-bzip2 no-dist-gzip -Wno-portability])
AM_MAINTAINER_MODE
AM_SILENT_RULES([yes])

LT_VERSION_INFO="gtkfw_lt_current:gtkfw_lt_revision:gtkfw_lt_age"
LT_CURRENT_MINUS_AGE=m4_eval(gtkfw_lt_current - gtkfw_lt_age)
AC_SUBST(LT_VERSION_INFO)
AC_SUBST(LT_CURRENT_MINUS_AGE)

GTKFW_MAJOR_VERSION=gtkfw_major_version
GTKFW_MINOR_VERSION=gtkfw_minor_version
GTKFW_MICRO_VERSION=gtkfw_micro_version
AC_SUBST(GTKFW_MAJOR_VERSION)
AC_SUBST(GTKFW_MINOR_VERSION)
AC_SUBST(GTKFW_MICRO_VERSION)

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
PKG_PROG_PKG_CONFIG

# Initialize libtool
AC_PROG_LIBTOOL

dnl ================================================================
dnl  Check for gtk
dnl ================================================================
AC_ARG_ENABLE([gtk3],
              AS_HELP_STRING([--disable-gtk3],[Build for gtk2 version. (default: enabled, for gtk3)]),
              [enable_gtk3=$enableval],
              [enable_gtk3=yes])
if test "x$enable_gtk3" != "xno"; then
       PKG_CHECK_MODULES(GTK, [ gtk+-3.0 >= 3.12 ])
else
       PKG_CHECK_MODULES(GTK, [ gtk+-2.0 >= 2.24 ])
fi
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

AC_CONFIG_FILES([
gtkfw-1.0.pc
gfw/Makefile
tests/Makefile
example/Makefile
Makefile
])

AC_OUTPUT
echo "
type \"make\" to build now.
"
