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
 * CVS: $Id: gaol_profile.cpp 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By:
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_profile.cpp
  \brief

  <long description>

  \author Frédéric Goualard
  \date   2001-10-02
*/

static const char *CVSid="@(#) $Id: gaol_profile.cpp 191 2012-03-06 17:08:58Z goualard $";

// To overcome problems with old versions of autoconf
#undef PACKAGE
#include "gaol/gaol_profile.h"

#if (HAVE_GETRUSAGE && (!GETRUSAGE_IN_HEADER))
  extern "C" int getrusage(int, struct rusage*);
#endif

namespace gaol {

  static long last_usr_time;
  static long last_reset_time;
} // namespace gaol

#if HAVE_GETRUSAGE
//================

#  include <cstdlib>
#  include <sys/time.h>
#  include <sys/resource.h>


#  include <unistd.h>

namespace gaol {

  long get_time(void)
  {
    struct rusage RsrUsage;
    getrusage(RUSAGE_SELF,&RsrUsage);
    return (RsrUsage.ru_utime.tv_sec*1000 + RsrUsage.ru_utime.tv_usec/1000);
  }
} // namespace gaol

#elif HAVE_CLOCK
//==============
#  include <time.h>

namespace gaol {

  long get_time(void)
  {
    return long((1000.*clock())/CLOCKS_PER_SEC);
  }
} // namespace gaol

#else
#  error "No timing function available"
#endif /* HAVE_GETRUSAGE */

namespace gaol {

  void reset_time(void)
  {
    last_reset_time=last_usr_time=get_time();
  }

  long intermediate_elapsed_time(void)
  {
    long usrtime,res;

    usrtime=get_time();
    res=usrtime-last_usr_time;
    last_usr_time=usrtime;
    return res;
  }

  long elapsed_time(void)
  {
    return get_time()-last_reset_time;
  }


  /*
    timepiece --

   */

  timepiece::timepiece()
  {
    the_last_time = 0;
    total_time = 0;
  }

  void timepiece::start(void)
  {
    the_last_time = get_time();
  }

  void timepiece::stop(void)
  {
    long tmp = get_time();
    total_time += tmp - the_last_time;
  }

  void timepiece::reset(void)
  {
    total_time = 0;
  }

  long timepiece::get_total_time(void) const
  {
    return total_time;
  }

  long timepiece::get_intermediate_time(void) const
  {
    return get_time()-the_last_time;
  }

} // namespace gaol
