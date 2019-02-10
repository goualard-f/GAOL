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
 * CVS: $Id: gaol_interval.cpp 281 2015-05-23 12:50:28Z goualard $
 * Last modified: Sun May 10 22:40:50 2009 on almighty
 * By: Frederic Goualard <Frederic.Goualard@univ-nantes.fr>
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_interval.cpp
  \brief

  <long description>

  \author Goualard Fr�d�ric
  \date   2001-09-28
*/

// TODO: Rewrite as many functions as possible to use SIMD instructions
// FIXME: Use of f_negate deprecates -ffloat-store?

#include <iostream>
#include <iomanip>
#include <ios>
#include <sstream>
#include <cmath>
#include <string>
#include <cerrno>
#include <sstream>

#if USING_SSE2_INSTRUCTIONS
#  include <pmmintrin.h>
#endif

#include "gaol/gaol_config.h"
#include "gaol/gaol_limits.h"
#include "gaol/gaol_fpu.h"
#include "gaol/gaol_common.h"
#include "gaol/gaol_parser.h"
#include "gaol/gaol_port.h"
#include "gaol/gaol_interval.h"
#include "gaol/gaol_flt_output.h"
#include "gaol/gaol_parameters.h"
#include "gaol/gaol_limits.h"
#include "gaol/gaol_exceptions.h"

using std::ostream;
using std::istream;
using std::sqrt;

//debug
using std::cout;
using std::endl;


namespace gaol {

const interval interval::cst_two_pi(2.0*pi_dn,2.0*pi_up); // No rounding when multiplying by 2
const interval interval::cst_pi(pi_dn,pi_up);
const interval interval::cst_half_pi(half_pi_dn,half_pi_up);
const interval interval::cst_one_plus_infinity(1.0,GAOL_INFINITY);
const interval interval::cst_one(1.0);
const interval interval::cst_minus_one_plus_one(-1.0,1.0);



#if USING_SSE2_INSTRUCTIONS
#  include "gaol/gaol_interval_sse.cpp"
#else
#  include "gaol/gaol_interval_fpu.cpp"
#endif // USING_SSE2_INSTRUCTIONS



  // Default format for output
  interval_format::format_t interval::output = interval_format::bounds;

  // Number of digits to print.
  std::streamsize interval::output_precision = 16;

  // Declaring some prototypes included by the files below.
  double inv_dn(double);
  double inv_up(double);

  /*
    \brief test for evenness
    \warning d should not be +/-oo
  */
  bool feven(double d)
  {
    return (std::floor(0.5*d)*2.0 == d);
  }





  /*!
    \brief Computes 1/a rounded upward
  */
  INLINE double inv_up(double a)
  {
    GAOL_RND_ENTER();
    double tmp = 1.0/a;
    GAOL_RND_LEAVE();
    return tmp;
  }


  /*!
    \brief Computes 1/a rounded downward
  */
  INLINE double inv_dn(double a)
  {
    GAOL_RND_ENTER();
    double res = f_negate(1/(-a));
    GAOL_RND_LEAVE();
    return res;
  }


  bool interval::is_canonical(void) const
  {
#if defined (_MSC_VER)
    return !is_empty() && (next_float(left())>=right());
#else
    // emptyset handled thanks to unorderedness of NaNs
    return next_float(left())>=right();
#endif
  }


  interval::interval(const char *const s)
  {
    interval tmp;
    bool ok = parse_interval(s,tmp);
    if (!ok) {
      std::string err_msg("Syntax error in interval initialization: ");
      err_msg += s;
      *this = interval::emptyset();
      GAOL_ERRNO = -1;
      gaol_ERROR(input_format_error,err_msg.c_str());
    } else {
#if USING_SSE2_INSTRUCTIONS
      xmmbounds = tmp.xmmbounds;
#else
      lb_ = tmp.lb_;
      rb_ = tmp.rb_;
#endif
    }
  }

