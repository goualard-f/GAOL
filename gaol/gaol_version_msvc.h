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
 * CVS: $Id: gaol_version_msvc.h 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By:
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_version_msvc.h
  \brief  Version information when using Microsoft Visual C++

  <long description>

  \author Frederic Goualard
  \date   2006-03-22
*/


#ifndef __gaol_version_msvc_h__
#define __gaol_version_msvc_h__

#define GAOL_MAJOR_VERSION 4
#define GAOL_MINOR_VERSION 1
#define GAOL_MICRO_VERSION 0
// Defining GAOL_VERSION requires some stringification tricks
#define GAOL_VERSION GAOL_VERSION1(GAOL_MAJOR,GAOL_MINOR,GAOL_MICRO)
#define GAOL_VERSION1(A,B,C) GAOL_VERSION2(A,B,C)
#define GAOL_VERSION2(A,B,C) #A "." #B "." #C

#endif /* __gaol_version_msvc_h__ */
