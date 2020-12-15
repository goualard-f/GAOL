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
 * SVN: $Id: gaol_interval2f.cpp 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By: Frederic Goualard <Frederic.Goualard@univ-nantes.fr>
 *--------------------------------------------------------------------------*/

#include <iostream>
#include <cmath>
#include <pmmintrin.h>
#include <limits>



#include "gaol/gaol_port.h"
#include "gaol/gaol_interval2f.h"
#include "gaol/gaol_interval.h"
#include "gaol/gaol_intervalf.h"

namespace gaol {

	std::ostream& operator<<(std::ostream& os, const interval2f& I)
	{
		os << "<" << I.second() << " | " << I.first() << ">";

		return os;
	}


	void* interval2f::operator new(size_t sz)
	{
		void *buf;
		if (MEMALIGN(buf,16,sz)) { // error ?
			throw std::bad_alloc();
		}
		return buf;
	}

    void interval2f::operator delete(void *p)
    {
        free(p);
    }

	void* interval2f::operator new[](size_t sz)
	{
		void *buf;
		if (MEMALIGN(buf,16,sz)) { // error ?
			throw std::bad_alloc();
		}
		return buf;
	}



    void interval2f::operator delete[](void *p)
    {
        free(p);
    }

	void* interval2f::operator new(size_t sz, void *p)
	{
		if ((unsigned long long)p % 16 != 0) {
			throw std::bad_alloc();
		}
		return p;
	}

    void interval2f::operator delete(void *p, void *place)
    {
        free(p);
    }


	interval2f::interval2f(const interval& I1, const interval& I2)
	{
		GAOL_RND_ENTER_SSE();
		xmm2d tmpl1 = {-I1.left(), I1.right()};
		__m128 tmpl2 = _mm_cvtpd_ps(_mm_load_pd(tmpl1));

		xmm2d tmpr1 = {-I2.left(), I2.right()};
		xmmbounds =  _mm_cvtpd_ps(_mm_load_pd(tmpr1));
		xmmbounds = _mm_shuffle_ps(tmpl2,xmmbounds,_MM_SHUFFLE(1,0,1,0));
		GAOL_RND_LEAVE_SSE();
	}

	interval2f::interval2f(const intervalf& I1, const intervalf& I2)
	{
		xmm4 tmp = {-I1.left(), I1.right(), -I2.left(), I2.right()};
		xmmbounds = _mm_load_ps(tmp);
	}

	interval2f::interval2f(const char* const str)
	{
		interval I(str);
		GAOL_RND_ENTER_SSE();
		xmm2d tmpl1 = {-I.left(), I.right()};
		__m128 tmpl2 = _mm_cvtpd_ps(_mm_load_pd(tmpl1));

		xmm2d tmpr1 = {-I.left(), I.right()};
		xmmbounds =  _mm_cvtpd_ps(_mm_load_pd(tmpr1));
		xmmbounds = _mm_shuffle_ps(tmpl2,xmmbounds,_MM_SHUFFLE(1,0,1,0));
		GAOL_RND_LEAVE_SSE();
		
	}

	interval interval2f::first() const
	{
		__m128d xmmIl = _mm_cvtps_pd(xmmbounds);
		return interval(xmmIl);
	}