  interval::interval(const char *const sl, const char *const sr)
  {
    interval tmpl, tmpr;
    if (!parse_interval(sl,tmpl)) {
      std::string err_msg("Syntax error in left bound initialization: ");
      err_msg += sl;
      *this = interval::emptyset();
      GAOL_ERRNO = -1;
      gaol_ERROR(input_format_error,err_msg.c_str());
    }
    if (!parse_interval(sr,tmpr)) {
	  std::string err_msg("Syntax error in right bound initialization: ");
      err_msg += sr;
      *this = interval::emptyset();
      GAOL_ERRNO = -1;
      gaol_ERROR(input_format_error,err_msg.c_str());
    }
#if USING_SSE2_INSTRUCTIONS
    xmm2d tmp = {tmpl.left_internal(), tmpr.right_internal()};
    xmmbounds = _mm_load_pd(tmp);
#else
    lb_ = tmpl.lb_;
    rb_ = tmpr.rb_;
#endif
  }

  void interval::format(interval_format::format_t f)
  {
    output = f;
  }

  interval_format::format_t interval::format(void)
  {
    return output;
  }


  istream& operator >>(istream& is, interval& I)
  {
    std::string buffer;

    getline(is,buffer);

    if (!parse_interval(buffer.c_str(),I)) {
      std::string err_msg("Syntax error in expression of interval: ");
      err_msg += buffer;
      I = interval::emptyset();
      gaol_ERROR(input_format_error,err_msg.c_str());
    }
    return is;
  }

  void display_bounds(double l, double r, ostream& os)
  {
    if (!(l <= r)) {
      os << "[empty]";
    } else {
      if (l == r) {
				round_downward();
				os << '<';
				os << l; //dtoa_downward(l,os);
				os << ", ";
				round_upward();
				os << r; //dtoa_upward(l,os);
				os << '>';
      } else {
				os << '[';
				round_downward();
				os << l; //dtoa_downward(l,os);
				os << ", ";
				round_upward();
				os << r; //dtoa_upward(r,os);
				os << ']';
      }
    }
  }


  ostream& operator<<(ostream& os, const interval& I)
  {
    //    double l = ((I.left()==0.0) ? 0.0  : I.left()); // Avoids printing -0
    //    double r = ((I.right()==0.0) ? 0.0 : I.right());  // Avoids printing -0
    GAOL_RND_ENTER();

    double l = I.left(), r = I.right();

    os.precision(interval::precision());

    switch (interval::format()) {
    case interval_format::bounds: // Display in the form "[ l, r ]"
      display_bounds(l,r,os);
      break;
    case interval_format::hexa: // Display in the form [H L, H L]
      if (I.is_empty()) {
				os << "[empty]";
      } else {
				uintdouble tmp;
				tmp.d = l;
				os.setf(std::ios_base::hex,std::ios_base::basefield);
				os << '[' <<  std::setw(8) << std::setfill('0') << HI_UINTDOUBLE(tmp)
	   			<<  std::setw(8) << std::setfill('0')  << LO_UINTDOUBLE(tmp) << ", ";
				tmp.d = r;
				os << std::setw(8) << std::setfill('0') << HI_UINTDOUBLE(tmp)
	   			<< std::setw(8) << std::setfill('0') << LO_UINTDOUBLE(tmp) << ']';
				os.setf(std::ios_base::dec,std::ios_base::basefield);
      }
      break;
    case interval_format::width: // Display in the form "c (+/- w)"
      if (I.is_empty()) {
				os << "empty";
      } else {
				if (l == r) {
	  			os << l;
				} else {
	  			round_nearest();
	  			if (l == -GAOL_INFINITY) {
	    			if (r == GAOL_INFINITY) { // [-oo, +oo]
	      			os << 0.0;
	    			} else {                        // [-oo, x]
	      			os << -std::numeric_limits<double>::max();
	    			}
	  			} else {
	    			if (r == GAOL_INFINITY) { // [x, +oo]
	      			os << std::numeric_limits<double>::max();
	    			} else {                        // [x, y]
	      			os << ((l+r)/2.0);
	    			}
	  			}
	  				os << " (+/- " << ((r-l)/2.0) << ")";
				}
      }
      break;
    case interval_format::center: // Display in the form "c"
      if (I.is_empty()) {
				os << "empty";
      } else {
				if (l == r) {
	  			os << l;
				} else {
	  			round_nearest();
	  			if (l == -GAOL_INFINITY) {
	    			if (r == GAOL_INFINITY) { // [-oo, +oo]
	      			os << 0.0;
	    			} else {                        // [-oo, x]
	      			os << -std::numeric_limits<double>::max();
	    			}
	  			} else {
	    			if (r == GAOL_INFINITY) { // [x, +oo]
	      			os << std::numeric_limits<double>::max();
	    			} else {                        // [x, y]
	      			os << ((l+r)/2.0);
	    			}
	  			}
				}
      }
      break;
    case interval_format::agreeing:
      if (I.is_empty()) {
				os << "[empty]";
      } else {
				if (I.right() > 10*I.left()) {
	  			display_bounds(l,r,os);
				} else {
	  			std::ostringstream lbound, rbound;
	  			std::string itv;
	  			lbound.precision(interval::precision());
	  			rbound.precision(interval::precision());

				  round_downward();
	  			lbound << std::showpoint << I.left();

				  round_upward();
	  			rbound << std::showpoint << I.right();

				  unsigned int i = 0;
	  			while (lbound.str()[i] == rbound.str()[i]) {
	    			itv += lbound.str()[i];
	    			++i;
	  			}
	  			size_t lblen = lbound.str().length();
	  			size_t rblen = rbound.str().length();

	  			// Some digits not in common?
	  			if (i < lblen || i < rblen) {
	    			itv += "~[";
	    			if (i >= lbound.str().length()) {
	      			itv += "0";
	    			} else {
	      			// Removing trailing zeroes
	      			size_t last_not_zero = lbound.str().find_last_not_of("0");
	      			itv += lbound.str().substr(i,last_not_zero-i+1);
	    			}
	    			itv += ", ";
	    			if (i >= rbound.str().length()) {
	      			itv += "0";
	    			} else {
	      			size_t last_not_zero = lbound.str().find_last_not_of("0");
	      			itv += rbound.str().substr(i,last_not_zero-i+1);
	    			}
	    			itv += "]";
	  			}
	  			os << itv;
				}
      }
    }
    GAOL_RND_LEAVE();
    return os;
  }



