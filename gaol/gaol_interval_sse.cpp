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
 * SVN: $Id: gaol_interval_sse.cpp 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By: Frederic Goualard <Frederic.Goualard@univ-nantes.fr>
 *--------------------------------------------------------------------------*/

/*
    This file is not meant to be compiled. It is only to be included
        by gaol/gaol_interval.cpp.
*/

// TODO: check existence of stdint.h in configure.in
#include <stdint.h>

	const __m128d interval::lbsignmask = _mm_set_pd(0.0,-0.0);
	const __m128d interval::lbrbsignmask = _mm_set1_pd(-0.0);
	const __m128d interval::m128d_02mask = _mm_cmpeq_pd(_mm_set1_pd(0.0),_mm_set_pd(0.0,1.0));
	const __m128d interval::m128d_20mask = _mm_cmpeq_pd(_mm_set1_pd(0.0),_mm_set_pd(1.0,0.0));
	const __m128d interval::m128_zero = _mm_set1_pd(0.0);
	const __m128d interval::m128_minus_one = _mm_set_pd(1.0,-1.0);
	const __m128d interval::m128_nan = _mm_set1_pd(std::numeric_limits<double>::quiet_NaN());
	const __m128d interval::m128_infinf = _mm_set1_pd(std::numeric_limits<double>::infinity());

	void* interval::operator new(size_t sz)
	{
		void *buf;
		if (MEMALIGN(buf,16,sz)) { // error ?
			throw std::bad_alloc();
		}
		return buf;
	}

    void interval::operator delete(void *p)
    {
        free(p);
    }

	void* interval::operator new[](size_t sz)
	{
		void *buf;
		if (MEMALIGN(buf,16,sz)) { // error ?
			throw std::bad_alloc();
		}
		return buf;
	}



    void interval::operator delete[](void *p)
    {
        free(p);
    }

	void* interval::operator new(size_t sz, void *p)
	{
		if ((unsigned long long)p % 16 != 0) {
			throw std::bad_alloc();
		}
		return p;
	}

    void interval::operator delete(void *p, void *place)
    {
        free(p);
    }

	double hausdorff(const interval &I1, const interval &I2)
	{
		if (I1.is_empty() || I2.is_empty()) {
			return std::numeric_limits<double>::quiet_NaN();
		} else {
			int cmpinf = (_mm_movemask_pd(_mm_cmpeq_pd(I1.get_xmminterval(),interval::m128_infinf)) << 2) +
							_mm_movemask_pd(_mm_cmpeq_pd(I2.get_xmminterval(),interval::m128_infinf));
			/*
				Possible values of cmpinf:
						   I1      I2
				0000 : [a, b]     [c, d]
				0001 : [a, b]     [-oo, b]
				0010 : [a, b]     [c, +oo]
				0011 : [a, b]     [-oo, +oo]
				0100 : [-oo, b]   [c, d]
				0101 : [-oo, b]   [-oo, d]
				0110 : [-oo, b]   [c, +oo]
				0111 : [-oo, b]   [-oo, +oo]
				1000 : [a,+oo]    [c, d]
				1001 : [a, +oo]   [-oo, d]
				1010 : [a, +oo]   [c, +oo]
				1011 : [a, +oo]   [-oo, +oo]
				1100 : [-oo, +oo] [c, d]
				1101 : [-oo, +oo] [-oo, d]
				1110 : [-oo, +oo] [c, +oo]
				1111 : [-oo, +oo] [-oo, +oo]
			*/
			switch (cmpinf) {
			case 0: // 0000
				return fmax(std::fabs(I1.left()-I2.left()),std::fabs(I1.right()-I2.right()));
			case 15: // 1111
				return 0.0;
			default:
				return std::numeric_limits<double>::infinity();
			}
		}
	}


