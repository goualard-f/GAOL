#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
# ------------------------------------------------------------------------
# gaol -- NOT Just Another Interval Library
# ------------------------------------------------------------------------
# This file is part of the gaol distribution. Gaol was primarily
# developed at the Swiss Federal Institute of Technology, Lausanne,
# Switzerland, and is now developed at the Laboratoire d'Informatique
# de Nantes-Atlantique, France.
dnl
# Copyright (c) 2001 Swiss Federal Institute of Technology, Switzerland
# Copyright (c) 2002-2016 Laboratoire d'Informatique de Nantes-Atlantique,
#                         France
# Copyright (c) 2017-2020 Laboratoire des Sciences du Numérique de Nantes,
#                         France
# ------------------------------------------------------------------------
# gaol is a software distributed WITHOUT ANY WARRANTY. Read the associated
# COPYING file for information.
# ------------------------------------------------------------------------
# This file is an input file used by the GNU "autoconf" program to
# generate the file "configure", which is run during gaol installation
# to configure the system for the local environment.
# ------------------------------------------------------------------------

AC_PREREQ(2.61)

# gaol library version information
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# Do NOT forget to modify gaol_version_msvc.h and gaol_version_mingw.h as well
# gaol library version information.
# (shamelessly stolen from GiNaC (http://www.ginac.de/)
# Making releases:
#   gaol_micro_version += 1;
#   gaol_interface_age += 1;
#   gaol_binary_age += 1;
# if any functions have been added, set gaol_interface_age to 0.
# if backwards compatibility has been broken,
# set gaol_binary_age and gaol_interface_age to 0.
# Version information for gaol
m4_define([gaol_major_version], [4])
m4_define([gaol_minor_version], [2])
m4_define([gaol_micro_version], [3])
m4_define([gaol_version], ["gaol_major_version.gaol_minor_version.gaol_micro_version"])
m4_define([gaol_release], [gaol_major_version.gaol_minor_version])
m4_define([gaol_interface_age], [0])
m4_define([gaol_binary_age], [0])

AC_INIT([gaol],[gaol_version],[goualard@users.sourceforge.net])
AM_INIT_AUTOMAKE([gnu 1.7 dist-bzip2 subdir-objects])

AM_MAINTAINER_MODE([disable])