	interval pow(const interval& I, int n)
	{
		if (I.is_empty()) {
			return I;
		}
		if (n < 0) {
			return inverse(uipow(I,-n)); // 1/uipow(I,-n)
		} else {
			if (n > 0) {
				return uipow(I,n);
			} else {
				return interval(1.0);
			}
		}
	}

  interval pow(const interval &I, const interval &J)
  {
	// FIXME: this function is not fully tested yet. Use with care
    if (J.is_an_int()) { // Degenerate case
      return pow(I,int(J.left()));
    }

    if (I.certainly_positive()) {
      return exp(J*log(I));
    }

    if (I.certainly_negative()) {
      interval tmp = exp(J*log(abs(I)));
      return interval(-tmp.left(),tmp.right());
    }

    // I.strictly_straddles_zero()
    return pow(interval(I.left(),0),J) | pow(interval(0,I.right()),J);
  }

  /*
    Code inspired by ia_math code by Timothy Hickey
  */
  interval nth_root_rel(const interval& J, unsigned int n, const interval& I)
  {
    switch (n) {
    case 0:
      if (J.set_contains(1.0)) {
	return I;
      } else {
	return interval::emptyset();
      }
    case 1:
      return J & I;
    case 2:
      return sqrt_rel(J,I);
    default:
      if (odd(n)) {
	if (J.is_empty() || I.is_empty()) {
	  return interval::emptyset();
	}
	interval inv_n = interval::one()/double(n);
	double n_lo = inv_n.left();
	double n_hi = inv_n.right();
	double l, r;
	if (J.left() >= 1.0) {
	  l = nthroot_dn(J.left(),n_lo);
	} else {
	  if (J.left() >= 0.0) {
	    l = nthroot_dn(J.left(),n_hi);
	  } else {
	    if (J.left() >= -1.0) {
	      /*
		current nthroot_up implementation does not work with non-integral
		exponent for negative first argument
	      */
	      l = -nthroot_up(-J.left(),n_lo);
	    } else {
	      l = -nthroot_up(-J.left(),n_hi);
	    }
	  }
	}
	if (J.right() >= 1.0) {
	  r = nthroot_up(J.right(),n_hi);
	} else {
	  if (J.right() >= 0.0) {
	    r = nthroot_up(J.right(),n_lo);
	  } else {
	    if (J.right() >= -1.0) {
	      r = -nthroot_dn(-J.right(),n_hi);
	    } else {
	      r = -nthroot_dn(-J.right(),n_lo);
	    }
	  }
	}
	return (interval(l,r) & I);
      }
      // n is even
      interval Jpos = interval(maximum(0.0,J.left()),J.right());
      if (Jpos.is_empty() || I.is_empty()) {
	return interval::emptyset();
      }
      interval inv_n = interval::one()/double(n);
      double n_lo = inv_n.left();
      double n_hi = inv_n.right();
      double l, r;
      if (Jpos.left() >= 1.0) {
	l = nthroot_dn(Jpos.left(),n_lo);
      } else {
	l = nthroot_dn(Jpos.left(),n_hi);
      }
      
      if (Jpos.right() >= 1.0) {
	r = nthroot_up(Jpos.right(),n_hi);
      } else {
	r = nthroot_up(Jpos.right(),n_lo);
      }
      interval tmp(l,r);
      if (I.certainly_positive()) {
	return (tmp & I);
      }
      if (I.certainly_negative()) {
	return (-tmp & I);
      }
      return ((tmp & I) | ((-tmp) & I));
    }
  }