interval div_rel(const interval &K, const interval &J, const interval &I)
{
  if (K.is_empty() || J.is_empty() || I.is_empty()) { // FIXME: optimize with intrinsics
	return interval::emptyset();
  }

  if ( K.right() < 0.0 ) { // [K] N1
    if ( J.right() < 0.0 ) { // [J] N1
      GAOL_RND_ENTER_SSE();
      __m128d r = _mm_xor_pd(K.xmmbounds,interval::lbsignmask);
      __m128d r2 = _mm_div_pd(_mm_shuffle_pd(r,r,1),J.xmmbounds);
      GAOL_RND_LEAVE_SSE();
      return I & interval(r2); // N1 N1
    } else { // [J] P or Z
      if ( J.right() == 0.0 ) { // [J] N0 or Z
        if ( J.left() == 0.0 ) { // [J] Z
          return interval::emptyset(); // N1 Z
        } else {
          GAOL_RND_ENTER_SSE();
          __m128d r = _mm_move_sd(interval::m128_infinf,_mm_div_pd(_mm_shuffle_pd(K.xmmbounds,K.xmmbounds,1),J.xmmbounds)); 
          GAOL_RND_LEAVE_SSE();
          return I & interval(r); // N1 N0
        }
      } else { // [J] P or M
        if ( J.left() < 0.0 ) { // [J] M // FIXME: replace with SSE code
			GAOL_RND_ENTER_SSE();
			interval tmp(-GAOL_INFINITY,K.right()/J.right());
			interval tmp2(K.right()/(J.left()),GAOL_INFINITY);
	    	GAOL_RND_LEAVE_SSE();
	    	return (I&tmp) | (I&tmp2); // N1 M
        } else { // [J] P0 or P1
          if ( J.left() == 0.0 ) { // [I] P0
            GAOL_RND_ENTER_SSE();
            __m128d r = _mm_move_sd(_mm_div_pd(K.xmmbounds,J.xmmbounds),interval::m128_infinf);// N1 P0
            GAOL_RND_LEAVE_SSE();
            return I & interval(r);
          } else { // [J] P1
            GAOL_RND_ENTER_SSE();
            __m128d r = _mm_div_pd(K.xmmbounds,_mm_xor_pd(J.xmmbounds,interval::lbsignmask)); // N1 P1
            GAOL_RND_LEAVE_SSE();
            return I & interval(r);
          }
        }
      }
    }
  } else { // [K] N0, Z, M, or P
	if ( K.right() == 0.0 ) { // [K] N0 or Z
	  if ( K.left() == 0.0 ) { // [K] Z
		if ( J.left() <= 0.0 && J.right() >= 0.0 ) {
		  return I; // Z (N0 M, P0)
		} else {
		  return I & interval::zero(); // Z (N1,P1)
		}
	  } else { // [K] N0
		if ( J.right() < 0.0 ) { // [J] N1
			GAOL_RND_ENTER_SSE();
			__m128d r1 = _mm_xor_pd(K.xmmbounds,interval::lbsignmask);
			__m128d r2 = _mm_move_sd(_mm_div_pd(_mm_shuffle_pd(r1,r1,1),J.xmmbounds),interval::m128_zero);	// N0 N1
			GAOL_RND_LEAVE_SSE();
			return I & interval(r2);
		} else { // [J] P or Z
		  if ( J.right() == 0.0 ) { // [J] N0 or Z
        if ( J.left() == 0.0 ) { // [J] Z
          return I; // N0 Z
        } else {
          return I;// N0 N0
        }
		  } else { // [I] P or M
			if ( J.left() < 0.0 ) { // [J] M
			  return I; // N0 M
			} else { // [J] P0 or P1
			  if ( J.left() == 0.0 ) { // [J] P0
          return I; // N0 P0
			  } else { // [J] P1
          GAOL_RND_ENTER_SSE();
          __m128d r = _mm_move_sd(interval::m128_zero,_mm_div_pd(K.xmmbounds,_mm_xor_pd(J.xmmbounds,interval::lbsignmask))); 
          GAOL_RND_LEAVE_SSE();
          return I & interval(r); // N0 P1
			  }
			}
		  }
		}
	  }
	} else { // [K] M, P0, or P1
	  if ( K.left() < 0.0 ) { // [K] M
		if ( J.right() < 0.0 ) { // [J] N1
      GAOL_RND_ENTER_SSE();
		  __m128d r1 = _mm_xor_pd(K.xmmbounds,interval::lbrbsignmask);
		  __m128d r2 = _mm_div_pd(_mm_shuffle_pd(r1,r1,1),_mm_unpackhi_pd(J.xmmbounds,J.xmmbounds));
      GAOL_RND_LEAVE_SSE();
      return I & interval(r2); // M N1
		} else { // [J] P or Z
		  if ( J.right() == 0.0 ) { // [J] N0 or Z
        return I; // M N0
		  } else { // [J] P or M
			if ( J.left() < 0.0 ) { // [J] M
			  return I; // M M
			} else { // [J] P0 or P1
			  if ( J.left() == 0.0 ) { // [J] P0
				return I; // M P0
			  } else { // [J] P1
          GAOL_RND_ENTER_SSE();
          __m128d r1 = _mm_xor_pd(J.xmmbounds,interval::lbsignmask);
          __m128d r2 = _mm_div_pd(K.xmmbounds,_mm_unpacklo_pd(r1,r1)); 
          GAOL_RND_LEAVE_SSE();
          return I & interval(r2); // M P1
			  }
			}
		  }
		}
	  } else { // [K] P0 or P1
		if ( K.left() == 0.0 ) { // [K] P0
		  if ( J.right() < 0.0 ) { // [J] N1
        GAOL_RND_ENTER_SSE();
        __m128d r1 = _mm_xor_pd(_mm_shuffle_pd(K.xmmbounds,K.xmmbounds,1),interval::lbsignmask);
        __m128d r2 = _mm_shuffle_pd(J.xmmbounds,J.xmmbounds,1);
        __m128d r3 = _mm_move_sd(interval::m128_zero,_mm_div_pd(r1,r2));
        GAOL_RND_LEAVE_SSE();
        return I & interval(r3); // P0 N1
		  } else { // [J] P or Z
			if ( J.right() == 0.0 ) { // [J] N0 or Z
        return I; // P0 (Z or N0)
			} else { // [J] P or M
			  if ( J.left() < 0.0 ) { // [J] M
				return I; // P0 M
			  } else { // [J] P0 or P1
				if ( J.left() == 0.0 ) { // [J] P0
				  return I; // P0 P0
				} else { // [J] P1
          GAOL_RND_ENTER_SSE();
				  __m128d r1 = _mm_xor_pd(J.xmmbounds,interval::lbsignmask);
				  __m128d r2 = _mm_shuffle_pd(r1,r1,1);
				  __m128d r3 = _mm_move_sd(_mm_div_pd(K.xmmbounds,r2),interval::m128_zero);// P0 P1
          GAOL_RND_LEAVE_SSE();
          return I & interval(r3);
				}
			  }
			}
		  }
		} else { // [K] P1
		  if ( J.right() < 0.0 ) { // [J] N1
        GAOL_RND_ENTER_SSE();
        __m128d r1 = _mm_xor_pd(_mm_shuffle_pd(K.xmmbounds,K.xmmbounds,1),interval::lbrbsignmask);
        __m128d r2 = _mm_xor_pd(J.xmmbounds,interval::lbsignmask);
        __m128d r3 = _mm_div_pd(r1,_mm_shuffle_pd(r2,r2,1)); 
        GAOL_RND_LEAVE_SSE();
        return I & interval(r3); // P1 N1
      } else { // [J] P or Z
        if ( J.right() == 0.0 ) { // [J] N0 or Z
			  if ( J.left() == 0.0 ) { // [J] Z
				return interval::emptyset(); // P1 Z
			  } else {
          GAOL_RND_ENTER_SSE();
          __m128d r = _mm_move_sd(_mm_div_pd(_mm_shuffle_pd(K.xmmbounds,K.xmmbounds,1),
                            _mm_shuffle_pd(J.xmmbounds,J.xmmbounds,1)),
                                interval::m128_infinf); 
          GAOL_RND_LEAVE_SSE();
          return I & interval(r); // P1 N0
			  }
			} else { // [J] P or M
			  if ( J.left() < 0.0 ) { // [J] M
          GAOL_RND_ENTER_SSE(); 
          __m128d r = _mm_move_sd(_mm_div_pd(_mm_shuffle_pd(K.xmmbounds,K.xmmbounds,1),
                            _mm_shuffle_pd(J.xmmbounds,J.xmmbounds,1)),
                                interval::m128_infinf); // P1 N0
				  __m128d r2 = _mm_move_sd(interval::m128_infinf,
                        _mm_div_pd(K.xmmbounds,
												_mm_shuffle_pd(J.xmmbounds,J.xmmbounds,1))); // P1 P0
          GAOL_RND_LEAVE_SSE();
          return (I & interval(r)) | (I & interval(r2)); // P1 M
			  } else { // [J] P0 or P1
				if ( J.left() == 0.0 ) { // [J] P0
          GAOL_RND_ENTER_SSE();
				  __m128d r = _mm_move_sd(interval::m128_infinf,
                        _mm_div_pd(K.xmmbounds,
												_mm_shuffle_pd(J.xmmbounds,J.xmmbounds,1))); // P1 P0
          GAOL_RND_LEAVE_SSE();
          return I & interval(r);
				} else { // [J] P1
          GAOL_RND_ENTER_SSE();
				  __m128d r1 = _mm_xor_pd(J.xmmbounds,interval::lbsignmask);
				  __m128d r2 = _mm_div_pd(K.xmmbounds,_mm_shuffle_pd(r1,r1,1));
          GAOL_RND_LEAVE_SSE();
          return I & interval(r2); // P1 P1
				}
			  }
			}
		  }
		}
	  }
	}
  }
}

