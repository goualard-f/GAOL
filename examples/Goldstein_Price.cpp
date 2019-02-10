/*-*-C++-*------------------------------------------------------------------
 * gaol -- Just Another Interval Library
 *--------------------------------------------------------------------------
 * This file is part of the gaol distribution. Gaol was primarily 
 * developed at the Swiss Federal Institute of Technology, Lausanne, 
 * Switzerland, and is now developed at the Institut de Recherche 
 * en Informatique de Nantes, France.
 *
 * Copyright (c) 2001 Swiss Federal Institute of Technology, Switzerland
 * Copyright (c) 2002 Institut de Recherche en Informatique de Nantes, France
 *--------------------------------------------------------------------------
 * gaol is a software distributed WITHOUT ANY WARRANTY. Read the associated 
 * COPYING file for information.
 *--------------------------------------------------------------------------
 * CVS: $Id: Goldstein_Price.cpp,v 1.8 2006/03/22 08:35:11 goualard Exp $
 * Last modified: Tue Mar 21 17:55:28 2006 on pc-dubreil-123.irin.sciences.univ-nantes.prive
 * By: Frederic Goualard <Frederic.Goualard@lina.univ-nantes.fr>
 *--------------------------------------------------------------------------*/

#include <iostream>
#include "gaol/gaol.h"


using std::cout;
using std::endl;

int main(void)
{
//  gaol::init();

  interval
    x(-2,2),
    y(-2,2), z("[0.1]");


  cout.precision(16);


  reset_time();
  for (unsigned int i=0;i<1000000;++i) {
    z=(interval(1,1)+pow(x+y,2)*(interval(19,19)-
				   interval(14)*x+
				   interval(3)*pow(x,2)-interval(14)*y+
				   interval(6)*x*y+interval(3)*pow(y,2)))*
      (interval(30,30)+pow(interval(2,2)*x-interval(3,3)*y,2)*
       (interval(18,18)-interval(32,32)*x+
	interval(12,12)*pow(x,2)+interval(48,48)*y-interval(36,36)*x*y+
	interval(27,27)*pow(y,2)));

  }
  cout << "z = " << z << endl;
  cout << "Elapsed time: " << elapsed_time() << endl;
// gaol::cleanup();
  return 0;
}
