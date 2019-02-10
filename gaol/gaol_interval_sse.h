/*-*-C++-*------------------------------------------------------------------
 * gaol -- NOT Just Another Interval Library
 *--------------------------------------------------------------------------
 * This file is part of the gaol distribution. Gaol was primarily
 * developed at the Swiss Federal Institute of Technology, Lausanne,
 * Switzerland, and is now developed at the Laboratoire d'Informatique de
 * Nantes-Atlantique, France.
 *
 * Copyright (c) 2001 Swiss Federal Institute of Technology, Switzerland
 * Copyright (c) 2002-2009 Laboratoire d'Informatique de
 *                         Nantes-Atlantique, France
 *--------------------------------------------------------------------------
 * gaol is a software distributed WITHOUT ANY WARRANTY. Read the associated
 * COPYING file for information.
 *--------------------------------------------------------------------------
 * SVN: $Id: gaol_interval_sse.h 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By: Frederic Goualard <Frederic.Goualard@univ-nantes.fr>
 *--------------------------------------------------------------------------*/

/*
    This file shall be included by gaol_interval.h only. It must not be included directly
        by any other file.
    This file contains all declarations related to the 'interval' class that consider
    the bounds to be an __m128d variable.
**/

#ifndef __gaol_interval_h__
#  error "File gaol_interval_sse.h shall only be included directly by gaol_interval.h"
#endif

#ifndef __gaol_interval_sse_h__
#define __gaol_interval_sse_h__

  __GAOL_PUBLIC__ std::ostream& operator<<(std::ostream& out, const __m128d& x);

  INLINE interval interval::universe(void)
    {
      return interval(interval::m128_infinf);
    }

  INLINE interval interval::zero(void)
    {
      return interval(interval::m128_zero);
    }

  INLINE interval interval::emptyset(void)
    {
      return interval(std::numeric_limits<double>::quiet_NaN());
    }

  INLINE interval::interval(const __m128d& xmm)
    {
      xmmbounds = xmm;
    }

  INLINE interval interval::positive(void)
    {
      return interval(0.0,std::numeric_limits<double>::infinity());
    }

  INLINE interval interval::negative(void)
    {
      return interval(-std::numeric_limits<double>::infinity(),0.0);
    }

  INLINE interval::interval(double v)
    {
      xmm2d tmp = {-v, v};
      xmmbounds = _mm_load_pd(tmp);
    }

  INLINE interval::interval(double l, double r)
    {
      xmm2d tmp = {-l, r};
      xmmbounds = _mm_load_pd(tmp);
    }

  INLINE interval::interval()
    {
      xmmbounds = _mm_set1_pd(std::numeric_limits<double>::infinity());
    }


  INLINE interval::interval(const interval& I)
    {
      xmmbounds = I.xmmbounds;
    }

	 INLINE interval& interval::operator&=(const interval& I)
	 {
	   if (is_empty()) {
      	return *this;
    	}
		xmm2d bd;
		_mm_store_pd(bd,xmmbounds);
		xmm2d Ibd;
        _mm_store_pd(Ibd,I.xmmbounds);

    	// From now on, "this" is known to be nonempty.
	    if (!(Ibd[0] >= bd[0])) { // I.left() == NaN => lb_ <- NaN
      	bd[0] = Ibd[0];
    	}
    	if (!(Ibd[1] >= bd[1])) {
	      bd[1] = Ibd[1];
    	}

        xmmbounds = _mm_load_pd(bd);
    	return *this;
	 }

	 INLINE interval& interval::operator|=(const interval& I)
	 {
	   if (is_empty()) {
      	*this = I;
	      return *this;
    	}
	   if (I.is_empty()) {
      	return *this;
    	}
		xmm2d bd, Ibd;
		_mm_store_pd(bd,xmmbounds);
        _mm_store_pd(Ibd,I.xmmbounds);

    	if (Ibd[0] > bd[0]) { // Left bounds negated
      	bd[0] = Ibd[0];
    	}
    	if (Ibd[1] > bd[1]) {
      	bd[1] = Ibd[1];
    	}
		xmmbounds = _mm_load_pd(bd);
    	return *this;
   }

  INLINE double interval::left_internal() const
    {
      GAOL_ALIGN16(double l);
      _mm_storel_pd(&l,xmmbounds);
      return l;
    }
  INLINE double interval::right_internal() const
    {
      GAOL_ALIGN16(double r);
      _mm_storeh_pd(&r,xmmbounds);
      return r;
    }

 INLINE void interval::get_bounds(interval::xmm2d& b) const
    {
      _mm_store_pd(b,xmmbounds);
    }

  INLINE const __m128d& interval::get_xmminterval(void) const
    {
      return xmmbounds;
    }

  INLINE __m128d& interval::get_xmminterval(void)
    {
      return xmmbounds;
    }



#endif // __gaol_interval_sse_h__