  /*
    Code inspired from ia_math by Timothy Hickey
  */
interval nth_root(const interval& I, unsigned int n)
{
	switch (n) {
	case 0:
		return interval::emptyset();
	case 1:
		return I;
	case 2:
		return sqrt(I);
	default:
		interval Ipos = interval(maximum(0.0,I.left()),I.right());
		if (Ipos.is_empty()) {
			return interval::emptyset();
		}
		interval inv_n = interval(1.0)/double(n);
		double n_lo = inv_n.left();
		double n_hi = inv_n.right();
		double l, r;
		if (odd(n)) {
			if (Ipos.left() >= 1.0) {
				l = nthroot_dn(Ipos.left(),n_lo);
			} else {
				l = nthroot_dn(Ipos.left(),n_hi);
			}
			if (I.right() >= 1.0) {
				r = nthroot_up(I.right(),n_hi);
			} else {
				r = nthroot_up(I.right(),n_lo);
			}
			return interval(l,r);
		}
		// n is even
		if (Ipos.left() >= 1.0) {
			l = nthroot_dn(Ipos.left(),n_lo);
		} else {
			l = nthroot_dn(Ipos.left(),n_hi);
		}

		if (Ipos.right() >= 1.0) {
			r = nthroot_up(Ipos.right(),n_hi);
		} else {
			r = nthroot_up(Ipos.right(),n_lo);
		}
		return interval(l,r);
	}
}

  ULONGLONGINT nb_fp_numbers(double a, double b)
  {
    if (!is_finite(a) || !is_finite(b) || (a > b)) {
      // Either a or b is a NaN or +/-oo, or [a,b] is empty?
      gaol_ERROR(invalid_action_error,"invalid argument(s) in call to nb_fp_numbers()");
      return std::numeric_limits<ULONGLONGINT>::max();
    }

    if (a == b) {
      return 1;
    }

    ullidouble ai, bi;
    ai.d = a;
    bi.d = b;
    if (a >= 0) {
      return (bi.i-ai.i)+1;
    }
    if (b <= 0) {
      return (ai.i-bi.i)+1;
    }
    ullidouble zi;
    zi.d = 0.0;
    ai.d = -ai.d;
    return (bi.i-zi.i)+(ai.i-zi.i)+1;
  }

  interval exp(const interval& I)
  {
	/* We intersect the result with [0, +oo] to ensure that the result is strictly positive
 		Otherwise, we might have: exp([-oo, -MAX] = [-v, +v] with v very small.
	*/
    return interval(maximum(0.0,exp_dn(I.left())),exp_up(I.right()));
  }

  interval log(const interval& I)
  {
    interval J = interval(maximum(0.0,I.left()), I.right());

    if (J.is_empty()) {
      return interval::emptyset();
    }

    return interval(log_dn(J.left()), log_up(J.right()));
  }


