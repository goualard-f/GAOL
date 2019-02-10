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

/*
	Computes a test set of intervals with specific characteristics.
*/

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>


#include "performance_test_set.h"
#include "../gaol/gaol.h"

using namespace std;
using namespace gaol;

/*
	create_denormal -- Returns a denormal number. 
*/
double create_denormal()
{
	union {
		double d;
		unsigned long long int i;
	} di;
	unsigned long long sign = rand() % 2;
	unsigned long long exponent = 0;
	unsigned long long fract = 0L;

	fract = ((unsigned long long)(rand()) << 21) | ((unsigned long long)(rand()) >> 10);

	di.i = (sign << 63) | (exponent << 52) | fract;
	return di.d;
}

/*
	create_zero -- Returns 0.0 or -0.0
*/	
double create_zero()
{
	return ((rand()%2)-1)*0.0;
}

/*
	create_inf -- Returns +oo or -oo
*/	
double create_inf()
{
	return ((rand() % 2) ? -1 : 1)*std::numeric_limits<double>::infinity();
}

/*
	create_normal -- Returns a normal number different from 0
*/
double create_normal()
{
	union {
		double d;
		unsigned long long int i;
	} di;
	unsigned long long sign = rand() % 2;
	unsigned long long exponent = (rand() % 2046)+1;
	unsigned long long fract = 0L;

	/* Obtaining randomly 52 bits in two calls to rand() (31+21). Bit 21 is taken from the first call
		to rand() and overwrites the one from the second call (which is 0 anyway, as rand() returns
		a value between 0 and 2^31-1)
	*/
		fract = ((unsigned long long)(rand()) << 21) | ((unsigned long long)(rand()) >> 10);

	di.i = (sign << 63) | (exponent << 52) | fract;
	return di.d;
}


double create_bound(double probabilities[])
{
	double b = 0;
	int ok;
	do  {
		int fun = rand() % 4;
		ok = (drand48() <= probabilities[fun]);
		if (ok) {
			switch (fun) {
				case 0:
					b = create_denormal();
					break;
				case 1:
					b = create_zero();
					break;
				case 2:
					b = create_inf();
					break;
				case 3:
					b = create_normal();
			}
		}
	} while(!ok);
	return b;
}

interval create_interval(double probabilities[])
{
	double l = create_bound(probabilities);
	double r = create_bound(probabilities);
	  
	if ((l == r) && std::isinf(l)) { // [ -oo, -oo ] || [ +oo, +oo ]
		l = -std::numeric_limits<double>::infinity();
		r = -l;
	} else {
		if (l > r) {
			double tmp = l;
			l = r;
			r = tmp;
		}
	}
	return interval(l,r);
}

void compute_table_I(vector<interval, aligned_allocator<interval> >& TI, double probabilities[])
{
	for (size_t i = 0; i < TI.size(); ++i) {
		TI[i] = create_interval(probabilities);
	}
}
