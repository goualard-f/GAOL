/*-*-C++-*------------------------------------------------------------------
 * gaol -- NOT Just Another Interval Library
 *--------------------------------------------------------------------------
 * This file is part of the gaol distribution. Gaol was primarily
 * developed at the Swiss Federal Institute of Technology, Lausanne,
 * Switzerland, and is now developed at the Laboratoire d'Informatique de
 * Nantes-Atlantique, France.
 *
 * Copyright (c) 2001 Swiss Federal Institute of Technology, Switzerland
 * Copyright (c) 2002-2006 Laboratoire d'Informatique de
 *                         Nantes-Atlantique, France
 *--------------------------------------------------------------------------
 * gaol is a software distributed WITHOUT ANY WARRANTY. Read the associated
 * COPYING file for information.
 *--------------------------------------------------------------------------
 * CVS: $Id: gaol_config_mingw.h 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By:
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_config_mingw.h
  \brief  Configuration file for Mingw-g++


  \author Frederic Goualard
  \date   2002-12-03
*/


#ifndef __gaol_config_mingw_h__
#define __gaol_config_mingw_h__

/* Define this to 1 if using the IBM APMathlib library */
#define GAOL_USING_APMATHLIB 1

#undef  WORDS_BIGENDIAN

/* Define if lex declares yytext as a char * by default, not a char[].  */
#define YYTEXT_POINTER 1

/* The number of bytes in a int.  */
#define SIZEOF_INT 4

/* The number of bytes in a long.  */
#define SIZEOF_LONG 4

/* The number of bytes in a long long int.  */
#define SIZEOF_LONG_LONG_INT 8

/* Define if you have the clock function.  */
#define HAVE_CLOCK 1

/* Define if you have the getrusage function.  */
#define HAVE_GETRUSAGE 0

/* Define if you have the rint function.  */
/* #undef HAVE_RINT */

#define HAVE_FINITE 1

/* Define this to 1 if we should use fast fp operations */
#define GAOL_FAST_MATH 1

/* Define if you have the round function.  */
/* #undef HAVE_ROUND */

/* Define if you have the __signbit function.  */
#define HAVE___SIGNBIT 1

/* Define if you have the <dlfcn.h> header file.  */
#define HAVE_DLFCN_H 1

/* Define if you have the <limits> header file.  */
#define HAVE_LIMITS 1

/* Define if you have the <MathLib.h> header file.  */
#define HAVE_MATHLIB_H 1

/* Define if you have the <sys/resource.h> header file.  */
#define HAVE_SYS_RESOURCE_H 1

/* Define if you have the <time.h> header file.  */
#define HAVE_TIME_H 1

/* define this if you want debugging facilities */
/* #undef GAOL_DEBUGGING */

/* Define this if relation symbols should stand for "set relations" */
/* #undef GAOL_SET_RELATIONS */

/* Define this if relation symbols should stand for "certainly relations" */
#define GAOL_CERTAINLY_RELATIONS 1

/* Define this if relation symbols should stand for "possibly relations" */
/* #undef GAOL_POSSIBLY_RELATIONS */

/* Define this to 1 to allow raising exceptions instead of simply aborting */
#define GAOL_EXCEPTIONS_ENABLED 1

/* Define this if your system is a Linux-based ix86 or compatible */
#define IX86_LINUX 1

/* Define this if your system is a SUN Sparc computer under Solaris */
/* #undef SPARC_SOLARIS */

/* Define this to 1 if function getrusage() is declared in sys/resource.h */
#define GETRUSAGE_IN_HEADER 1

/* Define this to 1 if function clock() is declared in time.h */
#define CLOCK_IN_HEADER 1

/* Define to 1 if you have the `nextafter' function. */
#define HAVE_NEXTAFTER 1

#define GAOL_USING_ASM 1

/* Define if you have the <fenv.h> header file.  */
#define HAVE_FENV_H 1

/* define this to 1 if rounding direction should be preserved */
#define GAOL_PRESERVE_ROUNDING 1

#define USING_SSE2_INSTRUCTIONS 1

#endif /* __gaol_config_mingw_h__ */