#if 0
	/*
		uipow_dnup --
		Given I=<b,a> with a>=0 and b>=0, computes <rndup(b^n), -rnddn(a^n)> 
	*/
   INLINE __m128d uipow_dnup(const __m128d& I, unsigned int n)
    { // On input, n is assumed to be non-null
      //__m128d _mm_lbsignmask = _mm_set_pd(0.0,-0.0);

      __m128d y = _mm_set_pd(1.0,-1.0);
      __m128d z = _mm_xor_pd(I,interval::lbsignmask);
      for (;;) {
		if (odd(n)) {
			y = _mm_xor_pd(y,interval::lbsignmask);
	  		y = _mm_mul_pd(y,z);
	  		if (n==1) { // On input, n is assumed to be non-null
	    		return y;
	  		}
		}
		n >>= 1;
		__m128d z2 = _mm_xor_pd(z,interval::lbsignmask);
		z = _mm_mul_pd(z,z2);
      }
    }

	/*
		uipow_upup --
		Given I=<b,a> with a>=0 and b>=0, computes <rndup(b^n), rndup(a^n)> 
	*/
  INLINE __m128d uipow_upup(const __m128d& I, unsigned int n)
    {
      __m128d y = _mm_set_pd(1.0,1.0);
      __m128d z = I;
      for (;;) {
		if (odd(n)) {
	  		y = _mm_mul_pd(y,z);
	  		if (n==1) { // On input, n is assumed to be non-null
	    		return y;
	  		}
		}
		n >>= 1;
		z = _mm_mul_pd(z,z);
      }
    }

#endif

#if 1
/*
  Returns the position of the Most Significant bit.
  Algorithm from: Bit Twiddling Hacks
        (http://graphics.stanford.edu/~seander/bithacks.html)
  NOTE: v should be different from 0 
  */
unsigned char MSB_position(uint32_t v)
{
    static const int MultiplyDeBruijnBitPosition[32] =
    {
        0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30,
        8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31
    };

    v |= v >> 1; // first round down to one less than a power of 2
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;

    return MultiplyDeBruijnBitPosition[(uint32_t)(v * 0x07C4ACDDU) >> 27];
}

/*
  Reverses the chain bit of v
  Algorithm adapted from: Bit Twiddling Hacks
        (http://graphics.stanford.edu/~seander/bithacks.html)
*/
INLINE uint32_t reverse_bits(uint32_t v)
{
    static const unsigned char reverse_lookup[] =
    {
     0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
     0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
     0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
     0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
     0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
     0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
     0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
     0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
     0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
     0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
     0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
     0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
     0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
     0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
     0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
     0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
    };
    static const int Mod37BitPosition[] = // map a bit value mod 37 to its position
    {
     32, 0, 1, 26, 2, 23, 27, 0, 3, 16, 24, 30, 28, 11, 0, 13, 4,
     7, 17, 0, 25, 22, 31, 15, 29, 10, 12, 6, 0, 21, 14, 9, 5,
     20, 8, 19, 18
    };

    uint32_t res =  (reverse_lookup[v & 0xff] << 24) |
                    (reverse_lookup[(v >> 8) & 0xff] << 16) |
                    (reverse_lookup[(v >> 16) & 0xff] << 8) |
                    (reverse_lookup[(v >> 24) & 0xff]);

    return res >> Mod37BitPosition[(-res & res) % 37];
}

	/*
		uipow_dnup --
		Given I=<b,a> with a>=0 and b>=0, computes <rndup(b^n), -rnddn(a^n)> 
	*/
  INLINE __m128d uipow_dnup(const __m128d& I, unsigned int n)
    { // n assumed to be different from 0
		__m128d res = _mm_xor_pd(I,interval::lbsignmask);
		unsigned char cpt = MSB_position(n);
		n = reverse_bits(n) >> 1;
		while (cpt > 0) {
			__m128d res2 = _mm_xor_pd(res,interval::lbsignmask);
			res = _mm_mul_pd(res,res2);
			if (odd(n)) {
				res = _mm_mul_pd(res,I);
			}
			n >>= 1;
			--cpt;
		}
		return res;
    }

	/*
		uipow_upup --
		Given I=<b,a> with a>=0 and b>=0, computes <rndup(b^n), rndup(a^n)> 
	*/
  INLINE __m128d uipow_upup(const __m128d& I, unsigned int n)
    { // n assumed to be different from 0
     	__m128d res = I;
		unsigned char cpt = MSB_position(n);
		n = reverse_bits(n) >> 1;

		while (cpt > 0) {
			res = _mm_mul_pd(res,res);
			if (odd(n)) {
				res = _mm_mul_pd(res,I);
			}
			n >>= 1;
			--cpt;
		}
		return res;
	}
#endif

  INLINE interval uipow(const interval& I, unsigned int e)
    {
      GAOL_RND_ENTER_SSE();
      __m128d res;
      __m128d Ix = I.get_xmminterval();

      int signI = _mm_movemask_pd(Ix);
      switch (signI) { // Remember that the sign of the left bound is negated
      case 0: // 00: I.straddles_zero()
	{
	  res = uipow_upup(Ix,e);
	  if (!odd(e)) {
	    __m128d res2 = _mm_shuffle_pd(res,res,1);
	    __m128d res3 = _mm_max_pd(res,res2);
	    res = _mm_and_pd(res3,interval::m128d_02mask);
	    //res = interval(0.0,fmax(-res.left(),res.right()));
	  }
	  break;
	}
      case 1: // 01: I.positive()
	{
	  Ix = _mm_xor_pd(Ix,interval::lbsignmask); // Make both stored bounds positive 
	  res = uipow_dnup(Ix,e);
	  break;
	}
      case 2: // 10: I.negative()
	{
	  Ix = _mm_shuffle_pd(Ix,Ix,1);
	  Ix = _mm_xor_pd(Ix,interval::lbsignmask); // Make both stored bounds positive 
	  res = uipow_dnup(Ix,e);
	  if (odd(e)) {
	    //res = -res;
	    res = _mm_shuffle_pd(res,res,1);
	  }
	  break;
	}
      case 3: // 11: I=[0, -0] (empty intervals have been filtered already)
	{
	  //res = interval(0.0);
	  res = _mm_set_pd(0.0,-0.0);
	  break;
	}
	  default: // This case should never be attained
		res= _mm_set_pd(-1.0,1.0);  // Just to keep the compiler happy.
      
      }

      GAOL_RND_LEAVE_SSE();
      return interval(res);
    }


  interval interval::operator-() const
  {
    return interval(_mm_shuffle_pd(xmmbounds,xmmbounds,_MM_SHUFFLE2(0,1)));
  }

  interval sqr(const interval& I)
  {
    if (I.is_empty()) {
        return I;
    } else {
        return uipow(I,2);
    }
  }


 interval& interval::operator+=(const interval& I)
    {
      GAOL_RND_ENTER_SSE();
      xmmbounds = _mm_add_pd(xmmbounds, I.xmmbounds);
      GAOL_RND_LEAVE_SSE();
      return *this;
    }

  interval& interval::operator-=(const interval& I)
    {
      GAOL_RND_ENTER_SSE();
      xmmbounds = _mm_add_pd(xmmbounds, _mm_shuffle_pd(I.xmmbounds,I.xmmbounds,_MM_SHUFFLE2(0,1)));
      GAOL_RND_LEAVE_SSE();
      return *this;
    }

   interval& interval::operator+=(double v)
    {
        // FIXME: optimize the code
        *this += interval(v);
        return *this;
    }

    interval& interval::operator-=(double v)
    {
        // FIXME: optimize the code
        *this -= interval(v);
        return *this;
    }

    interval& interval::operator*=(double v)
    {
        // FIXME: optimize the code
        *this *= interval(v);
        return *this;
    }

    interval& interval::operator/=(double v)
    {
        // FIXME: optimize the code
        *this /= interval(v);
        return *this;
    }

    interval& interval::operator%=(double v)
    {
        // FIXME: optimize the code
        *this %= interval(v);
        return *this;
    }


  interval interval::mid() const
  {
    if (is_empty()) {
      return interval::emptyset();
    }
    if (is_symmetric()) { // symmetric case handles [-oo, +oo]
      return 0.0;
    }
    if (left() == -GAOL_INFINITY) {
      return interval(-std::numeric_limits<double>::max());
    }
    if (right() == GAOL_INFINITY) {
      return interval(std::numeric_limits<double>::max());
    }
    GAOL_RND_ENTER();
    double l = left_internal();
    double r = right_internal();
    // FIXME: check formulas below in case of underflow (cf. midpoint())
    double mid_left  = -(.5*l - .5*r);
    double mid_right = (-.5*l) + .5*r;
    GAOL_RND_LEAVE();
    return interval(mid_left,mid_right);
  }

