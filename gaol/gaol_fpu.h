/*-*-C++-*------------------------------------------------------------------
 * gaol -- Just Another Interval Library
 *--------------------------------------------------------------------------
 * This file is part of the gaol distribution. Gaol was primarily
 * developed at the Swiss Federal Institute of Technology, Lausanne,
 * Switzerland, and is now developed at the Institut de Recherche
 * en Informatique de Nantes, France.
 *
 * Copyright (c) 2001 Swiss Federal Institute of Technology, Switzerland
 * Copyright (c) 2002 Institut de Recherche en Informatique de Nantes, France
 *--------------------------------------------------------------------------
 * gaol is a software distributed WITHOUT ANY WARRANTY. Read the associated
 * COPYING file for information.
 *--------------------------------------------------------------------------
 * CVS: $Id: gaol_fpu.h 191 2012-03-06 17:08:58Z goualard $
 * Last modified: Tue Sep 12 10:26:06 2006 on pc-goualard.lina.sciences.univ-nantes.prive
 * By: Frederic Goualard <Frederic.Goualard@lina.univ-nantes.fr>
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_fpu.h
  \brief

  <long description>

  \author Frédéric Goualard
  \date   2001-10-01
*/


#ifndef __gaol_fpu_h__
#define __gaol_fpu_h__

#include <cmath>
#include "gaol/gaol_config.h"


#if GAOL_PRESERVE_ROUNDING
/*
  Beware: GAOL_RND_ENTER() should always be called at the very
  beginning of a function to avoid
  problems with it being expanded to multiple statements.
  The same hold for GAOL_RND_ENTER_SSE()
*/
#  define GAOL_RND_ENTER()      unsigned short int _save_state=get_fpu_cw(); round_upward()
#  define GAOL_RND_LEAVE()      reset_fpu_cw(_save_state)
#  define GAOL_RND_PRESERVE()   unsigned short int _save_state=get_fpu_cw()
#  define GAOL_RND_RESTORE()    reset_fpu_cw(_save_state)
#  if USING_SSE2_INSTRUCTIONS
#     define GAOL_RND_ENTER_SSE() 	unsigned int _save_state_sse = _mm_getcsr(); round_upward_sse()
#     define GAOL_RND_LEAVE_SSE()	_mm_setcsr(_save_state_sse)
#  endif
#else // !GAOL_PRESERVE_ROUNDING
#  define GAOL_RND_ENTER()
#  define GAOL_RND_LEAVE()
#  if USING_SSE2_INSTRUCTIONS
#     define GAOL_RND_ENTER_SSE()
#     define GAOL_RND_LEAVE_SSE()
#  endif
#  define GAOL_RND_PRESERVE()
#  define GAOL_RND_RESTORE() round_upward()
#endif // GAOL_PRESERVE_ROUNDING


#if HAVE_FENV_H
#  include "gaol/gaol_fpu_fenv.h"
#elif defined (_MSC_VER)
#  include "gaol/gaol_fpu_msvc.h"
#else
#  error "Don't know how to define FPU manipulation functions"
#endif // HAVE_FENV_H

#endif /* __gaol_fpu_h__ */