	interval interval2f::second() const
	{
		__m128 xmmIr = _mm_shuffle_ps(xmmbounds,xmmbounds,_MM_SHUFFLE(1,0,3,2));
		__m128d xmmIr2 = _mm_cvtps_pd(xmmIr);
		return interval(xmmIr2);
	}

#if 0
	interval2f& interval2f::operator*=(const interval2f& I)
	{ // Brute force algorithm
		__m128 _mm_oppositemask = _mm_set_ps(-0.0f,-0.0f,-0.0f,-0.0f);
		__m128 _mm_signsmask = _mm_set_ps(0.0f,-0.0f,0.0f,-0.0f);
		__m128 rI = I.xmmbounds;

		// xmmbounds == <d, -c, b, -a>
		// I.xmmbounds == <h, -g, f, -e>
		GAOL_RND_ENTER_SSE();

		__m128 r1  = _mm_shuffle_ps(xmmbounds,xmmbounds,_MM_SHUFFLE(2,3,0,1)); 		// r1  = <-c, d, -a, b>
		__m128 r2  = _mm_xor_ps(r1,_mm_oppositemask); 						        // r2  = <c, -d, a, -b>
		__m128 r3  = _mm_xor_ps(rI,_mm_signsmask);    								// r3  = <h, g, f, e>
		__m128 r4  = _mm_shuffle_ps(r3,r3,_MM_SHUFFLE(2,3,0,1));               		// r4  = <g, h, e, f>
		//__m128 r5  = _mm_shuffle_ps(rI,rI,_MM_SHUFFLE(2,2,0,0)); 					// r5 = <-g, -g, -e, -e>
		//__m128 r6  = _mm_shuffle_ps(rI,rI,_MM_SHUFFLE(3,3,1,1));					// r6 = <h, h, f, f>
		__m128 r5 = _mm_moveldup_ps(rI);
		__m128 r6 = _mm_movehdup_ps(rI);
		__m128 r7  = _mm_mul_ps(xmmbounds,r3);              						// r7  = <dh, (-c)g, bf, (-a)e>
		__m128 ordr7 = _mm_cmpord_ps(r7,r7);
		r7 = _mm_and_ps(r7,ordr7);
		__m128 r8  = _mm_mul_ps(r1,r5);                     						// r8  = <(c)(-g), d(-g), (-a)(-e), b(-e)>
		__m128 ordr8 = _mm_cmpord_ps(r8,r8);
		r8 = _mm_and_ps(r8,ordr8);
		__m128 r9  = _mm_mul_ps(xmmbounds,r4);              						// r9  = <dg, (-c)h, be, (-a)f>
		__m128 ordr9 = _mm_cmpord_ps(r9,r9);
		r9 = _mm_and_ps(r9,ordr9);
		__m128 r10 = _mm_mul_ps(r2,r6);                     						// r10 = <ch, (-d)h, af, (-b)f>
		__m128 ordr10 = _mm_cmpord_ps(r10,r10);
		r10 = _mm_and_ps(r10,ordr10);
		__m128 r11 = _mm_max_ps(r7, r8);
		__m128 r12 = _mm_max_ps(r9, r10);
		xmmbounds   = _mm_max_ps(r11,r12);

		GAOL_RND_LEAVE_SSE();
		return *this;
	}
#endif

#if 1
interval2f& interval2f::operator*=(const interval2f& I)
{ // All cases
  // xmmbounds == <d, -c, b, -a>
  // I.xmmbounds == <h, -g, f, -e>
  GAOL_RND_ENTER_SSE();

  int signs = (_mm_movemask_ps(I.xmmbounds) << 4) | _mm_movemask_ps(xmmbounds);
  // signs: bits 0, 2, 4 and 6 shall be negated since the left bounds are stored negated
  switch (signs) {
// INSERT CODE HERE >>
case 0: // 00000000 => 01010101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,160);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,177);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,245);
__m128 r3 = I.xmmbounds;
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
}
break;
case 1: // 00000001 => 01010100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,165);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,180);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,245);
__m128 r3 = I.xmmbounds;
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
}
break;
case 2: // 00000010 => 01010111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,160);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,177);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,240);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,225);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
}
break;
case 3: // 00000011 => 01010110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,160);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,240);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 4: // 00000100 => 01010001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,240);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,225);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,245);
__m128 r3 = I.xmmbounds;
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
}
break;
case 5: // 00000101 => 01010000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,245);
__m128 r1 = I.xmmbounds;
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 6: // 00000110 => 01010011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,240);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,225);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 7: // 00000111 => 01010010
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,240);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 8: // 00001000 => 01011101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,160);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,177);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,165);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,180);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
}
break;
case 9: // 00001001 => 01011100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,165);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,180);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 10: // 00001010 => 01011111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,160);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,177);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 11: // 00001011 => 01011110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,160);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 12: // 00001100 => 01011001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,0);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,5);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 13: // 00001101 => 01011000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,5);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 14: // 00001110 => 01011011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,0);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 15: // 00001111 => 01011010
{
xmmbounds = interval2f::m128_zero(); }
break;
case 16: // 00010000 => 01000101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,164);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,181);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,244);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,229);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
}
break;
case 17: // 00010001 => 01000100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,164);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,180);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,244);
__m128 r3 = I.xmmbounds;
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
r2 = _mm_xor_ps(r2,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
}
break;
case 18: // 00010010 => 01000111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,164);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,177);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,244);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,225);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask2);
r3 = _mm_xor_ps(r3,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
}
break;
case 19: // 00010011 => 01000110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,160);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,240);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 20: // 00010100 => 01000001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,244);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,229);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 21: // 00010101 => 01000000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,244);
__m128 r1 = I.xmmbounds;
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 22: // 00010110 => 01000011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,244);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,225);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 23: // 00010111 => 01000010
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,240);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 24: // 00011000 => 01001101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,164);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,181);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 25: // 00011001 => 01001100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,164);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,180);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 26: // 00011010 => 01001111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,164);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,177);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 27: // 00011011 => 01001110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,160);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 28: // 00011100 => 01001001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,5);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 29: // 00011101 => 01001000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 30: // 00011110 => 01001011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 31: // 00011111 => 01001010
{
xmmbounds = interval2f::m128_zero(); }
break;
case 32: // 00100000 => 01110101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,161);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,241);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
}
break;
case 33: // 00100001 => 01110100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,161);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,180);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,241);
__m128 r3 = I.xmmbounds;
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask2);
r2 = _mm_xor_ps(r2,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
}
break;
case 34: // 00100010 => 01110111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,161);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,177);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,241);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,225);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
r2 = _mm_xor_ps(r2,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
}
break;
case 35: // 00100011 => 01110110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,160);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,240);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 36: // 00100100 => 01110001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,241);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 37: // 00100101 => 01110000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,241);
__m128 r1 = I.xmmbounds;
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 38: // 00100110 => 01110011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,241);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,225);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 39: // 00100111 => 01110010
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,240);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 40: // 00101000 => 01111101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,161);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 41: // 00101001 => 01111100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,161);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,180);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 42: // 00101010 => 01111111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,161);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,177);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 43: // 00101011 => 01111110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,160);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 44: // 00101100 => 01111001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,0);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 45: // 00101101 => 01111000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 46: // 00101110 => 01111011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 47: // 00101111 => 01111010
{
xmmbounds = interval2f::m128_zero(); }
break;
case 48: // 00110000 => 01100101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,160);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,240);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 49: // 00110001 => 01100100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,160);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,240);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 50: // 00110010 => 01100111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,160);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,240);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 51: // 00110011 => 01100110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,160);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,240);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 52: // 00110100 => 01100001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,240);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 53: // 00110101 => 01100000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,240);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 54: // 00110110 => 01100011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,240);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 55: // 00110111 => 01100010
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,240);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 56: // 00111000 => 01101101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,160);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 57: // 00111001 => 01101100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,160);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 58: // 00111010 => 01101111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,160);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 59: // 00111011 => 01101110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,160);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 60: // 00111100 => 01101001
{
xmmbounds = interval2f::m128_zero(); }
break;
case 61: // 00111101 => 01101000
{
xmmbounds = interval2f::m128_zero(); }
break;
case 62: // 00111110 => 01101011
{
xmmbounds = interval2f::m128_zero(); }
break;
case 63: // 00111111 => 01101010
{
xmmbounds = interval2f::m128_zero(); }
break;
case 64: // 01000000 => 00010101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,241);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,229);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,244);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
}
break;
case 65: // 01000001 => 00010100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,229);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,244);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 66: // 01000010 => 00010111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,241);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 67: // 01000011 => 00010110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,240);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 68: // 01000100 => 00010001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,225);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,229);
__m128 r3 = I.xmmbounds;
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
r2 = _mm_xor_ps(r2,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
}
break;
case 69: // 01000101 => 00010000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,229);
__m128 r1 = I.xmmbounds;
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 70: // 01000110 => 00010011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,225);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 71: // 01000111 => 00010010
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 72: // 01001000 => 00011101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,177);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,229);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,180);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask8);
r3 = _mm_xor_ps(r3,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
}
break;
case 73: // 01001001 => 00011100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,229);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,180);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 74: // 01001010 => 00011111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,177);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 75: // 01001011 => 00011110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 76: // 01001100 => 00011001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,0);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,5);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 77: // 01001101 => 00011000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,5);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 78: // 01001110 => 00011011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,0);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 79: // 01001111 => 00011010
{
xmmbounds = interval2f::m128_zero(); }
break;
case 80: // 01010000 => 00000101
{
__m128 r0 = xmmbounds;
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,245);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 81: // 01010001 => 00000100
{
__m128 r0 = xmmbounds;
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,244);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 82: // 01010010 => 00000111
{
__m128 r0 = xmmbounds;
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,241);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 83: // 01010011 => 00000110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,240);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 84: // 01010100 => 00000001
{
__m128 r0 = xmmbounds;
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,229);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 85: // 01010101 => 00000000
{
__m128 r0 = xmmbounds;
__m128 r1 = I.xmmbounds;
__m128 smask5 = _mm_set_ps(0.0f,-0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask5);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 86: // 01010110 => 00000011
{
__m128 r0 = xmmbounds;
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,225);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 87: // 01010111 => 00000010
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 88: // 01011000 => 00001101
{
__m128 r0 = xmmbounds;
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,181);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 89: // 01011001 => 00001100
{
__m128 r0 = xmmbounds;
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,180);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 90: // 01011010 => 00001111
{
__m128 r0 = xmmbounds;
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,177);
__m128 smask10 = _mm_set_ps(-0.0f,0.0f,-0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask10);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 91: // 01011011 => 00001110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 92: // 01011100 => 00001001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,5);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 93: // 01011101 => 00001000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 94: // 01011110 => 00001011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 95: // 01011111 => 00001010
{
xmmbounds = interval2f::m128_zero(); }
break;
case 96: // 01100000 => 00110101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,225);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,240);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 97: // 01100001 => 00110100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,225);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,244);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 98: // 01100010 => 00110111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,225);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,241);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 99: // 01100011 => 00110110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,240);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 100: // 01100100 => 00110001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,225);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 101: // 01100101 => 00110000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,225);
__m128 r1 = I.xmmbounds;
__m128 smask6 = _mm_set_ps(0.0f,-0.0f,-0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask6);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 102: // 01100110 => 00110011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,225);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,225);
__m128 smask5 = _mm_set_ps(0.0f,-0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask5);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 103: // 01100111 => 00110010
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 104: // 01101000 => 00111101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,225);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 105: // 01101001 => 00111100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,225);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,180);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask2);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 106: // 01101010 => 00111111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,225);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,177);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 107: // 01101011 => 00111110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 108: // 01101100 => 00111001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,0);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 109: // 01101101 => 00111000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 110: // 01101110 => 00111011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 111: // 01101111 => 00111010
{
xmmbounds = interval2f::m128_zero(); }
break;
case 112: // 01110000 => 00100101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,240);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 113: // 01110001 => 00100100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,240);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 114: // 01110010 => 00100111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,240);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 115: // 01110011 => 00100110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,240);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 116: // 01110100 => 00100001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 117: // 01110101 => 00100000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 118: // 01110110 => 00100011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 119: // 01110111 => 00100010
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 120: // 01111000 => 00101101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 121: // 01111001 => 00101100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 122: // 01111010 => 00101111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 123: // 01111011 => 00101110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,224);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 124: // 01111100 => 00101001
{
xmmbounds = interval2f::m128_zero(); }
break;
case 125: // 01111101 => 00101000
{
xmmbounds = interval2f::m128_zero(); }
break;
case 126: // 01111110 => 00101011
{
xmmbounds = interval2f::m128_zero(); }
break;
case 127: // 01111111 => 00101010
{
xmmbounds = interval2f::m128_zero(); }
break;
case 128: // 10000000 => 11010101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,161);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,181);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,164);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
}
break;
case 129: // 10000001 => 11010100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,181);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,164);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 130: // 10000010 => 11010111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,161);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 131: // 10000011 => 11010110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,160);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 132: // 10000100 => 11010001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,225);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,181);
__m128 r3 = I.xmmbounds;
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask8);
r2 = _mm_xor_ps(r2,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
}
break;
case 133: // 10000101 => 11010000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,181);
__m128 r1 = I.xmmbounds;
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 134: // 10000110 => 11010011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,225);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 135: // 10000111 => 11010010
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 136: // 10001000 => 11011101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,177);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,181);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,180);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
r2 = _mm_xor_ps(r2,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
}
break;
case 137: // 10001001 => 11011100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,181);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,180);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 138: // 10001010 => 11011111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,177);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 139: // 10001011 => 11011110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 140: // 10001100 => 11011001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,0);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,5);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 141: // 10001101 => 11011000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,5);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 142: // 10001110 => 11011011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,0);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 143: // 10001111 => 11011010
{
xmmbounds = interval2f::m128_zero(); }
break;
case 144: // 10010000 => 11000101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,180);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,165);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 145: // 10010001 => 11000100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,180);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,164);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 146: // 10010010 => 11000111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,180);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,161);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 147: // 10010011 => 11000110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,160);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 148: // 10010100 => 11000001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,180);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,229);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 149: // 10010101 => 11000000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,180);
__m128 r1 = I.xmmbounds;
__m128 smask9 = _mm_set_ps(-0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask9);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 150: // 10010110 => 11000011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,180);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,225);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask8);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 151: // 10010111 => 11000010
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 152: // 10011000 => 11001101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,180);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,181);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 153: // 10011001 => 11001100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,180);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,180);
__m128 smask5 = _mm_set_ps(0.0f,-0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask5);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 154: // 10011010 => 11001111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,180);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,177);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 155: // 10011011 => 11001110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 156: // 10011100 => 11001001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,5);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 157: // 10011101 => 11001000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 158: // 10011110 => 11001011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 159: // 10011111 => 11001010
{
xmmbounds = interval2f::m128_zero(); }
break;
case 160: // 10100000 => 11110101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,177);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,160);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 161: // 10100001 => 11110100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,177);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,164);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 162: // 10100010 => 11110111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,177);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,161);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 163: // 10100011 => 11110110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,160);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 164: // 10100100 => 11110001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,177);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 165: // 10100101 => 11110000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,177);
__m128 r1 = I.xmmbounds;
__m128 smask10 = _mm_set_ps(-0.0f,0.0f,-0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask10);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 166: // 10100110 => 11110011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,177);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,225);
__m128 smask9 = _mm_set_ps(-0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask9);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 167: // 10100111 => 11110010
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 168: // 10101000 => 11111101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,177);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 169: // 10101001 => 11111100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,177);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,180);
__m128 smask6 = _mm_set_ps(0.0f,-0.0f,-0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask6);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 170: // 10101010 => 11111111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,177);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,177);
__m128 smask5 = _mm_set_ps(0.0f,-0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask5);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
}
break;
case 171: // 10101011 => 11111110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 172: // 10101100 => 11111001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,0);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 173: // 10101101 => 11111000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 174: // 10101110 => 11111011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 175: // 10101111 => 11111010
{
xmmbounds = interval2f::m128_zero(); }
break;
case 176: // 10110000 => 11100101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,160);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 177: // 10110001 => 11100100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,160);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 178: // 10110010 => 11100111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,160);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 179: // 10110011 => 11100110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,160);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 180: // 10110100 => 11100001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 181: // 10110101 => 11100000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 182: // 10110110 => 11100011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 183: // 10110111 => 11100010
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,224);
__m128 smask8 = _mm_set_ps(-0.0f,0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask8);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 184: // 10111000 => 11101101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 185: // 10111001 => 11101100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 186: // 10111010 => 11101111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 187: // 10111011 => 11101110
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,176);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,176);
__m128 smask4 = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_01mask());
}
break;
case 188: // 10111100 => 11101001
{
xmmbounds = interval2f::m128_zero(); }
break;
case 189: // 10111101 => 11101000
{
xmmbounds = interval2f::m128_zero(); }
break;
case 190: // 10111110 => 11101011
{
xmmbounds = interval2f::m128_zero(); }
break;
case 191: // 10111111 => 11101010
{
xmmbounds = interval2f::m128_zero(); }
break;
case 192: // 11000000 => 10010101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,0);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,5);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 193: // 11000001 => 10010100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,5);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 194: // 11000010 => 10010111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,0);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 195: // 11000011 => 10010110
{
xmmbounds = interval2f::m128_zero(); }
break;
case 196: // 11000100 => 10010001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,0);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,5);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 197: // 11000101 => 10010000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,5);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 198: // 11000110 => 10010011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,0);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 199: // 11000111 => 10010010
{
xmmbounds = interval2f::m128_zero(); }
break;
case 200: // 11001000 => 10011101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,0);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,5);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 201: // 11001001 => 10011100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,5);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 202: // 11001010 => 10011111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,0);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 203: // 11001011 => 10011110
{
xmmbounds = interval2f::m128_zero(); }
break;
case 204: // 11001100 => 10011001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,0);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 r2 = _mm_shuffle_ps(xmmbounds,xmmbounds,5);
__m128 r3 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
__m128 r5 = _mm_mul_ps(r2,r3);
__m128 ord5 = _mm_cmpord_ps(r5,r5);
r5 = _mm_and_ps(r5,ord5);
xmmbounds = _mm_max_ps(xmmbounds, r5);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 205: // 11001101 => 10011000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,5);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 206: // 11001110 => 10011011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,0);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 207: // 11001111 => 10011010
{
xmmbounds = interval2f::m128_zero(); }
break;
case 208: // 11010000 => 10000101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,5);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 209: // 11010001 => 10000100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 210: // 11010010 => 10000111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 211: // 11010011 => 10000110
{
xmmbounds = interval2f::m128_zero(); }
break;
case 212: // 11010100 => 10000001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,5);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 213: // 11010101 => 10000000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 214: // 11010110 => 10000011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 215: // 11010111 => 10000010
{
xmmbounds = interval2f::m128_zero(); }
break;
case 216: // 11011000 => 10001101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,5);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 217: // 11011001 => 10001100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 218: // 11011010 => 10001111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 219: // 11011011 => 10001110
{
xmmbounds = interval2f::m128_zero(); }
break;
case 220: // 11011100 => 10001001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,5);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 221: // 11011101 => 10001000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 222: // 11011110 => 10001011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,4);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r1 = _mm_xor_ps(r1,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 223: // 11011111 => 10001010
{
xmmbounds = interval2f::m128_zero(); }
break;
case 224: // 11100000 => 10110101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,0);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 225: // 11100001 => 10110100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 226: // 11100010 => 10110111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 227: // 11100011 => 10110110
{
xmmbounds = interval2f::m128_zero(); }
break;
case 228: // 11100100 => 10110001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,0);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 229: // 11100101 => 10110000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 230: // 11100110 => 10110011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 231: // 11100111 => 10110010
{
xmmbounds = interval2f::m128_zero(); }
break;
case 232: // 11101000 => 10111101
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,0);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 233: // 11101001 => 10111100
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 234: // 11101010 => 10111111
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 235: // 11101011 => 10111110
{
xmmbounds = interval2f::m128_zero(); }
break;
case 236: // 11101100 => 10111001
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,0);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 237: // 11101101 => 10111000
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,4);
__m128 smask2 = _mm_set_ps(0.0f,0.0f,-0.0f,0.0f);
r0 = _mm_xor_ps(r0,smask2);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 238: // 11101110 => 10111011
{
__m128 r0 = _mm_shuffle_ps(xmmbounds,xmmbounds,1);
__m128 r1 = _mm_shuffle_ps(I.xmmbounds,I.xmmbounds,1);
__m128 smask1 = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
r0 = _mm_xor_ps(r0,smask1);
__m128 r4 = _mm_mul_ps(r0,r1);
__m128 ord4 = _mm_cmpord_ps(r4,r4);
xmmbounds = _mm_and_ps(r4,ord4);
xmmbounds = _mm_and_ps(xmmbounds,interval2f::m128_23mask());
}
break;
case 239: // 11101111 => 10111010
{
xmmbounds = interval2f::m128_zero(); }
break;
case 240: // 11110000 => 10100101
{
xmmbounds = interval2f::m128_zero(); }
break;
case 241: // 11110001 => 10100100
{
xmmbounds = interval2f::m128_zero(); }
break;
case 242: // 11110010 => 10100111
{
xmmbounds = interval2f::m128_zero(); }
break;
case 243: // 11110011 => 10100110
{
xmmbounds = interval2f::m128_zero(); }
break;
case 244: // 11110100 => 10100001
{
xmmbounds = interval2f::m128_zero(); }
break;
case 245: // 11110101 => 10100000
{
xmmbounds = interval2f::m128_zero(); }
break;
case 246: // 11110110 => 10100011
{
xmmbounds = interval2f::m128_zero(); }
break;
case 247: // 11110111 => 10100010
{
xmmbounds = interval2f::m128_zero(); }
break;
case 248: // 11111000 => 10101101
{
xmmbounds = interval2f::m128_zero(); }
break;
case 249: // 11111001 => 10101100
{
xmmbounds = interval2f::m128_zero(); }
break;
case 250: // 11111010 => 10101111
{
xmmbounds = interval2f::m128_zero(); }
break;
case 251: // 11111011 => 10101110
{
xmmbounds = interval2f::m128_zero(); }
break;
case 252: // 11111100 => 10101001
{
xmmbounds = interval2f::m128_zero(); }
break;
case 253: // 11111101 => 10101000
{
xmmbounds = interval2f::m128_zero(); }
break;
case 254: // 11111110 => 10101011
{
xmmbounds = interval2f::m128_zero(); }
break;
case 255: // 11111111 => 10101010
{
xmmbounds = interval2f::m128_zero(); }
break;
// <<=================
  }
 
  GAOL_RND_LEAVE_SSE();
  return *this;
}
#endif


