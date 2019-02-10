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
 * CVS: $Id: gaol_double_op_apmathlib.h 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By:
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_double_op_apmathlib.h
  \brief  Operations correctly rounded down and up.

  Fast version using standard arithmetic operations and the IBM APMathlib library

  \author Frédéric Goualard
  \date   2006-02-21
*/


#ifndef __gaol_double_op_apmathlib_h__
#define __gaol_double_op_apmathlib_h__

#include <cmath>
#include "gaol/gaol_config.h"
#include "gaol/gaol_port.h"
#include "gaol/gaol_fpu.h"
#include "gaol/gaol_assert.h"
#include "gaol/gaol_common.h"

#if defined (_MSC_VER)
# define _MATHLIB_DLL_ extern "C" __declspec(dllimport)
#elif defined (__GNUC__)
# define _MATHLIB_DLL_ extern "C" __attribute__ ((visibility("default")))
#else
# define _MATHLIB_DLL_ extern "C"
#endif

_MATHLIB_DLL_ unsigned short Init_Lib();
_MATHLIB_DLL_ void Exit_Lib(unsigned short);
_MATHLIB_DLL_ double uasin(double );
_MATHLIB_DLL_ double uacos(double );
_MATHLIB_DLL_ double uatan(double );
_MATHLIB_DLL_ double uatan2(double ,double );
_MATHLIB_DLL_ double ulog(double );
_MATHLIB_DLL_ double ulog2(double );
_MATHLIB_DLL_ double uexp(double );
_MATHLIB_DLL_ double uexp2(double );
_MATHLIB_DLL_ double usin(double );
_MATHLIB_DLL_ double ucos(double );
_MATHLIB_DLL_ double utan(double );
_MATHLIB_DLL_ double ucot(double );
_MATHLIB_DLL_ double usqrt(double );
_MATHLIB_DLL_ double upow(double , double );
_MATHLIB_DLL_ double uremainder(double , double );

namespace gaol {


  /*
    Computes d^n rounded upward with a binary exponentiation algorithm
    \warning "d" and "n" should be positive
   */

  INLINE double ipow_up(double d, unsigned int n)
  {
    GAOL_RND_ENTER();
    GAOL_ASSERT(d >= 0.0);

    double y = 1;
    double z = d;

    for (;;) {
      if (odd(n)) {
	n >>= 1;
	y *= z;
	if (n == 0) {
	  GAOL_RND_LEAVE();
	  return double(y);
	}
      } else {
	n>>=1;
      }
      z *= z;
    }
  }

  /*
    Computes d^n rounded downward with a binary exponentiation algorithm
    \warning "d" and "n" should be positive
   */
	INLINE double ipow_dn(double d, unsigned int n)
  	{
		GAOL_RND_ENTER();

    	GAOL_ASSERT(d >= 0.0);

	   double y = 1;
   	double z = d;

    	for (;;) {
      	if (odd(n)) {
				n >>= 1;
				y = opposite(opposite(y)*z);
				if (n == 0) {
	  				GAOL_RND_LEAVE();
	  				return y;
				}
      	} else {
				n>>=1;
      	}
      	z = opposite(opposite(z)*z);
    	}
	}

  /*!
    \brief pow() correctly rounded down

  */
  INLINE double pow_dn(double d, unsigned int e)
    {
      if (d >= 0) {
	return ipow_dn(d,e);
      } else { // d < 0
	if (even(e)) {
	  return ipow_dn(opposite(d),e);
	} else { // odd(e)
	  return opposite(ipow_up(opposite(d),e));
	}
      }
    }

  /*!
    \brief pow() correctly rounded up

  */
   INLINE double pow_up(double d, unsigned int e)
    {
      if (d >= 0) {
	return ipow_up(d,e);
      } else { // d < 0
	if (even(e)) {
	  return ipow_up(opposite(d),e);
	} else { // odd(e)
	  return opposite(ipow_dn(opposite(d),e));
	}
      }
    }

  /*!
    \brief nthroot() correctly rounded down

    Assumes a correctly rounded pow(double) when rounding direction
    is to nearest (such as ensured by the IBM math library).
    \caution The current rounding direction must be to nearest.
  */
  INLINE double nthroot_dn(double d, double e)
    {
      GAOL_RND_PRESERVE();
      round_nearest();
      double f=previous_float(upow(d,e));
      GAOL_RND_RESTORE();
      return f;
    }

  /*!
    \brief nthroot() correctly rounded up

    Assumes a correctly rounded pow(double) when rounding direction
    is to nearest (such as ensured by the IBM math library).
    \caution The current rounding direction must be to nearest.
  */
   INLINE double nthroot_up(double d, double e)
    {
      GAOL_RND_PRESERVE();
      round_nearest();
      double f=next_float(upow(d,e));
      GAOL_RND_RESTORE();
      return f;
    }

  /*!
    \brief Exponential correctly rounded downward

    Assumes a correctly rounded function exp(double) when rounding
    direction is to nearest (such as ensured by the IBM math library).
    \caution The current rounding direction must be to nearest.
  */
  INLINE double exp_dn(double d)
  {
    GAOL_RND_PRESERVE();
    round_nearest();
    double f=previous_float(uexp(d));
    GAOL_RND_RESTORE();
    return f;
  }

  /*!
    \brief Exponential correctly rounded upward

    Assumes a correctly rounded function exp(double) when rounding
    direction is to nearest (such as ensured by the IBM math library).
    \caution The current rounding direction must be to nearest.
  */
  INLINE double exp_up(double d)
  {
    GAOL_RND_PRESERVE();
    round_nearest();
    double f=next_float(uexp(d));
    GAOL_RND_RESTORE();
    return f;
  }

