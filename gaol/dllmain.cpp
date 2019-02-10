/*--------------------------------------------------------------------------
 * gaol -- Just Another Interval Library
 *--------------------------------------------------------------------------
 * This file is part of the gaol distribution. Gaol was primarily
 * developed at the Swiss Federal Institute of Technology, Lausanne,
 * Switzerland, and is now developed at the Institut de Recherche
 * en Informatique de Nantes, France.
 *
 * Copyright (c) 2001 Swiss Federal Institute of Technology, Switzerland
 * Copyright (c) 2002-2006 Laboratoire d'Informatique de Nantes-Atlantique, France
 *--------------------------------------------------------------------------
 * gaol is a software distributed WITHOUT ANY WARRANTY. Read the associated
 * COPYING file for information.
 *--------------------------------------------------------------------------
 * CVS: $Id: dllmain.cpp 191 2012-03-06 17:08:58Z goualard $
 * Last modified: Tue Sep 12 10:28:50 2006 on pc-goualard.lina.sciences.univ-nantes.prive
 * By: Frederic Goualard <Frederic.Goualard@lina.univ-nantes.fr>
 *--------------------------------------------------------------------------*/

/*!
  \file   dllmain.cpp
  \brief Main function for a dll created with Visual C++

  Automatic handling of initialization and cleanup of the library.
  With gcc, this task is performed through the definition of a static
  object of the class init_cleanup.
  With Visual C++ we use a switch() in DllMain().

  \author Frédéric Goualard
  \date   2001-10-03
*/

#if defined (_MSC_VER)

// Defining NO_MIN_MAX to avoid corresponding macros to be defined
#  define NO_MINMAX

#  include <windows.h>
//#  include "gaol/gaol_common.h"
#  include "gaol/gaol_init_cleanup.h"
#  include <iostream>


BOOL WINAPI DllMain(HINSTANCE hModule,
		      DWORD  ul_reason_for_call,
	      LPVOID lpReserved)
{
  switch( ul_reason_for_call ) {
  case DLL_PROCESS_ATTACH:
    gaol::initialization_process();
    break;
  case DLL_PROCESS_DETACH:
    gaol::cleanup_process();
    break;
  }
  return TRUE;
}
#endif // _MSC_VER
