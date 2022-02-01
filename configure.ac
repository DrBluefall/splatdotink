dnl Initialize autoconf and automake.

AC_INIT([splatdotink], [0.0.1])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects silent-rules])

dnl Initialize libtool.

AM_PROG_AR dnl because automake complained at me. - Dr. Bluefall
LT_INIT
LT_PREREQ([2.4.6])
AC_CONFIG_MACRO_DIRS([m4])
AC_SUBST([LIBTOOL_DEPS])

AC_COPYRIGHT([
splatdotink is free software: you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

splatdotink is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with splatdotink. If not, see <https://www.gnu.org/licenses/>.
])


PACKAGE_DESCRIPTION="A client for splatoon2.ink."
AC_SUBST([PACKAGE_DESCRIPTION])

AC_ARG_ENABLE([debug],
                [AS_HELP_STRING([--enable-debug],
                    [enable various debugging lines in the library. (defaults to no)])],
                [AC_DEFINE([SDI_DEBUG_BUILD])])


dnl Define the directory for pkg-config.
PKG_INSTALLDIR

dnl Make sure the source directory is in the right place.
AC_CONFIG_SRCDIR([src/lib.cpp])

dnl Verify the C++ compiler.
AC_PROG_CXX

dnl Check for restclient-cpp
AX_PKG_CHECK_MODULES([CPR],[cpr],[],[],[AC_MSG_ERROR([cannot find library 'cpr'. Install it from https://github.com/libcpr/cpr or your system package manager.])])

dnl Check for nlohmann::json
AX_PKG_CHECK_MODULES([NHJSON],[nlohmann_json],[],[],[AC_MSG_ERROR([cannot find library 'nlohmann_json'. Install it from https://github.com/nlohmann/json or your system package manager.])])

AC_CONFIG_FILES([Makefile
                 splatdotink.pc:infiles/splatdotink.pc.in])


AC_OUTPUT
