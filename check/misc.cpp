#include "tests.h"

class misc_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(misc_test);
  CPPUNIT_TEST(test_constants);
  CPPUNIT_TEST(test_predicates);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp() {
  }
  void tearDown() {
  }

  // --> Beginning of tests

  // Tests of logical predicates ===================================================================
  void test_predicates() {
    // is_empty()
    CPPUNIT_ASSERT(interval::emptyset().is_empty());
    CPPUNIT_ASSERT(interval(1.0,-1.0).is_empty());

    // is_symmetric()
    CPPUNIT_ASSERT(!interval::emptyset().is_symmetric());
    CPPUNIT_ASSERT(interval::universe().is_symmetric());
    CPPUNIT_ASSERT(interval(-6.0,6.0).is_symmetric());
    CPPUNIT_ASSERT(!interval(-6.0,-5.0).is_symmetric());
    CPPUNIT_ASSERT(!interval(5.0,6.0).is_symmetric());

  }

  // Tests of constants
  void test_constants() {
    CPPUNIT_ASSERT(interval::emptyset().is_empty());
    TEST_PEQ(interval::universe(),interval(-GAOL_INFINITY,+GAOL_INFINITY));
    TEST_SEQ(interval::zero(),interval(0.0,0.0));
    TEST_SEQ(interval::one(),interval(1.0,1.0));
    TEST_SEQ(interval::positive(),interval(0.0,+GAOL_INFINITY));
    TEST_SEQ(interval::negative(),interval(-GAOL_INFINITY,0.0));
    TEST_SEQ(interval::minus_one_plus_one(),interval(-1.0,1.0));
    TEST_PEQ(interval::pi(),interval(3.14159,3.1416));
    TEST_PEQ(interval::two_pi(),interval(6.2831,6.2832));
    TEST_PEQ(interval::half_pi(),interval(1.5707,1.5708));
    TEST_SEQ(interval::one_plus_infinity(),interval(1.0,+GAOL_INFINITY));
  }


  // <-- End of tests
};

GAOL_REGISTER_TEST(misc_test)
