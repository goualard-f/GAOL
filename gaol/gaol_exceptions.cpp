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
 * CVS: $Id: gaol_exceptions.cpp 54 2009-03-01 13:53:01Z goualard $
 * Last modified: 
 * By: 
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_exceptions.cpp
  \brief  

  <long description>

  \author Frederic Goualard
  \date   2002-12-02
*/

#include "gaol/gaol_config.h"

#if GAOL_EXCEPTIONS_ENABLED

static const char *CVSid="@(#) $Id: gaol_exceptions.cpp 54 2009-03-01 13:53:01Z goualard $";

#include <iostream>

#include "gaol/gaol_exceptions.h"

namespace gaol {

  // Class gaol_exception
  gaol_exception::gaol_exception(const char* f, unsigned l)
  {
    which_file_=f;
    which_line_=l;
  }

  gaol_exception::gaol_exception(const char* f, unsigned l, const string& e)
  {
    which_file_=f;
    which_line_=l;
    explanation_ = e;
  }

  const char*
  gaol_exception::file() const
  {
    return which_file_;
  }

  unsigned int
  gaol_exception::line() const
  {
    return which_line_;
  }


  string
  gaol_exception::explanation() const
  {
    return explanation_;
  }

  std::ostream& operator<<(std::ostream& out, const gaol_exception &e)
  {
    out << e.file() << ", line " << e.line() << ": exception " <<
      e.what() << " thrown";
    if (e.explanation().length() != 0) { // Some explanation given?
      out << ": " << e.explanation();
    }
    return out;
  }

} // namespace gaol

#endif /* GAOL_EXCEPTIONS_ENABLED */