#if 0
interval& interval::operator*=(const interval& I)
{
  if (is_empty() || I.is_empty()) { // FIXME: optimize with intrinsics
	*this = interval::emptyset();
	return *this;
  }
  
  if (is_zero()) {
	return *this;
  }
  if (I.is_zero()) {
	xmmbounds=interval::m128_zero;
	return *this;
  }
  GAOL_RND_ENTER_SSE();
  
  if (certainly_positive()) { // P ?
	if (I.certainly_positive()) { // P P
	  xmmbounds = _mm_mul_pd(_mm_xor_pd(xmmbounds,lbsignmask),I.xmmbounds);
	} else {
	  if (I.strictly_straddles_zero()) { // P M
		xmmbounds = _mm_mul_pd(_mm_unpackhi_pd(xmmbounds,xmmbounds),I.xmmbounds);
	  } else { // P N
		__m128d ba = _mm_xor_pd(xmmbounds,lbsignmask);
		xmmbounds = _mm_mul_pd(_mm_shuffle_pd(ba,ba,1),I.xmmbounds);
	  }
	}
  } else {
	if (strictly_straddles_zero()) { // M ?
	  if (I.certainly_positive()) { // M P
		xmmbounds = _mm_mul_pd(xmmbounds,_mm_unpackhi_pd(I.xmmbounds,I.xmmbounds));
	  } else {
		if (I.strictly_straddles_zero()) { // M M
		  xmmbounds = _mm_max_pd(_mm_mul_pd(_mm_unpacklo_pd(xmmbounds,xmmbounds),
											_mm_shuffle_pd(I.xmmbounds,I.xmmbounds,1)),
								 _mm_mul_pd(_mm_unpackhi_pd(xmmbounds,xmmbounds),
											I.xmmbounds));
		} else { // M N
		  xmmbounds = _mm_mul_pd(_mm_shuffle_pd(xmmbounds,xmmbounds,1),
								 _mm_unpacklo_pd(I.xmmbounds,I.xmmbounds));
		}
	  }
	} else { // N  (case Z already tested)
	  if (I.certainly_positive()) { // N P
		__m128d tmp = _mm_xor_pd(I.xmmbounds,lbsignmask);
		xmmbounds = _mm_mul_pd(xmmbounds,_mm_shuffle_pd(tmp,tmp,1));
	  } else {
		if (I.strictly_straddles_zero()) { // N M
		  xmmbounds = _mm_mul_pd(_mm_unpacklo_pd(xmmbounds,xmmbounds),
								 _mm_shuffle_pd(I.xmmbounds,I.xmmbounds,1));
		} else { // N N
		  xmmbounds = _mm_mul_pd(_mm_xor_pd(_mm_shuffle_pd(xmmbounds,xmmbounds,1),lbsignmask),
								 _mm_shuffle_pd(I.xmmbounds,I.xmmbounds,1));
		}
	  }
	}
  }
  
  GAOL_RND_LEAVE_SSE();
  return *this;
}
#endif 
#if 0
// Modified Lambov algorithm
  interval& interval::operator*=(const interval& Itv)
    {
      if (is_empty() || Itv.is_empty()) { // FIXME: optimize with intrinsics
       *this = interval::emptyset();
       return *this;
       }
      GAOL_RND_ENTER_SSE();

      // *this = <b, -a>
      // Itv = <d, -c>
      __m128d A = _mm_xor_pd(xmmbounds,interval::lbsignmask);  	// < b, a >
      __m128d B = _mm_shuffle_pd(Itv.xmmbounds,Itv.xmmbounds,1); 	// < -c, d >
      __m128d C = _mm_cmplt_pd(A,interval::m128_zero);				// < b<0, a<0 >
      __m128d D = _mm_xor_pd(B,interval::lbrbsignmask);		// < c, -d >
      __m128d G = _mm_shuffle_pd(C,C,1); 				// < a<0, b<0 >
      __m128d E = _mm_and_pd(C,D); 						// < (b<0)?c:0, (a<0)?-d:0 >
      __m128d F = _mm_andnot_pd(C,Itv.xmmbounds);		// < (b>=0)?d:0, (a>=0)?-c:0 >
      __m128d K = _mm_andnot_pd(G,Itv.xmmbounds);		// < (a>=0)?d:0, (b>=0)?-c:0 >
      __m128d I = _mm_and_pd(D,G);						// < (a<0)?c:0, (b<0)?-d:0 >
      __m128d H = _mm_or_pd(E,F);						// < (b<0)?c:d, (a<0)?-d:-c >
      __m128d L = _mm_shuffle_pd(A,A,1);				// < a, b >
      __m128d M = _mm_or_pd(I,K);						// < (a<0)?c:d, (b<0)?-d:-c >
      __m128d J = _mm_mul_pd(A,H);						// < (b<0)?bc:bd, (a<0)?a(-d):a(-c) >
      __m128d N = _mm_mul_pd(L,M);						// < (a<0)?ac:ad, (b<0)?b(-d):b(-c) >
      __m128d ordJ = _mm_cmpord_pd(J,J);				//
      __m128d newJ = _mm_and_pd(J,ordJ);
      __m128d ordN = _mm_cmpord_pd(N,N);
      __m128d newN = _mm_and_pd(N,ordN);

      xmmbounds = _mm_max_pd(newJ,newN);							// < (b<0)?max(ac,bc)
      // 		 :(a<0)?max(ac,bd)
      //					 :max(ad,bd),
      //	  (b<0)?max(b(-d),a(-d))
      //        :(a<0)?max(b(-c),a(-d))
      //			       :max(b(-c),a(-c)) >
      GAOL_RND_LEAVE_SSE();
      return *this;
    }