  /*!
    \brief Angle modulo pi (fast, boolean version)

    Computes k_left and k_right for I=[a,b] such that
    * a = k_left*\pi + a'  with |a'|<\pi
    * b = k_right*\pi + b' with |b'|<\pi
    \return true whevener k_left and k_right could be found and false
    otherwise.
    \note if I.left() (resp. I.right()) is very large, there might be
    more than one integer in I.left()/interval::pi() (resp.
    I.right()/interval::pi()).
  */
  bool fast_modulo_k_pi(const interval &I, double &k_left, double &k_right)
  {
    interval kl = floor(I.left()/interval::pi());
    if (kl.left() != kl.right()) {
      return false;
    }

    interval kr = floor(I.right()/interval::pi());
    if (kr.left() != kr.right()) {
      return false;
    }
    k_left  = kl.left();
    k_right = kr.right();
    return true;
  }


  /*!
    \brief Angle modulo pi (slower version with integer output)

    Computes k_left and k_right for I=[a,b] such that
    * a = k_left*\pi + a'  with |a'|<\pi
    * b = k_right*\pi + b' with |b'|<\pi
    \return 0 if neither l_left nor l_right could be computed accurately,
    1 if l_right could be computed, 2 if l_left could be computed and
    3 if both could be computed.

    \note There might be more than one integer in
    I.left()/interval::pi() (resp. I.right()/interval::pi()) when one of the bounds of I
    is close to a multiple of \pi
  */
  unsigned short int modulo_k_pi(const interval &I, double &k_left, double &k_right)
  {
    interval kl = floor(I.left()/interval::pi());
    interval kr = floor(I.right()/interval::pi());
    k_left  = kl.left();
    k_right = kr.right();
    return (unsigned short int)(kl.is_a_double() << 1) + (unsigned short int)kr.is_a_double();
  }


  interval tan(const interval& I)
  {
    GAOL_RND_ENTER();
    // Empty set?
    if (I.is_empty()) {
      GAOL_RND_LEAVE();
      return interval::emptyset();
    }

    if (I.width() >= pi_up) {
      GAOL_RND_LEAVE();
      return interval::universe();
    }

    /*
      If I is outside [-2^52,2^52], it is useless to compute
      the tangent because the rounding error in modulo_k_pi
      would forbid computing k_left or k_right accurately
    */
    if (I.right() < -two_power_52 || I.left() > two_power_52) {
      GAOL_RND_LEAVE();
      return interval::universe();
    }

    double kl, kr;
    if (!fast_modulo_k_pi(I+interval::half_pi(),kl,kr) || (kl != kr)) {
      GAOL_RND_LEAVE();
      return interval::universe();
    }
    GAOL_RND_LEAVE();
    return interval(tan_dn(I.left()),tan_up(I.right()));
  }


  interval acos(const interval& I)
  {
    interval J = I & interval::minus_one_plus_one();
    // J <- I \cap [-1,1]

    if (J.is_empty()) {
      return interval::emptyset();
    }

    return interval(acos_dn(J.right()),acos_up(J.left()));
  }

  interval asin(const interval& I)
  {
    interval J= I & interval::minus_one_plus_one();
    // J <- I \cap [-1,1]

    if (J.is_empty()) {
      return interval::emptyset();
    }
    return interval(asin_dn(J.left()),asin_up(J.right()));
  }

  interval atan(const interval& I)
  {
	if (I.is_empty()) {
	  return I;
	}
    return interval(atan_dn(I.left()),atan_up(I.right()));
  }

  interval atan2(const interval& Y, const interval& X)
  {
    gaol_ERROR(unavailable_feature_error,"atan2 not yet implemented");
    return interval::emptyset();
  }

  interval cosh(const interval& I)
  {
	if (I.is_empty()) {
		return I;
	}
    if (I.right() < 0) {
      return interval(cosh_dn(I.right()),cosh_up(I.left()));
    }
    if (I.left() > 0) {
      return interval(cosh_dn(I.left()),cosh_up(I.right()));
    }
    // 0 \in I
    double abs_l = -I.left();
    if (abs_l >= I.right()) {
      return interval(1.0,cosh_up(abs_l));
    } else {
      return interval(1.0,cosh_up(I.right()));
    }
  }

  interval sinh(const interval& I)
  {
	if (I.is_empty()) {
		return I;
	}
	return interval(sinh_dn(I.left()),sinh_up(I.right()));
  }

  interval tanh(const interval& I)
  {
	if (I.is_empty()) {
		return I;
	}
  	return interval(tanh_dn(I.left()),tanh_up(I.right())) & interval::minus_one_plus_one();
  }


