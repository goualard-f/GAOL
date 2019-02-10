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
 * CVS: $Id: gaol_init_cleanup.cpp 54 2009-03-01 13:53:01Z goualard $
 * Last modified: 
 * By: 
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_init_cleanup.cpp
  \brief  

  \author Frédéric Goualard
  \date   2005-05-10
*/

#include <iostream>
#include "gaol/gaol_common.h"
#include "gaol/gaol_init_cleanup.h"
using std::cerr;
using std::cout;
using std::endl;


static const char *CVSid="@(#) $Id: gaol_init_cleanup.cpp 54 2009-03-01 13:53:01Z goualard $";

namespace gaol {

  /**
	@class init_cleanup
	
	A static object of that class is created to automate initialization and cleanup of the library.
	*/
  class init_cleanup {
  public:
    init_cleanup();
    ~init_cleanup();
  };

  init_cleanup::init_cleanup()
  {
	initialization_process();
  }
  
  init_cleanup::~init_cleanup()
  {
	cleanup_process();
  }
  
  
  void gaol_init_lib(void)
  {
	static init_cleanup _ic_object;
  }
  
  void initialization_process(void)
  {
#if !GAOL_PRESERVE_ROUNDING
      GAOL_IF_VERBOSE(cerr << "Automatic initialization of gaol (rounding set to +oo)... ");	
#else
      GAOL_IF_VERBOSE(cerr << "Automatic initialization of gaol... ");	
#endif
      init();
      GAOL_IF_VERBOSE(cerr << "done" << endl);
  }
  
  void cleanup_process(void)
  {
      GAOL_IF_VERBOSE(cerr << "Automatic cleanup of gaol... ");
      cleanup();
      GAOL_IF_VERBOSE(cerr << "done" << endl);
  }
  
} // namespace gaol
