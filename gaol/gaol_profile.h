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
 * CVS: $Id: gaol_profile.h 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By:
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_profile.h
  \brief

  <long description>

  \author Frédéric Goualard
  \date   2001-10-02
*/


#ifndef __gaol_profile_h__
#define __gaol_profile_h__

#include "gaol/gaol_config.h"

namespace gaol {

  /*!
    \brief Object to record time for profiling purpose
   */
  class __GAOL_PUBLIC__ timepiece {
  public:
    timepiece();
    //! Starts the chronometer
    void start(void);
    //! Stops the chronometer temporarily
    void stop(void);
    //! Stops the chronometer and reset the counter
    void reset(void);
    //! Returns the time elapsed since the first call to start()
    long get_total_time(void) const;
    //! Returns the time elapsed since the last call to start()
    long get_intermediate_time(void) const;
  private:
    long the_last_time;
    long total_time;
  };

extern __GAOL_PUBLIC__ long get_time(void);
  /*!
    Sets the base for time tracking.
   */
extern __GAOL_PUBLIC__ void reset_time(void);
  /*!
    Returns the elapsed time since the last call to reset_time()
   */
extern __GAOL_PUBLIC__ long elapsed_time(void);
  /*!
    Returns the elapsed time since the last call to reset_time() or
    to intermediate_elapsed_time.
  */
extern __GAOL_PUBLIC__ long intermediate_elapsed_time(void);

} // namespace gaol

#endif /* __gaol_profile_h__ */
