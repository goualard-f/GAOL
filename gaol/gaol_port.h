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
 * CVS: $Id: gaol_port.h 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By:
 *--------------------------------------------------------------------------*/
/*!
  \file   gaol_port.h
  \brief  Functions defined for portability, in case they do not exist on one
  platform.

  <long description>

  \author Frédéric Goualard
  \date   2001-10-03
*/


#ifndef __gaol_port_h__
#define __gaol_port_h__

// To overcome problems with old versions of autoconf
#undef PACKAGE
#include "gaol/gaol_config.h"
#include "gaol/gaol_limits.h"

#if HAVE_FINITE
#  include <cmath>
#endif

// Alignment on an 'nbytes' bytes boundary
#if defined(_MSC_VER)
#	define GAOL_ALIGN(what,nbytes)	__declspec(align(nbytes)) what
#else
#	define GAOL_ALIGN(what,nbytes) what __attribute__((aligned(nbytes)))
#endif // defined(MSC_VER)
#define GAOL_ALIGN16(what) GAOL_ALIGN(what,16)

// Allocation of 'size' bytes on 'boundary' bytes.
// NOTE: MEMALIGN() must return null value if no allocation error
#if defined (__MINGW32__)
#  include <stdlib.h>
#  include <malloc.h>
#  define MEMALIGN(buf,boundary,size) (!(buf=_mm_malloc(size,boundary)))
#elif defined(IX86_LINUX) || defined(AARCH64_LINUX)
#  undef _XOPEN_SOURCE
#  define _XOPEN_SOURCE 600
#  include <stdlib.h>
#  define MEMALIGN(buf,boundary,size) posix_memalign(&buf,boundary,size)
#elif defined(IX86_MACOSX)
// According to man page, Intel/MacOSX's malloc aligns correctly for SSE-related types
#  define MEMALIGN(buf,boundary,size) (!(buf=malloc(size)))
#else
#  error "Don't know how to allocate memory aligned on a boundary"
#endif


#if defined (_MSC_VER)
#   if HAVE_NEXTAFTER
	// Nothing to do
#   elif HAVE__NEXTAFTER
#		include <float.h>
#		define nextafter _nextafter
#	else // !HAVE_NEXTAFER && !HAVE__NEXTAFTER
        double s_nextafter(double,double);
#       define nextafter s_nextafter
#   endif
#   ifndef HAVE_ISNAN
        int _isnan(double);
#       define isnan _isnan
#   endif
#endif // defined(MSC_VER)


namespace gaol {

#if HAVE_ROUNDING_MATH_OPTION
  INLINE double f_negate_simple(double x) { return -x; }
#  define opposite(x) f_negate_simple(x)
#else
#  define opposite(x) f_negate(x)
#endif // HAVE_ROUNDING_MATH_OPTION

  /*!
    \brief Sign of double

    \return 0 if the argument is positive and 1 otherwise
    \note Returns 1 for -0.0
  */
  extern __GAOL_PUBLIC__ int gaol_signbit(double);


#if SIZEOF_INT==4
#  define INT_FOR_DOUBLE int
#elif SIZEOF_LONG_INT==4
#  define INT_FOR_DOUBLE long
#else
#  error "Cannot find a 32 bits integer type!"
#endif

#if SIZEOF_LONG_INT==8
#  define ULONGLONGINT unsigned long int
#elif SIZEOF_LONG_LONG_INT==8
#  define ULONGLONGINT unsigned long long int
#else
#  error "Cannot find a 64 bits integer type!"
#endif

  typedef union {
    ULONGLONGINT i;
    double d;
  } ullidouble;

  typedef union {
    unsigned INT_FOR_DOUBLE i[2];
    double d;
  } uintdouble;


#if WORDS_BIGENDIAN
#  define IFBIGENDIAN(a,b)   (a), (b)
#  define __HI(x) (*(INT_FOR_DOUBLE*)&(x))
#  define __LO(x) (*((INT_FOR_DOUBLE)1+(INT_FOR_DOUBLE*)&(x)))
#  define LO_UINTDOUBLE(a) ((a).i[1])
#  define HI_UINTDOUBLE(a) ((a).i[0])
#else
#  define IFBIGENDIAN(a,b)   (b), (a)
#  define __HI(x) *((INT_FOR_DOUBLE)1+(INT_FOR_DOUBLE*)&(x))
#  define __LO(x) *(INT_FOR_DOUBLE*)&(x)
#  define LO_UINTDOUBLE(a) ((a).i[0])
#  define HI_UINTDOUBLE(a) ((a).i[1])
#endif

#ifndef GAOL_NAN
  static uintdouble NaN_val = {{IFBIGENDIAN(0x7ff80000, 0x0)}};
#define GAOL_NAN (gaol::NaN_val.d)
#endif

  // #define GAOL_INFINITY std::numeric_limits<double>::infinity()
  /* We cannot use the definition above because some versions of libc++ do not
     define the infinity() method correctly.
  */
#define GAOL_INFINITY HUGE_VAL

  /*
    Various constants rounded up and down
  */
  //! Pi rounded towards -oo.
  const uintdouble upi_dn = {{IFBIGENDIAN(1074340347,1413754136)}};
  //! Pi rounded towards +oo.
  const uintdouble upi_up   = {{IFBIGENDIAN(1074340347,1413754137)}};
  //! Pi/2 rounded towards -oo.
  const uintdouble uhalfpi_dn = {{IFBIGENDIAN(1073291771,1413754136)}};
  //! Pi/2 rounded towards +oo.
  const uintdouble uhalfpi_up = {{IFBIGENDIAN(1073291771,1413754137)}};
  // ln(2) rounded towards -oo
  const uintdouble uln2_dn = {{IFBIGENDIAN(0x3fe62e42,0xFEFA39EF)}};
  // ln(2) rounded towards +oo
  const uintdouble uln2_up = {{IFBIGENDIAN(0x3fe62e42,0xFEFA39F0)}};


  const double pi_dn = upi_dn.d;
  const double pi_up = upi_up.d;

  const double half_pi_dn = uhalfpi_dn.d;
  const double half_pi_up = uhalfpi_up.d;

  const double two_pi = 6.28318530717958647693;
  const double pi = 3.14159265358979323846;
  const double half_pi = 1.57079632679489661923;

  const double ln2_dn = uln2_dn.d;
  const double ln2_up = uln2_up.d;

  const double two_power_51 = 2251799813685248.0;
  const double two_power_52 = 4503599627370496.0;

  /*!
    \brief Returns 1 if d is neither a NaN nor an infinity
   */
  INLINE int is_finite(double d)
  {
#if HAVE_FINITE
    return finite(d);
#else
    return (d==d && d!=GAOL_INFINITY && d!=-GAOL_INFINITY);
#endif
  }


} // namespace gaol

#endif /* __gaol_port_h__ */