AC_REVISION([$Revision: 291 $])
AC_CONFIG_SRCDIR([gaol/gaol_config.h])
AC_CONFIG_HEADERS([gaol/gaol_configuration.h])
AC_COPYRIGHT([Copyright (c) 2002-2020 Laboratoire d'Informatique de Nantes-Atlantique, France])

AC_CONFIG_MACRO_DIR([m4])

AC_DEFINE([GAOL_MAJOR_VERSION],gaol_major_version,[Major version for gaol])
AC_DEFINE([GAOL_MINOR_VERSION],gaol_minor_version,[Minor version for gaol])
AC_DEFINE([GAOL_MICRO_VERSION],gaol_micro_version,[Micro version for gaol])
AC_DEFINE([GAOL_VERSION],gaol_version,[Version number for gaol])
# Version for the manual
GAOL_EDITION="4.5"
GAOL_UPDATED="10 May 2020"
GAOL_VERSION=gaol_version
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
AC_SUBST(GAOL_EDITION)
AC_SUBST(GAOL_UPDATED)
AC_SUBST(GAOL_VERSION)



dnl libtool versioning
m4_define([lt_revision], [gaol_interface_age])
m4_define([lt_current], [m4_eval(gaol_micro_version - gaol_interface_age)])
m4_define([lt_age], [m4_eval(gaol_binary_age - gaol_interface_age)])
LT_VERSION_INFO="lt_current:lt_revision:lt_age"
LT_RELEASE="gaol_release"

AC_SUBST(LT_VERSION_INFO)
AC_SUBST(LT_RELEASE)

LT_PREREQ([2.2.6])
LT_INIT([dlopen])

# RPM file versioning
GAOL_RPM_RELEASE=1
AC_SUBST(GAOL_RPM_RELEASE)


# Checks for programs.
AC_PROG_YACC

CFLAGS="${CFLAGS} -I.."
CXXFLAGS="${CXXFLAGS} -I.."

AC_PROG_CC([gcc])
AC_PROG_CXX

# --enable-simd
AC_ARG_ENABLE(simd,
                [AS_HELP_STRING([--enable-simd],
                        [use SIMD instructions to implement arithmetic operations])],
                [simd=${enable_simd}],
                [simd="yes"])
AM_CONDITIONAL([SIMD_COND],[test "$simd" == "yes"])
using_sse2=no
has_sse3=no
if test "$simd" = yes; then
    AC_MSG_CHECKING([whether SSE2 features are available])
    AC_RUN_IFELSE([AC_LANG_SOURCE([[int main(void) {unsigned int a, b, c, d; __asm__ __volatile__ ("cpuid":"=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (0)); if (a >= 1) {
        __asm__ __volatile__ ("cpuid":"=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (1)); return (d & 0x04000000) == 0; } else {
            return 1; } }]])],[has_sse2=yes],[has_sse2=no],[:])
    AC_MSG_RESULT(${has_sse2})
    if test ${has_sse2} = yes; then
        using_sse2=yes
        case $CXX in
        icpc)
            CXXFLAGS="${CXXFLAGS} -xP -mtune=pentium4"
            ;;
        g++)
            CXXFLAGS="${CXXFLAGS} -msse2"
            ;;
        esac
        AC_DEFINE([USING_SSE2_INSTRUCTIONS],1,
            [Define this to 1 if SSE2 instructions are to be used])
    fi
    AC_MSG_CHECKING([whether SSE3 features are available])
    AC_RUN_IFELSE([AC_LANG_SOURCE([[int main(void) {unsigned int a, b, c, d; __asm__ __volatile__ ("cpuid":"=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (0)); if (a >= 1) {
        __asm__ __volatile__ ("cpuid":"=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (1)); return (c & 0x00000001) == 0; } else {
            return 1; } }]])],[has_sse3=yes],[has_sse3=no],[:])
    AC_MSG_RESULT(${has_sse3})
    if test ${has_sse3} = yes; then
        AC_DEFINE([USING_SSE3_INSTRUCTIONS],1,
            [Define this to 1 if SSE3 instructions are to be used])
        if test $CXX = g++; then
            CXXFLAGS="${CXXFLAGS} -msse3"
        fi
    fi
fi
AM_CONDITIONAL([COMPILE_INTERVAL2],[test ${has_sse3} = yes])




# --with-mathlib(-path)
AC_ARG_WITH([mathlib-include],
		  [AS_HELP_STRING([--with-mathlib-include],[Specify the include path for the mathematical library])],
		  mathlib_incpath="-I${with_mathlib_include}",mathlib_incpath="")
AC_ARG_WITH([mathlib-lib],
		  [AS_HELP_STRING([--with-mathlib-lib],[Specify the library path for the mathematical library])],
		  mathlib_libpath="-L${with_mathlib_lib}",mathlib_libpath="")
CXXFLAGS="$CXXFLAGS ${mathlib_libpath}"
CPPFLAGS="$CPPFLAGS ${mathlib_incpath}"
AC_ARG_WITH([mathlib],
    [AS_HELP_STRING([--with-mathlib=(apmathlib,crlibm)],[Specify the mathematical library to use @<:@default=apmathlib@:>@])],
    use_mathlib="${with_mathlib}",use_mathlib="apmathlib")
if test "${use_mathlib}" = "apmathlib"; then
   OLD_LIBS=$LIBS
   OLD_LDFLAGS=$LDFLAGS
   LDFLAGS="${mathlib_libpath}"
   LIBS="-lm"
   AC_CHECK_LIB(ultim,utan,mathlib_found=yes,mathlib_found=no)
   LIBS=${OLD_LIBS}
   LDFLAGS=${OLD_LDFLAGS}
   if test "${mathlib_found}" = "no"; then
      AC_MSG_ERROR([APMathlib library not found])
   fi
   AC_CHECK_HEADERS(MathLib.h,mathlib_h=yes,mathlib_h=no)
   if test "${mathlib_h}" = "no"; then
      AC_MSG_ERROR([Header MathLib.h not found])
   fi
   AC_DEFINE(GAOL_USING_APMATHLIB,1,
      [Define this to 1 if using the IBM APMathlib library])
   MATHLIB="-lultim"
fi
if test "${use_mathlib}" = "crlibm"; then
   OLD_LIBS=$LIBS
   OLD_LDFLAGS=$LDFLAGS
   LDFLAGS="${mathlib_libpath}"
   LIBS="-lm"
   AC_CHECK_LIB(crlibm,cos_rn,crlibm_found=yes,crlibm_found=no)
   LIBS=${OLD_LIBS}
   LDFLAGS=${OLD_LDFLAGS}
   if test "${crlibm_found}" = "no"; then
      AC_MSG_ERROR([CRLibm library not found])
   fi
   AC_CHECK_HEADERS(crlibm.h,crlibm_h=yes,crlibm_h=no)
   if test "${crlibm_h}" = "no"; then
      AC_MSG_ERROR([Header crlibm.h not found])
   fi
   AC_DEFINE(GAOL_USING_CRLIBM,1,
      [Define this to 1 if using the CRLibm Mathematical library])
   MATHLIB="-lcrlibm"
fi
AC_SUBST([MATHLIB])
AC_CONFIG_LINKS([gaol/gaol_double_op.h:gaol/gaol_double_op_${use_mathlib}.h])



# --enable-verbose-mode
AC_ARG_ENABLE(verbose-mode,
                  [AS_HELP_STRING([--enable-verbose-mode],
                         [enable display of information])],
                 [verbose=${enable_verbose}],
                 [verbose="yes"])
if test "$verbose" = yes; then
  AC_DEFINE(GAOL_VERBOSE_MODE,1,
        [Define this to 1 to enable verbose mode])
fi

# --enable-optimize
AC_ARG_ENABLE(optimize,
	    [AS_HELP_STRING([--enable-optimize],
                            [compile gaol with full optimization @<:@default=yes@:>@])],
            [optimize=${enable_optimize}],
            [optimize="yes"])
if test $optimize = yes; then
    case $CXX in
    icpc)
        CXXFLAGS="${CXXFLAGS} -O3"
        ;;
    g++)
        CXXFLAGS="${CXXFLAGS} -O3 -funroll-loops -fomit-frame-pointer -fexpensive-optimizations"
        AC_LANG_PUSH([C++])
        OLD_CXXFLAGS=$CXXFLAGS
        CXXFLAGS="-fvisibility-inlines-hidden -fvisibility=hidden"
        AC_MSG_CHECKING(whether the C++ compiler accepts -fvisibility-inlines-hidden and -fvisibility)
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],[has_visibility_opt=yes],[has_visibility_opt=no])
        AC_MSG_RESULT(${has_visibility_opt})
        CXXFLAGS=${OLD_CXXFLAGS}
        if test "${has_visibility_opt}" = yes; then
            CXXFLAGS="${CXXFLAGS} -fvisibility-inlines-hidden -fvisibility=hidden"
            AC_DEFINE(HAVE_VISIBILITY_OPTIONS,1,
                        [Define this to 1 if the compiler supports the "-fvisibility" option])
        fi
        AC_LANG_POP([C++])
        ;;
    esac
