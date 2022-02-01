#### Set splatdotink up to be built with the autotools.

####################################################################################
# This file is part of splatdotink.												   #
# 																				   #
# splatdotink is free software: you can redistribute it and/or modify it under the #
# terms of the GNU Lesser General Public License as published by the Free Software #
# Foundation, either version 3 of the License, or (at your option) any later	   #
# version.																		   #
# 																				   #
# splatdotink is distributed in the hope that it will be useful, but WITHOUT ANY   #
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A  #
# PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.  #
# 																				   #
# You should have received a copy of the GNU Lesser General Public License along   #
# with splatdotink. If not, see <https://www.gnu.org/licenses/>.				   #
####################################################################################

# Partially lifted from https://www.eyrie.org/~eagle/notes/style/build.html
MAINTAINERCLEANFILES = Makefile.in aclocal.m4 build-aux/compile \
	build-aux/config.guess build-aux/config.sub build-aux/depcomp \
	build-aux/install-sh build-aux/ltmain.sh build-aux/missing \
	config.h.in config.h.in~ configure m4/libtool.m4 m4/ltoptions.m4 \
	m4/ltsugar.m4 m4/ltversion.m4 m4/lt~obsolete.m4 configure~ ltmain.sh \
	build-aux/ar-lib build-aux/test-driver

AM_CXXFLAGS = -Wall -std=c++20
ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libsplatdotink.la

pkginclude_HEADERS = src/include/splatdotink.hpp
libsplatdotink_la_SOURCES = src/lib.cpp
libsplatdotink_la_CPPFLAGS = -I$(top_srcdir)/src/include @CPR_CFLAGS@ @NHJSON_CFLAGS@
libsplatdotink_la_LDFLAGS = @CPR_LIBS@ -release @PACKAGE_VERSION@
pkgconfig_DATA = splatdotink.pc



LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status libtool


##################
# TEST FRAMEWORK #
##################

TESTS=test/sanity

check_PROGRAMS = test/sanity
test_sanity_SOURCES = test/sanity.cpp
test_sanity_CPPFLAGS = -I$(top_srcdir)/src/include
test_sanity_LDADD = libsplatdotink.la