#endif 

#if 0
// Brute force algorithm
interval& interval::operator*=(const interval& I)
{
        if (is_empty() || I.is_empty()) { // FIXME: optimize with intrinsics
                *this = interval::emptyset();
                return *this;
        }

	  GAOL_RND_ENTER_SSE();
        __m128d r1  = _mm_shuffle_pd(xmmbounds,xmmbounds,1);            // r1  = <-a, b>
        __m128d r2  = _mm_xor_pd(r1,lbrbsignmask);                    // r2  = <a, -b>
        __m128d r3  = _mm_xor_pd(I.xmmbounds,lbsignmask);             // r3  = <d, c>
        __m128d r4  = _mm_shuffle_pd(r3,r3,1);                          // r4  = <c, d>
        __m128d r5  = _mm_unpacklo_pd(I.xmmbounds,I.xmmbounds);         // r5  = <-c, -c>
        __m128d r6  = _mm_unpackhi_pd(I.xmmbounds,I.xmmbounds);         // r6  = <d, d>
        __m128d r7  = _mm_mul_pd(xmmbounds,r3);                         // r7  = <bd, (-a)c>
		__m128d ordr7 = _mm_cmpord_pd(r7,r7);
		r7 = _mm_and_pd(r7,ordr7);
        __m128d r8  = _mm_mul_pd(r1,r5);                                // r8  = <ac, b(-c)>
		__m128d ordr8 = _mm_cmpord_pd(r8,r8);
		r8 = _mm_and_pd(r8,ordr8);
        __m128d r9  = _mm_mul_pd(xmmbounds,r4);                         // r9  = <bc, (-a)d>
		__m128d ordr9 = _mm_cmpord_pd(r9,r9);
		r9 = _mm_and_pd(r9,ordr9);
        __m128d r10 = _mm_mul_pd(r2,r6);                                // r10 = <ad,(-b)d>
		__m128d ordr10 = _mm_cmpord_pd(r10,r10);
		r10 = _mm_and_pd(r10,ordr10);
        __m128d r11 = _mm_max_pd(r7, r8);
        __m128d r12 = _mm_max_pd(r9, r10);
        xmmbounds   = _mm_max_pd(r11,r12);
		GAOL_RND_LEAVE_SSE();
        return *this;

}
#endif

#if 1
// Test algorithm (9 cases, à la Wolff von Gudenberg)
interval& interval::operator*=(const interval& I)
{
  if (is_empty() || I.is_empty()) { // FIXME: optimize with intrinsics
	*this = interval::emptyset();
	return *this;
  }
  
  GAOL_RND_ENTER_SSE();
  
  int signs = (_mm_movemask_pd(I.xmmbounds) << 2) | _mm_movemask_pd(xmmbounds);
  // signs: bits 0 and 2 shall be negated since the left bounds are stored negated
  switch (signs) {
	case 3:	 // 0011b -> 0110b
	case 7:  // 0111b -> 0010b
	case 11: // 1011b -> 1110b
	case 12: // 1100b -> 1001b
	case 13: // 1101b -> 1000b
	case 15: // 1111b -> 1010b
	case 14: // 1110b -> 1011b
	  // [0, 0]
	  {
		xmmbounds = interval::m128_zero;
	  }
	  break;
	case 0:  // 0000b -> 0101b 
	  // (5) [min(ad,bc), max(ac, bd)]
	  {
		__m128d r0 = _mm_unpacklo_pd(xmmbounds,xmmbounds);
		__m128d r1 = _mm_shuffle_pd(I.xmmbounds,I.xmmbounds,1);
		__m128d r2 = _mm_mul_pd(r0, r1);
		__m128d ordr2 = _mm_cmpord_pd(r2,r2);
		r2 = _mm_and_pd(r2,ordr2);
			
		__m128d r3 = _mm_unpackhi_pd(xmmbounds,xmmbounds);
		__m128d r4 = _mm_mul_pd(r3, I.xmmbounds);
		__m128d ordr4 = _mm_cmpord_pd(r4,r4);
		r4 = _mm_and_pd(r4,ordr4);
		xmmbounds = _mm_max_pd(r2,r4);
	  }
	  break;		
	case 1:  // 0001b -> 0100b
	  // (8) [bc, bd]
	  {
		__m128d r0 = _mm_unpackhi_pd(xmmbounds,xmmbounds);
		__m128d r1 = _mm_mul_pd(r0,I.xmmbounds);
		__m128d ordr1= _mm_cmpord_pd(r1,r1);
		xmmbounds = _mm_and_pd(r1,ordr1);
	  }
	  break;
	case 2:  // 0010b -> 0111b
	  // (2) [ad, ac]
	  {
		__m128d r0 = _mm_unpacklo_pd(xmmbounds,xmmbounds);
		__m128d r1 = _mm_shuffle_pd(I.xmmbounds,I.xmmbounds,1);
		__m128d r2 = _mm_mul_pd(r0, r1);
		__m128d ordr2 = _mm_cmpord_pd(r2,r2);
		xmmbounds = _mm_and_pd(r2,ordr2);
	  }
	  break;
	case 4:  // 0100b -> 0001b
	  // (6) [ad, bd]
	  {
		__m128d r0 = _mm_unpackhi_pd(I.xmmbounds,I.xmmbounds);
		__m128d r1 = _mm_mul_pd(xmmbounds, r0);
		__m128d ordr1 = _mm_cmpord_pd(r1,r1);
		xmmbounds = _mm_and_pd(r1,ordr1);
	  }
	  break;
	case 5:  // 0101b -> 0000b
	  // (9) [ac, bd]
	  {
		__m128d r0 = _mm_xor_pd(xmmbounds,lbsignmask);
		__m128d r1 = _mm_mul_pd(r0,I.xmmbounds);
		__m128d ordr1 = _mm_cmpord_pd(r1,r1);
		xmmbounds = _mm_and_pd(r1,ordr1);
	  }
	  break;
	case 6:  // 0110b -> 0011b
	  // (3) [ad, bc]
	  {
		__m128d r0 = _mm_xor_pd(I.xmmbounds,lbsignmask);
		__m128d r1 = _mm_shuffle_pd(r0,r0,1);
		__m128d r2 = _mm_mul_pd(xmmbounds,r1);
		__m128d ordr2 = _mm_cmpord_pd(r2,r2);
		xmmbounds = _mm_and_pd(r2,ordr2);
	  }
	  break;
	case 8:  // 1000b -> 1101b
	  // (4) [bc, ac]
	  {
		__m128d r0 = _mm_shuffle_pd(xmmbounds,xmmbounds,1);	
		__m128d r1 = _mm_unpacklo_pd(I.xmmbounds,I.xmmbounds);
		__m128d r2 = _mm_mul_pd(r0, r1);
		__m128d ordr2 = _mm_cmpord_pd(r2,r2);
		xmmbounds = _mm_and_pd(r2,ordr2);
	  }
	  break;
	case 9:  // 1001b -> 1100b
	  // (7) [bc, ad]
	  {
		__m128d r0 = _mm_xor_pd(xmmbounds,lbsignmask);
		__m128d r1 = _mm_shuffle_pd(r0,r0,1);
		__m128d r2 = _mm_mul_pd(r1,I.xmmbounds);
		__m128d ordr2 = _mm_cmpord_pd(r2,r2);
		xmmbounds = _mm_and_pd(r2,ordr2);
	  }
	  break;
	case 10: // 1010b -> 1111b
	  // (1) [bd, ac]
	  {	
		__m128d r0 = _mm_shuffle_pd(xmmbounds,xmmbounds,1);
		__m128d r1 = _mm_xor_pd(r0, lbsignmask);
		__m128d r2 = _mm_shuffle_pd(I.xmmbounds,I.xmmbounds,1);
		__m128d r3 = _mm_mul_pd(r1,r2);
		__m128d ordr3 = _mm_cmpord_pd(r3,r3);
		xmmbounds = _mm_and_pd(r3,ordr3);
	  }
	  break;
  }
  GAOL_RND_LEAVE_SSE();
  return *this;
}
#endif