interval2f interval2f::inverse() const
  { // TODO: implement inverse method
  abort();
  }


	inline __m128 uipow_dnupdnup(const __m128& rI, unsigned int n)
	{
		__m128 _mm_signsmask = _mm_set_ps(0.0f,-0.0f,0.0f,-0.0f);

		__m128 y = _mm_set1_ps(1.0);
		__m128 z2 = rI;
		__m128 z1 = _mm_xor_ps(z2,_mm_signsmask);
		for (;;) {
			if (odd(n)) {
				y = _mm_xor_ps(_mm_mul_ps(y,z2),_mm_signsmask);
				if (n==1) { // On input, n is assumed to be non-null
					return _mm_xor_ps(y,_mm_signsmask);
				}
			}
			n >>= 1;
			z2 = _mm_mul_ps(z2,z1);
			z1 = _mm_xor_ps(z2,_mm_signsmask);
		}
	}

	inline __m128 uipow_upupupup(const __m128& rI, unsigned int n)
	{
		__m128 y = _mm_set1_ps(1.0);
		__m128 z = rI;
		for (;;) {
			if (odd(n)) {
				y = _mm_mul_ps(y,z);
				if (n==1) { // On input, n is assumed to be non-null
					return y;
				}
			}
			n >>= 1;
			z = _mm_mul_ps(z,z);
		}
	}

	inline __m128 uipow_upupdnup(const __m128& rI, unsigned int n)
	{
		__m128 _mm_lsignmask = _mm_set_ps(0.0f,0.0f,0.0f,-0.0f);
		__m128 y = _mm_set1_ps(1.0);
		__m128 z2 = rI;
		__m128 z1 = _mm_xor_ps(z2,_mm_lsignmask);
		for (;;) {
			if (odd(n)) {
				y = _mm_xor_ps(_mm_mul_ps(y,z2),_mm_lsignmask);
				if (n==1) { // On input, n is assumed to be non-null
					return _mm_xor_ps(y,_mm_lsignmask);
				}
			}
			n >>= 1;
			z2 = _mm_mul_ps(z2,z1);
			z1 = _mm_xor_ps(z2,_mm_lsignmask);
		}
	}

	inline __m128 uipow_upupzero(const __m128& rI, unsigned int n)
	{
		return uipow_upupupup(rI,n);
	}

	inline __m128 uipow_dnupzero(const __m128& rI, unsigned int n)
	{
		return uipow_dnupdnup(rI,n);
	}


	inline __m128 uipow_0x0(const __m128& rI, unsigned int e)
	{
		static __m128 _mm_02mask = _mm_cmpeq_ps(_mm_set1_ps(0.0f),_mm_set_ps(0.0f,1.0f,0.0f,1.0f));

		__m128 res = uipow_upupupup(rI,e);
		if (!odd(e)) {
			__m128 r1 = _mm_shuffle_ps(res,res,_MM_SHUFFLE(2,3,0,1));
			__m128 r2 = _mm_max_ps(res,r1);
			res = _mm_and_ps(r2,_mm_02mask);
		}
		return res;
	}

	inline __m128 uipow_0x1(const __m128& rI, unsigned int e)
	{
		static __m128 _mm_2mask = _mm_cmpeq_ps(_mm_set1_ps(0.0f),_mm_set_ps(0.0f,1.0f,0.0f,0.0f));

		__m128 res = uipow_upupdnup(rI,e);
		if (!odd(e)) {
			__m128 r1 = _mm_shuffle_ps(res,res,_MM_SHUFFLE(2,3,1,0));
			__m128 r2 = _mm_max_ps(res,r1);
			res = _mm_and_ps(r2,_mm_2mask);
		}
		return res;
	}

	inline __m128 uipow_0x2(const __m128& rI, unsigned int e)
	{
		static __m128 _mm_2mask = _mm_cmpeq_ps(_mm_set1_ps(0.0f),_mm_set_ps(0.0f,1.0f,0.0f,0.0f));

		__m128 res;
		__m128 r1 = _mm_shuffle_ps(rI,rI,_MM_SHUFFLE(3,2,0,1));
		__m128 r2 = uipow_upupdnup(r1,e);
		if (odd(e)) {
 			res = _mm_shuffle_ps(r2,r2,_MM_SHUFFLE(3,2,0,1));
		} else { // even(e)
			__m128 r1 = _mm_shuffle_ps(r2,r2,_MM_SHUFFLE(2,3,1,0));
			__m128 r3 = _mm_max_ps(r2,r1);
			res = _mm_and_ps(r3,_mm_2mask);
		}
		return res;
	}

	inline __m128 uipow_0x3(const __m128& rI, unsigned int e)
	{
		static __m128 _mm_01mask = _mm_cmpeq_ps(_mm_set1_ps(0.0f),_mm_set_ps(0.0f,0.0f,1.0f,1.0f));
		__m128 _mm_hsignmask = _mm_set_ps(0.0f,-0.0f,0.0f,0.0f);
		static __m128 _mm_2mask = _mm_cmpeq_ps(_mm_set1_ps(0.0f),_mm_set_ps(0.0f,1.0f,0.0f,0.0f));

		__m128 res = _mm_and_ps(uipow_upupzero(rI,e),_mm_01mask);
		if (!odd(e)) {
			__m128 r2 = _mm_xor_ps(res,_mm_hsignmask);
			__m128 r3 = _mm_shuffle_ps(res,res,_MM_SHUFFLE(2,3,1,0));
			__m128 r4 = _mm_max_ps(r2,r3);
			res = _mm_and_ps(r4,_mm_2mask);
		}
		return res;
	}

	inline __m128 uipow_0x6(const __m128& rI, unsigned int e)
	{
		__m128 res;
		__m128 r1 = _mm_shuffle_ps(rI,rI,_MM_SHUFFLE(3,2,0,1));
		__m128 r2 = uipow_dnupdnup(r1,e);
		if (odd(e)) {
 			res = _mm_shuffle_ps(r2,r2,_MM_SHUFFLE(3,2,0,1));
		} else {
			res = r2;
		}
		return res;
	}

	inline __m128 uipow_0x7(const __m128& rI, unsigned int e)
	{
		static __m128 _mm_01mask = _mm_cmpeq_ps(_mm_set1_ps(0.0f),_mm_set_ps(0.0f,0.0f,1.0f,1.0f));

		__m128 r1 = uipow_dnupzero(rI,e);
		__m128 res = _mm_and_ps(r1,_mm_01mask);
		return res;
	}

	inline __m128 uipow_0xa(const __m128& rI, unsigned int e)
	{
		__m128 r0 = _mm_shuffle_ps(rI,rI,_MM_SHUFFLE(2,3,0,1));
		__m128 res = uipow_dnupdnup(r0,e);
		if (odd(e)) {
			return _mm_shuffle_ps(res,res,_MM_SHUFFLE(2,3,0,1));
		} else {
			return res;
		}
	}

	inline __m128 uipow_0xb(const __m128& rI, unsigned int e)
	{
		static __m128 _mm_01mask = _mm_cmpeq_ps(_mm_set1_ps(0.0f),_mm_set_ps(0.0f,0.0f,1.0f,1.0f));

		__m128 r0 = _mm_shuffle_ps(rI,rI,_MM_SHUFFLE(2,3,0,1));
		__m128 res = uipow_dnupzero(r0,e);
		__m128 r1 = _mm_and_ps(res,_mm_01mask);
		if (odd(e)) {
			return _mm_shuffle_ps(r1,r1,_MM_SHUFFLE(2,3,0,1));
		} else {
			return r1;
		}
	}

	interval2f uipow(const interval2f& I, unsigned int e)
	{
		GAOL_RND_ENTER_SSE();
		__m128 res;
		__m128 rI = I.get_xmminterval();

		int signI = _mm_movemask_ps(rI);
		switch (signI) { // Remember that the sign of the left bound is negated
			case 0x0: // 0000: (0101) < 0\in Ir | 0\in Il >
				res = uipow_0x0(rI,e);
				break;
			case 0x1: // 0001: (0100) < 0\in Ir | Il >= 0  >
				res = uipow_0x1(rI,e);
				break;
			case 0x2: // 0010: (0111) < 0\in Ir | Il <= 0 >
				res = uipow_0x2(rI,e);
				break;
			case 0x3: // 0011: (0110) < 0\in Ir | Il == 0 >
				res = uipow_0x3(rI,e);
				break;
			case 0x4: // 0100: (0001) < Ir >= 0 | 0\in Il >
			{
				// FIXME: implemented as permutation of case 1
				__m128 r1 = _mm_shuffle_ps(rI,rI,_MM_SHUFFLE(1,0,3,2));
				__m128 r2 = uipow_0x1(r1,e);
				res = _mm_shuffle_ps(r2,r2,_MM_SHUFFLE(1,0,3,2));
				break;
			}
			case 0x5: // 0101: (0000) < Ir >= 0 | Il >= 0 >
				res = uipow_dnupdnup(rI,e);
				break;
			case 0x6: // 0110: (0011) < Ir >= 0 | Il <= 0 >
				res = uipow_0x6(rI,e);
				break;
			case 0x7: // 0111: (0010) < Ir >= 0 | Il == 0 >
				res = uipow_0x7(rI,e);
				break;
			case 0x8: // 1000: (1101) < Ir <= 0 | 0\in Il >
			{
				// FIXME: implemented as permutation of case 2
				__m128 r1 = _mm_shuffle_ps(rI,rI,_MM_SHUFFLE(1,0,3,2));
				__m128 r2 = uipow_0x2(r1,e);
				res = _mm_shuffle_ps(r2,r2,_MM_SHUFFLE(1,0,3,2));
				break;
			}
			case 0x9: // 1001: (1100) < Ir <= 0 | Il >= 0 >
			{
				// FIXME: implemented as permutation of case 6
				__m128 r1 = _mm_shuffle_ps(rI,rI,_MM_SHUFFLE(1,0,3,2));
				__m128 r2 = uipow_0x6(r1,e);
				res = _mm_shuffle_ps(r2,r2,_MM_SHUFFLE(1,0,3,2));
				break;
			}
			case 0xa: // 1010: (1111) < Ir <= 0 | Il <= 0 >
				res = uipow_0xa(rI,e);
				break;
			case 0xb: // 1011: (1110) < Ir <= 0 | Il == 0 >
				res = uipow_0xb(rI,e);
				break;
			case 0xc: // 1100: (1001) < Ir == 0 | 0\in Il >
			{
				// FIXME: implemented as permutation of case 3
				__m128 r1 = _mm_shuffle_ps(rI,rI,_MM_SHUFFLE(1,0,3,2));
				__m128 r2 = uipow_0x3(r1,e);
				res = _mm_shuffle_ps(r2,r2,_MM_SHUFFLE(1,0,3,2));
				break;
			}
			case 0xd: // 1101: (1000) < Ir == 0 | Il >= 0 >
			{
				// FIXME: implemented as permutation of case 7
				__m128 r1 = _mm_shuffle_ps(rI,rI,_MM_SHUFFLE(1,0,3,2));
				__m128 r2 = uipow_0x7(r1,e);
				res = _mm_shuffle_ps(r2,r2,_MM_SHUFFLE(1,0,3,2));
				break;
			}
			case 0xe: // 1110: (1011) < Ir == 0 | Il <= 0 >
			{
				// FIXME: implemented as permutation of case 0xb
				__m128 r1 = _mm_shuffle_ps(rI,rI,_MM_SHUFFLE(1,0,3,2));
				__m128 r2 = uipow_0xb(r1,e);
				res = _mm_shuffle_ps(r2,r2,_MM_SHUFFLE(1,0,3,2));
				break;
			}
			case 0xf: // 1111: (1010) < Ir == 0 | Il == 0 >
				res = _mm_set_ps(0.0f,-0.0f,0.0f,-0.0f); // < [0, 0] | [0, 0] >
				break;
			default: // This case should never be reached
			  res = _mm_set_ps(-1.0f,1.0f,-1.0f,1.0f); // Just ot keep the compiler happy
 		}

		GAOL_RND_LEAVE_SSE();
		return interval2f(res);
	}


/*
 	Integral power
*/
	interval2f pow(const interval2f& I, int n)
	{
		// FIXME: handle empty input
		if (n < 0) {
			return inverse(uipow(I,-n));
		} else {
			if (n > 0) {
				return uipow(I,n);
			} else {
				return interval2f(1.0);
			}
		}
	}





	bool interval2f::possibly_eq_all(const interval2f& I) const
	{
		return first().possibly_eq(I.first()) && second().possibly_eq(I.second());
	}

	bool interval2f::set_eq_all(const interval2f& I) const
	{
		return first().set_eq(I.first()) && second().set_eq(I.second());
	}

	bool interval2f::set_contains_all(const interval2f& I) const
	{
		return first().set_contains(I.first()) && second().set_contains(I.second());
	}


} // namespace gaol