else
   CXXFLAGS="${CXXFLAGS} -O"
fi

CFLAGS="${CFLAGS} -O2 -Wall -Wconversion"
CXXFLAGS="${CXXFLAGS} -Wall -Wconversion"


if test ${using_sse2} = no; then
    case $CXX in
    icpc)
        ;;
    g++)
        CXXFLAGS="${CXXFLAGS} -ffloat-store"
    ;;
    esac
fi

# --enable-debug
AC_ARG_ENABLE(debug,
            [AS_HELP_STRING([--enable-debug],
                            [compile gaol with debugging info @<:@default=no@:>@])],
            [debug=${enable_debug}],
            [debug="no"])
if test "$debug" = yes; then
   AC_DEFINE(GAOL_DEBUGGING,1,[define this if you want debugging facilities])
    case $CXX in
    icpc)
        CXXFLAGS="${CXXFLAGS} -g"
        ;;
    g++)
        CXXFLAGS="${CXXFLAGS} -g -ansi"
        CXXFLAGS="${CXXFLAGS} -Weffc++ -pedantic"
        ;;
    esac
fi

# --enable-preserve-rounding
AC_ARG_ENABLE(preserve-rounding,
            [AS_HELP_STRING([--enable-preserve-rounding],
                            [transparently manipulates the FPU control flags @<:@default=yes@:>@])],
            [preserve_rounding=${enable_preserve_rounding}],
            [preserve_rounding="yes"])
