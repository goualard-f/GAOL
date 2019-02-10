/*-*-C++-*------------------------------------------------------------------
 * gaol -- NOT Just Another Interval Library
 *--------------------------------------------------------------------------
 * This file is part of the gaol distribution. It has been developed at the
 * Laboratoire d'Informatique de Nantes-Atlantique (FRE CNRS 2729), France
 *
 * Copyright (c) 2001-2006 Laboratoire d'Informatique de Nantes-Atlantique
 *--------------------------------------------------------------------------
 * gaol is a software distributed WITHOUT ANY WARRANTY. Read the associated
 * COPYING file for information.
 *--------------------------------------------------------------------------
 * CVS: $Id: tests.h 247 2015-05-21 07:00:45Z goualard $
 * Last modified: Thu Mar 16 15:44:14 2006 on pc-dubreil-123.irin.sciences.univ-nantes.prive
 * By: Frederic Goualard <Frederic.Goualard@lina.univ-nantes.fr>
 *--------------------------------------------------------------------------*/

/*!
  \file   tests.h
  \brief

  <long description>

  \author Frederic Goualard
  \date   2006-03-16
*/


#ifndef __tests_h__
#define __tests_h__

#include <cmath>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <limits>
#include <sys/types.h>
#include <unistd.h>
#include <gaol/gaol>
#include <sys/types.h>
#include <unistd.h>

using namespace gaol;

#define TEST_PEQ(a,b)  CPPUNIT_ASSERT((a).possibly_eq(b))
#define TEST_SEQ(a,b)  CPPUNIT_ASSERT((a).set_eq(b))
#define TEST_EQ(a,b)   CPPUNIT_ASSERT(hausdorff(a,b)<=1e-8)
#define TEST_CONT(a,b) CPPUNIT_ASSERT((a).set_contains(b))
#define TEST_EMPTY(a)  CPPUNIT_ASSERT((a).is_empty())
#define TEST_TRUE(a)   CPPUNIT_ASSERT(a)
#define TEST_FALSE(a)   CPPUNIT_ASSERT(!(a))


#define TEST_PEQ4(a,b)  CPPUNIT_ASSERT(interval2f(a).possibly_eq_all(interval2f(b)))
#define TEST_SEQ4(a,b)  CPPUNIT_ASSERT(interval2f(a).set_eq_all(interval2f(b)))
#define TEST_EMPTY4(a)  CPPUNIT_ASSERT(interval2f(a).first().is_empty() || interval2f(a).second().is_empty())
#define TEST_CONTAINS4(a,b) CPPUNIT_ASSERT(interval2f(a).set_contains_all(interval2f(b)))
#define TEST_EQ4(a,b)   CPPUNIT_ASSERT(hausdorff((a).first(),(b).first())<=1e-5 \
													&& hausdorff((a).second(),(b).second())<=1e-5)




const interval max_inf(std::numeric_limits<double>::max(),+GAOL_INFINITY);
const interval m_inf_m_max(-GAOL_INFINITY,-std::numeric_limits<double>::max());

const interval m_min_zero(-std::numeric_limits<double>::min(),0);
const interval zero_min(0.0,std::numeric_limits<double>::min());
const interval m_min_min(-std::numeric_limits<double>::min(),std::numeric_limits<double>::min());

#define GAOL_REGISTER_TEST(a)                                                             \
  CPPUNIT_TEST_SUITE_REGISTRATION(a);                                                     \
                                                                                          \
  int main(int argc, char** argv)                                                         \
  {                                                                                       \
    gaol::init();                                                                         \
                                                                                          \
    CppUnit::TextUi::TestRunner runner;                                                   \
    CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry(); \
    runner.addTest(registry.makeTest());                                                  \
    bool success = runner.run();                                                          \
                                                                                          \
    gaol::cleanup();                                                                      \
    return !success;                                                                      \
  }



#endif /* __tests_h__ */
