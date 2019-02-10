#include "tests.h"


class interval2f_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(interval2f_test);
  CPPUNIT_TEST(test_addition);
  CPPUNIT_TEST(test_subtraction);
  CPPUNIT_TEST(test_multiplication);
  CPPUNIT_TEST(test_ipower);
  CPPUNIT_TEST(test_inverse);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp() {
  }
  void tearDown() {
  }

  // --> Beginning of tests

  void test_addition() {
			interval2f a(3,4), b(1,2);
			interval2f c = a+b;
			TEST_SEQ4(c,interval2f(4,6));
			TEST_SEQ4(interval2f::m128_universe()+interval2f::m128_universe(),interval2f::m128_universe());
			TEST_EMPTY4(interval2f::m128_emptyset()+interval2f::m128_emptyset());
 }


  void test_subtraction() {
			interval2f x;
			x = interval2f(3,6)-interval2f(1,7);
			TEST_SEQ4(interval2f(3,6)-interval2f(1,7),interval2f(-4,5));
			TEST_SEQ4(-x,interval2f(-5,4));
			TEST_SEQ4(interval2f::m128_universe()+interval2f::m128_universe(),interval2f::m128_universe());
			TEST_EMPTY4(interval2f::m128_emptyset()+interval2f::m128_emptyset());
  }

  void test_multiplication() {
			TEST_SEQ4(interval2f(3,4)*interval2f(6,8),interval2f(18,32));
			TEST_SEQ4(interval2f(0.,0.)*interval2f(0.,0.),interval2f(0.,0.));
			TEST_SEQ4(interval2f(-0.,0.)*interval2f(-0.,0.),interval2f(0.,0.));
			TEST_SEQ4(interval2f(0.,-0.)*interval2f(-0.,0.),interval2f(0.,0.));
			TEST_SEQ4(interval2f(0.,-0.)*interval2f(0.,-0.),interval2f(0.,0.));
			TEST_SEQ4(interval2f(-3,-2)*interval2f(-5,-2),interval2f(4,15));
			TEST_SEQ4(interval2f(-3,-2)*interval2f(0.,-0.),interval2f(0,0));
			TEST_SEQ4(interval2f(-3,-2)*interval2f(-2,5),interval2f(-15,6));
			TEST_SEQ4(interval2f(-3,-2)*interval2f(2,5),interval2f(-15,-4));
			TEST_SEQ4(interval2f(-3,2)*interval2f(-5,-3),interval2f(-10,15));
			TEST_SEQ4(interval2f(-3,2)*interval2f(-4,5),interval2f(-15,12));
			TEST_SEQ4(interval2f(-3,2)*interval2f(4,5),interval2f(-15,10));
			TEST_SEQ4(interval2f(3,4)*interval2f(-3,-2),interval2f(-12,-6));
			TEST_SEQ4(interval2f(3,4)*interval2f(-5,2),interval2f(-20,8));
			TEST_SEQ4(interval2f(-0.0,3)*interval2f(-5,-2),interval2f(-15,0));
			TEST_SEQ4(interval2f(3,4)*interval2f(2,5),interval2f(6,20));

			TEST_SEQ4(interval2f(-GAOL_INFINITY,-5.0)*interval2f(-2,0),interval2f(0,GAOL_INFINITY));
			TEST_SEQ4(interval2f(-GAOL_INFINITY,-5.0)*interval2f(0,0),interval2f(0,0));
			TEST_SEQ4(interval2f(-GAOL_INFINITY,-5.0)*interval2f(0,2),interval2f(-GAOL_INFINITY,0));
			TEST_SEQ4(interval2f(-GAOL_INFINITY,0.0)*interval2f(-GAOL_INFINITY,-2),interval2f(0,GAOL_INFINITY));
			TEST_SEQ4(interval2f(-GAOL_INFINITY,0.0)*interval2f(-GAOL_INFINITY,0),interval2f(0,GAOL_INFINITY));
			TEST_SEQ4(interval2f(-GAOL_INFINITY,0.0)*interval2f(0,0),interval2f(0,0));
			TEST_SEQ4(interval2f(-GAOL_INFINITY,0.0)*interval2f(-GAOL_INFINITY,GAOL_INFINITY),interval2f(-GAOL_INFINITY,GAOL_INFINITY));
			TEST_SEQ4(interval2f(-GAOL_INFINITY,0.0)*interval2f(0,GAOL_INFINITY),interval2f(-GAOL_INFINITY,0));
			TEST_SEQ4(interval2f(-GAOL_INFINITY,0.0)*interval2f(2,GAOL_INFINITY),interval2f(-GAOL_INFINITY,0));
			TEST_SEQ4(interval2f(0,0)*interval2f(-GAOL_INFINITY,-2),interval2f(0,0));
			TEST_SEQ4(interval2f(0,0)*interval2f(-GAOL_INFINITY,0),interval2f(0,0));
			TEST_SEQ4(interval2f(0,0)*interval2f(-GAOL_INFINITY,GAOL_INFINITY),interval2f(0,0));
			TEST_SEQ4(interval2f(0,0)*interval2f(0,GAOL_INFINITY),interval2f(0,0));
			TEST_SEQ4(interval2f(0,0)*interval2f(2,GAOL_INFINITY),interval2f(0,0));
			TEST_SEQ4(interval2f(-GAOL_INFINITY,GAOL_INFINITY)*interval2f(0,0),interval2f(0,0));
			TEST_SEQ4(interval2f(-GAOL_INFINITY,GAOL_INFINITY)*interval2f(0,GAOL_INFINITY),interval2f(-GAOL_INFINITY,GAOL_INFINITY));
			TEST_SEQ4(interval2f(0,GAOL_INFINITY)*interval2f(-GAOL_INFINITY,-5),interval2f(-GAOL_INFINITY,0));
			TEST_SEQ4(interval2f(0,GAOL_INFINITY)*interval2f(-GAOL_INFINITY,0),interval2f(-GAOL_INFINITY,0));
			TEST_SEQ4(interval2f(0,GAOL_INFINITY)*interval2f(0,0),interval2f(0,0));
			TEST_SEQ4(interval2f(0,GAOL_INFINITY)*interval2f(-GAOL_INFINITY,GAOL_INFINITY),interval2f(-GAOL_INFINITY,GAOL_INFINITY));
			TEST_SEQ4(interval2f(0,GAOL_INFINITY)*interval2f(0,GAOL_INFINITY),interval2f(0,GAOL_INFINITY));
			TEST_SEQ4(interval2f(0,GAOL_INFINITY)*interval2f(2,GAOL_INFINITY),interval2f(0,GAOL_INFINITY));
			TEST_SEQ4(interval2f(2,GAOL_INFINITY)*interval2f(-GAOL_INFINITY,0),interval2f(-GAOL_INFINITY,0));
			TEST_SEQ4(interval2f(2,GAOL_INFINITY)*interval2f(0,0),interval2f(0,0));
			TEST_SEQ4(interval2f(2,GAOL_INFINITY)*interval2f(0,GAOL_INFINITY),interval2f(0,GAOL_INFINITY));
  }

  void test_ipower() {
			TEST_SEQ4(pow(interval2f(3,4),0),interval2f(1,1));
			TEST_SEQ4(pow(interval2f(-4,-3),0),interval2f(1,1));
			TEST_SEQ4(pow(interval2f(0,0),0),interval2f(1,1));

			TEST_SEQ4(pow(interval2f(-4,-3,-4,-3),2),interval2f(9,16,9,16));
			TEST_SEQ4(pow(interval2f(-4,-3,-4,3),2),interval2f(9,16,0,16));
			TEST_SEQ4(pow(interval2f(-4,-3,-3,4),2),interval2f(9,16,0,16));
			TEST_SEQ4(pow(interval2f(-4,-3,3,4),2),interval2f(9,16,9,16));
			TEST_SEQ4(pow(interval2f(-4,-3,0,0),2),interval2f(9,16,0,0));

			TEST_SEQ4(pow(interval2f(-4,3,-4,-3),2),interval2f(0,16,9,16));
			TEST_SEQ4(pow(interval2f(-4,3,-4,3),2),interval2f(0,16,0,16));
			TEST_SEQ4(pow(interval2f(-4,3,-3,4),2),interval2f(0,16,0,16));
			TEST_SEQ4(pow(interval2f(-4,3,3,4),2),interval2f(0,16,9,16));
			TEST_SEQ4(pow(interval2f(-4,3,0,0),2),interval2f(0,16,0,0));

			TEST_SEQ4(pow(interval2f(-3,4,-4,-3),2),interval2f(0,16,9,16));
			TEST_SEQ4(pow(interval2f(-3,4,-4,3),2),interval2f(0,16,0,16));
			TEST_SEQ4(pow(interval2f(-3,4,-3,4),2),interval2f(0,16,0,16));
			TEST_SEQ4(pow(interval2f(-3,4,3,4),2),interval2f(0,16,9,16));
			TEST_SEQ4(pow(interval2f(-3,4,0,0),2),interval2f(0,16,0,0));

			TEST_SEQ4(pow(interval2f(3,4,-4,-3),2),interval2f(9,16,9,16));
			TEST_SEQ4(pow(interval2f(3,4,-4,3),2),interval2f(9,16,0,16));
			TEST_SEQ4(pow(interval2f(3,4,-3,4),2),interval2f(9,16,0,16));
			TEST_SEQ4(pow(interval2f(3,4,3,4),2),interval2f(9,16,9,16));
			TEST_SEQ4(pow(interval2f(3,4,0,0),2),interval2f(9,16,0,0));

			TEST_SEQ4(pow(interval2f(0,0,-4,-3),2),interval2f(0,0,9,16));
			TEST_SEQ4(pow(interval2f(0,0,-4,3),2),interval2f(0,0,0,16));
			TEST_SEQ4(pow(interval2f(0,0,-3,4),2),interval2f(0,0,0,16));
			TEST_SEQ4(pow(interval2f(0,0,3,4),2),interval2f(0,0,9,16));
			TEST_SEQ4(pow(interval2f(0,0,0,0),2),interval2f(0,0,0,0));


			TEST_SEQ4(pow(interval2f(-4,-3),8),interval2f(6561,65536));
			TEST_SEQ4(pow(interval2f(-4,3),8),interval2f(0,65536));
			TEST_SEQ4(pow(interval2f(-3,4),8),interval2f(0,65536));
			TEST_SEQ4(pow(interval2f(3,4),8),interval2f(6561,65536));
//			TEST_EQ4(pow(interval2f(11,12)/interval2f(10,10),2),interval2f(1.21,1.44));
//			TEST_EQ4(pow(interval2f(-12,-11)/interval2f(10,10),8),interval2f(2.14358881,4.299816959));
//			TEST_EQ4(pow(interval2f(-12,11)/interval2f(10,10),8),interval2f(0,4.299816959));
//			TEST_EQ4(pow(interval2f(-11,12)/interval2f(10,10),8),interval2f(0,4.299816959));
//			TEST_EQ4(pow(interval2f(11,12)/interval2f(10,10),8),interval2f(2.14358881,4.299816959));
//			TEST_CONTAINS4(pow(interval2f(11,12)/interval2f(10,10),2),interval2f(1.21,1.44));
//			TEST_CONTAINS4(pow(interval2f(-12,-11)/interval2f(10,10),8),interval2f(2.14358881,4.299816959));
//			TEST_CONTAINS4(pow(interval2f(-12,11)/interval2f(10,10),8),interval2f(0,4.299816959));
//			TEST_CONTAINS4(pow(interval2f(-11,12)/interval2f(10,10),8),interval2f(0,4.299816959));
//			TEST_CONTAINS4(pow(interval2f(11,12)/interval2f(10,10),8),interval2f(2.14358881,4.299816959));

			TEST_SEQ4(pow(interval2f(-3,-2,-3,-2),3),interval2f(-27,-8,-27,-8));
			TEST_SEQ4(pow(interval2f(-3,-2,-3,2),3),interval2f(-27,-8,-27,8));
			TEST_SEQ4(pow(interval2f(-3,-2,-2,3),3),interval2f(-27,-8,-8,27));
			TEST_SEQ4(pow(interval2f(-3,-2,2,3),3),interval2f(-27,-8,8,27));
			TEST_SEQ4(pow(interval2f(-3,-2,0,0),3),interval2f(-27,-8,0,0));

			TEST_SEQ4(pow(interval2f(-3,2,-3,-2),3),interval2f(-27,8,-27,-8));
			TEST_SEQ4(pow(interval2f(-3,2,-3,2),3),interval2f(-27,8,-27,8));
			TEST_SEQ4(pow(interval2f(-3,2,-2,3),3),interval2f(-27,8,-8,27));
			TEST_SEQ4(pow(interval2f(-3,2,2,3),3),interval2f(-27,8,8,27));
			TEST_SEQ4(pow(interval2f(-3,2,0,0),3),interval2f(-27,8,0,0));

			TEST_SEQ4(pow(interval2f(-2,3,-3,-2),3),interval2f(-8,27,-27,-8));
			TEST_SEQ4(pow(interval2f(-2,3,-3,2),3),interval2f(-8,27,-27,8));
			TEST_SEQ4(pow(interval2f(-2,3,-2,3),3),interval2f(-8,27,-8,27));
			TEST_SEQ4(pow(interval2f(-2,3,2,3),3),interval2f(-8,27,8,27));
			TEST_SEQ4(pow(interval2f(-2,3,0,0),3),interval2f(-8,27,0,0));

			TEST_SEQ4(pow(interval2f(2,3,-3,-2),3),interval2f(8,27,-27,-8));
			TEST_SEQ4(pow(interval2f(2,3,-3,2),3),interval2f(8,27,-27,8));
			TEST_SEQ4(pow(interval2f(2,3,-2,3),3),interval2f(8,27,-8,27));
			TEST_SEQ4(pow(interval2f(2,3,2,3),3),interval2f(8,27,8,27));
			TEST_SEQ4(pow(interval2f(2,3,0,0),3),interval2f(8,27,0,0));

			TEST_SEQ4(pow(interval2f(0,0,-3,-2),3),interval2f(0,0,-27,-8));
			TEST_SEQ4(pow(interval2f(0,0,-3,2),3),interval2f(0,0,-27,8));
			TEST_SEQ4(pow(interval2f(0,0,-2,3),3),interval2f(0,0,-8,27));
			TEST_SEQ4(pow(interval2f(0,0,2,3),3),interval2f(0,0,8,27));
			TEST_SEQ4(pow(interval2f(0,0,0,0),3),interval2f(0,0,0,0));


			TEST_SEQ4(pow(interval2f(-3,-2),11),interval2f(-177147,-2048));
			TEST_SEQ4(pow(interval2f(-3,2),11),interval2f(-177147,2048));
			TEST_SEQ4(pow(interval2f(-2,3),11),interval2f(-2048,177147));
			TEST_SEQ4(pow(interval2f(2,3),11),interval2f(2048,177147));
//			TEST_EQ4(pow(interval2f(-12,-11)/interval2f(10,10),11),interval2f(-7.43008370687, -2.85311670611));
//			TEST_EQ4(pow(interval2f(-12,11)/interval2f(10,10),11),interval2f(-7.43008370687,2.85311670611));
//			TEST_EQ4(pow(interval2f(-11,12)/interval2f(10,10),11),interval2f(-2.85311670611, 7.43008370687));
//			TEST_EQ4(pow(interval2f(11,12)/interval2f(10,10),11),interval2f(2.85311670611, 7.43008370687));
//			TEST_CONTAINS4(pow(interval2f(-12,-11)/interval2f(10,10),11),interval2f(-7.43008370687, -2.85311670611));
//			TEST_CONTAINS4(pow(interval2f(-12,11)/interval2f(10,10),11),interval2f(-7.43008370687,2.85311670611));
//			TEST_CONTAINS4(pow(interval2f(-11,12)/interval2f(10,10),11),interval2f(-2.85311670611, 7.43008370687));
//			TEST_CONTAINS4(pow(interval2f(11,12)/interval2f(10,10),11),interval2f(2.85311670611, 7.43008370687));

  }

  void test_inverse() { // TODO: implement tests for inverse method and function
  }

  // <-- End of tests
};

GAOL_REGISTER_TEST(interval2f_test)
