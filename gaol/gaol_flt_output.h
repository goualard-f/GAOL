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
 * CVS: $Id: gaol_flt_output.h 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By:
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_flt_output.h
  \brief  double to ASCII conversion


  Uses the dtoa() function written by Gay (see dtoa.c) to convert doubles
  into ASCII strings while respecting the current rounding direction.

  \author Frederic Goualard
  \date   2002-04-16
*/


#ifndef __gaol_flt_output_h__
#define __gaol_flt_output_h__

#include <iosfwd>
#include "gaol/gaol_config.h"

namespace gaol {

/*!
  \brief Prints the double d to the stream os with downward rounding for
  the conversion digits->ASCII
*/
  extern __GAOL_PUBLIC__ char* dtoa_downward(double d, std::ostream& os);
/*!
  \brief Prints the double d to the stream os with upward rounding for
  the conversion digits->ASCII
*/
  extern __GAOL_PUBLIC__ char* dtoa_upward(double d, std::ostream& os);


} // namespace gaol

#endif /* __gaol_flt_output_h__ */
