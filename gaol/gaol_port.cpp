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
 * CVS: $Id: gaol_port.cpp 191 2012-03-06 17:08:58Z goualard $
 * Last modified: 
 * By: 
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_port.cpp
  \brief  

  <long description>

  \author Frédéric Goualard
  \date   2001-10-03
*/

static const char *CVSid="@(#) $Id: gaol_port.cpp 191 2012-03-06 17:08:58Z goualard $";

#include <iostream>
#include "gaol/gaol_port.h"

namespace gaol {

  int gaol_signbit(double d)
  {
    // The explicit test '!=0' ensures that either 0 or 1 is returned
    // and not 0 or 0x80000000.
	//std::cout <<  d << std::endl;
	uintdouble v;
	v.d = d;
	return (HI_UINTDOUBLE(v) & 0x80000000)!=0;
    //return ((unsigned int)(__HI(d)) & 0x80000000)!=0;
  }

} // namespace gaol
