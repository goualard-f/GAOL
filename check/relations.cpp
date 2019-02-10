#include "tests.h"

class relations_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(relations_test);
  CPPUNIT_TEST(test_set);
  CPPUNIT_TEST(test_certainly);
  CPPUNIT_TEST(test_possibly);
  CPPUNIT_TEST(test_misc);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp() {
  }
  void tearDown() {
  }

  // --> Beginning of tests

  // Tests of logical predicates ===================================================================
  void test_set() {
    TEST_TRUE(interval(4,8).set_contains(interval(4,4)));
    TEST_TRUE(interval(-6,GAOL_INFINITY).set_contains(4.5));
    TEST_TRUE(interval(-6,GAOL_INFINITY).set_contains(GAOL_INFINITY));
    TEST_TRUE(interval::universe().set_contains(interval::emptyset()));
    TEST_TRUE(interval::emptyset().set_contains(interval::emptyset()));
    TEST_TRUE(!interval(4,8).set_strictly_contains(interval(4,4)));
    TEST_TRUE(interval::emptyset().set_strictly_contains(interval::emptyset()));
    TEST_TRUE(interval(3,4).set_disjoint(interval(5,8)));
    TEST_TRUE(interval(-4,-3).set_disjoint(interval(5,8)));
    TEST_TRUE(interval::emptyset().set_disjoint(interval::emptyset()));
    TEST_TRUE(interval::emptyset().set_disjoint(interval(3,4)));
    TEST_TRUE(interval(-3,5).set_disjoint(interval::emptyset()));
    TEST_TRUE(interval::universe().set_eq(interval::universe()));
    TEST_TRUE(interval::emptyset().set_eq(interval::emptyset()));
    TEST_TRUE(interval::emptyset().set_neq(interval::universe()));
    TEST_TRUE(interval::universe().set_neq(interval::emptyset()));
    TEST_TRUE(interval(4,5).set_le(interval(-3,12)));
    TEST_TRUE(interval::emptyset().set_le(interval(3.0)));
    TEST_FALSE(interval::emptyset().set_le(interval::emptyset()));
    TEST_FALSE(interval(-6,4).set_le(interval(3,7)));
    TEST_TRUE(interval(4.5,6).set_leq(interval(4.5,6)));
    TEST_FALSE(interval(3.5,9).set_leq(interval(2,6)));
    TEST_TRUE(interval::emptyset().set_leq(interval::emptyset()));
}
    void test_certainly() {
      TEST_FALSE(interval(3,4).certainly_eq(interval(3,4)));
      TEST_TRUE(interval(-6).certainly_eq(interval(-6,-6)));
      TEST_FALSE(interval::universe().certainly_eq(interval::universe()));
      TEST_TRUE(interval::emptyset().certainly_eq(interval::emptyset()));
      TEST_FALSE(interval::emptyset().certainly_eq(interval(3,4)));
      TEST_FALSE(interval(3,4).certainly_eq(interval::emptyset()));

      TEST_TRUE(interval(4,5).certainly_le(interval(6,9)));
      TEST_FALSE(interval(4,5).certainly_le(interval(5,9)));
      TEST_TRUE(interval::emptyset().certainly_le(interval(4,6)));
      TEST_TRUE(interval::emptyset().certainly_le(interval::emptyset()));

      TEST_TRUE(interval(4,5).certainly_leq(interval(6,9)));
      TEST_TRUE(interval(4,5).certainly_leq(interval(5,9)));
      TEST_FALSE(interval(5,9).certainly_leq(interval(4,5)));
      TEST_FALSE(interval(4,8).certainly_leq(interval(5,9)));
      TEST_TRUE(interval::emptyset().certainly_leq(interval(4,6)));

      TEST_TRUE(interval(8,10).certainly_geq(interval(4,8)));
      TEST_TRUE(interval(9,10).certainly_geq(interval(4,8)));
      TEST_TRUE(interval::emptyset().certainly_geq(interval::emptyset()));
      TEST_FALSE(interval::emptyset().certainly_geq(interval(3,5)));
      TEST_TRUE(interval(4,8).certainly_geq(interval::emptyset()));

      TEST_FALSE(interval(8,10).certainly_ge(interval(4,8)));
      TEST_TRUE(interval(9,10).certainly_ge(interval(4,8)));
      TEST_TRUE(interval::emptyset().certainly_ge(interval::emptyset()));
      TEST_FALSE(interval::emptyset().certainly_ge(interval(3,5)));
      TEST_TRUE(interval(4,8).certainly_ge(interval::emptyset()));

      TEST_TRUE(interval::emptyset().certainly_positive());
      TEST_FALSE(interval::universe().certainly_positive());
      TEST_TRUE(interval(0,5).certainly_positive());
      TEST_FALSE(interval(-5,0).certainly_positive());
      TEST_TRUE(interval::zero().certainly_positive());
      TEST_TRUE(interval(-0.0,0.0).certainly_positive());

      TEST_TRUE(interval::emptyset().certainly_strictly_positive());
      TEST_FALSE(interval::universe().certainly_strictly_positive());
      TEST_FALSE(interval(0,5).certainly_strictly_positive());
      TEST_FALSE(interval(-5,0).certainly_strictly_positive());
      TEST_FALSE(interval::zero().certainly_strictly_positive());
      TEST_FALSE(interval(-0.0,0.0).certainly_strictly_positive());
      TEST_TRUE(interval(4,5).certainly_strictly_positive());

      TEST_TRUE(interval::emptyset().certainly_negative());
      TEST_FALSE(interval::universe().certainly_negative());
      TEST_TRUE(interval(-5,-3).certainly_negative());
      TEST_TRUE(interval(-3,+0.0).certainly_negative());
      TEST_FALSE(interval(-4,5).certainly_negative());

      TEST_TRUE(interval::emptyset().certainly_strictly_negative());
      TEST_FALSE(interval::universe().certainly_strictly_negative());
      TEST_TRUE(interval(-5,-3).certainly_strictly_negative());
      TEST_FALSE(interval(-3,+0.0).certainly_strictly_negative());
      TEST_FALSE(interval(-4,5).certainly_strictly_negative());
 }

  void test_possibly() {
      TEST_TRUE(interval(4,5).possibly_eq(interval(4.5,6)));
      TEST_FALSE(interval(4,5).possibly_eq(interval::emptyset()));
      TEST_FALSE(interval::emptyset().possibly_eq(interval::emptyset()));

      TEST_TRUE(interval(4,5).possibly_neq(interval(4,6)));
      TEST_FALSE(interval(4,4).possibly_neq(interval(4,4)));
      TEST_FALSE(interval::emptyset().possibly_neq(interval::emptyset()));
      TEST_FALSE(interval::emptyset().possibly_neq(interval(4,5)));
      TEST_FALSE(interval(4,5).possibly_neq(interval::emptyset()));

      TEST_TRUE(interval(4,5).possibly_leq(interval(3,7)));
      TEST_FALSE(interval(4,5).possibly_leq(interval(2,3)));
      TEST_TRUE(interval(4,5).possibly_leq(interval(2,4)));
      TEST_FALSE(interval::emptyset().possibly_leq(interval::emptyset()));
      TEST_FALSE(interval::emptyset().possibly_leq(interval(3,4)));
      TEST_FALSE(interval(4,5).possibly_leq(interval::emptyset()));

      TEST_TRUE(interval(4,5).possibly_le(interval(3,7)));
      TEST_FALSE(interval(4,5).possibly_le(interval(2,3)));
      TEST_FALSE(interval(4,5).possibly_le(interval(2,4)));
      TEST_FALSE(interval::emptyset().possibly_le(interval::emptyset()));
      TEST_FALSE(interval::emptyset().possibly_le(interval(3,4)));
      TEST_FALSE(interval(4,5).possibly_le(interval::emptyset()));

      TEST_TRUE(interval(4,5).possibly_geq(interval(3,6)));
      TEST_TRUE(interval(4,5).possibly_geq(interval(5,6)));
      TEST_FALSE(interval(4,5).possibly_geq(interval(6,7)));
      TEST_FALSE(interval::emptyset().possibly_geq(interval::emptyset()));
      TEST_FALSE(interval::emptyset().possibly_geq(interval(3,4)));
      TEST_FALSE(interval(4,5).possibly_geq(interval::emptyset()));

      TEST_TRUE(interval(4,5).possibly_ge(interval(3,6)));
      TEST_FALSE(interval(4,5).possibly_ge(interval(5,6)));
      TEST_FALSE(interval(4,5).possibly_ge(interval(6,7)));
      TEST_FALSE(interval::emptyset().possibly_ge(interval::emptyset()));
      TEST_FALSE(interval::emptyset().possibly_ge(interval(3,4)));
      TEST_FALSE(interval(4,5).possibly_ge(interval::emptyset()));
  }

  void test_misc() {
    // straddles_zero()
    TEST_TRUE(interval(-4,5).straddles_zero());
    TEST_TRUE(!interval(-4,-3).straddles_zero());
    TEST_TRUE(!interval(3,5).straddles_zero());
    TEST_TRUE(interval::universe().straddles_zero());
    TEST_TRUE(!interval::emptyset().straddles_zero());
    TEST_TRUE(interval(-3,0).straddles_zero());
    TEST_TRUE(interval(0,3).straddles_zero());


    // strictly_straddles_zero()
    TEST_TRUE(interval(-4,5).strictly_straddles_zero());
    TEST_TRUE(!interval(-4,-3).strictly_straddles_zero());
    TEST_TRUE(!interval(3,5).strictly_straddles_zero());
    TEST_TRUE(interval::universe().strictly_straddles_zero());
    TEST_TRUE(!interval::emptyset().strictly_straddles_zero());
    TEST_TRUE(!interval(-3,0).strictly_straddles_zero());
    TEST_TRUE(!interval(0,3).strictly_straddles_zero());

    // is_a_double()
    TEST_TRUE(interval(3.0,3.0).is_a_double());
    TEST_TRUE(!interval(3.0,4.0).is_a_double());
    TEST_TRUE(!interval::emptyset().is_a_double());

    // is_an_int()
    TEST_TRUE(interval(3.0,3.0).is_an_int());
    TEST_TRUE(!interval(3.5,3.5).is_an_int());
    TEST_TRUE(!interval::emptyset().is_an_int());
    TEST_TRUE(!interval(1.0e100,1.0e100).is_an_int());

    // is_canonical()
    TEST_TRUE(!interval::emptyset().is_canonical());
    TEST_TRUE(interval("1/10.0").is_canonical());
	TEST_TRUE(interval(3,next_float(3)).is_canonical());
	TEST_TRUE(interval(previous_float(-3),-3).is_canonical());
    // is_empty()
    TEST_TRUE(interval::emptyset().is_empty());
    TEST_FALSE(interval::universe().is_empty());
    TEST_FALSE(interval("1.0/10").is_empty());
    TEST_FALSE(interval(4,5).is_empty());
    TEST_TRUE(interval(5,4).is_empty());
	TEST_FALSE(interval(-4,5).is_empty());
	TEST_TRUE(interval(std::numeric_limits<double>::quiet_NaN(),4).is_empty());
	TEST_TRUE(interval(-6,std::numeric_limits<double>::quiet_NaN()).is_empty());
	TEST_TRUE(interval(-std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::quiet_NaN()).is_empty());


    // is_zero()
    TEST_TRUE(interval::zero().is_zero());
    TEST_TRUE(interval(0.0,0.0).is_zero());
    TEST_TRUE(interval(-0.0,+0.0).is_zero());
    TEST_FALSE(interval(0,5).is_zero());
    TEST_FALSE(interval::emptyset().is_zero());

    // is_symmetric()
    TEST_FALSE(interval::emptyset().is_symmetric());
    TEST_TRUE(interval::universe().is_symmetric());
    TEST_TRUE(interval::zero().is_symmetric());
    TEST_TRUE(interval(-5,5).is_symmetric());
    TEST_FALSE(interval(-5,4).is_symmetric());

    // is_finite()
    TEST_TRUE(interval(4,5).is_finite());
    TEST_FALSE(interval(3,GAOL_INFINITY).is_finite());
    TEST_FALSE(interval::universe().is_finite());
    TEST_TRUE(interval::emptyset().is_finite());
  }

  // <-- End of tests
};


GAOL_REGISTER_TEST(relations_test)
