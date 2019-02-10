/***************************************************************************
 *   Copyright (C) 2008 by Frederic Goualard                               *
 *   Frederic.Goualard@univ-nantes.fr                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _performance_test_set_h_
#define _performance_test_set_h_

#include <vector>

#include "../gaol/gaol_interval.h"
#include "../gaol/gaol_allocator.h"


/*
	compute_table_I --
	Returns a vector of intervals whose bounds are chosen in the set {0,inf,denormal,normal} with
	probabilities given by the second parameter:
		probabilities[0] = probability of creating a denormalized bound
		probabilities[1] = probability of creating a +/-0 bound
		probabilities[2] = probability of creating a +/-oo bound
		probabilities[3] = probability of creating a normalized bound

	All probabilities should add to 1
		
	Note: the vector shall have been defined with a number of elements.
*/
extern void compute_table_I(std::vector<gaol::interval, gaol::aligned_allocator<gaol::interval> >& TI,
						  			 double probabilities[]);

extern gaol::interval create_interval(double probabilities[]);

#endif // _performance_test_set_h_