  interval acosh(const interval& I)
  {
    interval J = I &  interval::one_plus_infinity();

    if (J.is_empty()) {
      return J;
    }

  	return interval(acosh_dn(J.left()),acosh_up(J.right()));
  }



  interval asinh(const interval& I)
  {
    if (I.is_empty()) {
      return I;
    }

    return interval(asinh_dn(I.left()),asinh_up(I.right()));

  }

  interval atanh(const interval& I)
  {
	  interval J = I & interval::minus_one_plus_one();
    if (J.is_empty()) {
      return interval::emptyset();
    }
    return interval(atanh_dn(J.left()),atanh_up(J.right()));
  }

  interval acos_k(double i, const interval &Jacos)
  {

    if (!feven(i)) {
      return ((i+1)*interval::pi() - Jacos);
    } else { // even
      return (i*interval::pi() + Jacos);
    }
  }


  interval acos_i(double i, const interval &J, const interval &I)
  {
    double l, r;

    if (J.right() > 1.0) {
      l = 0.0;
    } else {
      l = acos_dn(J.right());
    }

    if (J.left() < -1.0) {
      r = pi_up;
    } else {
      r = acos_up(J.left());
    }

    if (!feven(i)) {
      return ((i+1)*interval::pi()-interval(l,r)) & I;
    } else { // even
      return (i*interval::pi()+interval(l,r)) & I;
    }
  }

  interval acos_rel(const interval& J, const interval &I)
  {
    if (J.is_empty() || I.is_empty() || J.left() > 1.0 || J.right() < -1.0) {
      return interval::emptyset();
    }
    if (J.set_contains(interval::minus_one_plus_one())) {
      return I;
    }
    GAOL_RND_ENTER();

    double kl = 0.0, kr = 0.0; // Meaningless definitions to keep the compiler happy
    interval Jacos = acos(J);

    interval Ileft;
    // Checking whether the left bound is too large to perform
    // a reliable range reduction (i.e., kl would be off by more than 1)
    if (I.left() < -two_power_52 || I.left() > two_power_52) {
      Ileft = I;
    } else {
      round_downward();
      if (I.left() < 0) {
	kl = std::floor(I.left()/interval::pi().left());
      } else {
	if (I.left() > 0) {
	  kl = std::floor(I.left()/interval::pi().right());
	} else {
	  kl = 0;
	}
      }
      round_upward();
      // From here, kl is at most off by 1 less than the true value
      Ileft = acos_k(kl,Jacos) & I;
      if (Ileft.is_empty()) {
	Ileft = acos_k(kl+1,Jacos) & I;
      }
    }
    interval Iright;
    // Checking whether the right bound is too large to perform
    // a reliable range reduction (i.e. kr would be off by more than 1)
    if (I.right() < -two_power_52 || I.right() > two_power_52) {
      Iright = I;
    } else {
      if (I.right() < 0) {
	kr = std::floor(I.right()/interval::pi().right());
      } else {
	if (I.right() > 0) {
	  kr = std::floor(I.right()/interval::pi().left());
	} else {
	  kr = 0;
	}
      }
      // From here, kr is at most off by 1 more than the true value
      if (kr == kl) {
	Iright = Ileft;
      } else {
	Iright = acos_k(kr,Jacos) & I;
	if (Iright.is_empty()) {
	  Iright = acos_k(kr-1,Jacos) & I;
	}
      }
    }
    GAOL_RND_LEAVE();
    return interval(Ileft.left(),Iright.right());
  }




  interval asin_rel(const interval& J, const interval &I)
  {
    return I & (interval::half_pi()+acos_rel(J,I-interval::half_pi()));
  }

  /*
    Returns the multiple of $\pi$ on which to project tan(x)
  */
  INLINE double tan_period(double x)
  {
    return std::floor((std::floor(x)+1.0)/2.0);
  }

