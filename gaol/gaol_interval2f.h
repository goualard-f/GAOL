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
 * SVN: $Id: gaol_interval2f.h 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By: Frederic Goualard <Frederic.Goualard@univ-nantes.fr>
 *--------------------------------------------------------------------------*/


#ifndef __gaol_interval2f_h__
#define __gaol_interval2f_h__

#include <new>
#include <iosfwd>

#include <limits>
#include <xmmintrin.h>
#include <pmmintrin.h>

#include "gaol/gaol_config.h"
#include "gaol/gaol_fpu.h"
#include "gaol/gaol_common.h"

namespace gaol {

	class interval; // Forward declaration for interval2f::first() and interval2f::second()
	class intervalf; // Forward declaration 

	/**
	@brief Representation of two floating-point intervals in an SSE3 register.

	Bounds are single precision floating-point numbers.

	Note: The left bounds are stored negated
	*/
	class interval2f {
		public:
			interval2f();
			interval2f(float v);
			interval2f(float l, float r);
			interval2f(float l1, float r1, float l2, float r2);
			interval2f(const interval2f& I);
			interval2f(const __m128& xmm);
			__GAOL_PUBLIC__ interval2f(const char* const str);
			__GAOL_PUBLIC__ interval2f(const interval& Il, const interval& Ir);
			__GAOL_PUBLIC__ interval2f(const intervalf& Il, const intervalf& Ir);

			__GAOL_PUBLIC__ interval first(void) const; // Returns the first interval (rightmost in register)
			__GAOL_PUBLIC__ interval second(void) const; // Returns the second interval (leftmost in register)

			interval2f& operator=(const interval2f& I);

			interval2f& operator+=(const interval2f& I);
			const interval2f operator+(const interval2f& I) const;
			const interval2f operator+(void) const;

			interval2f& operator-=(const interval2f& I);
			const interval2f operator-(const interval2f& I) const;
			const interval2f operator-(void) const;

			__GAOL_PUBLIC__ interval2f& operator*=(const interval2f& I);
			const interval2f operator*(const interval2f& I) const;

      __GAOL_PUBLIC__ interval2f inverse() const; // TODO: document inverse() method

			__m128& get_xmminterval(void);
			const __m128& get_xmminterval(void) const;

			typedef float xmm4[4] __attribute__((aligned(16)));
			typedef double xmm2d[2] __attribute__((aligned(16)));

			void get_bounds(xmm4& bounds) const;


			/*
				The following relations are true if both intervals verify them.
			*/
			__GAOL_PUBLIC__ bool possibly_eq_all(const interval2f& I) const;
			__GAOL_PUBLIC__ bool set_eq_all(const interval2f& I) const;
			__GAOL_PUBLIC__ bool set_contains_all(const interval2f& I) const;
			__GAOL_PUBLIC__ int set_contains(float v) const;


      // Allocators defined to enforce alignment on a 16 bytes boundary
		__GAOL_PUBLIC__ void *operator new(size_t sz);
		__GAOL_PUBLIC__ void operator delete(void *p);

		__GAOL_PUBLIC__ void *operator new[](size_t sz);
		__GAOL_PUBLIC__ void operator delete[](void *p);

		__GAOL_PUBLIC__ void *operator new(size_t sz, void *p);
		__GAOL_PUBLIC__ void operator delete(void *p, void *place);

			friend interval2f pow(const interval2f& I, int n);

			static __m128 m128_universe(void); // { [-oo, +oo] | [-oo, +oo] }
			static __m128 m128_emptyset(void); // { \emptyset | \emptyset }
			static __m128 m128_zero(void); // { 0 | 0 }
			static __m128 m128_signsmask(void); // { [0, -0] | [0, -0] }
			static __m128 m128_lsignmask(void);
			static __m128 m128_hsignmask(void);
			static __m128 m128_lsignsmask(void);
			static __m128 m128_hsignsmask(void);
			static __m128 m128_oppositemask(void); // { [-0, -0] | [-0, -0] }
			static __m128 m128_02mask(void);
			static __m128 m128_2mask(void);
			static __m128 m128_0mask(void);
			static __m128 m128_01mask(void);
			static __m128 m128_23mask(void);
			static __m128 m128_012mask(void);
			static __m128d m128d_signmask(void);

		private:
			__m128 xmmbounds; // Lower 64bits = left bound, upper 64 bits = right bound
	};

  inline interval2f inverse(const interval2f& I) // TODO: document inverse function
  {
    return I.inverse();
  }

	__GAOL_PUBLIC__ std::ostream& operator<<(std::ostream& out, const interval2f& I);
  __GAOL_PUBLIC__ interval2f pow(const interval2f& I, int n);

	inline void interval2f::get_bounds(xmm4& bounds) const
	{
		_mm_store_ps(bounds,xmmbounds);
	}

	inline __m128& interval2f::get_xmminterval(void)
	{
		return xmmbounds;
	}

	inline const __m128& interval2f::get_xmminterval(void) const
	{
		return xmmbounds;
	}

	inline __m128 interval2f::m128_universe(void)
	{
		return _mm_set1_ps(std::numeric_limits<float>::infinity());
	}

	inline __m128 interval2f::m128_emptyset(void)
	{
		return _mm_set1_ps(std::numeric_limits<float>::quiet_NaN());
	}

	inline __m128 interval2f::m128_zero(void)
	{
		return _mm_set1_ps(0.0f);
	}


