#-*-Makefile-*-----------------------------------------------------------------
#  gaol -- Just Another Interval Library
#------------------------------------------------------------------------------
# This file is part of the gaol distribution. Gaol was primarily
# developed at the Swiss Federal Institute of Technology, Lausanne,
# Switzerland, and is now developed at the Institut de Recherche
# en Informatique de Nantes, France.
#
# Copyright (c) 2001 Swiss Federal Institute of Technology, Switzerland
# Copyright (c) 2002-2006 Laboratoire d'Informatique de Nantes-Atlantique, France
#------------------------------------------------------------------------------
# gaol is a software distributed WITHOUT ANY WARRANTY. Read the associated
# COPYING file for information.
#------------------------------------------------------------------------------
# Note: To be used with GNU Make
# If this file has the name "Makefile.in" then it is a template for a
# Makefile; to generate the actual Makefile, run "./configure" from the base
# directory of gaol.
#------------------------------------------------------------------------------
# CVS: $Id: Makefile.am,v 1.22 2007/09/12 12:41:57 goualard Exp $
# Last modified: Wed Sep 27 18:12:40 2006 on pc-goualard.lina.sciences.univ-nantes.prive
# By: Frederic Goualard <Frederic.Goualard@lina.univ-nantes.fr>
#------------------------------------------------------------------------------

.PHONY: tags

SUFFIXES = .lpp .ypp

EXTRA_DIST = sysdeps/*.h stamp-h.in s_nextafter.c \
	gaol_interval_lexer.lpp gaol_interval_parser.ypp dllmain.cpp gaol_config_mingw.h \
	gaol_config_msvc.h gaol_version_msvc.h gaol_configuration.h.in gaol_interval_sse.cpp


lib_LTLIBRARIES = libgaol.la

DISTCLEANFILES = *.~* *.obj TAGS
MAINTAINERCLEANFILES = *.~* *.obj gaol_interval_lexer.cpp gaol_interval_parser.cpp \
	gaol_interval_parser.h TAGS

if COMPILE_INTERVAL2
INTERVAL2_SOURCE=gaol_interval2f.cpp
INTERVAL2_HEADER=gaol_interval2f.h
else
INTERVAL2_SOURCE=
INTERVAL2_HEADER=
endif

libgaol_la_SOURCES = \
	gaol_interval.cpp gaol_profile.cpp gaol_common.cpp \
	gaol_parser.cpp gaol_expression.cpp \
	gaol_interval_parser.cpp gaol_interval_lexer.cpp gaol_port.cpp \
	gaol_flt_output.cpp gaol_exceptions.cpp gaol_init_cleanup.cpp s_nextafter.c \
    gaol_exact.c gaol_intervalf.cpp $(INTERVAL2_SOURCE)


YFLAGS = -d -p gaol_
LFLAGS = -Pgaol_
libgaol_la_LDFLAGS = -version-info $(LT_VERSION_INFO) -release $(LT_RELEASE)

library_includedir=$(includedir)/gaol

library_include_HEADERS = gaol gaol_assert.h gaol_exact.h gaol_interval.h gaol_common.h gaol_exceptions.h \
	gaol_interval_parser.h gaol_config.h gaol_configuration.h gaol_expression.h gaol_limits.h \
	gaol_expr_eval.h  gaol_expr_visitor.h gaol_parameters.h \
	gaol_double_op_apmathlib.h gaol_double_op_crlibm.h gaol_flags.h gaol_parser.h gaol_double_op.h gaol_flt_output.h \
	gaol_port.h gaol_fpu.h gaol_fpu_fenv.h gaol_profile.h gaol.h \
	gaol_eval_stack.h gaol_init_cleanup.h gaol_version.h gaol_allocator.h \
	gaol_interval_fpu.h gaol_interval_sse.h gaol_intervalf.h $(INTERVAL2_HEADER)

LDADD = -lm @MATHLIB@

tags: TAGS
	etags --language=c++ *.cpp *.h *.c

TAGS: $(gaol_HEADERS) $(libgaol_la_SOURCES)

.lpp.cpp:
	$(LEX) $(AM_LFLAGS) $(LFLAGS) $< && mv lex.gaol_.c $@

.ypp.cpp:
	$(YACC) $(AM_YFLAGS) $(YFLAGS) $< && mv y.tab.c $*.cpp
	if test -f y.tab.h; then \
	if cmp -s y.tab.h $*.h; then rm -f y.tab.h; else mv y.tab.h $*.h; fi; \
	else :; fi


install-data-local:
	$(mkinstalldirs) $(includedir)/gaol/sysdeps
	$(INSTALL_DATA) $(srcdir)/sysdeps/*.h $(includedir)/gaol/sysdeps

