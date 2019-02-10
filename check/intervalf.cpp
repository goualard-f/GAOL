#include "tests.h"


class intervalf_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(intervalf_test);
  CPPUNIT_TEST(test_addition);
  CPPUNIT_TEST(test_subtraction);
  CPPUNIT_TEST(test_multiplication);
  CPPUNIT_TEST(test_division);
  CPPUNIT_TEST(test_inverse);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp() {
  }
  void tearDown() {
  }

  // --> Beginning of tests

  void test_addition() {
    TEST_SEQ(intervalf::universe()+intervalf::universe(),intervalf::universe());
    TEST_SEQ(intervalf(std::numeric_limits<float>::max(),std::numeric_limits<float>::infinity())+intervalf(std::numeric_limits<float>::max(),std::numeric_limits<float>::infinity()),intervalf(std::numeric_limits<float>::max(),std::numeric_limits<float>::infinity()));
    TEST_SEQ(intervalf(-std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::max())+intervalf(-std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::max()),intervalf(-std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::max()));

    TEST_EMPTY(intervalf::emptyset()+intervalf::emptyset());

    TEST_SEQ(m_min_zero+m_min_zero,2*m_min_zero);
    TEST_SEQ(m_min_zero+zero_min,m_min_min);
    TEST_SEQ(+intervalf(3,4),intervalf(3,4));
    TEST_EMPTY(+intervalf::emptyset());
    TEST_SEQ(intervalf(3,4)+intervalf(-6,2),intervalf(-3,6));
    TEST_SEQ(intervalf(3,4)+5,intervalf(8,9));
  }


  void test_subtraction() {
    TEST_SEQ(-intervalf(4,5),intervalf(-5,-4));
    TEST_SEQ(-intervalf(std::numeric_limits<float>::max(),std::numeric_limits<float>::infinity()),intervalf(-std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::max()));
    TEST_EMPTY(-intervalf::emptyset());
    TEST_SEQ(intervalf::universe()-intervalf::universe(),intervalf::universe());
    TEST_SEQ(intervalf(4,5)-intervalf(3,7),intervalf(-3,2));
    TEST_SEQ(intervalf(-std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::max())-
              intervalf(-std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::max()),intervalf::universe());
    TEST_SEQ(intervalf(3,4)-5,intervalf(-2,-1));
  }

  void test_multiplication() {
    TEST_EMPTY(intervalf::emptyset()*intervalf::emptyset());
    TEST_EMPTY(intervalf::emptyset()*intervalf(4,5));
    TEST_EMPTY(intervalf(4,5)*intervalf::emptyset());
    TEST_EMPTY(intervalf::emptyset()*intervalf::zero());
    TEST_EMPTY(intervalf::zero()*intervalf::emptyset());
    TEST_EMPTY(intervalf::emptyset()*3);
    TEST_EMPTY(3*intervalf::emptyset());
    TEST_EMPTY(intervalf::emptyset()*0.0);
    TEST_EMPTY(0.0*intervalf::emptyset());

    TEST_SEQ(intervalf::universe()*intervalf::universe(),intervalf::universe());
    TEST_SEQ(intervalf(2,4)*5,intervalf(10,20));
    TEST_SEQ(intervalf(-3,-2)*5,intervalf(-15,-10));
    TEST_SEQ(intervalf(2,4)*(-5),intervalf(-20,-10));
    TEST_SEQ(intervalf(-3,-2)*(-5),intervalf(10,15));
    TEST_SEQ(intervalf(3,4)*0.0f,intervalf::zero());
    TEST_EMPTY(intervalf::emptyset()*0.0f);
    TEST_SEQ(intervalf::universe()*0.0f,intervalf::zero());

    TEST_SEQ(intervalf(-3,4)*intervalf::zero(),intervalf::zero());
    TEST_SEQ(intervalf(-std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::max())*intervalf::zero(),intervalf::zero());
    TEST_SEQ(intervalf(std::numeric_limits<float>::max(),std::numeric_limits<float>::infinity())*intervalf::zero(),intervalf::zero());
    TEST_SEQ(intervalf::zero()*intervalf(-std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::max()),intervalf::zero());
    TEST_SEQ(intervalf::zero()*intervalf(std::numeric_limits<float>::max(),std::numeric_limits<float>::infinity()),intervalf::zero());

    // P P
    TEST_SEQ(intervalf(4,5)*intervalf(2,3),intervalf(8,15));
    // P M
    TEST_SEQ(intervalf(4,5)*intervalf(-2,3),intervalf(-10,15));
    // P N
    TEST_SEQ(intervalf(4,5)*intervalf(-3,-2),intervalf(-15,-8));
    // M P
    TEST_SEQ(intervalf(-3,-2)*intervalf(4,5),intervalf(-15,-8));
    // M M
    TEST_SEQ(intervalf(-3,2)*intervalf(-4,5),intervalf(-15,12));
    // M N
    TEST_SEQ(intervalf(-3,2)*intervalf(-5,-4),intervalf(-10,15));
    // N P
    TEST_SEQ(intervalf(-3,-2)*intervalf(4,5),intervalf(-15,-8));
    // N M
    TEST_SEQ(intervalf(-3,-2)*intervalf(-4,5),intervalf(-15,12));
    // N N
    TEST_SEQ(intervalf(-3,-2)*intervalf(-5,-4),intervalf(8,15));
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
	TEST_EQ(interval(-3,-2)/interval(-5,-4),interval(0.4,0.75));
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
	TEST_EQ(interval(-3,-2)/interval(4,5),interval(-0.75,-0.4));
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
	TEST_EQ(interval(2,3)/interval(-5,-4),interval(-0.75,-0.4));
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
	TEST_EQ(interval(2,3)/interval(4,5),interval(0.4,0.75));
 }

  void test_inverse() {
    TEST_EMPTY(inverse(intervalf::emptyset()));
    TEST_SEQ(inverse(intervalf::universe()),intervalf::universe());
    CPPUNIT_ASSERT(inverse(intervalf(10.0f)).set_contains(0.1f));

    TEST_SEQ(inverse(intervalf(-4.f,-2.f)),intervalf(-.5f,-.25f));
    intervalf tmp = inverse(intervalf(-2.f,0.f));
    CPPUNIT_ASSERT(tmp.left()==-std::numeric_limits<float>::infinity() && tmp.right()==-.5f);
    TEST_EMPTY(inverse(intervalf(0.0f)));
    TEST_SEQ(inverse(intervalf(-4.f,2.f)),intervalf::universe());
    TEST_SEQ(inverse(intervalf(-2.f,4.f)),intervalf::universe());
    tmp = inverse(intervalf(0.f,2.f));
    CPPUNIT_ASSERT(tmp.left()==0.5f && tmp.right()==std::numeric_limits<float>::infinity());
    TEST_SEQ(inverse(intervalf(2.f,4.f)),intervalf(0.25f,0.5f));
 }

  // <-- End of tests
};

GAOL_REGISTER_TEST(intervalf_test)
