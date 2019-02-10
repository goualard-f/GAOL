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
 * SVN: $Id: gaol_intervalf.h 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By: Frederic Goualard <Frederic.Goualard@univ-nantes.fr>
 *--------------------------------------------------------------------------*/

#ifndef __gaol_intervalf_h__
#define __gaol_intervalf_h__

#include <iosfwd>
#include <new>

#include <limits>

namespace gaol {

	/**
	@brief Representation of a floating-point interval.

	Bounds are single precision floating-point numbers. Operations are performed with the FPU.

	Note: The left bound is stored negated
	*/
	class intervalf {
		public:
			__GAOL_PUBLIC__ intervalf();
			__GAOL_PUBLIC__ intervalf(float v);
			__GAOL_PUBLIC__ intervalf(float l, float r);
			__GAOL_PUBLIC__ intervalf(const intervalf& I);
			__GAOL_PUBLIC__ intervalf(const char* const s); // BEWARE: not fully supported at present.
			float left(void) const;
			float right(void) const;

			__GAOL_PUBLIC__ intervalf& operator=(const intervalf& I);

			__GAOL_PUBLIC__ intervalf& operator+=(const intervalf& I);
			__GAOL_PUBLIC__ const intervalf operator+(const intervalf& I) const;
			__GAOL_PUBLIC__ const intervalf operator+(void) const;

			__GAOL_PUBLIC__ intervalf& operator-=(const intervalf& I);
			__GAOL_PUBLIC__ const intervalf operator-(const intervalf& I) const;
			__GAOL_PUBLIC__ const intervalf operator-(void) const;

			__GAOL_PUBLIC__ intervalf& operator*=(const intervalf& I);
			__GAOL_PUBLIC__ const intervalf operator*(const intervalf& I) const;

			__GAOL_PUBLIC__ intervalf& operator/=(const intervalf& I);
			__GAOL_PUBLIC__ const intervalf operator/(const intervalf& I) const;


      __GAOL_PUBLIC__ intervalf inverse() const; // FIXME: document the inverse() method

	/*
     \brief Returns the midpoint of an interval.

     Cases are:
     - \emptyset  -> Quiet NaN
     - [-oo, +oo] -> midP = 0.0
     - [-oo, b]   -> midP = -MAXREAL
     - [a, +oo]   -> midP = MAXREAL
     - [a, b]     -> midP = a + .5*(b-a) with + and * rounded toward
                            nearest and - rounded downward.
    		*/
    		__GAOL_PUBLIC__ float midpoint(void) const;
    		void split(intervalf &I1, intervalf &I2) const;

		    __GAOL_PUBLIC__ bool is_finite(void) const;
			bool is_empty(void) const;
			bool is_zero(void) const;

			bool certainly_positive(void) const;
			bool certainly_negative(void) const;
			bool strictly_straddles_zero(void) const;

			__GAOL_PUBLIC__ bool possibly_eq(const intervalf& I) const;
			__GAOL_PUBLIC__ bool possibly_neq(const intervalf& I) const;

		    __GAOL_PUBLIC__ bool set_strictly_contains(const intervalf& I) const;
    		__GAOL_PUBLIC__ bool set_strictly_contains(float d) const;
    		__GAOL_PUBLIC__ bool set_disjoint(const intervalf &I) const;
			__GAOL_PUBLIC__ bool set_eq(const intervalf& I) const;
			__GAOL_PUBLIC__ bool set_neq(const intervalf& I) const;
			__GAOL_PUBLIC__ bool set_contains(const intervalf& I) const;
			__GAOL_PUBLIC__ bool set_contains(float v) const;

			friend intervalf sqrt(const intervalf& I);
			friend intervalf uipow_dnup(const intervalf& I, unsigned int n);
			friend intervalf uipow_upup(const intervalf& I, unsigned int n);
			friend intervalf uipow(const intervalf& I, unsigned int e);

			static intervalf zero(void);
			static intervalf universe(void);
			static intervalf emptyset(void);
			static intervalf positive(void); // [0, +oo]
			static intervalf negative(void); // [-oo, 0]
		private:
			float _left, _right;
	};

  __GAOL_PUBLIC__ intervalf operator+(float d, const intervalf& I2);
  __GAOL_PUBLIC__ intervalf operator*(float d, const intervalf& I2);
  __GAOL_PUBLIC__ intervalf operator-(float d, const intervalf& I2);

	__GAOL_PUBLIC__ std::ostream& operator<<(std::ostream& out, const intervalf& I);

  INLINE intervalf inverse(const intervalf& I)
  {
    return I.inverse();
  }

	__GAOL_PUBLIC__ intervalf pow(const intervalf& I, int n);

	inline intervalf intervalf::universe(void)
	{
		return intervalf(-std::numeric_limits<float>::infinity(),std::numeric_limits<float>::infinity());
	}

	inline intervalf intervalf::zero(void)
	{
		return intervalf(0.0f);
	}

	inline intervalf intervalf::emptyset(void)
	{
		return intervalf(std::numeric_limits<float>::quiet_NaN());
	}

	inline intervalf intervalf::positive(void)
	{
		return intervalf(0.0f,std::numeric_limits<float>::infinity());
	}

	inline intervalf intervalf::negative(void)
	{
		return intervalf(-std::numeric_limits<float>::infinity(),0.0f);
	}

	inline float intervalf::left() const
	{
		return -_left; // left bound negated (remember?)
	}
	inline float intervalf::right() const
	{
		return _right;
	}

	inline bool intervalf::is_empty(void) const
	{
		// Testing the logical inverse to handle NaNs
		return !(left() <= right());
	}

	inline bool intervalf::is_zero(void) const
	{
		return _left==0.0f && _right==0.0f;
	}

	inline bool intervalf::certainly_positive(void) const
	{
		return _left<=0.0f;
	}

	inline bool intervalf::certainly_negative(void) const
	{
		return _right<=0.0f;
	}

	inline bool intervalf::strictly_straddles_zero(void) const
	{
		return _left>0.0f && _right>0.0f;
	}

  INLINE bool intervalf::set_strictly_contains(const intervalf& I) const
  {
    return (I.is_empty() || ((left()<I.left()) && (right()>I.right())));
  }

  /**
    @note d should not be a NaN
    */
  INLINE bool intervalf::set_strictly_contains(float d) const
  {
    return ((left()<d) && (right()>d));
  }

  INLINE bool intervalf::set_disjoint(const intervalf &I) const
  {
      return (right() < I.left()) || (left() > I.right())
	  || (is_empty() || I.is_empty());
  }

  INLINE void intervalf::split(intervalf &I1, intervalf &I2) const
  {
    float l = left(), m = midpoint(), r = right();
    I1 = intervalf(l,m);
    I2 = intervalf(m,r);
  }

} // namespace gaol



#endif // __gaol_intervalf_h__
