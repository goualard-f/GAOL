// $Id: reverse_mappings.cpp 191 2012-03-06 17:08:58Z goualard $
#include "tests.h"

/*
  Define this to 1 if the random generators should be initialized with the PID
  WARNING: if '1', the tests performed will vary from one invocation to the other
 */
#define RAND_INIT_PID 1

using std::cout;
using std::endl;

class reverse_mappings_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(reverse_mappings_test);
  CPPUNIT_TEST(test_acos_rel);
  CPPUNIT_TEST(test_asin_rel);
  CPPUNIT_TEST(test_atan_rel);
  CPPUNIT_TEST(test_sqrt_rel);
  CPPUNIT_TEST(test_nth_root_rel);
  CPPUNIT_TEST(test_invabs_rel);
  CPPUNIT_TEST(test_div_rel);
  CPPUNIT_TEST(test_acosh_rel);
  CPPUNIT_TEST(test_asinh_rel);
  CPPUNIT_TEST(test_atanh_rel);
  CPPUNIT_TEST_SUITE_END();
	static const unsigned int MAX = 100;
	//static const unsigned int MAX = 100000;
  public:
  void setUp() {
#if RAND_INIT_PID
	 srand48(getpid());
	 srand(getpid());
#else
    // MAgic value 10 was chosen "at random"
	 srand48(10);
	 srand(10);
#endif
  }
  void tearDown() {
  }

   // --> Beginning of tests
   void test_acos_rel() {
	  TEST_EMPTY(acos_rel(interval::emptyset(),interval::emptyset()));
	  TEST_EMPTY(acos_rel(interval(0,2),interval::emptyset()));
	  TEST_EMPTY(acos_rel(interval::universe(),interval::emptyset()));
	  TEST_EMPTY(acos_rel(interval::emptyset(),interval(0,2)));
	  TEST_EMPTY(acos_rel(interval::emptyset(),interval::universe()));

	  interval I, J;
     TEST_SEQ(acos_rel(interval::minus_one_plus_one(),interval(-5,6)),interval(-5,6));
	  TEST_EMPTY(acos_rel(interval(-3,-2),interval::universe()));
	  TEST_EMPTY(acos_rel(interval(2,4),interval::universe()));
	  I = interval(previous_float(two_power_52),two_power_52);
	  J = cos(I);
	  TEST_EQ(acos_rel(J,I),I);
	  I = interval(previous_float(previous_float(two_power_52)),previous_float(two_power_52));
	  J = cos(I);
	  TEST_EQ(acos_rel(J,I),I); // TODO: pb here under AARCH64. Investigate!
	  TEST_EQ(acos_rel(interval(.25,.5),interval("[1.02,1.32]")),interval("[1.047197551196,1.31811607165282]"));
	  I = interval(-1e8,next_float(-1e8));
	  J = cos(I);
	  TEST_EQ(acos_rel(J,I),I);
	  I = interval(previous_float(1e8),1e8);
	  J = cos(I);
	  TEST_EQ(acos_rel(J,I),I);
	  I = interval::pi();
	  J = cos(I);
	  TEST_EQ(acos_rel(J,I),I);
	  I = interval::pi();
	  J = cos(I);
	  TEST_EQ(acos_rel(J,I),I);
	  for (int i = 10; i <= 10000; i*=10) {
        I = interval(0.5,0.7)+i*interval::pi();
	  	  J = cos(I);
	  	  TEST_EQ(acos_rel(J,I),I);
	  }
	  I = interval(-6.3,6.3);
	  J = interval(0.5,0.75);
	  TEST_EQ(acos_rel(J,I),interval("[-5.560451059366172, 5.560451059366172]"));
	  I = interval(-2,6.3);
	  J = interval(0.5,0.75);
	  TEST_EQ(acos_rel(J,I),interval("[-1.047197551196598, 5.560451059366172]"));
	  I = interval(-6.3,2);
	  J = interval(0.5,0.75);
	  TEST_EQ(acos_rel(J,I),interval("[-5.560451059366172, 1.047197551196598]"));
	  I = interval(-0.32,0.2);
	  J = interval(0.9,0.95);
	  TEST_EQ(acos_rel(J,I),interval("[-0.32,-0.3175604292]"));
	  I = interval(-interval::pi().left(),interval::pi().left());
	  J = interval(0.5,0.75);
	  TEST_EQ(acos_rel(interval::universe(),interval(-3,2)),interval(-3,2));
	  TEST_EQ(acos_rel(interval::universe(),interval(0.5,0.6)),interval(0.5,0.6));
	}

  void test_asin_rel() {
  }

  void test_atan_rel() {
	 TEST_EMPTY(atan_rel(interval::emptyset(),interval::emptyset()));
	 TEST_EMPTY(atan_rel(interval::emptyset(),interval(-3,2)));
	 TEST_EMPTY(atan_rel(interval(-3,2),interval::emptyset()));
	 interval I = interval(4.0,6.0);
	 interval J = tan(I);
	 TEST_EQ(atan_rel(J,I),I);

	 interval TI[MAX];
	 for (unsigned int i = 0; i < MAX; ++i) {
		double Il = drand48()*(rand() % 1000000);
		double Ir = drand48()*(rand() % 1000000);
		reorder(Il,Ir);
		TI[i] = interval(Il,Ir);
	 }
	 for (unsigned int i = 0; i < MAX; ++i) {
	   interval J = tan(TI[i]);
		TEST_EQ(atan_rel(J,TI[i]),TI[i]);
	 }
	 TEST_EQ(atan_rel(interval(-5,5),interval::universe()),interval::universe());
	 TEST_EQ(atan_rel(interval::universe(),interval(4.0,5.5)),interval(4.0,5.5));
	 TEST_EQ(atan_rel(interval::universe(),interval(4.0,6.5)),interval(4.0,6.5));
	 TEST_EQ(atan_rel(interval(.5,.6),interval(3.7,7)),interval("[6.746832916180391, 6.823604807450172]"));
	 TEST_EQ(atan_rel(interval(.5,.6),interval(3.5,7)),interval("[3.605240262590599, 6.823604807450172]"));
	 TEST_EQ(atan_rel(interval(.5,.6),interval(3.62,7)),interval("[3.62, 6.823604807450172]"));
	 TEST_EQ(atan_rel(interval(.5,.6),interval(-7,-3.62)),interval("[-5.819537698178782, -5.742765806909001]"));
  }

  void test_sqrt_rel() {
  }

  void test_nth_root_rel() {
	TEST_EMPTY(nth_root_rel(interval(-3,-2),2,interval::universe()));
	TEST_EQ(nth_root_rel(interval(-3,2),2,interval::universe()),interval("[-1.41421356237,1.41421356237]"));
	TEST_EQ(nth_root_rel(interval(2,3),2,interval::universe()),interval("[-1.73205080756,1.73205080756]"));
	TEST_EQ(nth_root_rel(interval(2,3),2,interval(-2,-1.5)),interval("[-1.73205080756,-1.5]"));
	TEST_EQ(nth_root_rel(interval(2,3),2,interval(1.5,2)),interval("[1.5,1.73205080756]"));

	TEST_EQ(nth_root_rel(interval(2,3),5,interval::universe()),interval("[1.14869835,1.245730939]"));
	TEST_EQ(nth_root_rel(interval(0.5,3),5,interval::universe()),interval("[0.87055056,1.245730939]"));
	TEST_EQ(nth_root_rel(interval(-.2,3),5,interval::universe()),interval("[-0.72477966,1.245730939]"));
	TEST_EQ(nth_root_rel(interval(-3,-2),5,interval::universe()),interval("[-1.245730939,-1.14869835]"));

	TEST_EQ(nth_root_rel(interval(0.2,0.5),5,interval::universe()),interval("[0.72477966,0.87055056]"));
	TEST_EQ(nth_root_rel(interval(-3,-.2),5,interval::universe()),interval("[-1.245730939,-0.72477966]"));
	TEST_EQ(nth_root_rel(interval(-3,-2),5,interval::universe()),interval("[-1.245730939,-1.14869835]"));

  }

  void test_invabs_rel() {
	TEST_EMPTY(invabs_rel(interval::emptyset(),interval::emptyset()));
	TEST_EMPTY(invabs_rel(interval::emptyset(),interval::universe()));
	TEST_EMPTY(invabs_rel(interval::universe(),interval::emptyset()));
	TEST_SEQ(invabs_rel(interval::universe(),interval::universe()),interval::universe());
	TEST_SEQ(invabs_rel(interval(2,3),interval(1,2.5)),interval(2,2.5));
	TEST_SEQ(invabs_rel(interval(2,3),interval(-2.5,-1)),interval(-2.5,-2));
	TEST_SEQ(invabs_rel(interval(2,3),interval(-2.5,2.75)),interval(-2.5,2.75));
	TEST_SEQ(invabs_rel(interval(2,3),interval(-10,1)),interval(-3,-2));
	TEST_SEQ(invabs_rel(interval(-3,4),interval(2,3)),interval(2,3));
	TEST_SEQ(invabs_rel(interval(-3,4),interval(-3,-2)),interval(-3,-2));
  }

  void test_div_rel() {
	 interval TI[MAX], TJ[MAX];
	 for (unsigned int i = 0; i < MAX; ++i) {
		double Il = drand48()*(rand() % 1000000);
		double Ir = drand48()*(rand() % 1000000);
		reorder(Il,Ir);
		TI[i] = interval(Il,Ir);
	 }
	 for (unsigned int i = 0; i < MAX; ++i) {
		double Il = drand48()*(rand() % 1000000);
		double Ir = drand48()*(rand() % 1000000);
		reorder(Il,Ir);
		TJ[i] = interval(Il,Ir);
	 }
	 for (unsigned int i = 0; i < MAX; ++i) {
	   interval K = TI[i]*TJ[i];
		TEST_EQ(div_rel(K,TJ[i],TI[i]),TI[i]);
	 }
	
	TEST_EQ(div_rel(interval(-6, 0), interval(3.0), interval(-2,0)),interval(-2,0));
	TEST_EQ(div_rel(interval(-4.7, -1.5), interval(3.0), interval(-2, 0)),interval(-1.5666666666666669, -0.5));
	TEST_EQ(div_rel(interval(-1.5, 0), interval(3.0), interval(-2, 0)),interval(-0.5, 0));
	TEST_EQ(div_rel(interval(-3, 0), interval(3.0), interval(-2, 0)),interval(-1.0,0.0));

  }

  void test_acosh_rel() {
	 TEST_EMPTY(acosh_rel(interval::emptyset(),interval::emptyset()));
	 TEST_EMPTY(acosh_rel(interval(0,2),interval::emptyset()));
	 TEST_EMPTY(acosh_rel(interval::universe(),interval::emptyset()));
	 TEST_EMPTY(acosh_rel(interval::emptyset(),interval(0,2)));
	 TEST_EMPTY(acosh_rel(interval::emptyset(),interval::universe()));

	 interval TI[MAX];
	 for (unsigned int i = 0; i < MAX; ++i) {
		double Il = drand48()*(rand() % 1000000);
		double Ir = drand48()*(rand() % 1000000);
		reorder(Il,Ir);
		TI[i] = interval(Il,Ir);
	 }
	 for (unsigned int i = 0; i < MAX; ++i) {
	   interval J = cosh(TI[i]);
		TEST_EQ(acosh_rel(J,TI[i]),TI[i]);
	 }
  }

  void test_asinh_rel() {
	 TEST_EMPTY(asinh_rel(interval::emptyset(),interval::emptyset()));
	 TEST_EMPTY(asinh_rel(interval(0,2),interval::emptyset()));
	 TEST_EMPTY(asinh_rel(interval::universe(),interval::emptyset()));
	 TEST_EMPTY(asinh_rel(interval::emptyset(),interval(0,2)));
	 TEST_EMPTY(asinh_rel(interval::emptyset(),interval::universe()));

	 interval TI[MAX];
	 for (unsigned int i = 0; i < MAX; ++i) {
		double Il = drand48()*(rand() % 1000000);
		double Ir = drand48()*(rand() % 1000000);
		reorder(Il,Ir);
		TI[i] = interval(Il,Ir);
	 }
	 for (unsigned int i = 0; i < MAX; ++i) {
	   interval J = sinh(TI[i]);
		TEST_EQ(asinh_rel(J,TI[i]),TI[i]);
	 }

  }

  void test_atanh_rel() {
	 TEST_EMPTY(atanh_rel(interval::emptyset(),interval::emptyset()));
	 TEST_EMPTY(atanh_rel(interval(0,2),interval::emptyset()));
	 TEST_EMPTY(atanh_rel(interval::universe(),interval::emptyset()));
	 TEST_EMPTY(atanh_rel(interval::emptyset(),interval(0,2)));
	 TEST_EMPTY(atanh_rel(interval::emptyset(),interval::universe()));

	 interval TI[MAX];
	 for (unsigned int i = 0; i < MAX; ++i) {
		double Il = drand48()*(rand() % 1000000);
		double Ir = drand48()*(rand() % 1000000);
		reorder(Il,Ir);
		TI[i] = interval(Il,Ir);
	 }
	 for (unsigned int i = 0; i < MAX; ++i) {
	   interval J = tanh(TI[i]);
		TEST_EQ(atanh_rel(J,TI[i]),TI[i]);
	 }

  }
  // <-- End of tests
  protected:
  void reorder(double & l, double& r)
  {
	 if (l > r) {
		double tmp = l;
		l = r;
		r = tmp;
	 }
  }
};

GAOL_REGISTER_TEST(reverse_mappings_test)