interval& interval::operator/=(const interval& I)
{ // TODO: replace left() and right() by local variable
	if (is_empty() || I.is_empty()) { // TODO: optimize with intrinsics
		*this = interval::emptyset();
		return *this;
	}
	GAOL_RND_ENTER_SSE();

	if ( right() < 0.0 ) { // [this] N1
		if ( I.right() < 0.0 ) { // [I] N1
			__m128d r = _mm_xor_pd(xmmbounds,interval::lbsignmask);
			xmmbounds = _mm_div_pd(_mm_shuffle_pd(r,r,1),I.xmmbounds); // N1 N1
		} else { // [I] P or Z
			if ( I.right() == 0.0 ) { // [I] N0 or Z
				if ( I.left() == 0.0 ) { // [I] Z
					*this = interval::emptyset(); // N1 Z
				} else {
					xmmbounds = _mm_move_sd(interval::m128_infinf,_mm_div_pd(_mm_shuffle_pd(xmmbounds,xmmbounds,1),I.xmmbounds)); // N1 N0
				}
			} else { // [I] P or M
				if ( I.left() < 0.0 ) { // [I] M
					*this = interval::universe(); // N1 M
				} else { // [I] P0 or P1
					if ( I.left() == 0.0 ) { // [I] P0
						xmmbounds = _mm_move_sd(_mm_div_pd(xmmbounds,I.xmmbounds),interval::m128_infinf);// N1 P0
					} else { // [I] P1
						xmmbounds = _mm_div_pd(xmmbounds,_mm_xor_pd(I.xmmbounds,interval::lbsignmask)); // N1 P1
					}
				}
			}
		}
	} else { // [this] N0, Z, M, or P
		if ( right() == 0.0 ) { // [this] N0 or Z
			if ( left() == 0.0 ) { // [this] Z
				if ( I.left() == 0.0 && I.right() == 0.0 ) {
					*this = interval::emptyset(); // Z Z
				} else {
					xmmbounds = interval::m128_zero; // Z (N,M,P)
				}
			} else { // [this] N0
				if ( I.right() < 0.0 ) { // [I] N1
					__m128d r1 = _mm_xor_pd(xmmbounds,interval::lbsignmask);
					xmmbounds = _mm_move_sd(_mm_div_pd(_mm_shuffle_pd(r1,r1,1),I.xmmbounds),interval::m128_zero);	// N0 N1
				} else { // [I] P or Z
					if ( I.right() == 0.0 ) { // [I] N0 or Z
						if ( I.left() == 0.0 ) { // [I] Z
							*this = interval::emptyset(); // N0 Z
						} else {
							*this = interval::positive();// N0 N0
						}
					} else { // [I] P or M
						if ( I.left() < 0.0 ) { // [I] M
							*this = interval::universe(); // N0 M
						} else { // [I] P0 or P1
							if ( I.left() == 0.0 ) { // [I] P0
								*this = interval::negative(); // N0 P0
							} else { // [I] P1
								xmmbounds = _mm_move_sd(interval::m128_zero,_mm_div_pd(xmmbounds,_mm_xor_pd(I.xmmbounds,interval::lbsignmask))); // N0 P1
							}
						}
					}
				}
			}
		} else { // [this] M, P0, or P1
			if ( left() < 0.0 ) { // [this] M
				if ( I.right() < 0.0 ) { // [I] N1
					__m128d r1 = _mm_xor_pd(xmmbounds,interval::lbrbsignmask);
					xmmbounds = _mm_div_pd(_mm_shuffle_pd(r1,r1,1),_mm_unpackhi_pd(I.xmmbounds,I.xmmbounds));// M N1
				} else { // [I] P or Z
					if ( I.right() == 0.0 ) { // [I] N0 or Z
						if ( I.left() == 0.0 ) { // [I] Z
							*this = interval::emptyset(); // M Z
						} else {
							*this = interval::universe(); // M N0
						}
					} else { // [I] P or M
						if ( I.left() < 0.0 ) { // [I] M
							*this = interval::universe(); // M M
						} else { // [I] P0 or P1
							if ( I.left() == 0.0 ) { // [I] P0
								*this = interval::universe(); // M P0
							} else { // [I] P1
								__m128d r1 = _mm_xor_pd(I.xmmbounds,interval::lbsignmask);
								xmmbounds = _mm_div_pd(xmmbounds,_mm_unpacklo_pd(r1,r1)); // M P1
							}
						}
					}
				}
			} else { // [this] P0 or P1
				if ( left() == 0.0 ) { // [this] P0
					if ( I.right() < 0.0 ) { // [I] N1
						__m128d r1 = _mm_xor_pd(_mm_shuffle_pd(xmmbounds,xmmbounds,1),interval::lbsignmask);
						__m128d r2 = _mm_shuffle_pd(I.xmmbounds,I.xmmbounds,1);
						xmmbounds = _mm_move_sd(interval::m128_zero,_mm_div_pd(r1,r2));// P0 N1
					} else { // [I] P or Z
						if ( I.right() == 0.0 ) { // [I] N0 or Z
							if ( I.left() == 0.0 ) { // [I] Z
								*this = interval::emptyset(); // P0 Z
							} else {
								*this = interval::negative(); // P0 N0
							}
						} else { // [I] P or M
							if ( I.left() < 0.0 ) { // [I] M
								*this = interval::universe(); // P0 M
							} else { // [I] P0 or P1
								if ( I.left() == 0.0 ) { // [I] P0
									*this = interval::positive(); // P0 P0
								} else { // [I] P1
									__m128d r1 = _mm_xor_pd(I.xmmbounds,interval::lbsignmask);
									__m128d r2 = _mm_shuffle_pd(r1,r1,1);
									xmmbounds = _mm_move_sd(_mm_div_pd(xmmbounds,r2),interval::m128_zero);// P0 P1
								}
							}
						}
					}
				} else { // [this] P1
					if ( I.right() < 0.0 ) { // [I] N1
						__m128d r1 = _mm_xor_pd(_mm_shuffle_pd(xmmbounds,xmmbounds,1),interval::lbrbsignmask);
						__m128d r2 = _mm_xor_pd(I.xmmbounds,interval::lbsignmask);
						xmmbounds = _mm_div_pd(r1,_mm_shuffle_pd(r2,r2,1)); // P1 N1
					} else { // [I] P or Z
						if ( I.right() == 0.0 ) { // [I] N0 or Z
							if ( I.left() == 0.0 ) { // [I] Z
								*this = interval::emptyset(); // P1 Z
							} else {
								xmmbounds = _mm_move_sd(_mm_div_pd(_mm_shuffle_pd(xmmbounds,xmmbounds,1),
																			  _mm_shuffle_pd(I.xmmbounds,I.xmmbounds,1)),
															   interval::m128_infinf); // P1 N0
							}
						} else { // [I] P or M
							if ( I.left() < 0.0 ) { // [I] M
								*this = interval::universe(); // P1 M
							} else { // [I] P0 or P1
								if ( I.left() == 0.0 ) { // [I] P0
									xmmbounds = _mm_move_sd(interval::m128_infinf,
														         _mm_div_pd(xmmbounds,
                                                              _mm_shuffle_pd(I.xmmbounds,I.xmmbounds,1))); // P1 P0
								} else { // [I] P1
									__m128d r1 = _mm_xor_pd(I.xmmbounds,interval::lbsignmask);
									xmmbounds = _mm_div_pd(xmmbounds,_mm_shuffle_pd(r1,r1,1));// P1 P1
								}
							}
						}
					}
				}
			}
		}
	}

	GAOL_RND_LEAVE_SSE();
	return *this;
}

