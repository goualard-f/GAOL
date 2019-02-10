#include <iostream>
#include "tests.h"

class trigonometric_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(trigonometric_test);
  CPPUNIT_TEST(test_cos);
  CPPUNIT_TEST(test_sin);
  CPPUNIT_TEST(test_tan);
  CPPUNIT_TEST(test_acos);
  CPPUNIT_TEST(test_asin);
  CPPUNIT_TEST(test_atan);
  CPPUNIT_TEST(test_cosh);
  CPPUNIT_TEST(test_sinh);
  CPPUNIT_TEST(test_tanh);
  CPPUNIT_TEST(test_acosh);
  CPPUNIT_TEST(test_asinh);
  CPPUNIT_TEST(test_atanh);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp() {
  }
  void tearDown() {
  }

  // --> Beginning of tests
  void test_cos() {
    TEST_EMPTY(cos(interval::emptyset()));

 	TEST_PEQ(cos(interval(-1,-0.5)),interval("[0.540302305868139, 0.877582561890373]"));
    TEST_SEQ(cos(interval(-10,10)),interval(-1,1));
    TEST_SEQ(cos(interval::universe()),interval(-1,1));
    TEST_EQ(cos(interval::pi()),interval(-1,-1));

	TEST_EQ(cos(interval(3,3.5)),interval("[-1,-0.936456687290796]"));
	TEST_EQ(cos(interval(-3.5,-3)),interval("[-1,-0.936456687290796]"));
	TEST_EQ(cos(interval(-3.5,3)),interval(-1,1));
	TEST_EQ(cos(interval(10,12)),interval("[-0.839071529076452,0.843853958732493]"));
	TEST_EQ(cos(interval(13,14)),interval("[0.136737218207833,0.907446781450197]"));
	TEST_EQ(cos(interval(10,14)),interval("[-0.839071529076452,1]"));
	TEST_EQ(cos(interval(14,16)),interval("[-1,0.136737218207833]"));
	TEST_EQ(cos(interval(-11,-10)),interval("[-0.839071529076452,0.004425697988051]"));
	TEST_EQ(cos(interval(-14,-13)),interval("[0.136737218207833,0.907446781450197]"));
	TEST_EQ(cos(interval(-16,-14)),interval("[-1,0.136737218207833]"));
	TEST_EQ(cos(interval(-102,-100)),interval("[0.101585703696621,1]"));
	TEST_EQ(cos(interval(4.6e15,4.7e15)),interval(-1,1));
	TEST_EQ(cos(interval(4503599627370495,4503599627370496)),interval("[-0.48553486774222065, 0.4732928859543091]"));
  }

  void test_sin() {
	// No test because it uses cos()
  }
  void test_tan() {
	TEST_EMPTY(tan(interval::emptyset()));
 	TEST_SEQ(tan(interval::universe()),interval::universe());
	TEST_EQ(tan(interval(0,1)),interval(0,1.55740772465490223051));
	TEST_EQ(tan(interval(-1,0)),interval(-1.55740772465490223051,0));
	TEST_EQ(tan(interval(-2,-1)),interval::universe());
	TEST_EQ(tan(interval(202,203)),interval::universe());
  }

  void test_acos() {
	TEST_EMPTY(acos(interval::emptyset()));
	TEST_EMPTY(acos(interval(-6,-3)));
	TEST_EMPTY(acos(interval(3,5)));
	TEST_EQ(acos(interval(-1.5,-.5)),interval(2.09439510239319549231,3.14159265358979323846));
	TEST_EQ(acos(interval(-0.5,0.5)),interval(1.04719755119659774615,2.09439510239319549231));
	TEST_EQ(acos(interval("0.1")),interval("1.47062890563333682289"));
  }
  void test_asin() {
	TEST_EMPTY(asin(interval::emptyset()));
	TEST_EMPTY(asin(interval(-6,-3)));
	TEST_EMPTY(asin(interval(3,5)));
	TEST_EQ(asin(interval(-1.5,-.5)),interval(-1.57079632679489661923,-0.52359877559829887308));
	TEST_EQ(asin(interval(-0.5,0.5)),interval(-0.52359877559829887308,0.52359877559829887308));
	TEST_EQ(asin(interval("0.1")),interval("0.1001674211615"));
  }
  void test_atan() {
	TEST_EMPTY(atan(interval::emptyset()));
	TEST_EQ(atan(max_inf),interval("1.57079632679489661923"));
 	TEST_EQ(atan(m_inf_m_max),interval("-1.57079632679489661923"));
	TEST_EQ(atan(interval(-10,3)),interval(-1.47112767430373459185,1.24904577239825442583));
  }

  void test_cosh() {
	TEST_EMPTY(cosh(interval::emptyset()));
	TEST_SEQ(cosh(max_inf),max_inf);
	TEST_SEQ(cosh(m_inf_m_max),max_inf);
	TEST_EQ(cosh(interval(-5,-3)),interval("[10.06766199577776584195,74.20994852478784444411]"));
	TEST_EQ(cosh(interval(-3,5)),interval(1.0,74.20994852478784444411));
	TEST_EQ(cosh(interval(-5,3)),interval(1.0,74.20994852478784444411));
	TEST_EQ(cosh(interval(3,5)),interval("[10.06766199577776584195,74.20994852478784444411]"));
  }
  void test_sinh() {
	TEST_EMPTY(sinh(interval::emptyset()));
	TEST_SEQ(sinh(m_inf_m_max),m_inf_m_max);
	TEST_SEQ(sinh(max_inf),max_inf);
	TEST_EQ(sinh(interval(-3,5)),interval(-10.01787492740990189897,74.20321057778875897701));
	TEST_EQ(sinh(interval::universe()),interval::universe());
  }

  void test_tanh() {
	TEST_EMPTY(tanh(interval::emptyset()));
	TEST_EQ(tanh(m_inf_m_max),interval(-1,next_float(-1)));
	TEST_SEQ(tanh(max_inf),interval(previous_float(1),1));
	TEST_EQ(tanh(interval::universe()),interval::minus_one_plus_one());
	TEST_EQ(tanh(interval(-3,5)),interval(-0.99505475368673045133,0.99990920426259513121));
	TEST_EQ(tanh(interval(-1,.5)),interval(-0.76159415595576488812,0.4621171572600097585));
  }

  void test_acosh() {
	TEST_EMPTY(acosh(interval::emptyset()));
	TEST_EMPTY(acosh(m_inf_m_max));
	TEST_CONT(interval(700,GAOL_INFINITY),acosh(max_inf));
	TEST_EQ(acosh(interval(0,1)),interval::zero());
	TEST_EQ(acosh(interval(2,4)),interval(1.31695789692481670863,2.06343706889556054673));
	TEST_EQ(acosh(interval("100.1")),interval("5.29929191587240895417"));

  }
  void test_asinh() {
	TEST_EMPTY(asinh(interval::emptyset()));
	TEST_SEQ(asinh(interval::universe()),interval::universe());
	TEST_CONT(interval(-GAOL_INFINITY,-700),asinh(m_inf_m_max));
	TEST_CONT(interval(700,GAOL_INFINITY),asinh(max_inf));
	TEST_EQ(asinh(interval(-3,2)),interval(-1.81844645923206682348,1.44363547517881034249));
  }

  void test_atanh() {
	TEST_EMPTY(atanh(interval::emptyset()));
	TEST_EMPTY(atanh(m_inf_m_max));
	TEST_EMPTY(atanh(max_inf));
	TEST_SEQ(atanh(interval::universe()),interval::universe());
	interval tmp = atanh(interval(-2,-.5));
	CPPUNIT_ASSERT(tmp.left() == -GAOL_INFINITY && fabs(tmp.right()+0.5493061443340548457)<=1e-8);
 	tmp = atanh(interval(0,3));
	CPPUNIT_ASSERT(fabs(tmp.left())<=1e-8 && tmp.right()==GAOL_INFINITY);
	TEST_EQ(atanh(interval(-0.5,0.25)),interval(-0.5493061443340548457,0.2554128118829953416));
  }

  // <-- End of tests
};

GAOL_REGISTER_TEST(trigonometric_test)
