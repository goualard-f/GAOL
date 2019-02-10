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
 * CVS: $Id: gaol_parser.h 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By:
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_parser.h
  \brief

  <long description>

  \author Frédéric Goualard
  \date   2001-10-19
*/


#ifndef __gaol_parser_h__
#define __gaol_parser_h__

#include "gaol/gaol_config.h"
#include <iosfwd>

namespace gaol {

  class interval;

  /*!
    \brief Parse a string to create an interval.

    \param s   the string used for the input
    \param out	the interval created.
    \return false if the input could not be parsed into a valid interval
    and true otherwise

    An interval has one of the following formats:

    - \f$"n"\f$ where \f$"n"\f$ is an integer or a rational number
    - \f$"[n]"\f$ where \f$"n"\f$ is an integer or a rational number
    - \f$"[l, f]"\f$ where \f$"l"\f$ and \f$"r"\f$ are integers or rational
    numbers
    - \f$[l,+\textrm{inf}]\f$, \f$[-\textrm{inf},r]\f$,
      \f$[-\textrm{inf},+\textrm{inf}]\f$ where \f$"\textrm{inf}"\f$ stands
      for the \emph{infinity}. The "plus" sign before \f$"\textrm{inf}"\f$ is
      optional
    - \f$[empty]\f$ to denote the empty interval

    \note A rational number must be expressed in the usual format
    for integral or non-integral numbers (e.g. "0.3", ".12e3", "12" and
    so on).
    If this number is not representable with floating-point numbers, it is
    replaced by the smallest floating-point interval containing it. For
    example, giving \f$[0.1]\f$ as input results in the creation of the
    interval
    \f$[0.09999999\dots,0.100\dots01]\f$. Notations \f$"[n]"\f$ and \f$"n"\f$
    are equivalent. Note also that spaces are not significant except in numbers
    and that case is significant for constants such as \em empty and
    \emp inf. "+inf" (resp. "-inf") is not allowed to appear as a left
    (resp. right) bound.

    \note The out interval is not modified whenever an error occur.
  */
  extern __GAOL_PUBLIC__ bool parse_interval(const char* const s, interval& out);

} // namespace gaol

#endif /* __gaol_parser_h__ */