#define OPT_INV 1
#if OPT_INV
interval interval::inverse() const
{
  if (is_empty()) {
    return *this;
  }
  GAOL_RND_ENTER_SSE();
  __m128d r1 = _mm_xor_pd(xmmbounds,interval::lbsignmask); // r1 == < d, c >
  __m128d r2 = _mm_shuffle_pd(r1,r1,1); // r2 == <c, d>
  __m128d r3 = _mm_div_pd(interval::m128_minus_one,r2); // r3 == < -1/c, -1/d >
  __m128d r4 = _mm_cmpeq_pd(r2,interval::m128_zero); // r4 == < c==0, d==0 >
  __m128d r5 = _mm_shuffle_pd(r4,r4,1); // r5 == < d==0, c==0 >
  __m128d r6 = _mm_and_pd(r4,r5); // r6 == < c==0 && d==0, c==0 && d==0 >
  __m128d r7 = _mm_and_pd(r6,interval::m128_nan); // < r7 == < (c==0 && d==0) ? NaN : 0, (c==0 && d==0) ? NaN : 0 >
  __m128d r8 = _mm_and_pd(r4,interval::m128_infinf); // r8 == < (c==0) ? inf : 0, (d==0) ? inf : 0 >
  __m128d r9 = _mm_andnot_pd(r4,r3); // r9 == < (c!=0) ? -1/c : 0, (d!=0) ? -1/d : 0 >
  __m128d r10 = _mm_or_pd(r9,r8); // r10 == < (c==0) ? inf : -1/c, (d==0) ? inf : -1/d >
  __m128d r11 = _mm_andnot_pd(r6,r10); // r11 == < (c!=0 || d!=0) ? hi(r10) : 0, (c!=0 || d!=0) ? lo(r10) : 0 >
  __m128d r12 = _mm_or_pd(r11,r7); // r12 == < (c==0 && d==0) ? NaN : hi(r10), (c==0 && d==0) ? NaN : lo(r10) >
  __m128d r13 = _mm_cmpgt_pd(xmmbounds,interval::m128_zero); // r13 == < d>0, (-c)>0 >
  __m128d r14 = _mm_shuffle_pd(r13,r13,1); // r14 == < (-c)>0, d>0 >
  __m128d r15 = _mm_and_pd(r13,r14); // r15 == < d>0 && (-c)>0, (-c)>0 && d>0 >
  __m128d r16 = _mm_and_pd(r15, interval::m128_infinf); // r14 == < ((-c)>0 && d>0) ? inf : 0, ((-c)>0 && d>0) ? inf : 0 >
  __m128d r17 = _mm_andnot_pd(r15, r12); // r17 ==  < ((-c)<=0 || d<=0) ? hi(r12) : 0, ((-c)<=0 || d<=0) ? lo(r12) : 0 >
  __m128d r18 = _mm_or_pd(r16,r17); // < ((-c)>0 && d>0) ? inf : hi(r12), ((-c)>0 && d>0) ? inf : lo(r12) >
  GAOL_RND_LEAVE_SSE();
  return interval(r18);
}
#else
  interval interval::inverse() const
  {
    if (is_empty()) {
      return *this;
    }

    interval res;
    xmm2d itv;
    get_bounds(itv);
    GAOL_RND_ENTER_SSE();
    if (itv[1] <= 0.0) { // (Z N0 N1)
      if (itv[0] > 0) { // (N0 N1)
        if (itv[1] < 0) { // N1
          __m128d r0 = _mm_set1_pd(-1.0); // r0 == <-1, -1>
          __m128d r1 = _mm_shuffle_pd(xmmbounds,xmmbounds,1); // r1 == <-c, d>
          res.xmmbounds = _mm_div_pd(r0,r1);
        } else { // N0
          __m128d r0 = _mm_set_pd(-1.0,1.0); // r0 == <-1, 1>
          __m128d r1 = _mm_shuffle_pd(xmmbounds,xmmbounds,1); // r1 == <-c, d>
          __m128d r2 = _mm_and_pd(r1,interval::m128d_02mask); // r2 == <-c, 0>
          res.xmmbounds = _mm_div_pd(r0,r2);
        }
      } else { // Z
        res = interval::emptyset();
      }
    } else { // (M P0 P1)
      if (itv[0]>0) { // M : straddles_zero() (left bound negated)
            res = interval::universe();
      } else { // (P0 P1)
        if (itv[0] == 0.0) { // P0
          __m128d r0 = _mm_set_pd(1.0,-1.0); // r0 == <1, -1>
          __m128d r1 = _mm_xor_pd(xmmbounds,interval::lbsignmask); // r1 == <d, c>
          __m128d r2 = _mm_shuffle_pd(r1,r1,1); // r2 == <c, d>
          __m128d r3 = _mm_and_pd(r2,interval::m128d_20mask); // r3 == <0, d>
          res.xmmbounds = _mm_div_pd(r0,r3);
        } else { // P1
          __m128d r0 = _mm_set1_pd(-1.0); // r0 == <-1, -1>
          __m128d r1 = _mm_shuffle_pd(xmmbounds,xmmbounds,1); // r1 == <-c, d>
          res.xmmbounds = _mm_div_pd(r0,r1);
        }
      }
    }
    GAOL_RND_LEAVE_SSE();
    return res;
  }