if test "${preserve_rounding}" = yes; then
   AC_DEFINE(GAOL_PRESERVE_ROUNDING,1,[define this to 1 if rounding direction should be preserved])
fi

# --enable-relations
AC_ARG_ENABLE(relations,
            [AS_HELP_STRING([--enable-relations=KIND],
                            [use KIND symbols of relations, with  KIND being "set", "certainly" or "possibly" @<:@default=certainly@:>@])],
            [relations=${enable_relations}],
            [relations="certainly"])
case "$relations" in
set)
    AC_DEFINE(GAOL_SET_RELATIONS,1,
    [Define this if relation symbols should stand for \"set relations\"])
    ;;
certainly)
    AC_DEFINE(GAOL_CERTAINLY_RELATIONS,1,
    [Define this if relation symbols should stand for \"certainly relations\"])
    ;;
possibly)
    AC_DEFINE(GAOL_POSSIBLY_RELATIONS,1,
    [Define this if relation symbols should stand for \"possibly relations\"])
    ;;
*)
    AC_MSG_ERROR(\"$relations\": unrecognized default relation (set/possibly/certainly))
   ;;
esac


# --enable-exceptions
AC_ARG_ENABLE(exceptions,
              [AS_HELP_STRING([--enable-exceptions],
                              [raise exceptions to signal errors @<:@default=yes@:>@])],
              [exceptions=${enable_exceptions}],
              [exceptions="yes"])
if test "$exceptions" = "yes"; then
   AC_DEFINE(GAOL_EXCEPTIONS_ENABLED,1,
    [Define this to 1 to allow raising exceptions instead of simply aborting])
fi

# --enable-asm
AC_ARG_ENABLE(asm,
              [AS_HELP_STRING([--enable-asm],
                              [use assembler code @<:@default=yes@:>@])],
              [using_asm=${enable_asm}],
              [using_asm="yes"])
if test "${using_asm}" = yes; then
   AC_DEFINE(GAOL_USING_ASM,1,[define this to 1 if you want to allow ASM support])
fi


# Determines the kind of architecture to use
AC_CANONICAL_HOST

case $host in
i?86-*-linux*)
	AC_DEFINE(IX86_LINUX,1,
          [Define this if your system is a Linux-based ix86 or compatible])
	AM_CONDITIONAL([IS_CYGWIN_COND],[false])
	;;
*86_64-*-linux*)
	AC_DEFINE(IX86_LINUX,1,
          [Define this if your system is a Linux-based ix86 or compatible])
	AM_CONDITIONAL([IS_CYGWIN_COND],[false])
	AC_DEFINE(X86_64BITS,1,
		  [Define this if the OS is a 64 bits one])
	;;
i?86-*-cygwin*)
	AC_DEFINE(IX86_CYGWIN,1,
          [Define this if your system is a cygwin-based ix86 or compatible])
	AM_CONDITIONAL([IS_CYGWIN_COND],[true])
	;;
i?86-apple-*)
	AC_DEFINE(IX86_MACOSX,1,
          [Define this if your system is a Linux-based ix86 or compatible])
	AM_CONDITIONAL([IS_CYGWIN_COND],[false])
	;;
sparc*-solaris*)
	AC_DEFINE(SPARC_SOLARIS,1,
	  [Define this if your system is a SUN Sparc computer under Solaris])
	AM_CONDITIONAL([IS_CYGWIN_COND],[false])
	;;
powerpc*)
	AC_DEFINE(POWERPC_UNIX,1,
	  [Define this if your system is a PowerPc under MacOs X])
	AM_CONDITIONAL([IS_CYGWIN_COND],[false])
	;;
aarch64-*-linux-*)
	AC_DEFINE(AARCH64_LINUX,1,
		[Define this if your system is an AARCH64-based computer under Linux])
	AM_CONDITIONAL([IS_CYGWIN_COND],[false])
	;;
*)
	AC_MSG_ERROR([Sorry, this system is not yet supported by gaol])
