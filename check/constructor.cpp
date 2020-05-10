#include "tests.h"

#define TEST_INOUT_EQ(Istr,Ires)  \
  try {                           \
       interval I(Istr);          \
       TEST_EQ(I,Ires);           \
  } catch (input_format_error) {  \
    CPPUNIT_FAIL(string("Wrong format: ")+string(Istr)); \
  }

#define TEST_INOUT_SEQ(Istr,Ires) \
  try {                           \
       interval I(Istr);          \
       TEST_SEQ(I,Ires);          \
  } catch (input_format_error) {  \
    CPPUNIT_FAIL(string("Wrong format: ")+string(Istr)); \
  }


class constructor_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(constructor_test);
  CPPUNIT_TEST(test_constructor_string);
  CPPUNIT_TEST(test_constructor_numbers);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp() {
  }
  void tearDown() {
  }

  // --> Beginning of tests
  void test_constructor_numbers() {
      interval a;
      TEST_SEQ(a,interval::universe());
      interval b(-4,6);
      CPPUNIT_ASSERT(b.left()==-4 && b.right() == 6);
      interval c(6.5);
      CPPUNIT_ASSERT(c.left() == c.right() && c.left() == 6.5);
      interval d(b);
      CPPUNIT_ASSERT(d.left()==-4 && d.right() == 6);
  }

  void test_constructor_string() {
    interval a("empty"), b("[empty]");
		interval c("5.80258497501207e-14");
		CPPUNIT_ASSERT(!c.is_empty());
    TEST_EMPTY(a);
    TEST_EMPTY(b);
    TEST_INOUT_EQ("[3,4]",interval(3,4));
    TEST_INOUT_EQ("4.5",interval(4.5,4.5));
    TEST_INOUT_EQ("1.0/10",interval(1.0)/interval(10.0));
    TEST_INOUT_EQ("1.0/10.0",interval("[0.1,0.1]","[0.1,0.1]"));
}
  // <-- End of tests
};


GAOL_REGISTER_TEST(constructor_test)