#endif

  interval& interval::operator%=(const interval& I)
  {
	if (is_empty() || I.is_empty()) { // FIXME: optimize with intrinsics
		*this = interval::emptyset();
		return *this;
	}

	GAOL_RND_ENTER_SSE();
	if ( right() < 0.0 ) { // [this] N1
		if ( I.right() < 0.0 ) { // [I] N1
			__m128d r = _mm_xor_pd(xmmbounds,interval::lbsignmask);
			xmmbounds = _mm_div_pd(_mm_shuffle_pd(r,r,1),I.xmmbounds); // N1 N1
		} else { // [I] P or Z
			if ( I.right() == 0.0 ) { // [I] N0 or Z
				if ( I.left() == 0.0 ) { // [I] Z
					*this = interval::emptyset(); // N1 Z
				} else {
					xmmbounds = _mm_move_sd(interval::m128_infinf,_mm_div_pd(_mm_shuffle_pd(xmmbounds,xmmbounds,1),I.xmmbounds)); // N1 N0
				}
			} else { // [I] P or M
				if ( I.left() < 0.0 ) { // [I] M
					*this = interval::universe(); // N1 M
				} else { // [I] P0 or P1
					if ( I.left() == 0.0 ) { // [I] P0
						xmmbounds = _mm_move_sd(_mm_div_pd(xmmbounds,I.xmmbounds),interval::m128_infinf);// N1 P0
					} else { // [I] P1
						xmmbounds = _mm_div_pd(xmmbounds,_mm_xor_pd(I.xmmbounds,interval::lbsignmask)); // N1 P1
					}
				}
			}
		}
	} else { // [this] N0, Z, M, or P
		if ( right() == 0.0 ) { // [this] N0 or Z
			if ( left() == 0.0 ) { // [this] Z
				if ( I.right() < 0.0 || I.left() > 0.0 ) {
					xmmbounds = interval::m128_zero; // Z (N1 P1)
				} else {
					*this = interval::universe(); // Z (N0 Z M P0)
				}
			} else { // [this] N0
				if ( I.right() < 0.0 ) { // [I] N1
					__m128d r1 = _mm_xor_pd(xmmbounds,interval::lbsignmask);
					xmmbounds = _mm_move_sd(_mm_div_pd(_mm_shuffle_pd(r1,r1,1),I.xmmbounds),interval::m128_zero);	// N0 N1
				} else { // [I] P or Z
					if ( I.right() == 0.0 ) { // [I] N0 or Z
							*this = interval::universe(); // N0 Z
					} else { // [I] P or M
						if ( I.left() <= 0.0 ) { // [I] M
							*this = interval::universe(); // N0 P0
            } else { // [I] P1
							xmmbounds = _mm_move_sd(interval::m128_zero,_mm_div_pd(xmmbounds,_mm_xor_pd(I.xmmbounds,interval::lbsignmask))); // N0 P1
						}
					}
				}
			}
		} else { // [this] M, P0, or P1
			if ( left() < 0.0 ) { // [this] M
				if ( I.right() < 0.0 ) { // [I] N1
					__m128d r1 = _mm_xor_pd(xmmbounds,interval::lbrbsignmask);
					xmmbounds = _mm_div_pd(_mm_shuffle_pd(r1,r1,1),_mm_unpackhi_pd(I.xmmbounds,I.xmmbounds));// M N1
				} else { // [I] P or Z
					if ( I.right() == 0.0 ) { // [I] N0 or Z
						*this = interval::universe(); // M N0
					} else { // [I] P or M
						if ( I.left() <= 0.0 ) { // [I] M
							*this = interval::universe(); // M M
            } else { // [I] P1
								__m128d r1 = _mm_xor_pd(I.xmmbounds,interval::lbsignmask);
								xmmbounds = _mm_div_pd(xmmbounds,_mm_unpacklo_pd(r1,r1)); // M P1
            }
					}
				}
			} else { // [this] P0 or P1
				if ( left() == 0.0 ) { // [this] P0
					if ( I.right() < 0.0 ) { // [I] N1
						__m128d r1 = _mm_xor_pd(_mm_shuffle_pd(xmmbounds,xmmbounds,1),interval::lbsignmask);
						__m128d r2 = _mm_shuffle_pd(I.xmmbounds,I.xmmbounds,1);
						xmmbounds = _mm_move_sd(interval::m128_zero,_mm_div_pd(r1,r2));// P0 N1
					} else { // [I] P or Z
						if ( I.right() == 0.0 ) { // [I] N0 or Z
							*this = interval::universe(); // P0 N0
						} else { // [I] P or M
							if ( I.left() <= 0.0 ) { // [I] M
								*this = interval::universe(); // P0 M
              } else { // [I] P1
									__m128d r1 = _mm_xor_pd(I.xmmbounds,interval::lbsignmask);
									__m128d r2 = _mm_shuffle_pd(r1,r1,1);
									xmmbounds = _mm_move_sd(_mm_div_pd(xmmbounds,r2),interval::m128_zero);// P0 P1
							}
						}
					}
				} else { // [this] P1
					if ( I.right() < 0.0 ) { // [I] N1
						__m128d r1 = _mm_xor_pd(_mm_shuffle_pd(xmmbounds,xmmbounds,1),interval::lbrbsignmask);
						__m128d r2 = _mm_xor_pd(I.xmmbounds,interval::lbsignmask);
						xmmbounds = _mm_div_pd(r1,_mm_shuffle_pd(r2,r2,1)); // P1 N1
					} else { // [I] P or Z
						if ( I.right() == 0.0 ) { // [I] N0 or Z
							if ( I.left() == 0.0 ) { // [I] Z
								*this = interval::emptyset(); // P1 Z
							} else {
								xmmbounds = _mm_move_sd(_mm_div_pd(_mm_shuffle_pd(xmmbounds,xmmbounds,1),
																			  _mm_shuffle_pd(I.xmmbounds,I.xmmbounds,1)),
															   interval::m128_infinf); // P1 N0
							}
						} else { // [I] P or M
							if ( I.left() < 0.0 ) { // [I] M
								*this = interval::universe(); // P1 M
							} else { // [I] P0 or P1
								if ( I.left() == 0.0 ) { // [I] P0
									xmmbounds = _mm_move_sd(interval::m128_infinf,
														         _mm_div_pd(xmmbounds,
                                                              _mm_shuffle_pd(I.xmmbounds,I.xmmbounds,1))); // P1 P0
								} else { // [I] P1
									__m128d r1 = _mm_xor_pd(I.xmmbounds,interval::lbsignmask);
									xmmbounds = _mm_div_pd(xmmbounds,_mm_shuffle_pd(r1,r1,1));// P1 P1
								}
							}
						}
					}
				}
			}
		}
	}

	GAOL_RND_LEAVE_SSE();
	return *this;
  }

  std::ostream& operator<<(std::ostream& out, const __m128d& x)
	{
		interval::xmm2d tmp;
		_mm_store_pd(tmp,x);
		out << tmp[1] << " : " << tmp[0];
		return out;
	}
