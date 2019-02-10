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
 * CVS: $Id: gaol_exact.h 54 2009-03-01 13:53:01Z goualard $
 * Last modified: 
 * By: 
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_exact.h
  \brief  Functions needed by dtoa.c to report inexact translation from
  ASCII to double by strtod().

  \note This is C code, not C++. Moreover, it is not meant to be included in
  any namespace.

  <long description>

  \author Frederic Goualard
  \date   2002-12-05
*/


#ifndef __gaol_exact_h__
#define __gaol_exact_h__

// To overcome problems with old versions of autoconf
#undef PACKAGE
#include "gaol/gaol_config.h"

#ifdef __cplusplus
extern "C" {
#endif
    
int get_inexact(void);
void clear_inexact(void);

#ifdef __cplusplus
}
#endif
   
    
#endif /* __gaol_exact_h__ */
