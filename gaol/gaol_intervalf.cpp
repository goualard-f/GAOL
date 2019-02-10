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
 * SVN: $Id: gaol_intervalf.cpp 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By: Frederic Goualard <Frederic.Goualard@univ-nantes.fr>
 *--------------------------------------------------------------------------*/

#include <iostream>

#include <cmath>
#include <limits>


#include "gaol/gaol_fpu.h"
#include "gaol/gaol_port.h"
#include "gaol/gaol_intervalf.h"
#include "gaol/gaol_common.h"
#include "gaol/gaol_interval.h"

namespace gaol {


	intervalf::intervalf()
	{
		_left=_right=std::numeric_limits<float>::infinity();
	}

	intervalf::intervalf(float v)
	{
		_left = -v;
		_right= v;
	}

	intervalf::intervalf(float l, float r)
	{
		_left = -l;
		_right = r;
	}

	intervalf::intervalf(const intervalf& I)
	{
		_left = I._left;
		_right = I._right;
	}

	intervalf::intervalf(const char* const s)
	{ // BEWARE: quick and dirty implementation for now. Use at your own risks!
		
	 interval tmp(s);
	 _left = float(tmp.left());
	 _right = float(tmp.right());
	}

	intervalf& intervalf::operator=(const intervalf& I)
	{
		_left = I._left;
		_right = I._right;
		return *this;
	}


	intervalf& intervalf::operator+=(const intervalf& I)
	{
		GAOL_RND_ENTER();
		_left += I._left;
		_right += I._right;
		GAOL_RND_LEAVE();
		return *this;
	}

	const intervalf intervalf::operator+(const intervalf& I) const
	{
		return intervalf(*this) += I;
	}

	const intervalf intervalf::operator+(void) const
	{
		return *this;
	}

	intervalf& intervalf::operator-=(const intervalf& I)
	{
		GAOL_RND_ENTER();
		_left += I._right;
		_right += I._left;
		GAOL_RND_LEAVE();
		return *this;
	}

	const intervalf intervalf::operator-(const intervalf& I) const
	{
		return intervalf(*this) -= I;
	}

	const intervalf intervalf::operator-(void) const
	{
		return intervalf(-_right,_left);
	}

	inline float to_zero_if_nan(float x)
	{
		return (std::isnan(x)) ? 0.0f : x;
	}

  intervalf operator+(float d, const intervalf& I2)
  {
    return intervalf(d)+=I2;
  }

  intervalf operator-(float d, const intervalf& I2)
  {
    return intervalf(d)-=I2;
  }

