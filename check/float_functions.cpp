/*-*-C++-*------------------------------------------------------------------
 * gaol -- NOT Just Another Interval Library
 *--------------------------------------------------------------------------
 * This file is part of the gaol distribution. Gaol was primarily 
 * developed at the Swiss Federal Institute of Technology, Lausanne, 
 * Switzerland, and is now developed at the Laboratoire d'Informatique de 
 * Nantes-Atlantique, France.
 *
 * Copyright (c) 2001 Swiss Federal Institute of Technology, Switzerland
 * Copyright (c) 2002-2006 Laboratoire d'Informatique de 
 *                         Nantes-Atlantique, France
 *--------------------------------------------------------------------------
 * gaol is a software distributed WITHOUT ANY WARRANTY. Read the associated 
 * COPYING file for information.
 *--------------------------------------------------------------------------
 * CVS: $Id: float_functions.cpp 191 2012-03-06 17:08:58Z goualard $
 * Last modified: 
 * By: 
 *--------------------------------------------------------------------------*/


#include "tests.h"


class float_functions_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(float_functions_test);
  CPPUNIT_TEST(test_functions);
  CPPUNIT_TEST(test_types);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp() {
  }
  void tearDown() {
  }

  // --> Beginning of tests
  void test_functions() {
      CPPUNIT_ASSERT(feven(4.0));
      CPPUNIT_ASSERT(!feven(4.5));
      CPPUNIT_ASSERT(!feven(5.0));
      CPPUNIT_ASSERT(!feven(5.5));
      CPPUNIT_ASSERT(feven(-4.0));
      CPPUNIT_ASSERT(!feven(-4.5));
      CPPUNIT_ASSERT(!feven(5.0));
      CPPUNIT_ASSERT(!feven(5.5));
      CPPUNIT_ASSERT(feven(GAOL_INFINITY));
      CPPUNIT_ASSERT(!feven(GAOL_NAN));
      
      CPPUNIT_ASSERT(next_float(1.0)== (1.0+std::numeric_limits<double>::epsilon()));
      CPPUNIT_ASSERT(std::isnan(next_float(GAOL_NAN)));
      CPPUNIT_ASSERT(std::isinf(next_float(GAOL_INFINITY)));      
      CPPUNIT_ASSERT(next_float(-GAOL_INFINITY) == -std::numeric_limits<double>::max());
      
      CPPUNIT_ASSERT(previous_float(1.0+std::numeric_limits<double>::epsilon())==1.0);
      CPPUNIT_ASSERT(std::isnan(previous_float(GAOL_NAN)));
      CPPUNIT_ASSERT(previous_float(GAOL_INFINITY)==std::numeric_limits<double>::max());      
      
      CPPUNIT_ASSERT(is_signed(-5.0));
      CPPUNIT_ASSERT(is_signed(-GAOL_INFINITY));
      CPPUNIT_ASSERT(!is_signed(5.0));
      CPPUNIT_ASSERT(!is_signed(GAOL_INFINITY));
      
      CPPUNIT_ASSERT(minimum(5,6)==5);
      CPPUNIT_ASSERT(minimum(6,5)==5);
      CPPUNIT_ASSERT(minimum(-5,-6)==-6);
      CPPUNIT_ASSERT(minimum(-5,-6)==-6);
      CPPUNIT_ASSERT(std::isnan(minimum(5,GAOL_NAN)));
      CPPUNIT_ASSERT(std::isnan(minimum(GAOL_NAN,5)));
      CPPUNIT_ASSERT(is_signed(minimum(0.0,-0.0)));
      CPPUNIT_ASSERT(is_signed(minimum(-0.0,0.0)));
      
      CPPUNIT_ASSERT(maximum(5,6)==6);
      CPPUNIT_ASSERT(maximum(6,5)==6);
      CPPUNIT_ASSERT(maximum(-5,-6)==-5);
      CPPUNIT_ASSERT(maximum(-5,-6)==-5);
      CPPUNIT_ASSERT(std::isnan(maximum(5,GAOL_NAN)));
      CPPUNIT_ASSERT(std::isnan(maximum(GAOL_NAN,5)));
      CPPUNIT_ASSERT(!is_signed(maximum(0.0,-0.0)));
      CPPUNIT_ASSERT(!is_signed(maximum(-0.0,0.0)));
      
      CPPUNIT_ASSERT(nb_fp_numbers(3.5,next_float(3.5))==2);
      CPPUNIT_ASSERT(nb_fp_numbers(-3.5,next_float(-3.5))==2);
      CPPUNIT_ASSERT(nb_fp_numbers(4,4)==1);
      
      { 
	  bool ok = false;
	  try {	
	      ULONGLONGINT x = nb_fp_numbers(GAOL_NAN,5);
	  } catch (invalid_action_error &e) {
	      ok = true;
	  } catch (gaol_exception &e) {
	      // Should have been an invalid_action_error.
	      // There is a problem here.
	  }
	  CPPUNIT_ASSERT(ok);
      }
      
      { 
	  bool ok = false;
	  try {	
	      ULONGLONGINT x = nb_fp_numbers(5,GAOL_NAN);
	  } catch (invalid_action_error &e) {
	      ok = true;
	  } catch (gaol_exception &e) {
	      // Should have been an invalid_action_error.
	      // There is a problem here.
	  }
	  CPPUNIT_ASSERT(ok);
      }
      { 
	  bool ok = false;
	  try {	
	      ULONGLONGINT x = nb_fp_numbers(GAOL_NAN,-5);
	  } catch (invalid_action_error &e) {
	      ok = true;
	  } catch (gaol_exception &e) {
	      // Should have been an invalid_action_error.
	      // There is a problem here.
	  }
	  CPPUNIT_ASSERT(ok);
      }
      { 	
	  bool ok = false;
	  try {	
	      ULONGLONGINT x = nb_fp_numbers(-5,GAOL_NAN);
	  } catch (invalid_action_error &e) {
	      ok = true;
	  } catch (gaol_exception &e) {
	      // Should have been an invalid_action_error.
	      // There is a problem here.
	  }
	  CPPUNIT_ASSERT(ok);
      }
      
      
      { 
	  bool ok = false;
	  try {	
	      ULONGLONGINT x = nb_fp_numbers(GAOL_INFINITY,5);
	  } catch (invalid_action_error &e) {
	      ok = true;
	  } catch (gaol_exception &e) {
	      // Should have been an invalid_action_error.
	      // There is a problem here.
	  }
	  CPPUNIT_ASSERT(ok);
      }
      
      { 
	  bool ok = false;
	  try {	
	      ULONGLONGINT x = nb_fp_numbers(5,GAOL_INFINITY);
	  } catch (invalid_action_error &e) {
	      ok = true;
	  } catch (gaol_exception &e) {
	      // Should have been an invalid_action_error.
	      // There is a problem here.
	  }
	  CPPUNIT_ASSERT(ok);
      }
      { 
	  bool ok = false;
	  try {	
	      ULONGLONGINT x = nb_fp_numbers(GAOL_INFINITY,-5);
	  } catch (invalid_action_error &e) {
	      ok = true;
	  } catch (gaol_exception &e) {
	      // Should have been an invalid_action_error.
	      // There is a problem here.
	  }
	  CPPUNIT_ASSERT(ok);
      }
      { 	
	  bool ok = false;
	  try {	
	      ULONGLONGINT x = nb_fp_numbers(-5,GAOL_INFINITY);
	  } catch (invalid_action_error &e) {
	      ok = true;
	  } catch (gaol_exception &e) {
	      // Should have been an invalid_action_error.
	      // There is a problem here.
	  }
	  CPPUNIT_ASSERT(ok);
      }

  }
  
  void test_types() {
      CPPUNIT_ASSERT(sizeof(ULONGLONGINT)==8);
  }
  // <-- End of tests
};


GAOL_REGISTER_TEST(float_functions_test)
