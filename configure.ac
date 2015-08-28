dnl   configuration script for cadrolizer
dnl   Process this file with autoconf to produce configure.
dnl

define(CADROLIZER_CONFIGURE_COPYRIGHT, [[
Copyright (C) 2015 Artyom V. Poptsov <poptsov.artyom@gmail.com>

This file is part of Cadrolizer.

Cadroilizer is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

Cadrolizer is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with Cadrolizer.  If not, see <http://www.gnu.org/licenses/>.
]])

AC_INIT([cadrolizer], [0.0.0], [poptsov.artyom@gmail.com],
        [cadrolizer],
        [https://github.com/cadrspace/cadrolizer])

AC_COPYRIGHT(CADROLIZER_CONFIGURE_COPYRIGHT)

AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([color-tests])
m4_ifdef([AM_SILENT_RULES],
         [AM_SILENT_RULES([yes])],
         [AC_SUBST([AM_DEFAULT_VERBOSITY],1)])

AC_PROG_CXX

AC_ARG_WITH([iotivitydir],
        [AS_HELP_STRING([--with-iotivitydir],
                        [A path to IoTivity distribution])],
        [case "x$withval" in
                xyes|xno) iotivitydir="";;
                *)        iotivitydir="$withval";;
         esac],
        [iotivitydir=""])

AC_SUBST([iotivitydir])

AC_CONFIG_FILES([Makefile src/Makefile build-aux/Makefile])

# Generate a Makefile, based on the results.
AC_OUTPUT
