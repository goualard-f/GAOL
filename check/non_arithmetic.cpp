#include "tests.h"

class non_arithmetic_test : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(non_arithmetic_test);
  	CPPUNIT_TEST(test_pow_int);
	CPPUNIT_TEST(test_pow);
  	CPPUNIT_TEST(test_sqr);
  	CPPUNIT_TEST(test_sqrt);
  	CPPUNIT_TEST(test_log);
  	CPPUNIT_TEST(test_exp);
	CPPUNIT_TEST(test_nth_root);
  	CPPUNIT_TEST_SUITE_END();
public:
  void setUp() {
  }
  void tearDown() {
  }

  // --> Beginning of tests
  void test_pow_int() {
    TEST_SEQ(pow(interval::zero(),0),interval::one());
    CPPUNIT_ASSERT(pow(interval::emptyset(),0).is_empty());
    CPPUNIT_ASSERT(pow(interval::emptyset(),2).is_empty());
    CPPUNIT_ASSERT(pow(interval::emptyset(),3).is_empty());
    CPPUNIT_ASSERT(pow(interval::emptyset(),-2).is_empty());
    CPPUNIT_ASSERT(pow(interval::emptyset(),-3).is_empty());
    TEST_PEQ(pow(interval(-3,5),2),interval(0,25));
    TEST_EQ(pow(interval(-3,5),2),interval(0,25));

   // Negative interval
    TEST_EQ(pow(interval(-3,-2),-4),interval("[0.012345679,0.0625]"));
    TEST_EQ(pow(interval(-3,-2),-3),interval("[-0.125,-0.037037038]"));
    TEST_EQ(pow(interval(-3,-2),-2),interval("[0.111111111,0.25]"));
    TEST_EQ(pow(interval(-3,-2),-1),interval("[-0.5,-0.333333333]"));
    TEST_SEQ(pow(interval(-3,-2),0),interval::one());
    TEST_EQ(pow(interval(-3,-2),1),interval(-3,-2));
    TEST_EQ(pow(interval(-3,-2),2),interval(4,9));
    TEST_EQ(pow(interval(-3,-2),3),interval(-27,-8));
    TEST_EQ(pow(interval(-3,-2),4),interval(16,81));

     // Positive interval
    TEST_EQ(pow(interval(2,3),-4),interval("[0.012345679,0.0625]"));
    TEST_EQ(pow(interval(2,3),-3),interval("[0.037037037,0.125]"));
    TEST_EQ(pow(interval(2,3),-2),interval("[0.111111111,0.25]"));
    TEST_EQ(pow(interval(2,3),-1),interval("[0.333333333,0.5]"));
    TEST_SEQ(pow(interval(2,3),0),interval::one());
    TEST_EQ(pow(interval(2,3),1),interval(2,3));
    TEST_EQ(pow(interval(2,3),2),interval(4,9));
    TEST_EQ(pow(interval(2,3),3),interval(8,27));
    TEST_EQ(pow(interval(2,3),4),interval(16,81));

    // Interval straddling 0
    TEST_PEQ(pow(interval(-3,2),-4),interval("[0.01234567901,+inf]"));
    TEST_SEQ(pow(interval(-3,2),-3),interval::universe());
    TEST_PEQ(pow(interval(-3,2),-2),interval("[0.1111111111,+inf]"));
    TEST_SEQ(pow(interval(-3,2),-1),interval::universe());
    TEST_SEQ(pow(interval(-3,2),0),interval::one());
    TEST_EQ(pow(interval(-3,2),1),interval(-3,2));
    TEST_EQ(pow(interval(-3,2),2),interval(0,9));
    TEST_EQ(pow(interval(-3,2),3),interval(-27,8));
    TEST_EQ(pow(interval(-3,2),4),interval(0,81));

    TEST_PEQ(pow(interval(-2,3),-4),interval("[0.012345679,+inf]"));
    TEST_SEQ(pow(interval(-2,3),-3),interval::universe());
    TEST_PEQ(pow(interval(-2,3),-2),interval("[0.11111111,+inf]"));
    TEST_SEQ(pow(interval(-2,3),-1),interval::universe());
    TEST_SEQ(pow(interval(-2,3),0),interval::one());
    TEST_EQ(pow(interval(-2,3),1),interval(-2,3));
    TEST_EQ(pow(interval(-2,3),2),interval(0,9));
    TEST_EQ(pow(interval(-2,3),3),interval(-8,27));
    TEST_EQ(pow(interval(-2,3),4),interval(0,81));

    TEST_CONT(pow(interval(-15,-15),17),-98526125335693359375.0);
    TEST_CONT(pow(interval(15,15),17),98526125335693359375.0);
    TEST_CONT(pow(interval(-15,-15),18),1477891880035400390625.0);
    TEST_CONT(pow(interval(15,15),18),1477891880035400390625.0);

    TEST_SEQ(pow(m_inf_m_max,3),m_inf_m_max);
    TEST_SEQ(pow(m_inf_m_max,4),max_inf);
    TEST_SEQ(pow(max_inf,3),max_inf);
    TEST_SEQ(pow(max_inf,4),max_inf);
	TEST_SEQ(pow(interval::universe(),0),interval::one());
  }

	void test_pow() {
		TEST_SEQ(pow(interval::universe(),interval::zero()),interval::one());
		TEST_EQ(pow(interval(3,4),interval(2,3)),interval(9,64));

	}


  void test_sqr() {
    TEST_EMPTY(sqr(interval::emptyset()));
    TEST_SEQ(sqr(interval::universe()),interval::positive());
    TEST_SEQ(sqr(m_inf_m_max),max_inf);
    TEST_SEQ(sqr(max_inf),max_inf);

    TEST_SEQ(sqr(interval(-3,-2)),interval(4,9));
    TEST_SEQ(sqr(interval(2,3)),interval(4,9));
    TEST_SEQ(sqr(interval(-3,2)),interval(0,9));
    TEST_SEQ(sqr(interval(-2,3)),interval(0,9));
  }

  void test_sqrt() {
    TEST_EMPTY(sqrt(interval::emptyset()));
    TEST_SEQ(sqrt(interval::universe()),interval::positive());

    TEST_EMPTY(sqrt(interval(-3,-2)));
    TEST_EQ(sqrt(interval(2,3)),interval("[1.414213562373095, 1.732050807568877]"));
    TEST_EQ(sqrt(interval(-3,2)),interval("[0,1.414213562373096]"));
    TEST_EQ(sqrt(interval(-2,3)),interval("[0,1.732050807568877]"));

  }

  void test_log() {
    TEST_SEQ(log(interval(-4.0,0.0)),interval(-GAOL_INFINITY,-std::numeric_limits<double>::max()));
    TEST_EMPTY(log(interval::emptyset()));
	interval tmp = log(interval(-3,4));
	CPPUNIT_ASSERT(tmp.left()==-GAOL_INFINITY && (tmp.right()-1.386294361119891)<=1e-8);
	TEST_EQ(log(interval(5,9)),interval(1.6094379124341003746,2.19722457733621938279));
	TEST_EQ(log(interval("56.1")),interval("4.02713581252865063169"));
  }

  void test_exp() {
    TEST_EMPTY(exp(interval::emptyset()));
    TEST_EQ(exp(interval::zero()),interval::one());
    TEST_EQ(exp(interval::one()),interval("2.7182818284590452"));
    TEST_SEQ(exp(interval(740.0)),interval(std::numeric_limits<double>::max(),GAOL_INFINITY));
    TEST_EQ(exp(interval(-800.0)),interval::zero());
    TEST_EQ(exp(interval(-10.0,-5.0)),interval("[4.53999297624848e-5,6.7379469990855e-3]"));
    TEST_EQ(exp(interval(-5.0,9.0)),interval("[6.7379469990854e-3,8103.0839275754]"));
    TEST_EQ(exp(interval(9.0,11.0)),interval("[8103.0839275754,59874.14171519782]"));
    TEST_EQ(exp(interval(-3.5)),interval("3.01973834223185e-2"));
    TEST_EQ(exp(interval(3.5)),interval("33.1154519586923"));
	CPPUNIT_ASSERT(exp(m_inf_m_max).certainly_positive());
  }

	void test_nth_root() {
		TEST_EMPTY(nth_root(interval::universe(),0));
		TEST_SEQ(nth_root(interval(4,5),1),interval(4,5));
		TEST_EQ(nth_root(interval(4,5),2),interval(2,2.23606797749));
		TEST_EMPTY(nth_root(interval::emptyset(),2));
		TEST_EMPTY(nth_root(interval::emptyset(),5));
		TEST_EMPTY(nth_root(interval::emptyset(),4));
		TEST_EQ(nth_root(interval(-4,3),4),interval(0,1.3160740129524));
		TEST_EQ(nth_root(interval(-4,3),5),interval(0,1.24573093961));
		TEST_EQ(nth_root(interval(0.5,2),4),interval(0.84089641525371,1.189207115002));
		TEST_EQ(nth_root(interval(0.2,0.5),4),interval(0.668740304976422024,0.84089641525371454303));
		TEST_EQ(nth_root(interval(0.5,2),5),interval(0.8705505632,1.14869835));
		TEST_EQ(nth_root(interval(0.2,0.5),5),interval(0.72477966,0.8705505632));
		TEST_EQ(nth_root(interval("0.1"),4),interval("0.562341325190349"));
		TEST_EQ(nth_root(interval("0.1"),5),interval("0.630957344480193"));
		TEST_EQ(nth_root(interval("2.1"),4),interval("1.203801343502715"));
		TEST_EQ(nth_root(interval("2.1"),5),interval("1.159962258654001"));

	}
  // <-- End of tests
};

GAOL_REGISTER_TEST(non_arithmetic_test)