  intervalf operator*(float d, const intervalf& I2)
  {
    return intervalf(d)*=I2;
  }

#if 0
	intervalf& intervalf::operator*=(const intervalf& I)
	{
		if (is_empty() || I.is_empty()) {
			*this = intervalf::emptyset();
			return *this;
		}
		GAOL_RND_ENTER();

		float Jl, Jr, Nl, Nr;
		if (_right < 0.0f) {
			Jr = _right*(-I._left);
			Nl = _right*(-I._right);
		} else {
			Jr = _right*I._right;
			Nl = _right*I._left;
		}
		if (_left > 0.0f) {
			Jl = _left*I._right;
			Nr = _left*I._left;
		} else {
			Jl = (-_left)*I._left;
			Nr =  (-_left)*I._right;
		}
		Jl = to_zero_if_nan(Jl);
		Jr = to_zero_if_nan(Jr);
		Nl = to_zero_if_nan(Nl);
		Nr = to_zero_if_nan(Nr);

		_left = fmaxf(Jl,Nl);
		_right = fmaxf(Jr, Nr);

		GAOL_RND_LEAVE();
		return *this;
	}
#endif

#if 1
  intervalf& intervalf::operator*=(const intervalf& I)
  {
    if (is_empty() || I.is_empty()) {
      *this=intervalf::emptyset();
      return *this;
    }

    if (is_zero()) {
      return *this;
    }
    if (I.is_zero()) {
      *this=I;
      return *this;
    }

	GAOL_RND_ENTER();
    /* Multiplication --
       Algorithm from:
       Interval Arithmetic: from Principles to Implementation
       Timothy J. Hickey, Qun Ju, and Maarten H. van Emden
       J. ACM 48:5, p. 1038--1068, sep. 2001
    */
    if (certainly_positive()) { // P ?
      if (I.certainly_positive()) { // P P
				_left *= -I._left;
				_right *= I._right;
      } else {
				if (I.strictly_straddles_zero()) { // P M
					_left = _right*I._left;
					_right *= I._right;
				} else { // P N
					float tmp = _right*I._left;
					_right = (-_left)*I._right;
					_left = tmp;
				}
      }
    } else {
      if (strictly_straddles_zero()) { // M ?
				if (I.certainly_positive()) { // M P
					_left *= I._right;
					_right *= I._right;
				} else {
	  			if (I.strictly_straddles_zero()) { // M M
						float l1 = _left*I._right;
						float l2 = _right*I._left;
						float r1 = _left*I._left;
						float r2 = _right*I._right;
						_left = (l1 > l2) ? l1 : l2;
						_right = (r1 > r2) ? r1 : r2;
	  			} else { // M N
						float tmp = _right*I._left;
						_right = _left*I._left;
						_left = tmp;
	  			}
				}
      } else { // N  (case Z already tested)
				if (I.certainly_positive()) { // N P
					_left *= I._right;
					_right *= -I._left;
				} else {
	  			if (I.strictly_straddles_zero()) { // N M
						float tmp = _left*I._right;
						_right = _left*I._left;
						_left = tmp;
	  			} else { // N N
						float tmp = _right*(-I._right);
						_right = _left*I._left;
						_left = tmp;
	  			}
				}
      }
    }
    GAOL_RND_LEAVE();
    return *this;
  }
#endif

	intervalf& intervalf::operator/=(const intervalf& I)
	{
		*this *= I.inverse(); // FIXME: implement proper division
		return *this;
	}

  intervalf intervalf::inverse() const
  {
    if (is_empty()) {
      return intervalf::emptyset();
    }

    intervalf res;
    GAOL_RND_ENTER();
    if (right() <= 0.0f) { // (Z N0 N1)
      if (left() < 0.0f) { // (N0 N1)
        if (right() < 0.0f) { // N1
          res._left = (-1.0f)/_right;
          res._right = (-1.0f)/_left;
        } else { // N0
          res._left = std::numeric_limits<float>::infinity();
          res._right = (-1.0f)/_left;
        }
      } else { // Z
        res = intervalf::emptyset();
      }
    } else { // (M P0 P1)
      if (left() < 0.0f) { // M
        res = intervalf::universe();
      } else { // (P0 P1)
        if (left() == 0.0f) { // P0
          res._left = (-1.0f)/_right;
          res._right = std::numeric_limits<float>::infinity();
        } else { // P1
          res._left = (-1.0f)/_right;
          res._right = (-1.0f)/_left;
        }
      }
    }
    GAOL_RND_LEAVE();
    return res;
  }


	const intervalf intervalf::operator*(const intervalf& I) const
	{
		return intervalf(*this) *= I;
	}

	const intervalf intervalf::operator/(const intervalf& I) const
	{
		return intervalf(*this) /= I;
	}

	bool intervalf::possibly_eq(const intervalf& I) const
	{
		return (left() <= I.right()) && (right() >= I.left());
	}


	bool intervalf::set_eq(const intervalf& I) const
	{
    return (is_empty() && I.is_empty()) || ((left()==I.left()) && (right()==I.right()));
	}

	bool intervalf::set_neq(const intervalf& I) const
	{
		return !set_eq(I);
	}

	bool intervalf::set_contains(const intervalf& I) const
	{
    	return (I.is_empty() || ((left()<=I.left()) && (right()>=I.right())));
	}

