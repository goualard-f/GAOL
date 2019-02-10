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
 * CVS: $Id: gaol_double_op_m.h 54 2009-03-01 13:53:01Z goualard $
 * Last modified: 
 * By: 
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_double_op_m.h
  \brief  Operations correctly rounded down and up.

  Fast version using standard arithmetic operations and the default mathematical 
	library (libm)

  \author Frédéric Goualard
  \date   2006-02-21
*/


#ifndef __gaol_double_op_m_h__
#define __gaol_double_op_m_h__

#include <cmath>
#include <limits>
#include "gaol/gaol_config.h"
#include "gaol/gaol_port.h"
#include "gaol/gaol_fpu.h"
#include "gaol/gaol_assert.h"
#include "gaol/gaol_common.h"


namespace gaol {

  static const double small_quantity_neg = (1.0-2.0*std::numeric_limits<double>::epsilon());
  static const double small_quantity_pos = (1.0+2.0*std::numeric_limits<double>::epsilon());

  /*
    \brief Downward rounding by adding a small quantity to the parameter
    \note The error estimation is 1 ulp
  */
  INLINE double round_down(double d)
  {
	if (d == -GAOL_INFINITY) {
 		return -std::numeric_limits<double>::max();
	} else {
		
    return ((d<0.0) ? small_quantity_pos*d-std::numeric_limits<double>::min() 
	    : small_quantity_neg*d-std::numeric_limits<double>::min());
  }

  /*
    \brief Upward rounding by adding a small quantity to the parameter
    \note The error estimation is 1 ulp
  */
  INLINE double round_up(double d)
  {
    return ((d<0.0) ? small_quantity_neg*d+std::numeric_limits<double>::min() 
	    : small_quantity_pos*d+std::numeric_limits<double>::min());
  }

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
      GAOL_RND_ENTER();
      round_nearest();
      double f=round_down(pow(d,e));
      GAOL_RND_LEAVE();
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
      GAOL_RND_ENTER();
      round_nearest();
      double f=round_up(pow(d,e));
      GAOL_RND_LEAVE();
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
    GAOL_RND_ENTER();
    round_nearest();
    double f=round_down(exp(d));
    GAOL_RND_LEAVE();
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
    GAOL_RND_ENTER();
    round_nearest();
    double f=round_up(exp(d));
    GAOL_RND_LEAVE();
    return f;
  }

  INLINE double cos_dn(double d)
  {
    GAOL_RND_ENTER();
    round_nearest();
    double f=round_down(cos(d));
    GAOL_RND_LEAVE();
    return f;
  }

  INLINE double cos_up(double d)
  {
    GAOL_RND_ENTER();
    round_nearest();
    double f=round_up(cos(d));
    GAOL_RND_LEAVE();
    return f;
  }

  INLINE double tan_dn(double d)
  {
    GAOL_RND_ENTER();
    round_nearest();
    double f=round_down(tan(d));
    GAOL_RND_LEAVE();
    return f;
  }

  INLINE double tan_up(double d)
  {
    GAOL_RND_ENTER();
    round_nearest();
    double f=round_up(tan(d));
    GAOL_RND_LEAVE();
    return f;
  }

   INLINE double log_dn(double d)
  {
    GAOL_RND_ENTER();
    round_nearest();
    double f=round_down(log(d));
    GAOL_RND_LEAVE();
    return f;
  }

  INLINE double log_up(double d)
  {
    GAOL_RND_ENTER();
    round_nearest();
    double f=round_up(log(d));
    GAOL_RND_LEAVE();
    return f;
  }


  INLINE double acos_dn(double d)
  {
    GAOL_RND_ENTER();
    round_nearest();
    double f=round_down(acos(d));
    GAOL_RND_LEAVE();
    return f;
  }

  INLINE double acos_up(double d)
  {
    GAOL_RND_ENTER();
    round_nearest();
    double f=round_up(acos(d));
    GAOL_RND_LEAVE();
    return f;
  }

  INLINE double asin_dn(double d)
  {
    GAOL_RND_ENTER();
    round_nearest();
    double f=round_down(asin(d));
    GAOL_RND_LEAVE();
    return f;
  }

  INLINE double asin_up(double d)
  {
    GAOL_RND_ENTER();
    round_nearest();
    double f=round_up(asin(d));
    GAOL_RND_LEAVE();
    return f;
  }

  INLINE double atan_dn(double d)
  {
    GAOL_RND_ENTER();
    round_nearest();
    double f=round_down(atan(d));
    GAOL_RND_LEAVE();
    return f;
  }

  INLINE double atan_up(double d)
  {
    GAOL_RND_ENTER();
    round_nearest();
    double f=round_up(atan(d));
    GAOL_RND_LEAVE();
    return f;
  }

  INLINE double cosh_dn(double x)
  {
	GAOL_RND_ENTER();
	round_nearest();
	double f = round_down(cosh(x)); // From libm, not libultim
	GAOL_RND_LEAVE();
	return f;
  }
  
  INLINE double cosh_up(double x)
  {
	GAOL_RND_ENTER();
	round_nearest();
	double f = round_up(cosh(x)); // From libm, not libultim
	GAOL_RND_LEAVE();
	return f;
  }

  INLINE double sinh_dn(double x)
  {
	GAOL_RND_ENTER();
	round_nearest();
	double f = round_down(sinh(x)); // From libm, not libultim
	GAOL_RND_LEAVE();
	return f;
  }
  
  INLINE double sinh_up(double x)
  {
	GAOL_RND_ENTER();
	round_nearest();
	double f = round_up(sinh(x)); // From libm, not libultim
	GAOL_RND_LEAVE();
	return f;
  }

  INLINE double tanh_dn(double x)
  {
	  GAOL_RND_ENTER();
	  round_nearest();
	  double f = round_down(tanh(x)); // From libm, not libultim
	  GAOL_RND_LEAVE();
	  return f;
  }
  
  INLINE double tanh_up(double x)
  {
	  GAOL_RND_ENTER();
	  round_nearest();
	  double f = round_up(tanh(x)); // From libm, not libultim
	  GAOL_RND_LEAVE();
	  return f;
  }




  INLINE double acosh_dn(double x)
  {
	  GAOL_RND_ENTER();
	  round_nearest();
	  double f = round_down(acosh(x)); // From libm, not libultim
	  GAOL_RND_LEAVE();
	  return f;
  }
  
  INLINE double acosh_up(double x)
  {
	  GAOL_RND_ENTER();
	  round_nearest();
	  double f = round_up(acosh(x)); // From libm, not libultim
	  GAOL_RND_LEAVE();
	  return f;
  }

  INLINE double asinh_dn(double x)
  {
	  GAOL_RND_ENTER();
	  round_nearest();
	  double f = round_down(asinh(x)); // From libm, not libultim
	  GAOL_RND_LEAVE();
	  return f;
  }
  
  INLINE double asinh_up(double x)
  {
	  GAOL_RND_ENTER();
	  round_nearest();
	  double f = round_up(asinh(x)); // From libm, not libultim
	  GAOL_RND_LEAVE();
	  return f;
  }

  INLINE double atanh_dn(double x)
  {
	  GAOL_RND_ENTER();
	  round_nearest();
	  double f = round_down(atanh(x)); // From libm, not libultim
	  GAOL_RND_LEAVE();
	  return f;
  }
  
  INLINE double atanh_up(double x)
  {
	  GAOL_RND_ENTER();
	  round_nearest();
	  double f = round_up(atanh(x)); // From libm, not libultim
	  GAOL_RND_LEAVE();
	  return f;
  }



} // namespace gaol

#endif /* __gaol_double_op_m_h__ */
