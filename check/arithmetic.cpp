#include "tests.h"

class arithmetic_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(arithmetic_test);
  CPPUNIT_TEST(test_addition);
  CPPUNIT_TEST(test_subtraction);
  CPPUNIT_TEST(test_multiplication);
  CPPUNIT_TEST(test_division);
  CPPUNIT_TEST(test_reldivision);
  CPPUNIT_TEST(test_inverse);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp() {
  }
  void tearDown() {
  }

  // --> Beginning of tests

  void test_addition() {
    TEST_SEQ(interval::universe()+interval::universe(),interval::universe());
    TEST_SEQ(max_inf+max_inf,max_inf);
    TEST_SEQ(m_inf_m_max+m_inf_m_max,m_inf_m_max);

    TEST_EMPTY(interval::emptyset()+interval::emptyset());

    TEST_SEQ(m_min_zero+m_min_zero,2*m_min_zero);
    TEST_SEQ(m_min_zero+zero_min,m_min_min);
	TEST_SEQ(+interval(3,4),interval(3,4));
	TEST_EMPTY(+interval::emptyset());
	TEST_SEQ(interval(3,4)+interval(-6,2),interval(-3,6));
	TEST_SEQ(interval(3,4)+5,interval(8,9));
 }


  void test_subtraction() {
	TEST_SEQ(-interval(4,5),interval(-5,-4));
	TEST_SEQ(-max_inf,m_inf_m_max);
	TEST_EMPTY(-interval::emptyset());
    TEST_SEQ(interval::universe()-interval::universe(),interval::universe());
    TEST_SEQ(interval(4,5)-interval(3,7),interval(-3,2));
	TEST_SEQ(m_inf_m_max-m_inf_m_max,interval::universe());
	TEST_SEQ(interval(3,4)-5,interval(-2,-1));
  }

  void test_multiplication() {
     TEST_EMPTY(interval::emptyset()*interval::emptyset());
    TEST_EMPTY(interval::emptyset()*interval(4,5));
    TEST_EMPTY(interval(4,5)*interval::emptyset());
    TEST_EMPTY(interval::emptyset()*interval::zero());
    TEST_EMPTY(interval::zero()*interval::emptyset());
    TEST_EMPTY(interval::emptyset()*3);
    TEST_EMPTY(3*interval::emptyset());
    TEST_EMPTY(interval::emptyset()*0.0);
    TEST_EMPTY(0.0*interval::emptyset());

    TEST_SEQ(interval::universe()*interval::universe(),interval::universe());
	TEST_SEQ(interval(2,4)*5,interval(10,20));
	TEST_SEQ(interval(-3,-2)*5,interval(-15,-10));
	TEST_SEQ(interval(2,4)*(-5),interval(-20,-10));
	TEST_SEQ(interval(-3,-2)*(-5),interval(10,15));
	TEST_SEQ(interval(3,4)*0,interval::zero());
	TEST_EMPTY(interval::emptyset()*0);
	TEST_SEQ(interval::universe()*0,interval::zero());

	TEST_SEQ(interval(-3,4)*interval::zero(),interval::zero());
	TEST_SEQ(m_inf_m_max*interval::zero(),interval::zero());
	TEST_SEQ(max_inf*interval::zero(),interval::zero());
	TEST_SEQ(interval::zero()*m_inf_m_max,interval::zero());
	TEST_SEQ(interval::zero()*max_inf,interval::zero());

	// P P
	TEST_SEQ(interval(4,5)*interval(2,3),interval(8,15));
	// P M
	TEST_SEQ(interval(4,5)*interval(-2,3),interval(-10,15));
	// P N
	TEST_SEQ(interval(4,5)*interval(-3,-2),interval(-15,-8));
	// M P
	TEST_SEQ(interval(-3,-2)*interval(4,5),interval(-15,-8));
	// M M
	TEST_SEQ(interval(-3,2)*interval(-4,5),interval(-15,12));
	// M N
	TEST_SEQ(interval(-3,2)*interval(-5,-4),interval(-10,15));
	// N P
	TEST_SEQ(interval(-3,-2)*interval(4,5),interval(-15,-8));
	// N M
	TEST_SEQ(interval(-3,-2)*interval(-4,5),interval(-15,12));
	// N N
	TEST_SEQ(interval(-3,-2)*interval(-5,-4),interval(8,15));
  }

 void test_division() {
 	TEST_EMPTY(interval::emptyset()/interval::emptyset());
	TEST_EMPTY(interval::emptyset()/interval::universe());
	TEST_EMPTY(interval::universe()/interval::emptyset());
	TEST_SEQ(interval(3,4)/2,interval(1.5,2));
	TEST_SEQ(interval(3,4)/(-2),interval(-2,-1.5));
	TEST_SEQ(interval(-4,-3)/2,interval(-2,-1.5));
	TEST_SEQ(interval(-4,-3)/(-2),interval(1.5,2));
	TEST_EMPTY(interval(3,4)/0);
	TEST_EMPTY(interval::universe()/0);

	// N1 N1
	TEST_EQ(interval(-3,-2)/interval(-5,-4),interval("[0.4,0.75]"));
	// N1 Z
	TEST_EMPTY(interval(-3,-2)/interval::zero());

	interval tmp;
	// N1 N0
	tmp = interval(-3,-2)/interval(-4,0);
	CPPUNIT_ASSERT(tmp.left() == 0.5 && tmp.right()==GAOL_INFINITY);
	// N1 M
	TEST_SEQ(interval(-3,-2)/interval(-4,5),interval::universe());
	// N1 P0
	tmp = interval(-3,-2)/interval(0,4);
	CPPUNIT_ASSERT(tmp.left()==-GAOL_INFINITY && tmp.right()==-.5);
	// N1 P1
	TEST_EQ(interval(-3,-2)/interval(4,5),interval("[-0.75,-0.4]"));
	// Z Z
	TEST_EMPTY(interval::zero()/interval::zero());
	// Z (NMP)
	TEST_SEQ(interval::zero()/interval(-3,-2),interval::zero());
	TEST_SEQ(interval::zero()/interval(-3,2),interval::zero());
	TEST_SEQ(interval::zero()/interval(2,3),interval::zero());
	// N0 N1
	TEST_SEQ(interval(-2,0)/interval(-5,-4),interval(0,0.5));
	// N0 Z
	TEST_EMPTY(interval(-2,0)/interval::zero());
	// N0 N0
	tmp = interval(-2,0)/interval(-4,0);
	CPPUNIT_ASSERT(tmp.left()==0 && tmp.right()==GAOL_INFINITY);
	// N0 M
	TEST_SEQ(interval(-3,0)/interval(-4,2),interval::universe());
	// N0 P0
	tmp = interval(-2,0)/interval(0,4);
	CPPUNIT_ASSERT(tmp.left()==-GAOL_INFINITY && tmp.right()==0);
	// N0 P1
	TEST_SEQ(interval(-2,0)/interval(4,5),interval(-0.5,0));
	// M N1
	TEST_SEQ(interval(-3,2)/interval(-5,-4),interval(-0.5,0.75));
	// M Z
	TEST_EMPTY(interval(-3,2)/interval::zero());
	// M N0
	TEST_SEQ(interval(-3,2)/interval(-4,0),interval::universe());
	// M M
	TEST_SEQ(interval(-3,2)/interval(-4,5),interval::universe());
	// M P0
	TEST_SEQ(interval(-3,2)/interval(0,5),interval::universe());
	// M P1
	TEST_SEQ(interval(-3,2)/interval(4,5),interval(-0.75,0.5));
	// P0 N1
	TEST_SEQ(interval(0,2)/interval(-5,-4),interval(-0.5,0.0));
	// P0 Z
	TEST_EMPTY(interval(0,2)/interval::zero());
	// P0 N0
	tmp = interval(0,2)/interval(-4,0);
	CPPUNIT_ASSERT(tmp.left()==-GAOL_INFINITY && tmp.right()==0);
	// P0 M
	TEST_SEQ(interval(0,2)/interval(-4,5),interval::universe());
	// P0 P0
	tmp = interval(0,2)/interval(0,5);
	CPPUNIT_ASSERT(tmp.left()==0 && tmp.right()==GAOL_INFINITY);
	// P0 P1
	TEST_SEQ(interval(0,2)/interval(4,5),interval(0,0.5));
	// P1 N1
	TEST_EQ(interval(2,3)/interval(-5,-4),interval("[-0.75,-0.4]"));
	// P1 Z
	TEST_EMPTY(interval(2,3)/interval::zero());
	// P1 N0
	tmp = interval(2,3)/interval(-4,0);
	CPPUNIT_ASSERT(tmp.left() == -GAOL_INFINITY && tmp.right() == -0.5);
	// P1 M
	TEST_SEQ(interval(2,3)/interval(-4,5),interval::universe());
	// P1 P0
	tmp = interval(2,3)/interval(0,4);
	CPPUNIT_ASSERT(tmp.left() == 0.5 && tmp.right() == GAOL_INFINITY);
	// P1 P1
	TEST_EQ(interval(2,3)/interval(4,5),interval("[0.4,0.75]"));
 }

  void test_reldivision() { // Test for %
 	TEST_EMPTY(interval::emptyset()%interval::emptyset());
	TEST_EMPTY(interval::emptyset()%interval::universe());
	TEST_EMPTY(interval::universe()%interval::emptyset());

	TEST_SEQ(interval(3,4)%2,interval(1.5,2));
	TEST_SEQ(interval(3,4)%(-2),interval(-2,-1.5));
	TEST_SEQ(interval(-4,-3)%2,interval(-2,-1.5));
	TEST_SEQ(interval(-4,-3)%(-2),interval(1.5,2));
	TEST_EMPTY(interval(3,4)%0);
	TEST_SEQ(interval::universe()%0,interval::universe());
	TEST_SEQ(interval(-3,4)%0,interval::universe());

	interval tmp;
	// N1 N1
	TEST_EQ(interval(-3,-2)%interval(-5,-4),interval("[0.4,0.75]"));
	// N1 Z
	TEST_EMPTY(interval(-3,-2)%interval::zero());
	// N1 N0
	tmp = interval(-3,-2)%interval(-4,0);
	CPPUNIT_ASSERT(tmp.left() == 0.5 && tmp.right()==GAOL_INFINITY);
	// N1 M
	TEST_SEQ(interval(-3,-2)%interval(-4,5),interval::universe());
	// N1 P0
	tmp = interval(-3,-2)%interval(0,4);
	CPPUNIT_ASSERT(tmp.left()==-GAOL_INFINITY && tmp.right()==-.5);
	// N1 P1
	TEST_EQ(interval(-3,-2)%interval(4,5),interval("[-0.75,-0.4]"));
	// Z Z
	TEST_SEQ(interval::zero()%interval::zero(),interval::universe());
	// Z (NMP)
	TEST_SEQ(interval::zero()%interval(-3,-2),interval::zero());
	TEST_SEQ(interval::zero()%interval(-3,0),interval::universe());
	TEST_SEQ(interval::zero()%interval(-3,2),interval::universe());
	TEST_SEQ(interval::zero()%interval(0,3),interval::universe());
	TEST_SEQ(interval::zero()%interval(2,3),interval::zero());
	// N0 N1
	TEST_SEQ(interval(-2,0)%interval(-5,-4),interval(0,0.5));
	// N0 Z
	TEST_SEQ(interval(-2,0)%interval::zero(),interval::universe());
	// N0 N0
	TEST_SEQ(interval(-2,0)%interval(-4,0),interval::universe());
	// N0 M
	TEST_SEQ(interval(-3,0)%interval(-4,2),interval::universe());
	// N0 P0
	TEST_SEQ(interval(-2,0)%interval(0,4),interval::universe());
	// N0 P1
	TEST_SEQ(interval(-2,0)%interval(4,5),interval(-0.5,0));
	// M N1
	TEST_SEQ(interval(-3,2)%interval(-5,-4),interval(-0.5,0.75));
	// M Z
	TEST_SEQ(interval(-3,2)%interval::zero(),interval::universe());
	// M N0
	TEST_SEQ(interval(-3,2)%interval(-4,0),interval::universe());
	// M M
	TEST_SEQ(interval(-3,2)%interval(-4,5),interval::universe());
	// M P0
	TEST_SEQ(interval(-3,2)%interval(0,5),interval::universe());
	// M P1
	TEST_SEQ(interval(-3,2)%interval(4,5),interval(-0.75,0.5));
	// P0 N1
	TEST_SEQ(interval(0,2)%interval(-5,-4),interval(-0.5,0.0));
	// P0 Z
	TEST_SEQ(interval(0,2)%interval::zero(),interval::universe());
	// P0 N0
	TEST_SEQ(interval(0,2)%interval(-4,0),interval::universe());
	// P0 M
	TEST_SEQ(interval(0,2)%interval(-4,5),interval::universe());
	// P0 P0
	TEST_SEQ(interval(0,2)%interval(0,5),interval::universe());
	// P0 P1
	TEST_SEQ(interval(0,2)%interval(4,5),interval(0,0.5));
	// P1 N1
	TEST_EQ(interval(2,3)%interval(-5,-4),interval("[-0.75,-0.4]"));
	// P1 Z
	TEST_EMPTY(interval(2,3)%interval::zero());
	// P1 N0
	tmp = interval(2,3)%interval(-4,0);
	CPPUNIT_ASSERT(tmp.left() == -GAOL_INFINITY && tmp.right() == -0.5);
	// P1 M
	TEST_SEQ(interval(2,3)%interval(-4,5),interval::universe());
	// P1 P0
	tmp = interval(2,3)%interval(0,4);
	CPPUNIT_ASSERT(tmp.left() == 0.5 && tmp.right() == GAOL_INFINITY);
	// P1 P1
	TEST_EQ(interval(2,3)%interval(4,5),interval("[0.4,0.75]"));
  }

  void test_inverse() {
    TEST_EMPTY(inverse(interval::emptyset()));
    TEST_SEQ(inverse(interval::universe()),interval::universe());
    TEST_EQ(inverse(interval(10.0)),interval("0.1"));

    TEST_SEQ(inverse(interval(-4,-2)),interval(-.5,-.25));
    interval tmp = inverse(interval(-2,0));
    CPPUNIT_ASSERT(tmp.left()==-GAOL_INFINITY && tmp.right()==-.5);
    TEST_EMPTY(inverse(interval::zero()));
    TEST_SEQ(inverse(interval(-4,2)),interval::universe());
    TEST_SEQ(inverse(interval(-2,4)),interval::universe());
    tmp = inverse(interval(0,2));
    CPPUNIT_ASSERT(tmp.left()==0.5 && tmp.right()==GAOL_INFINITY);
    TEST_SEQ(inverse(interval(2,4)),interval(0.25,0.5));
 }

  // <-- End of tests
};

GAOL_REGISTER_TEST(arithmetic_test)
