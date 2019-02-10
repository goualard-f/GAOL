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
 * CVS: $Id: input_output.cpp 264 2015-05-22 14:01:41Z goualard $
 * Last modified:
 * By:
 *--------------------------------------------------------------------------*/

#include <sstream>
#include <string>
#include "gaol/gaol_config.h"
#include "gaol/gaol_exceptions.h"
#include "gaol/gaol_limits.h"
#include "tests.h"


class input_output_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(input_output_test);
  CPPUNIT_TEST(test_input);
  CPPUNIT_TEST(test_output);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp() {
  }
  void tearDown() {
  }

  // --> Beginning of tests
	void test_input() {
		// Test of an ill-formed chain
		interval I;
      	std::istringstream instr;
#if GAOL_EXCEPTIONS_ENABLED
      	instr.str("[4.5,");
      	try {
	  		instr >> I;
      	} catch (input_format_error e) {
	  		CPPUNIT_ASSERT(e.explanation().substr(0,26) == "Syntax error in expression");
		}
#endif

#if GAOL_EXCEPTIONS_ENABLED
      	instr.str("<3, 4>");
      	try {
	  		instr >> I;
      	} catch (input_format_error e) {
	  		CPPUNIT_ASSERT(e.explanation().substr(0,26) == "Syntax error in expression");
		}
#endif

#if GAOL_EXCEPTIONS_ENABLED
		try {
#endif
			instr.clear();
			instr.str("<3,3>");
			instr >> I;
			TEST_SEQ(I,interval(3.0,3.0));
			instr.clear();
			instr.str("[4.5,6]");
			instr >> I;
      TEST_SEQ(I,interval(4.5,6));
	  	instr.clear();
	  	instr.str("[4, 6 ]");
	  	instr >> I;
	  	TEST_SEQ(I,interval(4,6));
	  	instr.clear();
	  	instr.str("  [-1.0e5 , 18 ]");
	  	instr >> I;
	  	TEST_SEQ(I,interval(-1e5,18));
	  	instr.clear();
	  	instr.str("[empty]");
	  	instr >> I;
	  	TEST_EMPTY(I);
	  	instr.clear();
	  	instr.str("empty");
	  	instr >> I;
	  	TEST_EMPTY(I);
	  	instr.clear();
	  	instr.str("[(-3.5+1.5)*2/2+(exp(0))]");
	  	instr >> I;
	  	TEST_EQ(I,interval(-1));
	  	instr.clear();
	  	instr.str("exp(0)+log(exp(1))+3*cos(pi)");
	  	instr >> I;
	  	TEST_EQ(I,interval(-1));
	  	instr.clear();
	  	instr.str("[4,5]+[4,10]+[2,3]");
	  	instr >> I;
	  	TEST_SEQ(I,interval(10,18));
	  	instr.clear();
	  	instr.str("[4]+[3]");
	  	instr >> I;
	  	TEST_SEQ(I,interval(7,7));
	  	instr.clear();
	  	instr.str("[-inf,dmin]");
	  	instr >> I;
	  	CPPUNIT_ASSERT(std::isinf(I.left()) && (I.right()==std::numeric_limits<double>::min()));
	  	instr.clear();
	  	instr.str("[0,dmax]");
	  	instr >> I;
	  	CPPUNIT_ASSERT(I.left()==0.0 && (I.right()==std::numeric_limits<double>::max()));
      instr.clear();
      instr.str("-inf");
      instr >> I;
      CPPUNIT_ASSERT(I.left()==-GAOL_INFINITY && I.right()==-std::numeric_limits<double>::max());
      instr.clear();
      instr.str("<-inf,-inf>");
      instr >> I;
      CPPUNIT_ASSERT(I.left()==-GAOL_INFINITY && I.right()==-std::numeric_limits<double>::max());
      instr.clear();
      instr.str("[-inf, -inf]");
      instr >> I;
      CPPUNIT_ASSERT(I.left()==-GAOL_INFINITY && I.right()==-std::numeric_limits<double>::max());
      instr.clear();
      instr.str("[-inf]");
      instr >> I;
      CPPUNIT_ASSERT(I.left()==-GAOL_INFINITY && I.right()==-std::numeric_limits<double>::max());
      instr.clear();
      instr.str("inf");
      instr >> I;
      CPPUNIT_ASSERT(I.left()==std::numeric_limits<double>::max() && I.right()==GAOL_INFINITY);
      instr.clear();
      instr.str("[inf]");
      instr >> I;
      CPPUNIT_ASSERT(I.left()==std::numeric_limits<double>::max() && I.right()==GAOL_INFINITY);
      instr.clear();
      instr.str("[inf,inf]");
      instr >> I;
      CPPUNIT_ASSERT(I.left()==std::numeric_limits<double>::max() && I.right()==GAOL_INFINITY);
      instr.clear();
      instr.str("<inf,inf>");
      instr >> I;
      CPPUNIT_ASSERT(I.left()==std::numeric_limits<double>::max() && I.right()==GAOL_INFINITY);
#if GAOL_EXCEPTIONS_ENABLED
		} catch (input_format_error e) {
	  		std::cerr <<  e;
      	}
#endif
	}

	void test_output() {
		interval I("[3.452, 3.453]");
		std::ostringstream os;
		
		os << I;
		interval K(os.str().c_str());
		TEST_PEQ(K,interval(3.452, 3.453));
		os.str("");
		interval::precision(2);
		os << I;
		CPPUNIT_ASSERT(os.str() == string("[3.4, 3.5]"));

		os.str("");
		interval::format(interval_format::width);
		interval::precision(2);
		os << I;
		CPPUNIT_ASSERT(os.str() == string("3.5 (+/- 0.0005)"));

		os.str("");
		interval::format(interval_format::center);
		interval::precision(2);
		os << I;
		CPPUNIT_ASSERT(os.str() == string("3.5"));

		os.str("");
		interval::format(interval_format::hexa);
		interval::precision(2);
		os << I;
		CPPUNIT_ASSERT(os.str() == string("[400b9db22d0e5604, 400b9fbe76c8b43a]"));

		os.str("");
		interval::format(interval_format::agreeing);
		interval::precision(4);
		os << I;
		CPPUNIT_ASSERT(os.str() == string("3.45~[1, 4]"));

      	interval J(3,4);
		interval::format(interval_format::bounds);
		string s = "test line embedding " + string(J) + " as a string";
      	CPPUNIT_ASSERT(s == "test line embedding [3, 4] as a string");
  	}
  // <-- End of tests
};


GAOL_REGISTER_TEST(input_output_test)
