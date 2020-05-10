#-*-Makefile-*-----------------------------------------------------------------
#  gaol -- Just Another Interval Library
#------------------------------------------------------------------------------
# This file is part of the gaol distribution. Gaol was primarily
# developed at the Swiss Federal Institute of Technology, Lausanne,
# Switzerland, and is now developed at the Institut de Recherche
# en Informatique de Nantes, France.
#
# Copyright (c) 2001 Swiss Federal Institute of Technology, Switzerland
# Copyright (c) 2002 Institut de Recherche en Informatique de Nantes, France
#------------------------------------------------------------------------------
# gaol is a software distributed WITHOUT ANY WARRANTY. Read the associated
# COPYING file for information.
#------------------------------------------------------------------------------
# Note: To be used with GNU Make
# If this file has the name "Makefile.in" then it is a template for a
# Makefile; to generate the actual Makefile, run "./configure" from the base
# directory of gaol.
#------------------------------------------------------------------------------
# CVS: $Id: Makefile.am,v 1.19 2007/12/10 20:19:52 goualard Exp $
# Last modified: Wed Mar 22 09:55:09 2006 on pc-dubreil-123.irin.sciences.univ-nantes.prive
# By: Frederic Goualard <Frederic.Goualard@lina.univ-nantes.fr>
#------------------------------------------------------------------------------


# Requires automake 1.4
AUTOMAKE_OPTIONS = 1.4

#MOSTLYCLEANFILES=
#CLEANFILES =
DISTCLEANFILES = gaolcc
MAINTAINERCLEANFILES = gaolcc

if SIMD_COND
OBSIMD_FILES = interval2f
else
SIMD_FILES = 
endif

AM_DEFAULT_SOURCE_EXT = .cpp

if CPPUNIT_COND
check_PROGRAMS = arithmetic assignment constants constructor float_functions fpu generic_functions input_output \
	interval_functions misc non_arithmetic relations reverse_mappings trigonometric intervalf essai $(SIMD_FILES)
CPPUNIT_LIB = -lcppunit
else
CPPUNIT_LIB = 
endif

noinst_PROGRAMS = performances


EXTRA_DIST = gaolcc.in tests.h

TESTS = $(check_PROGRAMS)

#// TODO (goualard#4#): Handle cppunit under Windows
if IS_CYGWIN_COND
LDADD = -lgaol -lm @MATHLIB@ $(CPPUNIT_LIB)
else
LDADD = -lm -lgaol $(CPPUNIT_LIB) -ldl @MATHLIB@
endif
AM_LDFLAGS = -L../gaol/.libs 



performances_SOURCES = performance_test_set.cpp performances.cpp

PROBABILITIES=.05 .1 .1 .75

# Test of performances. Launch with "make perf"
perf:
	@date | tee performances_result.txt
	@hostname | tee -a performances_result.txt
	@echo "Compiler: @CXX@" | tee -a performances_result.txt
	@echo "Mathematical library: @MATHLIB@" | tee -a performances_result.txt
	./performances add 10 $(PROBABILITIES) | tee -a performances_result.txt
	./performances sub 10 $(PROBABILITIES) | tee -a performances_result.txt
	./performances mul 10 $(PROBABILITIES) | tee -a performances_result.txt
	./performances div 10 $(PROBABILITIES) | tee -a performances_result.txt
	./performances inv 10 $(PROBABILITIES) | tee -a performances_result.txt
	./performances pow 10 $(PROBABILITIES) | tee -a performances_result.txt
	./performances cos 10 $(PROBABILITIES) | tee -a performances_result.txt
	./performances sin 10 $(PROBABILITIES) | tee -a performances_result.txt
	./performances tan 10 $(PROBABILITIES) | tee -a performances_result.txt
	./performances acos 10 $(PROBABILITIES) | tee -a performances_result.txt
	./performances asin 10 $(PROBABILITIES) | tee -a performances_result.txt
	./performances atan 10 $(PROBABILITIES) | tee -a performances_result.txt
	./performances cosh 10 $(PROBABILITIES) | tee -a performances_result.txt
	./performances sinh 10 $(PROBABILITIES) | tee -a performances_result.txt
	./performances tanh 10 $(PROBABILITIES) | tee -a performances_result.txt
	./performances acosh 10 $(PROBABILITIES) | tee -a performances_result.txt
	./performances asinh 10 $(PROBABILITIES) | tee -a performances_result.txt
	./performances atanh 10 $(PROBABILITIES) | tee -a performances_result.txt
	./performances exp 10 $(PROBABILITIES) | tee -a performances_result.txt
	./performances log 10 $(PROBABILITIES) | tee -a performances_result.txt