  interval atan_rel(const interval& J, const interval& I)
  {
    if (I.is_empty() || J.is_empty()) {
      return interval::emptyset();
    }
    GAOL_RND_ENTER();
    double kl, kr;
    interval atanJ = atan(J);

    interval Ileft;
    // Checking whether the left bound is too large to perform
    // a reliable range reduction (i.e., kl would be off by more than 1)
    if (I.left() < - two_power_51 || I.left() > two_power_51) {
      Ileft = I;
    } else {
      round_downward();
      if (I.left() < 0) {
	kl = tan_period(I.left()/interval::half_pi().left());
      } else {
	if (I.left() > 0) {
	  kl = tan_period(I.left()/interval::half_pi().right());
	} else {
	  kl = 0;
	}
      }
      round_upward();
      // From here, kl is at most off by 1 less than the true value
      interval tmp = atanJ + kl*interval::pi();
      Ileft = tmp & I;
      if (Ileft.is_empty()) {
	Ileft = (tmp + interval::pi()) & I;
      }
    }
    interval Iright;
    // Checking whether the right bound is too large to perform
    // a reliable range reduction (i.e. kr would be off by more than 1)
    if (I.right() < -two_power_51 || I.right() > two_power_51) {
      Iright = I;
    } else {
      if (I.right() < 0) {
	kr = tan_period(I.right()/interval::half_pi().right());
      } else {
	if (I.right() > 0) {
	  kr = tan_period(I.right()/interval::half_pi().left());
	} else {
	  kr = 0;
	}
      }
      // From here, kr is at most off by 1 more than the true value
      if (kr == kl) {
	Iright = Ileft;
      } else {
	interval tmp = atanJ + kr*interval::pi();
	Iright = tmp & I;
	if (Iright.is_empty()) {
	  Iright = (tmp - interval::pi()) & I;
	}
      }
    }
    GAOL_RND_LEAVE();
    return interval(Ileft.left(),Iright.right());
  }

  interval acosh_rel(const interval &J, const interval &I)
  {
    if (I.is_empty() || J.is_empty()) {
      return interval::emptyset();
    }

    interval tmp = acosh(J);

    if (I.certainly_positive()) {
      return I & tmp;
    }
    if (I.certainly_negative()) {
      return I & (-tmp);
    }
    return (I & -tmp) | (I & tmp);
  }

  interval asinh_rel(const interval &J, const interval &I)
  {
    return asinh(J) & I;
  }

  interval atanh_rel(const interval &J, const interval &I)
  {
    return atanh(J) & I;
  }

  /*
   * abs
   */
  interval abs(const interval& I)
  {
    if (I.is_empty()) {
      return I;
    }

    if (I.certainly_positive()) {
      return I;
    }
    if (I.certainly_negative()) {
      return interval(-I.right(),-I.left());
    }
    return interval(0.0,maximum(-I.left(),I.right()));
  }

  /*
   * invabs_rel
   */
  interval invabs_rel(const interval &J, const interval &I)
  {
    if (I.certainly_geq(interval::zero())) {
      return (J & I);
    }

    if (I.certainly_leq(interval::zero())) {
      return ((-J) & I);
    }
    return (J&I) | ((-J)&I);
  }


  double chi(const interval &I)
  {
    if (I.is_zero()) {
      return -1.0;
    } else {
      if (!I.is_finite()) {
	if (I.set_eq(interval::universe())) {
	  return 1.0;
	} else {
	  return 0.0;
	}
      } else {
	double res;
	GAOL_RND_PRESERVE();
	round_nearest();
	if (std::fabs(I.left()) <= std::fabs(I.right())) {
	  res = I.left() / I.right();
	} else {
	  res = I.right() / I.left();
	}
	GAOL_RND_RESTORE();
	return res;
      }
    }
  }

  bool interval::is_finite(void) const
  {
    return !std::isinf(left()) && !std::isinf(right());
  }

  interval  max(const interval &I, const interval &J)
  {
    return interval(maximum(I.left(),J.left()),maximum(I.right(),J.right()));
  }

  interval  min(const interval &I, const interval &J)
  {
    return interval(minimum(I.left(),J.left()),minimum(I.right(),J.right()));
  }


  double interval::smig(void) const
  {
    if (is_empty()) {
      return GAOL_NAN;
    } else {
      if (set_contains(0)) {
	return 0.0;
      }
      if (right() < 0.0) {
	return right();
      } else { // left() > 0.0
	return left();
      }
    }
  }

  double interval::mig(void) const
  {
    if (is_empty()) {
      return GAOL_NAN;
    } else {
      if (set_contains(0)) {
	return 0.0;
      }
      if (right() < 0.0) {
	return -right();
      } else { // left() > 0.0
	return left();
      }
    }
  }

  double interval::mag(void) const
  {
    return maximum(std::fabs(left()),std::fabs(right()));
  }


