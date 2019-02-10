#include "tests.h"

class constants_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(constants_test);
  CPPUNIT_TEST(test_interval_const);
  CPPUNIT_TEST(test_floating_point_const);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp() {
  }
  void tearDown() {
  }

  // --> Beginning of tests

  // Tests of logical predicates ===================================================================
  void test_interval_const() {
    TEST_EQ(interval::two_pi(),interval("[6.283185307179586, 6.283185307179587]"));
    TEST_EQ(interval::pi(),interval("[3.141592653589793, 3.141592653589794]"));
    TEST_EQ(interval::half_pi(),interval("[1.570796326794897, 1.570796326794897]"));
    TEST_SEQ(interval::positive(),interval(0,GAOL_INFINITY));
    TEST_SEQ(interval::negative(),interval(-GAOL_INFINITY,0));
    TEST_SEQ(interval::minus_one_plus_one(),interval(-1.0,1.0));
    TEST_SEQ(interval::one_plus_infinity(),interval(1.0,GAOL_INFINITY));
    TEST_SEQ(interval::universe(),interval(-GAOL_INFINITY,+GAOL_INFINITY));
    TEST_SEQ(interval::zero(),interval(0.0,0.0));
    TEST_SEQ(interval::one(),interval(1.0,1.0));
    TEST_EMPTY(interval::emptyset());
  }

  void test_floating_point_const() {
      TEST_EQ(two_pi,interval::two_pi());
      TEST_EQ(pi,interval::pi());
      TEST_EQ(half_pi,interval::half_pi());
      TEST_EQ(pi_dn,interval::pi());
      TEST_EQ(pi_up,interval::pi());
      CPPUNIT_ASSERT(pi_dn < pi_up);
      TEST_EQ(half_pi_dn,interval::half_pi());
      TEST_EQ(half_pi_up,interval::half_pi());
      CPPUNIT_ASSERT(half_pi_dn < half_pi_up);
      TEST_EQ(ln2_dn,log(interval(2)));
      TEST_EQ(ln2_up,log(interval(2)));
      CPPUNIT_ASSERT(ln2_dn < ln2_up);
      TEST_EQ(two_power_52,pow(interval(2),52));
		TEST_EQ(two_power_51,pow(interval(2),51));
		CPPUNIT_ASSERT(std::isnan(GAOL_NAN));
      CPPUNIT_ASSERT(std::isinf(GAOL_INFINITY));
      CPPUNIT_ASSERT(GAOL_INFINITY > 0);
  }
  // <-- End of tests
};


GAOL_REGISTER_TEST(constants_test)