	bool intervalf::set_contains(float v) const
	{
    	return (left()<=v) && (right()>=v);
	}

	gaol::intervalf uipow_dnup(const gaol::intervalf& I, unsigned int n) // FIXME: this program does not work when an infinity is produced
	{                                                                    // FIXME: in the 80 bits registers and not translated into MAX_FLOAT
		float ly = 1.0f, ry = 1.0f;
		float lz2 = I._left, rz2 = I._right;
		float lz1 = -lz2;
		for (;;) {
			if (odd(n)) {
				ry *= rz2;
				ly = -(ly*lz2);
				if (n==1) { // On input, n is assumed to be non-null
					return intervalf(ly, ry);
				}
			}
			n >>= 1;
			rz2 *= rz2;
			lz2 *= lz1;
			lz1 = -lz2;
		}
	}

	intervalf uipow_upup(const intervalf& I, unsigned int n)
	{
		float ly = 1.0f, ry = 1.0f;

		float lz = I._left, rz = I._right;

		for (;;) {
			if (odd(n)) {
				ly *= lz;
				ry *= rz;
				if (n==1) { // On input, n is assumed to be non-null
					return intervalf(-ly,ry);
				}
			}
			n >>= 1;
			lz *= lz;
			rz *= rz;
		}
	}

	intervalf uipow(const intervalf& I, unsigned int e)
	{
		GAOL_RND_ENTER();
		intervalf res;

		int signI = ((I._right < 0) << 1) |  (I._left < 0);
		switch (signI) { // Remember that the sign of the left bound is negated
			case 0: // 00: I.straddles_zero()
			{
				res = uipow_upup(I,e);
				if (!odd(e)) {
					res = intervalf(0.0f,fmaxf(res._left,res._right));
				}
				break;
			}
			case 1: // 01: I.positive()
			{
				res = uipow_dnup(I,e);
				break;
			}
			case 2: // 10: I.negative()
			{   
				res = uipow_dnup(-I,e);
				if (odd(e)) {
					res = -res;
				}
				break;
			}
			case 3: // 11: I=[0, -0] (empty intervals have been filtered already)
			{
				res = intervalf(0.0f);
				break;
			}
			// No other meaningful cases
		}

		GAOL_RND_LEAVE();
		return res;
	}

	intervalf pow(const intervalf& I, int n)
	{
		if (I.is_empty()) {
			return I;
		}
		if (n < 0) {
			return uipow(I,-n).inverse(); 
		} else {
			if (n > 0) {
				return uipow(I,n);
			} else {
				return intervalf(1.0);
			}
		}
		return intervalf::emptyset(); // Never reached.
	}

	intervalf sqrt(const intervalf& I)
	{
		return I; // FIXME: implement sqrt()
	}

  float intervalf::midpoint() const
  {
    if (is_empty()) {
      return NAN;
    }
    if (left() == -right()) { // Symmetric case handles [-oo, +oo]
	  return 0.0;
    }
    if (left() == -std::numeric_limits<float>::infinity()) {
      return -std::numeric_limits<float>::max();
    }
    if (right() == std::numeric_limits<float>::infinity()) {
      return std::numeric_limits<float>::max();
    }

    GAOL_RND_ENTER();
    float middle = left()+(.5*right() - .5*left()); 
    GAOL_RND_LEAVE();
    return middle;
  }


	std::ostream& operator<<(std::ostream& out, const intervalf& I)
	{
        GAOL_RND_PRESERVE();
		if (I.is_empty()) {
			out << "[empty]";
		} else {
			out.put('[');
			round_downward();
			out << I.left() << ", ";
			round_upward();
			out << I.right();
			out.put(']');
		}
        GAOL_RND_RESTORE();
		return out;
	}

  bool intervalf::is_finite(void) const
  {
    return !std::isinf(left()) && !std::isinf(right());
  }

} // namespace gaol
