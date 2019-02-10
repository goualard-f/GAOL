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
 * CVS: $Id: gaol_version.h 54 2009-03-01 13:53:01Z goualard $
 * Last modified: 
 * By: 
 *--------------------------------------------------------------------------*/
/*!
  \file   gaol_version.h
  \brief  

  <long description>

  \author Goualard Frédéric
  \date   2001-09-28
*/


#ifndef __gaol_version_h__
#define __gaol_version_h__

#if defined (_MSC_VER)
# include "gaol/gaol_version_msvc.h"

#elif defined (__MINGW32__)
// Nothing for now. FIXME!

#elif defined (__GNUC__)
# include "gaol/gaol_configuration.h"
#endif

namespace gaol {
  const unsigned int version_major = GAOL_MAJOR_VERSION;
  const unsigned int version_minor = GAOL_MINOR_VERSION;
  const unsigned int version_micro = GAOL_MICRO_VERSION;
  const char *const version = GAOL_VERSION;
}
#endif /* __gaol_version_h__ */
