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
 * CVS: $Id: gaol_expression.h 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By:
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_expression.h
  \brief  Classes to represent interval expressions

  These classes are used to construct an internal representation of an interval
  obtained from a parsed string.

  \author Goualard Frédéric
  \date   2001-09-28
*/


#ifndef __gaol_expression_h__
#define __gaol_expression_h__

#include <iosfwd>

#include "gaol/gaol_config.h"
#include "gaol/gaol_common.h"
#include "gaol/gaol_interval.h"
#include "gaol/gaol_flags.h"
#include "gaol/gaol_expr_visitor.h"

namespace gaol {

/*!
  Use of Interval_struct instead of interval to overcome the fact that a
  class with a constructor cannot appear in a union.
  \note This class is only used for exchanging intervals between the lexer
  and the parser.
 */
typedef struct {
  double l, r;
} Interval_struct;

  class expr_node;

  class expression {
    friend class expr_node;
  public:
    expression();
    expression(double d);
    expression(const interval& I);
    expression(const expression& e);
    expression(const expr_node& e);
    virtual ~expression();

    expression& operator=(const expression& e);
    expression& operator+=(const expression& e);
    expression& operator-=(const expression& e);
    expression& operator*=(const expression& e);
    expression& operator/=(const expression& e);
    expr_node* get_root() const;

    friend std::ostream& operator<<(std::ostream& os, const expression& e);

  protected:
    //! Actual root of the expr: the_null_expr if the expression is empty
    mutable expr_node *root;
  };


  /*!
    \brief Abstract class for a node of an arithmetic expression
   */
  class expr_node {
    friend class expression;
  public:
    expr_node();
    expr_node(const expr_node& e);

    virtual ~expr_node();

    virtual expr_node* clone() const =0;

    //! Entry point for visitors.
    virtual void accept(expr_visitor& visitor) =0;

    virtual unsigned int get_precedence() const =0;
    virtual std::ostream& display(std::ostream& os) const =0;

    /*!
      Used to increment refcount of an object "a" from an object "b"
      whose type, though deriving also from expr_node, is not necessarily
      the same as "a".
      \see The C++ programming language, B. Stroustrup, 3rd ed. §15.3.1
    */
    unsigned int inc_refcount();
    /*!
      Used to decrement refcount of an object "a" from an object "b"
      whose type, though deriving also from expr_node, is not necessarily
      the same as "a".
      \see The C++ programming language, B. Stroustrup, 3rd ed. §15.3.1
    */
    unsigned int dec_refcount();
    /*!
      \brief put parentheses around the expression if necessary, that
      is, if the precedence of the expr_node is smaller than the one of
      the node calling the method.
     */
    static void parenthesize_if_necessary(unsigned int calling_prec,
					  std::ostream& os,
					  expr_node* e);
  private:
    unsigned int refcount; // Number of references (for garbage collection)
  };

  /*!
    \brief Represents a node with no information.

    Avoids the need for testing whether an expression points to a
    node or not.
  */
  class null_node : public expr_node {
  public:
    null_node();
    ~null_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
  protected:
    static const unsigned int precedence;
  };

  extern null_node* the_null_expr;

  /*!
    \brief floating-point number in IEEE754 double format

    Class coding a fp number in an expression.
  */
  class double_node : public expr_node {
  public:
    double_node(double d);
    ~double_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);

