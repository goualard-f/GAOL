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
 * CVS: $Id: fpu.cpp 55 2009-03-01 13:55:03Z goualard $
 * Last modified: 
 * By: 
 *--------------------------------------------------------------------------*/

/*!
  \file	fpu.cpp
   
  \brief  Test of functions manipulating the fpu


  <long description>

  \author Goualard Frédéric
  \date   2006-10-13
*/

#include "tests.h"

class fpu_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(fpu_test);
  CPPUNIT_TEST(test_rounding);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp() {
  }
  void tearDown() {
  }

  void test_rounding() {
      
  }
  
   // --> Beginning of tests
  // <-- End of tests
};

GAOL_REGISTER_TEST(fpu_test);