	inline __m128 interval2f::m128_signsmask(void)
	{
		return _mm_set_ps(0.0f,-0.0f,0.0f,-0.0f);
	}

	inline __m128 interval2f::m128_lsignmask(void)
	{
		return _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
	}

	inline __m128 interval2f::m128_lsignsmask(void)
	{
		return _mm_set_ps(0.0f,0.0f,-0.0f,-0.0f);
	}

	inline __m128 interval2f::m128_hsignmask(void)
	{
		return _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
	}

	inline __m128 interval2f::m128_hsignsmask(void)
	{
		return _mm_set_ps(-0.0f,-0.0f,0.0f,0.0f);
	}

	inline __m128 interval2f::m128_oppositemask(void)
	{
		return _mm_set_ps(-0.0f,-0.0f,-0.0f,-0.0f);
	}

	inline __m128 interval2f::m128_02mask(void)
	{
		return _mm_cmpeq_ps(_mm_set1_ps(0.0f),_mm_set_ps(0.0f,1.0f,0.0f,1.0f));
	}

	inline __m128 interval2f::m128_2mask(void)
	{
		return _mm_cmpeq_ps(_mm_set1_ps(0.0f),_mm_set_ps(0.0f,1.0f,0.0f,0.0f));
	}

	inline __m128 interval2f::m128_0mask(void)
	{
		return _mm_cmpeq_ps(_mm_set1_ps(0.0f),_mm_set_ps(0.0f,0.0f,0.0f,1.0f));
	}

	inline __m128 interval2f::m128_01mask(void)
	{
		return _mm_cmpeq_ps(_mm_set1_ps(0.0f),_mm_set_ps(0.0f,0.0f,1.0f,1.0f));
	}
	inline __m128 interval2f::m128_23mask(void)
	{
		return _mm_cmpeq_ps(_mm_set1_ps(0.0f),_mm_set_ps(1.0f,1.0f,0.0f,0.0f));
	}

	inline __m128 interval2f::m128_012mask(void)
	{
		return _mm_cmpeq_ps(_mm_set1_ps(0.0f),_mm_set_ps(0.0f,1.0f,1.0f,1.0f));
	}


	inline __m128d interval2f::m128d_signmask(void)
	{
		return _mm_set_pd(0.0,-0.0);
	}






	inline interval2f::interval2f()
	{
		xmmbounds = _mm_set1_ps(std::numeric_limits<float>::infinity());
	}

	inline interval2f::interval2f(float v)
	{
		xmm4 tmp = {-v, v, -v, v};
		xmmbounds = _mm_load_ps(tmp);
	}

	inline interval2f::interval2f(float l, float r)
	{

		xmm4 tmp = {-l, r, -l, r};
		xmmbounds = _mm_load_ps(tmp);
	}

	inline interval2f::interval2f(float l1, float r1, float l2, float r2)
	{

		xmm4 tmp = {-l1, r1, -l2, r2};
		xmmbounds = _mm_load_ps(tmp);
	}


	inline interval2f::interval2f(const interval2f& I)
	{
		xmmbounds = I.xmmbounds;
	}

	inline interval2f::interval2f(const __m128& xmm)
	{
		xmmbounds = xmm;
	}


	inline interval2f& interval2f::operator=(const interval2f& I)
	{
		xmmbounds = I.xmmbounds;
		return *this;
	}


	inline interval2f& interval2f::operator+=(const interval2f& I)
	{
		GAOL_RND_ENTER_SSE();
		xmmbounds = _mm_add_ps(xmmbounds,I.xmmbounds);
		GAOL_RND_LEAVE_SSE();
		return *this;
	}

	inline interval2f& interval2f::operator-=(const interval2f& I)
	{
		GAOL_RND_ENTER_SSE();
		xmmbounds = _mm_add_ps(xmmbounds,_mm_shuffle_ps(I.xmmbounds,I.xmmbounds,_MM_SHUFFLE(2,3,0,1)));
		GAOL_RND_LEAVE_SSE();
		return *this;
	}




	inline const interval2f interval2f::operator-(void) const
	{
		return interval2f(_mm_shuffle_ps(xmmbounds,xmmbounds,_MM_SHUFFLE(2,3,0,1)));
	}

	inline const interval2f interval2f::operator+(const interval2f& I) const
	{
		GAOL_RND_ENTER_SSE();
		__m128 tmp = _mm_add_ps(xmmbounds,I.xmmbounds);
		GAOL_RND_LEAVE_SSE();
		return interval2f(tmp);
	}

	inline const interval2f interval2f::operator-(const interval2f& I) const
	{
		GAOL_RND_ENTER_SSE();
		__m128 tmp = _mm_add_ps(xmmbounds,_mm_shuffle_ps(I.xmmbounds,I.xmmbounds,_MM_SHUFFLE(2,3,0,1)));
		GAOL_RND_LEAVE_SSE();
		return interval2f(tmp);
	}

	inline const interval2f interval2f::operator*(const interval2f& I) const
	{
		return interval2f(*this) *= I;
	}



	inline int interval2f::set_contains(float v) const
	{
		xmm4 tmp;
		_mm_store_ps(tmp,xmmbounds);
		return ((((-tmp[0])<=v) && tmp[1]>=v)) << 1 || (((-tmp[2])<=v) && tmp[3]>=v);
	}






} // namespace cell


#endif // __gaol_interval2f_h__