esac


# Checking for dl library
AC_CHECK_LIB([dl], [dlopen],have_dl=yes,have_dl=no)
if test "${have_dl}" = no; then
   dllib=""
else
   dllib="-ldl"
fi

# Checking for cppunit
AC_LANG_PUSH([C++])
AC_ARG_WITH([cppunit-include],
		  [AS_HELP_STRING([--with-cppunit-include],[Specify the include path for cppunit])],
		  cppunit_incpath="-I${with_cppunit_include}",cppunit_incpath="")
AC_ARG_WITH([cppunit-lib],
		  [AS_HELP_STRING([--with-cppunit-lib],[Specify the library path for cppunit])],
		  cppunit_libpath="-L${with_cppunit_lib}",cppunit_libpath="")
LDFLAGS="$LDFLAGS ${cppunit_libpath}"
CXXFLAGS="${CXXFLAGS} ${cppunit_incpath}"
AC_MSG_CHECKING([for cppunit/ui/text/TestRunner.h])
AC_CHECK_HEADERS("cppunit/ui/text/TestRunner.h",have_TestRunner=yes,have_TestRunner=no)
if test "${have_TestRunner}" = no; then
  AM_CONDITIONAL([CPPUNIT_COND],[test ${have_TestRunner} = yes])
    AC_MSG_WARN("*******************************************")
    AC_MSG_WARN("Library 'cppunit' not present.")
    AC_MSG_WARN("You will not be able to check the library.")
    AC_MSG_WARN("*******************************************")
else
  AC_MSG_CHECKING([for libcppunit])
  OLD_LIBS=$LIBS
  LIBS="-lcppunit $dllib"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <cppunit/ui/text/TestRunner.h>]], [[CppUnit::TextUi::TestRunner runner;]])],[have_cppunit=yes],[have_cppunit=no])
  AC_MSG_RESULT(${have_cppunit})
  AM_CONDITIONAL([CPPUNIT_COND],[test ${have_cppunit} = yes])
  LIBS=$OLD_LIBS
fi
AC_LANG_POP([C++])

