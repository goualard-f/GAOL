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
 * CVS: $Id: gaol_config.h 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By:
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_config.h
  \brief  Configuration file

  This file contains the declarations of all the macros used to control the
  way gaol is compiled, depending on the host. It acts as a front-end for
  the configuration file created by configure and the one used when
  compiling gaol with Borland C++ Builder.


  \author Frederic Goualard
  \date   2002-12-03
*/

#ifndef __gaol_config_h__
#define __gaol_config_h__

#if defined (_MSC_VER)

# define GAOL_ERRNO errno
# ifndef __GAOL_PUBLIC__
#   ifdef _COMPILING__GAOL_PUBLIC__
#     define __GAOL_PUBLIC__ __declspec(dllexport)
#   else
#     define __GAOL_PUBLIC__ __declspec(dllimport)
#   endif
# endif
# define INLINE inline
# include "gaol/gaol_config_msvc.h"

#elif defined (__MINGW32__)

# define GAOL_ERRNO errno
# undef PACKAGE
# undef VERSION
# include "gaol/gaol_config_mingw.h"
# ifndef __GAOL_PUBLIC__
#  if defined (HAVE_VISIBILITY_OPTIONS)
#     define __GAOL_PUBLIC__ __attribute__ ((visibility("default")))
#  else
#     define __GAOL_PUBLIC__
#  endif
# endif
# define INLINE inline

#elif defined (__GNUC__)
# define GAOL_ERRNO errno
# undef PACKAGE
# undef VERSION
# include "gaol/gaol_configuration.h"
# ifndef __GAOL_PUBLIC__
#  if defined (HAVE_VISIBILITY_OPTIONS)
#     define __GAOL_PUBLIC__ __attribute__ ((visibility("default")))
#  else
#     define __GAOL_PUBLIC__
#  endif
# endif
# define INLINE inline

#else

# define GAOL_ERRNO errno
# undef PACKAGE
# undef VERSION
# ifndef __GAOL_PUBLIC__
#  define __GAOL_PUBLIC__
# endif
# define INLINE inline
# include "gaol/gaol_configuration.h"
#endif

#endif /* __gaol_config_h__ */