  INLINE double cos_dn(double d)
  {
    GAOL_RND_PRESERVE();
    round_nearest();
    double f=previous_float(ucos(d));
    GAOL_RND_RESTORE();
    return f;
  }

  INLINE double cos_up(double d)
  {
    GAOL_RND_PRESERVE();
    round_nearest();
    double f=next_float(ucos(d));
    GAOL_RND_RESTORE();
    return f;
  }

  INLINE double tan_dn(double d)
  {
    GAOL_RND_PRESERVE();
    round_nearest();
    double f=previous_float(utan(d));
    GAOL_RND_RESTORE();
    return f;
  }

  INLINE double tan_up(double d)
  {
    GAOL_RND_PRESERVE();
    round_nearest();
    double f=next_float(utan(d));
    GAOL_RND_RESTORE();
    return f;
  }

   INLINE double log_dn(double d)
  {
    GAOL_RND_PRESERVE();
    round_nearest();
    double f=previous_float(ulog(d));
    GAOL_RND_RESTORE();
    return f;
  }

  INLINE double log_up(double d)
  {
    GAOL_RND_PRESERVE();
    round_nearest();
    double f=next_float(ulog(d));
    GAOL_RND_RESTORE();
    return f;
  }


  INLINE double acos_dn(double d)
  {
    GAOL_RND_PRESERVE();
    round_nearest();
    double f=previous_float(uacos(d));
    GAOL_RND_RESTORE();
    return f;
  }

  INLINE double acos_up(double d)
  {
    GAOL_RND_PRESERVE();
    round_nearest();
    double f=next_float(uacos(d));
    GAOL_RND_RESTORE();
    return f;
  }

  INLINE double asin_dn(double d)
  {
    GAOL_RND_PRESERVE();
    round_nearest();
    double f=previous_float(uasin(d));
    GAOL_RND_RESTORE();
    return f;
  }

  INLINE double asin_up(double d)
  {
    GAOL_RND_PRESERVE();
    round_nearest();
    double f=next_float(uasin(d));
    GAOL_RND_RESTORE();
    return f;
  }

  INLINE double atan_dn(double d)
  {
    GAOL_RND_PRESERVE();
    round_nearest();
    double f=previous_float(uatan(d));
    GAOL_RND_RESTORE();
    return f;
  }

  INLINE double atan_up(double d)
  {
    GAOL_RND_PRESERVE();
    round_nearest();
    double f=next_float(uatan(d));
    GAOL_RND_RESTORE();
    return f;
  }

  INLINE double cosh_dn(double x)
  {
	GAOL_RND_PRESERVE();
	round_nearest();
	double f = previous_float(cosh(x)); // From libm, not libultim
	GAOL_RND_RESTORE();
	return f;
  }

  INLINE double cosh_up(double x)
  {
	GAOL_RND_PRESERVE();
	round_nearest();
	double f = next_float(cosh(x)); // From libm, not libultim
	GAOL_RND_RESTORE();
	return f;
  }

  INLINE double sinh_dn(double x)
  {
	GAOL_RND_PRESERVE();
	round_nearest();
	double f = previous_float(sinh(x)); // From libm, not libultim
	GAOL_RND_RESTORE();
	return f;
  }

  INLINE double sinh_up(double x)
  {
	GAOL_RND_PRESERVE();
	round_nearest();
	double f = next_float(sinh(x)); // From libm, not libultim
	GAOL_RND_RESTORE();
	return f;
  }

  INLINE double tanh_dn(double x)
  {
	  GAOL_RND_PRESERVE();
	  round_nearest();
	  double f = previous_float(tanh(x)); // From libm, not libultim
	  GAOL_RND_RESTORE();
	  return f;
  }

  INLINE double tanh_up(double x)
  {
	  GAOL_RND_PRESERVE();
	  round_nearest();
	  double f = next_float(tanh(x)); // From libm, not libultim
	  GAOL_RND_RESTORE();
	  return f;
  }




  INLINE double acosh_dn(double x)
  {
	  GAOL_RND_PRESERVE();
	  round_nearest();
	  double f = previous_float(acosh(x)); // acosh() not from libultim
	  GAOL_RND_RESTORE();
	  return f;
  }

  INLINE double acosh_up(double x)
  {
	  GAOL_RND_PRESERVE();
	  round_nearest();
	  double f = next_float(acosh(x)); // acosh() not from libultim
	  GAOL_RND_RESTORE();
	  return f;
  }

  INLINE double asinh_dn(double x)
  {
	  GAOL_RND_PRESERVE();
	  round_nearest();
	  double f = previous_float(asinh(x)); // asinh() not from libultim
	  GAOL_RND_RESTORE();
	  return f;
  }

  INLINE double asinh_up(double x)
  {
	  GAOL_RND_PRESERVE();
	  round_nearest();
	  double f = next_float(asinh(x)); // asinh() not from libultim
	  GAOL_RND_RESTORE();
	  return f;
  }

  INLINE double atanh_dn(double x)
  {
	  GAOL_RND_PRESERVE();
	  round_nearest();
	  double f = previous_float(atanh(x)); // atanh() not from libultim
	  GAOL_RND_RESTORE();
	  return f;
  }

  INLINE double atanh_up(double x)
  {
	  GAOL_RND_PRESERVE();
	  round_nearest();
	  double f = next_float(atanh(x)); // atanh() not from libultim
	  GAOL_RND_RESTORE();
	  return f;
  }



} // namespace gaol

#endif /* __gaol_double_op_apmathlib_h__ */
