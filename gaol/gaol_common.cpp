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
 * CVS: $Id: gaol_common.cpp 284 2015-06-03 15:00:39Z goualard $
 * Last modified:
 * By:
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_common.cpp
  \brief  Implementation for functions of common use

  \author Frédéric Goualard
  \date   2001-10-03
*/


#include <iostream>
#include <cstdio>
#include <cmath>
#include "gaol/gaol_config.h"
#include "gaol/gaol_fpu.h"
#include "gaol/gaol_common.h"
#include "gaol/gaol_expression.h"

#if GAOL_USING_APMATHLIB
#include "gaol/gaol_double_op_apmathlib.h"
//    extern  unsigned short Init_Lib();
//	extern  void Exit_Lib(unsigned short);
#elif GAOL_USING_CRLIBM
#include "gaol/gaol_double_op_crlibm.h"
//	extern unsigned long long crlibm_init(void);
//	extern  void crlibm_exit(unsigned long long);
#endif

namespace gaol {

  extern void gaol_init_lib(void);

  // Saves the state of the FPU at init time to be restored at the end.
  static short save_fpu_state;
  static unsigned long long save_fpu_state_crlibm;

  static bool _already_cleaned = false;
  static bool _already_initialized = false;

  int debug_level;

	bool init(int dbg_lvl)
	{
		/*
       		Call Initialization of MathLib.
       		NOTE: it is crucial to call Init_Lib() at the very beginning of this
       		function to avoid having MathLib messing with the FPU state set by
       		gaol.
    	*/
		if (!_already_initialized) {
			debug_level = dbg_lvl;

#if GAOL_USING_APMATHLIB
           save_fpu_state = Init_Lib();
#elif GAOL_USING_CRLIBM
            save_fpu_state_crlibm = ::crlibm_init();
#endif
#if !GAOL_PRESERVE_ROUNDING
            reset_fpu_cw(GAOL_FPU_MASK); // 53 bits precision, all exceptions masked, rounding to +oo
#   if USING_SSE2_INSTRUCTIONS
            round_upward_sse();
#   endif
#endif
	  		the_null_expr = new null_node;
	  		the_null_expr->inc_refcount();
	  		interval::precision(16);
	  		_already_initialized=true;
	  		return true;
      	} else {
	  		debug_level = dbg_lvl;
	  		return false;
      	}
	}

  bool cleanup(void)
  {
    if (!_already_cleaned) {
#if GAOL_USING_APMATHLIB
		Exit_Lib(save_fpu_state);
#elif GAOL_USING_CRLIBM
		::crlibm_exit(save_fpu_state_crlibm);
#endif
		delete the_null_expr;
		_already_cleaned=true;
		return true;
    } else {
      return false;
    }
  }

  void gaol_warning(const char *file, int line, const char *warn)
  {
    std::cerr << "[gaol warning in " << file << ':' << line << "]: " << warn << std::endl;
  }

  void gaol_warning(const char *warn)
  {
    std::cerr << "[gaol warning]: " << warn << std::endl;
  }

  void gaol_error(const char *file, int line, const char *err)
  {
    std::cerr << "[gaol error in " << file << ':' << line << "]: " << err << std::endl;
  }

  void gaol_error(const char *err)
  {
    std::cerr << "[gaol error]: " << err << std::endl;
  }

  double maximum(double a, double b)
  {
    if (std::isnan(a) || std::isnan(b)) {
      return GAOL_NAN;
    }
    if (is_signed(a)) { // To handle +0 and -0 such that +0 is returned
      return ((b >= a) ? b : a);
    } else {
    return ((a >= b) ? a : b);
    }
  }

  double minimum(double a, double b)
  {
    if (std::isnan(a) || std::isnan(b)) {
      return GAOL_NAN;
  }
    if (is_signed(a)) { // To handle +0 and -0 such that -0 is returned
      return ((a <= b) ? a : b);
    } else {
      return ((b <= a) ? b : a);
    }
  }










} // namespace gaol



#if defined (__GNUC__)
  /*
    Function declared __attribute__ ((constructor)) to be called
    automatically before main() starts. It creates also in effect a
    destructor by defining a static instance of an init_cleanup object.
  */
  void gaol_init(void)
  {
    /* The constructor and destructor for this object will do the initialization
       and cleanup for te whole cacao library. Code for these stages should be
       put in them (see gaol_init_cleanup.cpp)
    */
    gaol::gaol_init_lib();
  }

#elif defined (_MSC_VER)

#endif

/**
 * \mainpage gaol (Just Another Interval Library)
 * \section authors Authors
 * Frédéric Goualard    \<Frederic.Goualard@irin.univ-nantes.fr\> <P>
 * \section copyright Copyright Notice
 * <tt>
 <hr>
 Read the COPYING file for the LEGAL NOTICE.


 <hr>
 </tt>
 */
