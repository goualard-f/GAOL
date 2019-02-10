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
 * CVS: $Id: interval_functions.cpp 286 2016-10-05 09:03:25Z goualard $
 * Last modified:
 * By:
 *--------------------------------------------------------------------------*/

#include "tests.h"
#include <limits>

class interval_function_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(interval_function_test);
  CPPUNIT_TEST(test_properties);
  CPPUNIT_TEST(test_splitting);
  CPPUNIT_TEST(test_set_operations);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp() {
  }
  void tearDown() {
  }

  // --> Beginning of tests
  void test_properties() {
      TEST_TRUE(interval(4,5).width()==1);
      TEST_TRUE(interval::emptyset().width()==-1);
      TEST_TRUE(interval(1,1).width()==0);
      TEST_TRUE(interval(1.0,next_float(1.0)).width() == std::numeric_limits<double>::epsilon());

      TEST_TRUE(interval(4,5).mig() == 4);
      TEST_TRUE(interval(-10,-6).mig() == 6);
      TEST_TRUE(std::isnan(interval::emptyset().mig()));
      TEST_TRUE(interval(-5,6).mig() == 0);
      TEST_TRUE(interval::universe().mig() == 0);

      TEST_TRUE(interval(4,5).smig() == 4);
      TEST_TRUE(interval(-10,-6).smig() == -6);
      TEST_TRUE(std::isnan(interval::emptyset().smig()));
      TEST_TRUE(interval(-5,6).smig() == 0);
      TEST_TRUE(interval::universe().smig() == 0);

      TEST_TRUE(interval(4,5).mag() == 5);
      TEST_TRUE(interval(-10,-6).mag() == 10);
      TEST_TRUE(std::isnan(interval::emptyset().mag()));
      TEST_TRUE(interval(-5,6).mag() == 6);
      TEST_TRUE(std::isinf(interval::universe().mag()));

      TEST_TRUE(std::isnan(hausdorff(interval::emptyset(),interval::emptyset())));
      TEST_TRUE(hausdorff(interval(4,5),interval(6,8))==3);
      TEST_TRUE(hausdorff(interval(-3,3),interval(-4,4))==1);

      TEST_TRUE(hausdorff(interval(3,4),interval(5,GAOL_INFINITY))==GAOL_INFINITY);
      TEST_TRUE(hausdorff(interval(3,4),interval(-GAOL_INFINITY,6))==GAOL_INFINITY);
      TEST_TRUE(hausdorff(interval(3,4),interval::universe())==GAOL_INFINITY);
      TEST_TRUE(hausdorff(interval(3,GAOL_INFINITY),interval(5,6))==GAOL_INFINITY);
      TEST_TRUE(hausdorff(interval(3,GAOL_INFINITY),interval(5,GAOL_INFINITY))==GAOL_INFINITY);
      TEST_TRUE(hausdorff(interval(3,GAOL_INFINITY),interval(-GAOL_INFINITY,6))==GAOL_INFINITY);
      TEST_TRUE(hausdorff(interval(3,GAOL_INFINITY),interval::universe())==GAOL_INFINITY);
      TEST_TRUE(hausdorff(interval(-GAOL_INFINITY,4),interval(5,6))==GAOL_INFINITY);
      TEST_TRUE(hausdorff(interval(-GAOL_INFINITY,4),interval(5,GAOL_INFINITY))==GAOL_INFINITY);
      TEST_TRUE(hausdorff(interval(-GAOL_INFINITY,4),interval(-GAOL_INFINITY,6))==GAOL_INFINITY);
      TEST_TRUE(hausdorff(interval(-GAOL_INFINITY,4),interval::universe())==GAOL_INFINITY);
      TEST_TRUE(hausdorff(interval::universe(),interval(5,6))==GAOL_INFINITY);
      TEST_TRUE(hausdorff(interval::universe(),interval(5,GAOL_INFINITY))==GAOL_INFINITY);
      TEST_TRUE(hausdorff(interval::universe(),interval(-GAOL_INFINITY,6))==GAOL_INFINITY);
      TEST_TRUE(hausdorff(interval::universe(),interval::universe())==0);

      TEST_TRUE(std::isnan(interval::emptyset().midpoint()));
      TEST_TRUE(std::isnan(interval(6,-4).midpoint()));
      TEST_TRUE(interval(4,4).midpoint() == 4);
      TEST_TRUE(interval(10,next_float(10)).set_contains(interval(10,next_float(10))));
      TEST_TRUE(interval(6,10).midpoint() == 8);
      TEST_TRUE(interval(previous_float(1.0),
			 next_float(1.0)).set_contains(interval(previous_float(1.0),
								next_float(1.0))));
      TEST_TRUE(interval(-6,GAOL_INFINITY).midpoint()==std::numeric_limits<double>::max());
      TEST_TRUE(interval(-GAOL_INFINITY,5).midpoint()==-std::numeric_limits<double>::max());
      TEST_TRUE(interval(-GAOL_INFINITY,GAOL_INFINITY).midpoint()==0);
        CPPUNIT_ASSERT(interval(-1e8,1e8).set_contains(interval(-1e8,1e8).midpoint()));
        CPPUNIT_ASSERT(interval(-std::numeric_limits<double>::max(),
                       std::numeric_limits<double>::max()).set_contains(interval(-std::numeric_limits<double>::max(),
                       std::numeric_limits<double>::max()).midpoint()));
	  TEST_TRUE(interval(std::numeric_limits<double>::max()).set_contains(
					interval(std::numeric_limits<double>::max()).midpoint()));
	  TEST_TRUE(interval(std::numeric_limits<double>::min()).set_contains(
					interval(std::numeric_limits<double>::min()).midpoint()));
	  TEST_TRUE(interval(std::numeric_limits<double>::denorm_min()).set_contains(
				       interval(std::numeric_limits<double>::denorm_min()).midpoint()));
	  TEST_TRUE(interval(previous_float(1.0),next_float(1.0)).set_contains(
					interval(previous_float(1.0),next_float(1.0)).midpoint()));
      TEST_TRUE(interval::emptyset().mid().is_empty());
      TEST_TRUE(interval(previous_float(1.0),
			 next_float(1.0)).set_contains(interval(previous_float(1.0),
								next_float(1.0)).mid()));
      TEST_TRUE(interval::universe().mid().set_eq(interval::zero()));
      TEST_TRUE(interval(-5,GAOL_INFINITY).mid().set_eq(interval(std::numeric_limits<double>::max())));
      TEST_TRUE(interval(-GAOL_INFINITY,5).mid().set_eq(interval(-std::numeric_limits<double>::max())));
      TEST_TRUE(interval(-2,5).mid().set_eq(interval(1.5)));
      
      TEST_SEQ(abs(interval(4,5)),interval(4,5));
      TEST_SEQ(abs(interval(-5,6)),interval(0,6));
      TEST_SEQ(abs(interval(-4,-2)),interval(2,4));
      TEST_SEQ(abs(interval(-5,-5)),interval(5));
      TEST_SEQ(abs(interval::universe()),interval::positive());
      TEST_EMPTY(abs(interval::emptyset()));
      TEST_SEQ(abs(interval(-GAOL_INFINITY,-4)),interval(4,GAOL_INFINITY));

      TEST_TRUE(chi(interval::zero())==-1);
      TEST_TRUE(chi(interval(-0.0,+0.0))==-1);
      TEST_TRUE(std::isnan(chi(interval::emptyset())));
      TEST_TRUE(chi(interval::universe())==1);
      TEST_TRUE(chi(interval(-5,10))==-0.5);
      TEST_TRUE(chi(interval(-5,GAOL_INFINITY))==0.0);
      TEST_TRUE(chi(interval(3,6))==0.5);
      TEST_TRUE(chi(interval(-6,3))==-0.5);

      TEST_SEQ(min(interval(4,7),interval(3,10)),interval(3,7));
      TEST_SEQ(min(interval::emptyset(),interval::emptyset()),interval::emptyset());
      TEST_SEQ(min(interval::emptyset(),interval(4,5)),interval::emptyset());
      TEST_EMPTY(min(interval(4,6),interval::emptyset()));
      TEST_SEQ(min(interval::universe(),interval::universe()),interval::universe());
      TEST_SEQ(min(interval(-6,-3),interval::universe()),interval(-GAOL_INFINITY,-3));
      TEST_SEQ(min(interval::universe(),interval(-6,-3)),interval(-GAOL_INFINITY,-3));

      TEST_SEQ(max(interval(4,7),interval(3,10)),interval(4,10));
      TEST_SEQ(max(interval::emptyset(),interval::emptyset()),interval::emptyset());
      TEST_SEQ(max(interval::emptyset(),interval(4,5)),interval::emptyset());
      TEST_EMPTY(max(interval(4,6),interval::emptyset()));
      TEST_SEQ(max(interval::universe(),interval::universe()),interval::universe());
      TEST_SEQ(max(interval(-6,-3),interval::universe()),interval(-6,GAOL_INFINITY));
      TEST_SEQ(max(interval::universe(),interval(-6,-3)),interval(-6,GAOL_INFINITY));

      TEST_SEQ(floor(interval(4.5,6.5)),interval(4,6));
      TEST_SEQ(floor(interval::universe()),interval::universe());
      TEST_EMPTY(floor(interval::emptyset()));
      TEST_SEQ(floor(interval("[-10.4,3.5]")),interval(-11,3));

      TEST_SEQ(ceil(interval(4.5,6.5)),interval(5,7));
      TEST_SEQ(ceil(interval::universe()),interval::universe());
      TEST_EMPTY(ceil(interval::emptyset()));
      TEST_SEQ(ceil(interval("[-10.4,3.5]")),interval(-10,4));

      TEST_SEQ(integer(interval::universe()),interval::universe());
      TEST_EMPTY(integer(interval::emptyset()));
      TEST_SEQ(integer(interval(4.5,6.5)),interval(5,6));
      TEST_SEQ(integer(interval(-6.5,3.75)),interval(-6,3));
      TEST_EMPTY(integer(interval("1/10")));
  }

  void test_splitting() {
      interval I1, I2;

      interval(4,5).split(I1,I2); TEST_SEQ(I1,interval(4,4.5)); TEST_SEQ(I2,interval(4.5,5));
      interval::universe().split(I1,I2); TEST_SEQ(I1,interval::negative()); TEST_SEQ(I2,interval::positive());
      interval::emptyset().split(I1,I2); TEST_EMPTY(I1); TEST_EMPTY(I2);
      interval("1/10").split(I1,I2); TEST_CONT(interval("1/10"),I1); TEST_CONT(interval("1/10"),I2);
      interval(5.0).split(I1,I2); TEST_SEQ(I1, interval(5.0)); TEST_SEQ(I2,interval(5.0));

      TEST_EMPTY(interval::emptyset().split_left());
      TEST_SEQ(interval::universe().split_left(),interval::negative());

      TEST_EMPTY(interval::emptyset().split_right());
      TEST_SEQ(interval::universe().split_right(),interval::positive());
  }

  void test_set_operations() {
      TEST_SEQ(interval(3,4) | interval(6,7),interval(3,7));
      TEST_SEQ(interval(4,5) | interval::emptyset(),interval(4,5));
      TEST_SEQ(interval::emptyset() | interval(4,5),interval(4,5));

      TEST_EMPTY(interval(4,5) & interval::emptyset());
      TEST_EMPTY(interval::emptyset() & interval(4,5));
      TEST_EMPTY(interval(3,4) & interval(6,7));
      TEST_SEQ(interval(3,7) & interval(5,7),interval(5,7));
      TEST_SEQ(interval(2,7) & interval(4,10),interval(4,7));
  }

  // <-- End of tests
};


GAOL_REGISTER_TEST(interval_function_test)