    //! Accessor
    INLINE double get_val() const;
  protected:
    static const unsigned int precedence;
  private:
    double val;
  };

  /*!
    \brief Interval with floating-point bounds

  */
  class interval_node : public expr_node {
  public:
    interval_node(const interval& I);
    ~interval_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);

    //! Accessor
    INLINE interval get_val() const;
  protected:
    static const unsigned int precedence;
  private:
    interval *val;
  };

  /*!
    \brief Node for an addition

  */
  class add_node : public expr_node {
  public:
    add_node(const expression &el, const expression &er);
    ~add_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);

    //! Accessors
    //@{
    INLINE expr_node* get_left();
    INLINE expr_node* get_right();
    //@}
  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_left, *e_right;
  };

  /*
    \brief Node for a unary minus
  */
  class unary_minus_node : public expr_node {
  public:
    unary_minus_node(const expression &e);
    ~unary_minus_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);

    // Accessor -
    INLINE expr_node* get_subexpr();
  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_uminus;
  };

  /*!
    \brief Node for a subtraction

  */
  class sub_node : public expr_node {
  public:
    sub_node(const expression &el, const expression &er);
    ~sub_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_left();
    INLINE expr_node* get_right();
    //@}
  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_left, *e_right;
  };


  /*!
    \brief Node for a multiplication

  */
  class mult_node : public expr_node {
  public:
    mult_node(const expression &el, const expression &er);
    ~mult_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_left();
    INLINE expr_node* get_right();
    //@}
  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_left, *e_right;
  };

  /*!
    \brief Node for a division

  */
  class div_node : public expr_node {
  public:
    div_node(const expression &el, const expression &er);
    ~div_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_left();
    INLINE expr_node* get_right();
    //@}
  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_left, *e_right;
  };

  /*!
    \brief Node for an exponentiation (e^n)
  */
  class pow_node : public expr_node {
  public:
    pow_node(const expression& e, int n);
    ~pow_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_subexpr();
    INLINE int get_exponent() const;
    //@}

  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_pow;
    int exponent;
  };

  /*!
    \brief Node for an interval power (I^J)
    \note e2 should evaluate to an interval.
  */
  class pow_itv_node : public expr_node {
  public:
    pow_itv_node(const expression& e1, const expression& e2);
    ~pow_itv_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_left();
    INLINE expr_node* get_right();
    //@}

  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_left;
    expr_node *e_right;
  };

  /*!
    \brief Node for an inverse exponentiation (e^(1/n))
  */
  class nth_root_node : public expr_node {
  public:
    nth_root_node(const expression& e, unsigned int n);
    ~nth_root_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_subexpr();
    INLINE unsigned int get_exponent() const;
    //@}

  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_nth_root;
    unsigned int exponent;
  };

  class cos_node : public expr_node {
  public:
    cos_node(const expression& e);
    ~cos_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_subexpr();
    //@}

  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_cos;
  };


  class sin_node : public expr_node {
  public:
    sin_node(const expression& e);
    ~sin_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_subexpr();
    //@}

  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_sin;
  };


  class tan_node : public expr_node {
  public:
    tan_node(const expression& e);
    ~tan_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_subexpr();
    //@}

  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_tan;
  };

  class atan2_node : public expr_node {
  public:
    atan2_node(const expression& e1, const expression& e2);
    ~atan2_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_Y();
     INLINE expr_node* get_X();
   //@}

  protected:
    static const unsigned int precedence;
  private:
    expr_node *Y, *X;
  };

  class acos_node : public expr_node {
  public:
    acos_node(const expression& e);
    ~acos_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_subexpr();
    //@}

  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_acos;
  };

  class asin_node : public expr_node {
  public:
    asin_node(const expression& e);
    ~asin_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_subexpr();
    //@}

  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_asin;
  };

  class atan_node : public expr_node {
  public:
    atan_node(const expression& e);
    ~atan_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_subexpr();
    //@}

  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_atan;
  };


  class cosh_node : public expr_node {
  public:
    cosh_node(const expression& e);
    ~cosh_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_subexpr();
    //@}

  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_cosh;
  };

  class sinh_node : public expr_node {
  public:
    sinh_node(const expression& e);
    ~sinh_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_subexpr();
    //@}

  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_sinh;
  };

  class tanh_node : public expr_node {
  public:
    tanh_node(const expression& e);
    ~tanh_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_subexpr();
    //@}

  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_tanh;
  };



  class acosh_node : public expr_node {
  public:
    acosh_node(const expression& e);
    ~acosh_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_subexpr();
    //@}

  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_acosh;
  };

  class asinh_node : public expr_node {
  public:
    asinh_node(const expression& e);
    ~asinh_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_subexpr();
    //@}

  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_asinh;
  };

  class atanh_node : public expr_node {
  public:
    atanh_node(const expression& e);
    ~atanh_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_subexpr();
    //@}

  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_atanh;
  };


  class exp_node : public expr_node {
  public:
    exp_node(const expression& e);
    ~exp_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_subexpr();
    //@}

  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_exp;
  };

  class log_node : public expr_node {
  public:
    log_node(const expression& e);
    ~log_node();
    expr_node* clone() const;
    std::ostream& display(std::ostream& os) const;
    unsigned int get_precedence() const;
    INLINE void accept(expr_visitor& visitor);
    //! Accessors
    //@{
    INLINE expr_node* get_subexpr();
    //@}

  protected:
    static const unsigned int precedence;
  private:
    expr_node *e_log;
  };


  //! Construction operators
  //@{
  const expression operator+(const expression& el, const expression& er);
  const expression operator-(const expression& e);
  const expression operator-(const expression& el, const expression& er);
  const expression operator*(const expression& el, const expression& er);
  const expression operator/(const expression& el, const expression& er);
  const expression pow(const expression& e, int n);
  const expression pow(const expression& e1, const expression& e2);
  const expression nth_root(const expression& e, unsigned int n);
  const expression cos(const expression& e);
  const expression sin(const expression& e);
  const expression tan(const expression& e);
  const expression atan2(const expression& e1, const expression& e2);
  const expression acos(const expression& e);
  const expression asin(const expression& e);
  const expression atan(const expression& e);
  const expression cosh(const expression& e);
  const expression sinh(const expression& e);
  const expression tanh(const expression& e);
  const expression acosh(const expression& e);
  const expression asinh(const expression& e);
  const expression atanh(const expression& e);
  const expression exp(const expression& e);
  const expression log(const expression& e);
  //@}

  /*!
    \brief Evaluation of trees for left and right bounds of intervals

    Proper rounding is ensured by performing the computation over intervals
    for both trees and by taking the left bound for el and the right
    bound for er;
    \return true if the parsing was possible and flase otherwise
  */
  //@{
  bool evaluate_left_right(const expression& el, const expression& er,
			   interval* itv);
  bool evaluate_left_right(const expression& e,
			   interval* itv);
  //@}
  /*!
    \brief Evaluates the tree denoted by e and returns the result as
    an interval

    \return true if the parsing was possible and flase otherwise
  */
  bool evaluate_expr(const expression& e,interval& itv);

  /*
    INLINE methods ---
   */

  /*
    null_node --
  */
  INLINE void null_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "null_node accepting visitor" << std::endl);
    visitor.visit(this);
  }


  /*
    double_node --
  */
  INLINE void double_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "double_node accepting visitor" << std::endl);
    visitor.visit(this);
  }

  INLINE double double_node::get_val() const
  {
    return val;
  }

  /*
    interval_node --
  */
  INLINE void interval_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "interval_node accepting visitor" << std::endl);
    visitor.visit(this);
  }

  INLINE interval interval_node::get_val() const
  {
    return *val;
  }


  /*
    add_node --
  */
  INLINE void add_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "add_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* add_node::get_left()
  {
    return e_left;
  }

  INLINE expr_node* add_node::get_right()
  {
    return e_right;
  }

  /*
    unary_minus_node --
  */
  INLINE void unary_minus_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "unary_minus_node accepting visitor"
		<< std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* unary_minus_node::get_subexpr()
  {
    return e_uminus;
  }

  /*
    sub_node --
  */
  INLINE void sub_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "sub_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* sub_node::get_left()
  {
    return e_left;
  }

  INLINE expr_node* sub_node::get_right()
  {
    return e_right;
  }

  /*
    mult_node --
  */
  INLINE void mult_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "mult_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* mult_node::get_left()
  {
    return e_left;
  }

  INLINE expr_node* mult_node::get_right()
  {
    return e_right;
  }

  /*
    div_node --
  */
  INLINE void div_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "div_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* div_node::get_left()
  {
    return e_left;
  }

  INLINE expr_node* div_node::get_right()
  {
    return e_right;
  }

  /*
    pow_node --
  */
  INLINE void pow_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "pow_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* pow_node::get_subexpr()
  {
    return e_pow;
  }


  INLINE int pow_node::get_exponent() const
  {
    return exponent;
  }

  /*
    pow_itv_node --
  */
  INLINE void pow_itv_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "pow_itv_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* pow_itv_node::get_left()
  {
    return e_left;
  }

  INLINE expr_node* pow_itv_node::get_right()
  {
    return e_right;
  }


  /*
    nth_root_node --
  */
  INLINE void nth_root_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "nth_root_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* nth_root_node::get_subexpr()
  {
    return e_nth_root;
  }


  INLINE unsigned int nth_root_node::get_exponent() const
  {
    return exponent;
  }

  /*
    cos_node --
  */
  INLINE void cos_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "cos_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* cos_node::get_subexpr()
  {
    return e_cos;
  }

  /*
    sin_node --
  */
  INLINE void sin_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "sin_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* sin_node::get_subexpr()
  {
    return e_sin;
  }

  /*
    tan_node --
  */
  INLINE void tan_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "tan_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* tan_node::get_subexpr()
  {
    return e_tan;
  }

  /*
    atan2_node --
  */
  INLINE void atan2_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "atan2_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* atan2_node::get_Y()
  {
    return Y;
  }

  INLINE expr_node* atan2_node::get_X()
  {
    return X;
  }

  /*
    acos_node --
  */
  INLINE void acos_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "acos_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* acos_node::get_subexpr()
  {
    return e_acos;
  }

  /*
    asin_node --
  */
  INLINE void asin_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "asin_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* asin_node::get_subexpr()
  {
    return e_asin;
  }

  /*
    atan_node --
  */
  INLINE void atan_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "atan_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* atan_node::get_subexpr()
  {
    return e_atan;
  }

  /*
    cosh_node --
  */
  INLINE void cosh_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "cosh_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* cosh_node::get_subexpr()
  {
    return e_cosh;
  }

  /*
    sinh_node --
  */
  INLINE void sinh_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "sinh_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* sinh_node::get_subexpr()
  {
    return e_sinh;
  }

  /*
    tanh_node --
  */
  INLINE void tanh_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "tanh_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* tanh_node::get_subexpr()
  {
    return e_tanh;
  }

  /*
    acosh_node --
  */
  INLINE void acosh_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "acosh_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* acosh_node::get_subexpr()
  {
    return e_acosh;
  }

  /*
    asinh_node --
  */
  INLINE void asinh_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "asinh_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* asinh_node::get_subexpr()
  {
    return e_asinh;
  }

  /*
    atanh_node --
  */
  INLINE void atanh_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "atanh_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* atanh_node::get_subexpr()
  {
    return e_atanh;
  }

  /*
    exp_node --
  */
  INLINE void exp_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "exp_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* exp_node::get_subexpr()
  {
    return e_exp;
  }

  /*
    log_node --
  */
  INLINE void log_node::accept(expr_visitor& visitor)
  {
    GAOL_DEBUG(2,std::cout << "log_node accepting visitor" << std::endl;);
    visitor.visit(this);
  }

  INLINE expr_node* log_node::get_subexpr()
  {
    return e_log;
  }


} // namespace gaol

#endif /* __gaol_expression_h__ */
