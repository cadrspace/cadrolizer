## Copyright (C) 2015 Artyom V. Poptsov <poptsov.artyom@gmail.com>
##
## This file is part of cadrolizer.
##
## Cadrolizer is free software: you can redistribute it and/or
## modify it under the terms of the GNU General Public License as
## published by the Free Software Foundation, either version 3 of the
## License, or (at your option) any later version.
##
## Cadrolizer is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with cadroizer.  If not, see <http://www.gnu.org/licenses/>.

bin_PROGRAMS = cadr cadrolizer

RESOURCE_DIR = $(iotivitydir)/resource
OUT_DIR = $(iotivitydir)/out

AM_CPPFLAGS = \
	-std=c++0x				\
	-Wall					\
	-I$(RESOURCE_DIR)/include/		\
	-I$(RESOURCE_DIR)/csdk/stack/include	\
	-I$(RESOURCE_DIR)/csdk/ocsocket/include	\
	-I$(RESOURCE_DIR)/csdk/ocrandom/include	\
	-I$(RESOURCE_DIR)/csdk/logger/include	\
	-I$(RESOURCE_DIR)/csdk/libcoap		\
	-I$(RESOURCE_DIR)/oc_logger/include

AM_LDFLAGS = \
	-L$(OUT_DIR)/linux/x86/release/		\
	-L$(OUT_DIR)/linux/x86_64/release/	\
	-loc					\
	-loctbstack				\
	-loc_logger				\
	-lpthread				\
	-lboost_program_options

cadr_SOURCES = \
	cadr.cpp	\
	common.hpp

cadrolizer_SOURCES = \
	cadrolizer.cpp \
	CadrolizerException.hpp	 \
	CadrolizerResource.cpp	\
	CadrolizerResource.hpp	\
	common.hpp	\
	os.cpp		\
	os.hpp

CLEANFILES = $(bin_PROGRAMS)

## Makefile.am ends here
