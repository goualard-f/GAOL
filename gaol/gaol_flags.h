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
 * CVS: $Id: gaol_flags.h 54 2009-03-01 13:53:01Z goualard $
 * Last modified: 
 * By: 
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_flags.h
  \brief  

  <long description>

  \author Frédéric Goualard
  \date   2001-10-30
*/


#ifndef __gaol_flags_h__
#define __gaol_flags_h__

namespace gaol {


  /*!
    \brief Type for priority of operators in expressions
   */
  class prec_t {
  public:
    enum { 
      null_prec   =  0, // null node
      plus_prec   =  2, // +, -, 
      mult_prec   =  5, // *, /
      uminus_prec =  6, // unary minus, pow, 
      cst_prec    = 10  // constant, variable, sine, cosine, ...
    };
  };

} // namespace gaol

#endif /* __gaol_flags_h__ */
