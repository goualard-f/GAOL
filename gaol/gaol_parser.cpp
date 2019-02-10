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
 * CVS: $Id: gaol_parser.cpp 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By:
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_parser.cpp
  \brief

  <long description>

  \author Frédéric Goualard
  \date   2001-10-19
*/

static const char *CVSid="@(#) $Id: gaol_parser.cpp 191 2012-03-06 17:08:58Z goualard $";

#include <iostream>
#include <cstring>
#include <cctype>

using std::istream;

#include "gaol/gaol_parser.h"
#include "gaol/gaol_interval.h"
#include "gaol/gaol_common.h"
#include "gaol/gaol_fpu.h"
#include "gaol/gaol_expr_eval.h"
#include "gdtoa/gdtoa.h"

extern void gaol_initialize_parsing(const char* const str,
				    gaol::interval* itv);
extern bool gaol_cleanup_parsing(void);
extern int gaol_parse(void);

namespace gaol {

  bool parse_interval(const char* const s, interval& out)
  {
    interval itv;
    gaol_initialize_parsing(s,&itv);
    gaol_parse();
    bool parsing_ok = gaol_cleanup_parsing();
    if (parsing_ok) {
      out = itv;
      // Handling cases "-inf" and "+inf" to avoid them
      // leading to <-inf, -inf> and <inf, inf>
      if (out.right()==-GAOL_INFINITY) {
        if (out.left()==-GAOL_INFINITY) {
          out = interval(-GAOL_INFINITY,-std::numeric_limits<double>::max());
        }
      } else {
        if (out.left() == GAOL_INFINITY) {
          if (out.right() == GAOL_INFINITY) {
            out = interval(std::numeric_limits<double>::max(),GAOL_INFINITY);
          }
        }
      }
    }
    return parsing_ok;
  }




} // namespace gaol