  interval::operator string() const
  {
    std::ostringstream output;
    output.precision(interval::precision());
    output << *this;
    return output.str();
  }

  std::streamsize interval::precision(void)
  {
    return output_precision;
  }

  std::streamsize interval::precision(std::streamsize n)
  {
    std::streamsize old = output_precision;
    output_precision = n;
    return old;
  }




  double interval::midpoint() const
  {
    if (is_empty()) {
      return NAN;
    }
	 if (is_symmetric()) {
		return 0.0;
	 }
    if (left() == -GAOL_INFINITY) {
      return -std::numeric_limits<double>::max();
    }
    if (right() == GAOL_INFINITY) {
      return std::numeric_limits<double>::max();
    }

    unsigned short int _save_state=get_fpu_cw(); round_nearest();
    double middle = 0.5*(left()+right());
	 if (std::isinf(middle)) {
		middle = 0.5*left() + 0.5*right();
	 }
    GAOL_RND_LEAVE();
    return middle;
  }


  interval sqrt(const interval& I)
  {
    interval Ipos = interval(maximum(0.0,I.left()),I.right());

    if (Ipos.is_empty()) {
      return interval::emptyset();
    }

    if (Ipos.left() == 0.0) {
			GAOL_RND_ENTER();
			interval tmp = interval(0.0,::sqrt(Ipos.right()));
      GAOL_RND_LEAVE();
      return tmp;
    } else {
			GAOL_RND_ENTER();
			double l = Ipos.left_internal()/::sqrt(Ipos.left());
			double r = ::sqrt(Ipos.right());
      GAOL_RND_LEAVE();
      return interval(-l,r);
    }
  }


  interval sqrt_rel(const interval& J, const interval& I)
  { // TODO: Rewrite sqrt_rel (eliminate round_downward()?)
    interval Jpos = interval(maximum(0.0,J.left()),J.right());

    if (Jpos.is_empty() || I.is_empty()) {
      return interval::emptyset();
    }

	double l, r;

    GAOL_RND_ENTER();
    if (Jpos.left() == 0.0) {
      l = 0.0;
      r = ::sqrt(Jpos.right());
    } else {
      round_downward();
			l = ::sqrt(Jpos.left());
      round_upward();
			r = ::sqrt(Jpos.right());
    }
    GAOL_RND_LEAVE();

	interval Res(l,r);

    if (I.certainly_positive()) {
      return Res & I;
    }
    if (I.certainly_negative()) {
      return (-Res) & I;
    }
    return (I & Res) | (I & (-Res));
  }


  interval cos(const interval& I)
  {
    if (I.is_empty()) {
      return interval::emptyset();
    }

    GAOL_RND_ENTER();
    double a,b, Il, Ir;
    Il = I.left_internal();
    Ir = I.right_internal();
    double Ileft = -Il;
    double Iright = Ir;
    
    if (Ileft >= 0) {
      a = -(Il/pi_up);
      b = Ir/pi_dn;
    } else {
      a = -(Il/pi_dn);
      if (Iright > 0) {
	b = Ir/pi_dn;
      } else {
	b = Ir/pi_up;
      }
    }
    
    double m=std::floor(a),
      n=std::ceil(b),
      u, v;

    double nm = (n-m); // Must be rounded towards +oo
    
    if (nm < 2.0) {
      if (feven(m)) { // even(m)? No conversion to int in order
	// to avoid overflow
	u=cos_dn(Iright);// use of cos(x)=cos(-x)
	v=cos_up(Ileft);
      } else { // odd(m)?
	u=cos_dn(Ileft);
	v=cos_up(Iright);
      }
    } else {
      if (nm == 2.0) {
	if (feven(m)) {
	  double
	    u1=cos_up(Ileft),
	    u2=cos_up(Iright);
	  u= -1.0;
	  v= ((u1 > u2) ? u1 : u2);
	} else {
	  double
	    u1=cos_dn(Ileft),
	    u2=cos_dn(Iright);
	  
	  u= ((u1 < u2) ? u1 : u2);
	  v= 1.0;
	}
      } else {
	GAOL_RND_LEAVE();
	return interval::minus_one_plus_one();
      }
    }
    GAOL_RND_LEAVE();
    return interval(u,v);
  }




} // namespace gaol
