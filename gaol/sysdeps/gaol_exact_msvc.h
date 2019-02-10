/*-*-C++-*------------------------------------------------------------------
 * gaol -- Just Another Interval Library
 *--------------------------------------------------------------------------
 * This file is part of the gaol distribution. It has been developed at
 * the Institut de Recherche en Informatique de Nantes, France
 * 
 * Copyright (c) 2002-2006 Laboratoire d'Informatique de Nantes-Atlantique
 *--------------------------------------------------------------------------
 * gaol is a software distributed WITHOUT ANY WARRANTY. Read the associated 
 * COPYING file for information.
 *--------------------------------------------------------------------------
 * CVS: $Id: gaol_exact_msvc.h 55 2009-03-01 13:55:03Z goualard $
 * Last modified: Thu Dec  5 14:56:11 2002 on victory.irin.sciences.univ-nantes.fr
 * By: Frederic Goualard <Frederic.Goualard@irin.univ-nantes.fr>
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_exact_msvc.h
  \brief  functions get_inexact() and clear_inexact() for Microsoft Visual C++

  \author Frederic Goualard
  \date   2006-03-17
*/

#ifndef __gaol_exact_msvc_h__
#define __gaol_exact_msvc_h__

#if GAOL_USING_ASM
INLINE int get_inexact(void)
{
  unsigned int res_exact;
  __asm {
    fstsw word ptr res_exact
  }
  return res_exact & 0x20;
}


INLINE void clear_inexact(void)
{
  __asm {
    fnclex
  }
}
#else // !GAOL_USING_ASM
# error "no C code for 'inexact' in Microsoft Visual C++"
#endif // GAOL_USING_ASM

#endif /* __gaol_exact_msvc_h__ */
