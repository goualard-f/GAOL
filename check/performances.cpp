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

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>
using std::string;

#include "../gaol/gaol.h"
#include "../gaol/gaol_allocator.h"
#include "performance_test_set.h"


using namespace std;
using namespace gaol;


// Maximum bound on exponent used for pow
#define EXP_BOUND 10


const size_t MAX = 1000000;
const size_t LOOPS = 10;

void compute_table_int(std::vector<int>& Texp)
{
	for (size_t i = 0; i < Texp.size(); ++i) {
		Texp[i] = rand()%EXP_BOUND;
	}
}

int main(int argc, char *argv[])
{
	gaol::init();

	if (argc != 7) {
		std::cerr << "performances <oper> seed p(denorm) p(zero) p(inf) p(norm)" << std::endl;
		std::cerr << "\twith oper in {add,sub,mul,div,inv,pow,cos,sin,tan,acos,asin,\n\t\tatan,cosh,sinh,tanh,acosh,asinh,atanh,exp,log}" << std::endl;
		return 1;
	}

	istringstream seedstr(argv[2]);
	long int seed;
	seedstr >> seed;
	istringstream pdenormstr(argv[3]);
	istringstream pzerostr(argv[4]);
	istringstream pinfstr(argv[5]);
	istringstream pnormstr(argv[6]);
	double pdenorm, pzero, pinf,pnorm;
	pdenormstr >> pdenorm;
	pzerostr >> pzero;
	pinfstr >> pinf;
	pnormstr >> pnorm;

	string fun(argv[1]);

	srand(seed);
	srand48(seed);

	double prob[4] = { pdenorm, // denormal
							 pzero, // zero
							 pinf, // inf
							 pnorm  // normal
	};


	if (fun == string("add")) {

		vector<interval, aligned_allocator<interval> > TI1(MAX);
		vector<interval, aligned_allocator<interval> > TI2(MAX);

		compute_table_I(TI1,prob);
		compute_table_I(TI2,prob);

		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
		  for (size_t i = 0; i < MAX; ++i) {
		    Res += TI1[i]+TI2[i];
		  }
		}
		std::cerr << Res << std::endl;
		std::cout << elapsed_time() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}

	if (fun == string("sub")) {

		vector<interval, aligned_allocator<interval> > TI1(MAX);
		vector<interval, aligned_allocator<interval> > TI2(MAX);

		compute_table_I(TI1,prob);
		compute_table_I(TI2,prob);

		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
		  for (size_t i = 0; i < MAX; ++i) {
		    Res += TI1[i]-TI2[i];
		  }
		}
		std::cerr << Res << std::endl;
		std::cout << elapsed_time() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}

	if (fun == string("mul")) {

		vector<interval, aligned_allocator<interval> > TI1(MAX);
		vector<interval, aligned_allocator<interval> > TI2(MAX);

		compute_table_I(TI1,prob);
		compute_table_I(TI2,prob);

		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
		  for (size_t i = 0; i < MAX; ++i) {
		    Res += TI1[i]*TI2[i];
		  }
		}
		std::cerr << Res << std::endl;
		std::cout << elapsed_time() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}

	if (fun == string("div")) {

		vector<interval, aligned_allocator<interval> > TI1(MAX);
		vector<interval, aligned_allocator<interval> > TI2(MAX);

		compute_table_I(TI1,prob);
		compute_table_I(TI2,prob);

		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
		  for (size_t i = 0; i < MAX; ++i) {
			Res += TI1[i]/TI2[i];
		  }
		}
		std::cerr << Res.left() << " " << Res.right() << std::endl;
		std::cout << elapsed_time() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}

	if (fun == string("inv")) {
		std::vector<interval, aligned_allocator<interval> > TI1(MAX);
		compute_table_I(TI1,prob);
		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
			for (size_t i = 0; i < MAX; ++i) {
				Res += inverse(TI1[i]);
			}
		}
		std::cout << elapsed_time() << std::endl;
		std::cerr << Res.left() << " " << Res.right() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}

	if (fun == string("pow")) {
		std::vector<interval, aligned_allocator<interval> > TI1(MAX);
		std::vector<int> Texp(MAX);
		compute_table_I(TI1,prob);
		compute_table_int(Texp);
		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
			for (size_t i = 0; i < MAX; ++i) {
				Res += gaol::pow(TI1[i],Texp[i]);
			}
		}
		std::cout << elapsed_time() << std::endl;
		std::cerr << Res.left() << " " << Res.right() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}

	if (fun == string("cos")) {
		std::vector<interval, aligned_allocator<interval> > TI1(MAX);
		compute_table_I(TI1,prob);
		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
			for (size_t i = 0; i < MAX; ++i) {
				Res += gaol::cos(TI1[i]);
			}
		}
		std::cout << elapsed_time() << std::endl;
		std::cerr << Res.left() << " " << Res.right() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}

	if (fun == string("sin")) {
		std::vector<interval, aligned_allocator<interval> > TI1(MAX);
		compute_table_I(TI1,prob);
		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
			for (size_t i = 0; i < MAX; ++i) {
				Res += gaol::sin(TI1[i]);
			}
		}
		std::cout << elapsed_time() << std::endl;
		std::cerr << Res.left() << " " << Res.right() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}

	if (fun == string("tan")) {
		std::vector<interval, aligned_allocator<interval> > TI1(MAX);
		compute_table_I(TI1,prob);
		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
			for (size_t i = 0; i < MAX; ++i) {
				Res += gaol::tan(TI1[i]);
			}
		}
		std::cout << elapsed_time() << std::endl;
		std::cerr << Res.left() << " " << Res.right() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}

	if (fun == string("acos")) {
		std::vector<interval, aligned_allocator<interval> > TI1(MAX);
		compute_table_I(TI1,prob);
		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
			for (size_t i = 0; i < MAX; ++i) {
				Res += gaol::acos(TI1[i]);
			}
		}
		std::cout << elapsed_time() << std::endl;
		std::cerr << Res.left() << " " << Res.right() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}

	if (fun == string("asin")) {
		std::vector<interval, aligned_allocator<interval> > TI1(MAX);
		compute_table_I(TI1,prob);
		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
			for (size_t i = 0; i < MAX; ++i) {
				Res += gaol::asin(TI1[i]);
			}
		}
		std::cout << elapsed_time() << std::endl;
		std::cerr << Res.left() << " " << Res.right() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}

	if (fun == string("atan")) {
		std::vector<interval, aligned_allocator<interval> > TI1(MAX);
		compute_table_I(TI1,prob);
		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
			for (size_t i = 0; i < MAX; ++i) {
				Res += gaol::atan(TI1[i]);
			}
		}
		std::cout << elapsed_time() << std::endl;
		std::cerr << Res.left() << " " << Res.right() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}

	if (fun == string("cosh")) {
		std::vector<interval, aligned_allocator<interval> > TI1(MAX);
		compute_table_I(TI1,prob);
		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
			for (size_t i = 0; i < MAX; ++i) {
				Res += gaol::cosh(TI1[i]);
			}
		}
		std::cout << elapsed_time() << std::endl;
		std::cerr << Res.left() << " " << Res.right() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}

	if (fun == string("sinh")) {
		std::vector<interval, aligned_allocator<interval> > TI1(MAX);
		compute_table_I(TI1,prob);
		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
			for (size_t i = 0; i < MAX; ++i) {
				Res += gaol::sinh(TI1[i]);
			}
		}
		std::cout << elapsed_time() << std::endl;
		std::cerr << Res.left() << " " << Res.right() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}

	if (fun == string("tanh")) {
		std::vector<interval, aligned_allocator<interval> > TI1(MAX);
		compute_table_I(TI1,prob);
		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
			for (size_t i = 0; i < MAX; ++i) {
				Res += gaol::tanh(TI1[i]);
			}
		}
		std::cout << elapsed_time() << std::endl;
		std::cerr << Res.left() << " " << Res.right() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}

	if (fun == string("acosh")) {
		std::vector<interval, aligned_allocator<interval> > TI1(MAX);
		compute_table_I(TI1,prob);
		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
			for (size_t i = 0; i < MAX; ++i) {
				Res += gaol::acosh(TI1[i]);
			}
		}
		std::cout << elapsed_time() << std::endl;
		std::cerr << Res.left() << " " << Res.right() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}

	if (fun == string("asinh")) {
		std::vector<interval, aligned_allocator<interval> > TI1(MAX);
		compute_table_I(TI1,prob);
		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
			for (size_t i = 0; i < MAX; ++i) {
				Res += gaol::asinh(TI1[i]);
			}
		}
		std::cout << elapsed_time() << std::endl;
		std::cerr << Res.left() << " " << Res.right() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}

	if (fun == string("atanh")) {
		std::vector<interval, aligned_allocator<interval> > TI1(MAX);
		compute_table_I(TI1,prob);
		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
			for (size_t i = 0; i < MAX; ++i) {
				Res += gaol::atanh(TI1[i]);
			}
		}
		std::cout << elapsed_time() << std::endl;
		std::cerr << Res.left() << " " << Res.right() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}

	if (fun == string("exp")) {
		std::vector<interval, aligned_allocator<interval> > TI1(MAX);
		compute_table_I(TI1,prob);
		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
			for (size_t i = 0; i < MAX; ++i) {
				Res += gaol::exp(TI1[i]);
			}
		}
		std::cout << elapsed_time() << std::endl;
		std::cerr << Res.left() << " " << Res.right() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}

	if (fun == string("log")) {
		std::vector<interval, aligned_allocator<interval> > TI1(MAX);
		compute_table_I(TI1,prob);
		reset_time();
		interval Res;
		for (size_t l = 0; l < LOOPS; ++l) {
			for (size_t i = 0; i < MAX; ++i) {
				Res += gaol::log(TI1[i]);
			}
		}
		std::cout << elapsed_time() << std::endl;
		std::cerr << Res.left() << " " << Res.right() << std::endl;
		cleanup();
		return EXIT_SUCCESS;
	}



	gaol::cleanup();
	return EXIT_SUCCESS;
}
