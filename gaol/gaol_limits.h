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
 * CVS: $Id: gaol_limits.h 54 2009-03-01 13:53:01Z goualard $
 * Last modified: 
 * By: 
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_limits.h
  \brief  

  <long description>

  \author Frédéric Goualard
  \date   2001-10-01
*/


#ifndef __gaol_limits_h__
#define __gaol_limits_h__

#include "gaol/gaol_config.h"

#if HAVE_LIMITS
#  include <limits>
#else
namespace std {
  template<class T>
  class numeric_limits {
  public:
    static const bool is_specialized = false;
  };
  
  class numeric_limits<double> {
  public:
    static const bool is_specialized = true;
    INLINE static double min() {
      return 4.94065645841246544e-324;
    }
    INLINE static double max() {
      return 1.797693134862315708e+308;
    }
    INLINE static double epsilon() {
      return 2.2204460492503131e-16;
    }
    INLINE static double infinity() {
      return 1.0/0.0;
    }
  };
  
#if SIZEOF_INT==4
  class numeric_limits<int> {
  public:
    static const bool is_specialized = true;
    INLINE static int min() {
      return (-2147483647-1);
    }
    INLINE static int max() {
      return 2147483647; 
    }
    INLINE static int epsilon() {
      return 0;
    }
  };
#elif SIZEOF_INT==8
  class numeric_limits<int> {
  public:
    static const bool is_specialized = true;
    INLINE static int min() {
      return (-9223372036854775807LL-1);
    }
    INLINE static int max() {
      return 9223372036854775807LL; 
    }
    INLINE static int epsilon() {
      return 0;
    }
  };
#endif

} // namespace std
#endif /* HAVE_LIMITS */

#endif /* __gaol_limits_h__ */