AC_CHECK_LIB([m],[nextafter],nextafter_found=yes,nextafter_found=no)
if test "${nextafter_found}" = yes; then
   AC_DEFINE(HAVE_NEXTAFTER,1,[ Define to 1 if you have the `nextafter' function.])
fi

# Checking for tools to re-create the pdf documentation
AC_CHECK_PROG(PDFLATEX,pdflatex,pdflatex,no)
if test "$PDFLATEX" = "no"; then
    AC_MSG_WARN("****************************************************")
    AC_MSG_WARN("No tool to compile a LaTeX file into a PDF file found")
    AC_MSG_WARN("You will not be able to re-create the PDF documentation")
    AC_MSG_WARN("****************************************************")
fi
AC_CHECK_PROG(BIBTEX,bibtex,bibtex,no)
AC_CHECK_PROG(MAKEINDEX,makeindex,makeindex,no)
if test "$MAKEINDEX" = "no"; then
  AC_CHECK_PROG(MAKEINDEX,makeidx,makeidx,no)
fi
AM_CONDITIONAL(PDF_TO_LATEX,
               [test "${PDFLATEX}" != "no" -a "$MAKEINDEX" != "no" -a "$BIBTEX" != "no"])



# Checking for tools to re-create the html documentation
AC_CHECK_PROG(DOXYGEN,doxygen,doxygen,no)
if test "${DOXYGEN}" = "no"; then
   AC_MSG_WARN("****************************************************")
   AC_MSG_WARN("'doxygen' not found:                                ")
   AC_MSG_WARN("You will not be able to create html documentation   ")
   AC_MSG_WARN("****************************************************")
fi
AM_CONDITIONAL(DOXYGEN, test "${DOXYGEN}" = "doxygen")
AC_CHECK_PROG(DOT,dot,dot,no)
if test "${DOT}" = "no"; then
   AC_MSG_WARN("******************************************************")
   AC_MSG_WARN("'dot' not found:                                      ")
   AC_MSG_WARN("You will not be able to create full html documentation")
   AC_MSG_WARN("You should consider installing the GraphViz package   ")
   AC_MSG_WARN("                           http://www.graphviz.org/   ")
   AC_MSG_WARN("******************************************************")
fi


# Checks for C header files
AC_HEADER_STDC
AC_CHECK_HEADERS([fenv.h float.h stddef.h stdlib.h string.h sys/time.h unistd.h])

# Checking for C++ header files
AC_LANG([C++])
AC_CHECK_HEADERS([limits cassert])


# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

AC_C_BIGENDIAN
AC_CHECK_SIZEOF(int, 4)
AC_CHECK_SIZEOF(long, 4)
AC_CHECK_SIZEOF(long long int,8)


# Checking for availability of -frounding-math
AC_LANG([C++])
if test "$CXX" = "g++"; then
    OLD_CXXFLAGS=$CXXFLAGS
    CXXFLAGS="-frounding-math"
    AC_MSG_CHECKING(whether the C++ compiler accepts -frounding-math)
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],[has_rounding_math_opt=yes],[has_rounding_math_opt=no])
    AC_MSG_RESULT(${has_rounding_math_opt})
    CXXFLAGS=${OLD_CXXFLAGS}
    AC_LANG([C])
    if test "${has_rounding_math_opt}" = yes; then
        CXXFLAGS="${CXXFLAGS} -frounding-math"
        AC_DEFINE(HAVE_ROUNDING_MATH_OPTION,1,
            [Define this to 1 if the compiler supports the "-frounding-math" option])
    fi
fi


# Checks for library functions.
AC_LANG([C++])
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([fesetround floor localeconv memset pow sqrt])
AC_CHECK_FUNCS(getrusage clock __signbit round rint finite)

# Checking for timing functions
AC_CHECK_HEADERS(sys/resource.h,sys_resource_found=yes,sys_resource_found=no)
if test "${sys_resource_found}" = yes; then
   AC_EGREP_HEADER("getrusage",sys/resource.h,
	    AC_DEFINE(GETRUSAGE_IN_HEADER,1,
               [Define this to 1 if function getrusage() is declared in sys/resource.h]),
	    AC_DEFINE(GETRUSAGE_IN_HEADER,0,
               [Define this to 1 if function getrusage() is declared in sys/resource.h]))
else
   AC_CHECK_HEADERS(time.h,time_found=yes,time_found=no)
   if test "${time_found}" = yes; then
      AC_EGREP_HEADER("clock",time.h,
	AC_DEFINE(CLOCK_IN_HEADER,1,
	  [Define this to 1 if function clock() is declared in time.h]),
	AC_DEFINE(CLOCK_IN_HEADER,0,
	  [Define this to 1 if function clock() is declared in time.h]))
   fi
fi



AC_SUBST(CPPFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(CFLAGS)
AC_SUBST(CXX)
AC_SUBST(CC)
AC_SUBST(LDFLAGS)

AC_CONFIG_FILES([Makefile
        check/Makefile
		  doc/gaol_doxygen.cfg
        doc/Makefile
        doc/gaol_version.tex
		  gaol.spec
		  gaol/Makefile
		  examples/Makefile])

AC_OUTPUT

printf "\nConfigured gaol v. ${GAOL_VERSION} on $host\n"
printf "CC\t\t\t= ${CC}\n"
printf "CFLAGS\t\t\t= ${CFLAGS}\n"
printf "CXX\t\t\t= ${CXX}\n"
printf "CXXFLAGS\t\t= ${CXXFLAGS}\n"
printf "mathlib\t\t\t= ${use_mathlib}\n"
printf "optimize\t\t= $optimize\n"
printf "debug\t\t\t= $debug\n"
printf "preserve rounding\t= ${preserve_rounding}\n"
printf "relations\t\t= $relations\n"
printf "exceptions\t\t= $exceptions\n"
printf "asm\t\t\t= ${using_asm}\n"
printf "SSE2\t\t\t= ${using_sse2}\n"
printf "SSE3\t\t\t= ${has_sse3}\n"
printf "doxygen\t\t\t= ${DOXYGEN}\n"
printf "dot\t\t\t= ${DOT}\n"
printf "pdflatex\t\t= $PDFLATEX\n"
printf "makeindex\t\t= $MAKEINDEX\n"
printf "bibtex\t\t\t= $BIBTEX\n"

