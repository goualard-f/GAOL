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
 * CVS: $Id: gaol_exceptions.h 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By:
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_exceptions.h
  \brief  Declaration of standard exceptions in gaol.

  Exceptions are used only if gaol has been configured with --enable-exceptions

  \author Frederic Goualard
  \date   2002-12-02
*/


#ifndef __gaol_exceptions_h__
#define __gaol_exceptions_h__

#include "gaol/gaol_config.h"

#if GAOL_EXCEPTIONS_ENABLED

#include <stdexcept>
using std::exception;

#include <iosfwd>
#include <string>

using std::string;

namespace gaol {

  /*!
    \brief Base class for all gaol exceptions

    This class serves as a base class for all gaol exceptions in order
    to provide a uniform framework. Every exception thrown should at
    least contain the file and line where the exception was thrown.
  */
  class __GAOL_PUBLIC__ gaol_exception : public exception {
  public:
    /*!
      \param f Name of the file where the exception is thrown
      \param l Line in the file where the exception is thrown
    */
    gaol_exception(const char* f, unsigned l);

    /*!
      \param f Name of the file where the exception is thrown
      \param l Line in the file where the exception is thrown
      \param e Explanation concerning the throwing
    */
    gaol_exception(const char* f, unsigned l, const string& e);

    ~gaol_exception() throw () {}
    /*!
      \brief Accessor for the file name

      \return the name of the C++ file in which occurred the exception
    */
    const char* file() const;
    /*!
      \brief Accessor for the line number

      \return the line in the C++ file in which occurred the exception
    */
    unsigned int line() const;

    /*!
      \brief Accessor for the explanation string

      \return the string providing some explanation for having thrown
      the exception or the empty string if none was given.
    */
    string explanation() const;

  protected:
    /*!
     \brief File where the exception occurred.

     This information must be provided by using the macro __FILE__
     */
    const char* which_file_;
    /*!
      \brief Line where the exception occurred.

      This information must be provided by using the macro __LINE__
    */
    unsigned which_line_;
    /*!
      \brief Short explanation concerning the exception thrown.
     */
    string explanation_;
  };


  /*!
    \brief Exception thrown when an input interval does not follow
    the syntax.

    \see Refer to the documentation of the parser for intervals
    for the expected syntax (file gaol_interval_parser.ypp)
  */
  class __GAOL_PUBLIC__ input_format_error : public gaol_exception {
  public:
    input_format_error(const char* f, unsigned l) : gaol_exception(f,l) {}
    input_format_error(const char* f, unsigned l, const char* e) :
      gaol_exception(f,l,e) {}
    input_format_error(const char* f, unsigned l, const string& e) :
      gaol_exception(f,l,e) {}
  };


  /*!
    \brief Exception thrown whenever a feature is called for while not
    yet implemented, or is not available due to the way the library was
    configured.
  */
  class __GAOL_PUBLIC__ unavailable_feature_error : public gaol_exception {
  public:
    unavailable_feature_error(const char* f, unsigned l) : gaol_exception(f,l) {}
    unavailable_feature_error(const char* f, unsigned l, const char* e) :
      gaol_exception(f,l,e) {}
    unavailable_feature_error(const char* f, unsigned l, const string& e) :
      gaol_exception(f,l,e) {}
  };


  /*!
    \brief Exception thrown whenever an invalid action has been requested
    (such as computing the number of floating point numbers between
    two floating-point numbers, at least one of them being either a NaN or
    an infinite value).
  */
  class __GAOL_PUBLIC__ invalid_action_error : public gaol_exception {
  public:
    invalid_action_error(const char* f, unsigned l) : gaol_exception(f,l) {}
    invalid_action_error(const char* f, unsigned l, const char* e) :
      gaol_exception(f,l,e) {}
    invalid_action_error(const char* f, unsigned l, const string& e) :
      gaol_exception(f,l,e) {}
  };

  /*!
    \brief Display of an exception.

    Convenient operator to display the file, line and explanation for
    the exception thrown.
   */
  extern __GAOL_PUBLIC__ std::ostream& operator<<(std::ostream& out,
					      const gaol_exception &e);

} // namespace gaol

#endif /* GAOL_EXCEPTIONS_ENABLED */
#endif /* __gaol_exceptions_h__ */
