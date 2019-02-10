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
 * CVS: $Id: gaol_expression.cpp 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By:
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_expression.cpp
  \brief


  \author Goualard Frédéric
  \date   2001-09-28
*/

static const char *CVSid="@(#) $Id: gaol_expression.cpp 191 2012-03-06 17:08:58Z goualard $";

#include <iostream>
#include <cmath>


#include "gaol/gaol_expression.h"
#include "gaol/gaol_expr_eval.h"

namespace gaol {
  // Node used for empty expressions to have something to point at
  null_node* the_null_expr;

  const unsigned null_node::precedence = prec_t::null_prec;
  const unsigned double_node::precedence = prec_t::cst_prec;
  const unsigned interval_node::precedence = prec_t::cst_prec;
  const unsigned add_node::precedence = prec_t::plus_prec;
  const unsigned sub_node::precedence = prec_t::plus_prec;
  const unsigned mult_node::precedence = prec_t::mult_prec;
  const unsigned div_node::precedence = prec_t::mult_prec;
  const unsigned unary_minus_node::precedence = prec_t::uminus_prec;
  const unsigned pow_node::precedence = prec_t::uminus_prec;
  const unsigned pow_itv_node::precedence = prec_t::uminus_prec;
  const unsigned nth_root_node::precedence = prec_t::uminus_prec;
  const unsigned cos_node::precedence = prec_t::uminus_prec;
  const unsigned sin_node::precedence = prec_t::uminus_prec;
  const unsigned tan_node::precedence = prec_t::uminus_prec;
  const unsigned atan2_node::precedence = prec_t::uminus_prec;
  const unsigned cosh_node::precedence = prec_t::uminus_prec;
  const unsigned sinh_node::precedence = prec_t::uminus_prec;
  const unsigned tanh_node::precedence = prec_t::uminus_prec;
  const unsigned acos_node::precedence = prec_t::uminus_prec;
  const unsigned asin_node::precedence = prec_t::uminus_prec;
  const unsigned atan_node::precedence = prec_t::uminus_prec;
  const unsigned acosh_node::precedence = prec_t::uminus_prec;
  const unsigned asinh_node::precedence = prec_t::uminus_prec;
  const unsigned atanh_node::precedence = prec_t::uminus_prec;
  const unsigned exp_node::precedence = prec_t::uminus_prec;
  const unsigned log_node::precedence = prec_t::uminus_prec;

  /*
   * expression --
   */

  expression::expression() : root(the_null_expr)
  {
    ++root->refcount;
    GAOL_DEBUG(3,std::cout << "creating null expression" << std::endl);
  }

  expression::expression(double d)
  {
    GAOL_DEBUG(3,std::cout << "creating expression from " << d << std::endl);
    root= new double_node(d);
    ++root->refcount;
  }

  expression::expression(const interval& I)
  {
    GAOL_DEBUG(3,std::cout << "creating expression from " << I << std::endl);
    root= new interval_node(I);
    ++root->refcount;
  }

  expression::expression(const expression& e) : root(e.root)
  {
    GAOL_DEBUG(3,std::cout << "creating expression from expression "
		<< std::endl);
    ++root->refcount;
  }

  expression::expression(const expr_node& e)
  {
    GAOL_DEBUG(3,std::cout << "creating expression from node " << std::endl);
    root = &const_cast<expr_node&>(e);
    ++root->refcount;
  }


  expression::~expression()
  {
    GAOL_DEBUG(3,std::cout << "deleting expression" << std::endl);

    if (--root->refcount == 0) {
      delete root;
    }
  }

  expression& expression::operator=(const expression& e)
  {
    ++e.root->refcount;
    if (--root->refcount == 0) {
      delete root;
    }
    root=e.root;
    return *this;
  }

  expression& expression::operator+=(const expression& e)
  {
    // Decrementing because it will be reset to its original value by mult_node
    --root->refcount;
    root=new add_node(*this,e);
    return *this;
  }

  expression& expression::operator-=(const expression& e)
  {
    // Decrementing because it will be reset to its original value by mult_node
    --root->refcount;
    root=new sub_node(*this,e);
    return *this;
  }

  expression& expression::operator*=(const expression& e)
  {
    // Decrementing because it will be reset to its original value by mult_node
    --root->refcount;
    root=new mult_node(*this,e);
    return *this;
  }

  expr_node* expression::get_root() const
  {
    return root;
  }


  std::ostream& operator<<(std::ostream& os, const expression& e)
  {
    (e.root)->display(os);
    return os;
  }


  /*
   * expr_node --
   */

  expr_node::expr_node() : refcount(0)
  {
    // nothing else to do
  }

  expr_node::expr_node(const expr_node& e) : refcount(0)
  {
    // nothing else to do
  }

  expr_node::~expr_node()
  {
    // nothing to do
  }

  unsigned int expr_node::inc_refcount()
  {
    return ++refcount;
  }

  unsigned int expr_node::dec_refcount()
  {
    return --refcount;
  }

  void expr_node::parenthesize_if_necessary(unsigned int calling_prec,
					    std::ostream& os,
					    expr_node* e)
  {
    if (e->get_precedence() <= calling_prec) {
      os.put('('); e->display(os); os.put(')');
    } else {
      e->display(os);
    }
  }

  /*
    null_node ---
  */
  null_node::null_node()
  {
    GAOL_DEBUG(3,std::cout << "null_node created" << std::endl);
  }


  null_node::~null_node()
  {
    GAOL_DEBUG(3,std::cout << "null_node destroyed" << std::endl);
  }

  expr_node* null_node::clone() const
  {
    return new null_node;
  }

  std::ostream& null_node::display(std::ostream& os) const
  {
    os << ":null:";
    return os;
  }

  unsigned int null_node::get_precedence() const
  {
    return precedence;
  }

  /*
   * double_node --
   */

  double_node::double_node(double d) : val(d)
  {
    GAOL_DEBUG(3,std::cout << "double_node created" << std::endl);
  }

  double_node::~double_node()
  {
    GAOL_DEBUG(3,std::cout << "double_node destroyed" << std::endl);
  }

  std::ostream& double_node::display(std::ostream& os) const
  {
    os << val;
    return os;
  }

  expr_node* double_node::clone() const
  {
    double_node* e = new double_node(val);
    return e;
  }

  unsigned int double_node::get_precedence() const
  {
    return precedence;
  }

  /*
   * interval_node --
   */

  interval_node::interval_node(const interval& I)
  {
    val = new interval(I);
    GAOL_DEBUG(3,std::cout << "interval_node created" << std::endl);
  }

  interval_node::~interval_node()
  {
    delete val;
    GAOL_DEBUG(3,std::cout << "interval_node destroyed" << std::endl);
  }

  std::ostream& interval_node::display(std::ostream& os) const
  {
    os << *val;
    return os;
  }

  expr_node* interval_node::clone() const
  {
    interval_node* e = new interval_node(*val);
    return e;
  }

  unsigned int interval_node::get_precedence() const
  {
    return precedence;
  }

  /*
   * add_node --
   */

  add_node::add_node(const expression &el,
		     const expression &er) : e_left(el.get_root()),
					     e_right(er.get_root())
  {
    e_left->inc_refcount();
    e_right->inc_refcount();
     GAOL_DEBUG(3,std::cout << "add_node created" << std::endl);
  }

  add_node::~add_node()
  {
    if (e_left->dec_refcount() == 0) {
      delete e_left;
    }
    if (e_right->dec_refcount() == 0) {
      delete e_right;
    }
    GAOL_DEBUG(3,std::cout << "add_node destroyed" << std::endl);
  }

  std::ostream& add_node::display(std::ostream& os) const
  {
    parenthesize_if_necessary(precedence,os,e_left);
    os.put('+');
    parenthesize_if_necessary(precedence,os,e_right);
    return os;
  }


  expr_node* add_node::clone() const
  {
    add_node* e = new add_node(*e_left,*e_right);
    return e;
  }

   unsigned int add_node::get_precedence() const
  {
    return precedence;
  }

 /*
    unary_minus_node ---
  */
  unary_minus_node::unary_minus_node(const expression& e) : e_uminus(e.get_root())
  {
    e_uminus->inc_refcount();
    GAOL_DEBUG(3,std::cout << "unary_minus_node created" << std::endl);
  }

  unary_minus_node::~unary_minus_node()
  {
    if (e_uminus->dec_refcount() == 0) {
      delete e_uminus;
    }
    GAOL_DEBUG(3,std::cout << "unary_minus_node destroyed" << std::endl);
  }

  std::ostream& unary_minus_node::display(std::ostream& os) const
  {
    os.put('-');
    if (e_uminus->get_precedence()<precedence) {
      os.put('('); os << (*e_uminus); os.put(')');
    } else {
      os << (*e_uminus);
    }
    return os;
  }

  expr_node* unary_minus_node::clone() const
  {
    unary_minus_node* e = new unary_minus_node(*e_uminus);
    return e;
  }

  unsigned int unary_minus_node::get_precedence() const
  {
    return precedence;
  }

  /*
   * sub_node --
   */

  sub_node::sub_node(const expression &el,
		     const expression &er) : e_left(el.get_root()),
					     e_right(er.get_root())
  {
    e_left->inc_refcount();
    e_right->inc_refcount();
     GAOL_DEBUG(3,std::cout << "sub_node created" << std::endl);
  }

  sub_node::~sub_node()
  {
    if (e_left->dec_refcount() == 0) {
      delete e_left;
    }
    if (e_right->dec_refcount() == 0) {
      delete e_right;
    }
    GAOL_DEBUG(3,std::cout << "sub_node destroyed" << std::endl);
  }

  std::ostream& sub_node::display(std::ostream& os) const
  {
    parenthesize_if_necessary(precedence,os,e_left);
    os.put('-');
    parenthesize_if_necessary(precedence,os,e_right);
    return os;
  }


  expr_node* sub_node::clone() const
  {
    sub_node* e = new sub_node(*e_left,*e_right);
    return e;
  }

  unsigned int sub_node::get_precedence() const
  {
    return precedence;
  }

  /*
   * mult_node --
   */

  mult_node::mult_node(const expression &el,
		       const expression &er) : e_left(el.get_root()),
					       e_right(er.get_root())
  {
    e_left->inc_refcount();
    e_right->inc_refcount();
    GAOL_DEBUG(3,std::cout << "mult_node created" << std::endl);
  }

  mult_node::~mult_node()
  {
    if (e_left->dec_refcount() == 0) {
      delete e_left;
    }
    if (e_right->dec_refcount() == 0) {
      delete e_right;
    }
    GAOL_DEBUG(3,std::cout << "mult_node destroyed" << std::endl);
  }

  std::ostream& mult_node::display(std::ostream& os) const
  {
    parenthesize_if_necessary(precedence,os,e_left);
    os.put('*');
    parenthesize_if_necessary(precedence,os,e_right);
    return os;
  }


  expr_node* mult_node::clone() const
  {
    mult_node* e = new mult_node(*e_left,*e_right);
    return e;
  }

  unsigned int mult_node::get_precedence() const
  {
    return precedence;
  }

  /*
   * div_node --
   */

  div_node::div_node(const expression &el,
		       const expression &er) : e_left(el.get_root()),
					       e_right(er.get_root())
  {
    e_left->inc_refcount();
    e_right->inc_refcount();
    GAOL_DEBUG(3,std::cout << "div_node created" << std::endl);
  }

  div_node::~div_node()
  {
    if (e_left->dec_refcount() == 0) {
      delete e_left;
    }
    if (e_right->dec_refcount() == 0) {
      delete e_right;
    }
    GAOL_DEBUG(3,std::cout << "div_node destroyed" << std::endl);
  }

  std::ostream& div_node::display(std::ostream& os) const
  {
    parenthesize_if_necessary(precedence,os,e_left);
    os.put('*');
    parenthesize_if_necessary(precedence,os,e_right);
    return os;
  }


  expr_node* div_node::clone() const
  {
    div_node* e = new div_node(*e_left,*e_right);
    return e;
  }

  unsigned int div_node::get_precedence() const
  {
    return precedence;
  }

  /*
    pow_node
  */
  pow_node::pow_node(const expression& e, int n) : e_pow(e.get_root())
  {
    exponent = n;
    e_pow->inc_refcount();
    GAOL_DEBUG(3,std::cout << "pow_node created" << std::endl);
  }

  pow_node::~pow_node()
  {
    if (e_pow->dec_refcount() == 0) {
      delete e_pow;
    }
    GAOL_DEBUG(3,std::cout << "pow_node destroyed" << std::endl);
  }

  std::ostream& pow_node::display(std::ostream& os) const
  {
    parenthesize_if_necessary(precedence,os,e_pow);
    os.put('^');
    os << exponent;
    return os;
  }

  expr_node* pow_node::clone() const
  {
    pow_node* e = new pow_node(*e_pow,exponent);
    return e;
  }

  unsigned int pow_node::get_precedence() const
  {
    return precedence;
  }

  /*
    pow_itv_node
  */
  pow_itv_node::pow_itv_node(const expression& e1, const expression &e2) : e_left(e1.get_root()),
									   e_right(e2.get_root())
  {
    e_left->inc_refcount();
    e_right->inc_refcount();
    GAOL_DEBUG(3,std::cout << "pow_itv_node created" << std::endl);
  }

  pow_itv_node::~pow_itv_node()
  {
    if (e_left->dec_refcount() == 0) {
      delete e_left;
    }
    if (e_right->dec_refcount() == 0) {
      delete e_right;
    }
    GAOL_DEBUG(3,std::cout << "pow_itv_node destroyed" << std::endl);
  }

  std::ostream& pow_itv_node::display(std::ostream& os) const
  {
    parenthesize_if_necessary(precedence,os,e_left);
    os.put('^');
    parenthesize_if_necessary(precedence,os,e_right);
    return os;
  }

  expr_node* pow_itv_node::clone() const
  {
    pow_itv_node* e = new pow_itv_node(*e_left,*e_right);
    return e;
  }

  unsigned int pow_itv_node::get_precedence() const
  {
    return precedence;
  }

  /*
    nth_root_node
  */
  nth_root_node::nth_root_node(const expression& e, unsigned int n) :
    e_nth_root(e.get_root())
  {
    exponent = n;
    e_nth_root->inc_refcount();
    GAOL_DEBUG(3,std::cout << "nth_root_node created" << std::endl);
  }

  nth_root_node::~nth_root_node()
  {
    if (e_nth_root->dec_refcount() == 0) {
      delete e_nth_root;
    }
    GAOL_DEBUG(3,std::cout << "nth_root_node destroyed" << std::endl);
  }

  std::ostream& nth_root_node::display(std::ostream& os) const
  {
    parenthesize_if_necessary(precedence,os,e_nth_root);
    os << "^(1/" << exponent << ')';
    return os;
  }

  expr_node* nth_root_node::clone() const
  {
    nth_root_node* e = new nth_root_node(*e_nth_root,exponent);
    return e;
  }

  unsigned int nth_root_node::get_precedence() const
  {
    return precedence;
  }

  /*
    cos_node
  */
  cos_node::cos_node(const expression& e) : e_cos(e.get_root())
  {
    e_cos->inc_refcount();
    GAOL_DEBUG(3,std::cout << "cos_node created" << std::endl);
  }

  cos_node::~cos_node()
  {
    if (e_cos->dec_refcount() == 0) {
      delete e_cos;
    }
    GAOL_DEBUG(3,std::cout << "cos_node destroyed" << std::endl);
  }

  std::ostream& cos_node::display(std::ostream& os) const
  {
    os << "cos(";
    e_cos->display(os);
    os.put(')');
    return os;
  }

  expr_node* cos_node::clone() const
  {
    cos_node* e = new cos_node(*e_cos);
    return e;
  }

  unsigned int cos_node::get_precedence() const
  {
    return precedence;
  }

  /*
    sin_node
  */
  sin_node::sin_node(const expression& e) : e_sin(e.get_root())
  {
    e_sin->inc_refcount();
    GAOL_DEBUG(3,std::cout << "sin_node created" << std::endl);
  }

  sin_node::~sin_node()
  {
    if (e_sin->dec_refcount() == 0) {
      delete e_sin;
    }
    GAOL_DEBUG(3,std::cout << "sin_node destroyed" << std::endl);
  }

  std::ostream& sin_node::display(std::ostream& os) const
  {
    os << "sin(";
    e_sin->display(os);
    os.put(')');
    return os;
  }

  expr_node* sin_node::clone() const
  {
    sin_node* e = new sin_node(*e_sin);
    return e;
  }

  unsigned int sin_node::get_precedence() const
  {
    return precedence;
  }

  /*
    tan_node
  */
  tan_node::tan_node(const expression& e) : e_tan(e.get_root())
  {
    e_tan->inc_refcount();
    GAOL_DEBUG(3,std::cout << "tan_node created" << std::endl);
  }

  tan_node::~tan_node()
  {
    if (e_tan->dec_refcount() == 0) {
      delete e_tan;
    }
    GAOL_DEBUG(3,std::cout << "tan_node destroyed" << std::endl);
  }

  std::ostream& tan_node::display(std::ostream& os) const
  {
    os << "tan(";
    e_tan->display(os);
    os.put(')');
    return os;
  }

  expr_node* tan_node::clone() const
  {
    tan_node* e = new tan_node(*e_tan);
    return e;
  }

  unsigned int tan_node::get_precedence() const
  {
    return precedence;
  }

  /*
    atan2_node
  */
  atan2_node::atan2_node(const expression& e1, const expression& e2) : Y(e1.get_root()),
								       X(e2.get_root())
  {
    Y->inc_refcount();
    X->inc_refcount();
    GAOL_DEBUG(3,std::cout << "atan2_node created" << std::endl);
  }

  atan2_node::~atan2_node()
  {
    if (Y->dec_refcount() == 0) {
      delete Y;
    }
     if (X->dec_refcount() == 0) {
      delete X;
    }
   GAOL_DEBUG(3,std::cout << "atan2_node destroyed" << std::endl);
  }

  std::ostream& atan2_node::display(std::ostream& os) const
  {
    os << "atan2(";
    Y->display(os);
    os.put(',');
    X->display(os);
    os.put(')');
    return os;
  }

  expr_node* atan2_node::clone() const
  {
    atan2_node* e = new atan2_node(*Y,*X);
    return e;
  }

  unsigned int atan2_node::get_precedence() const
  {
    return precedence;
  }

  /*
    acos_node
  */
  acos_node::acos_node(const expression& e) : e_acos(e.get_root())
  {
    e_acos->inc_refcount();
    GAOL_DEBUG(3,std::cout << "acos_node created" << std::endl);
  }

  acos_node::~acos_node()
  {
    if (e_acos->dec_refcount() == 0) {
      delete e_acos;
    }
    GAOL_DEBUG(3,std::cout << "acos_node destroyed" << std::endl);
  }

  std::ostream& acos_node::display(std::ostream& os) const
  {
    os << "acos(";
    e_acos->display(os);
    os.put(')');
    return os;
  }

  expr_node* acos_node::clone() const
  {
    acos_node* e = new acos_node(*e_acos);
    return e;
  }

  unsigned int acos_node::get_precedence() const
  {
    return precedence;
  }

  /*
    asin_node
  */
  asin_node::asin_node(const expression& e) : e_asin(e.get_root())
  {
    e_asin->inc_refcount();
    GAOL_DEBUG(3,std::cout << "asin_node created" << std::endl);
  }

  asin_node::~asin_node()
  {
    if (e_asin->dec_refcount() == 0) {
      delete e_asin;
    }
    GAOL_DEBUG(3,std::cout << "asin_node destroyed" << std::endl);
  }

  std::ostream& asin_node::display(std::ostream& os) const
  {
    os << "asin(";
    e_asin->display(os);
    os.put(')');
    return os;
  }

  expr_node* asin_node::clone() const
  {
    asin_node* e = new asin_node(*e_asin);
    return e;
  }

  unsigned int asin_node::get_precedence() const
  {
    return precedence;
  }

  /*
    atan_node
  */
  atan_node::atan_node(const expression& e) : e_atan(e.get_root())
  {
    e_atan->inc_refcount();
    GAOL_DEBUG(3,std::cout << "atan_node created" << std::endl);
  }

  atan_node::~atan_node()
  {
    if (e_atan->dec_refcount() == 0) {
      delete e_atan;
    }
    GAOL_DEBUG(3,std::cout << "atan_node destroyed" << std::endl);
  }

  std::ostream& atan_node::display(std::ostream& os) const
  {
    os << "atan(";
    e_atan->display(os);
    os.put(')');
    return os;
  }

  expr_node* atan_node::clone() const
  {
    atan_node* e = new atan_node(*e_atan);
    return e;
  }

  unsigned int atan_node::get_precedence() const
  {
    return precedence;
  }

  /*
    cosh_node
  */
  cosh_node::cosh_node(const expression& e) : e_cosh(e.get_root())
  {
    e_cosh->inc_refcount();
    GAOL_DEBUG(3,std::cout << "cosh_node created" << std::endl);
  }

  cosh_node::~cosh_node()
  {
    if (e_cosh->dec_refcount() == 0) {
      delete e_cosh;
    }
    GAOL_DEBUG(3,std::cout << "cosh_node destroyed" << std::endl);
  }

  std::ostream& cosh_node::display(std::ostream& os) const
  {
    os << "cosh(";
    e_cosh->display(os);
    os.put(')');
    return os;
  }

  expr_node* cosh_node::clone() const
  {
    cosh_node* e = new cosh_node(*e_cosh);
    return e;
  }

  unsigned int cosh_node::get_precedence() const
  {
    return precedence;
  }

  /*
    sinh_node
  */
  sinh_node::sinh_node(const expression& e) : e_sinh(e.get_root())
  {
    e_sinh->inc_refcount();
    GAOL_DEBUG(3,std::cout << "sinh_node created" << std::endl);
  }

  sinh_node::~sinh_node()
  {
    if (e_sinh->dec_refcount() == 0) {
      delete e_sinh;
    }
    GAOL_DEBUG(3,std::cout << "sinh_node destroyed" << std::endl);
  }

  std::ostream& sinh_node::display(std::ostream& os) const
  {
    os << "sinh(";
    e_sinh->display(os);
    os.put(')');
    return os;
  }

  expr_node* sinh_node::clone() const
  {
    sinh_node* e = new sinh_node(*e_sinh);
    return e;
  }

  unsigned int sinh_node::get_precedence() const
  {
    return precedence;
  }

  /*
    tanh_node
  */
  tanh_node::tanh_node(const expression& e) : e_tanh(e.get_root())
  {
    e_tanh->inc_refcount();
    GAOL_DEBUG(3,std::cout << "tanh_node created" << std::endl);
  }

  tanh_node::~tanh_node()
  {
    if (e_tanh->dec_refcount() == 0) {
      delete e_tanh;
    }
    GAOL_DEBUG(3,std::cout << "tanh_node destroyed" << std::endl);
  }

  std::ostream& tanh_node::display(std::ostream& os) const
  {
    os << "tanh(";
    e_tanh->display(os);
    os.put(')');
    return os;
  }

  expr_node* tanh_node::clone() const
  {
    tanh_node* e = new tanh_node(*e_tanh);
    return e;
  }

  unsigned int tanh_node::get_precedence() const
  {
    return precedence;
  }


  /*
    acosh_node
  */
  acosh_node::acosh_node(const expression& e) : e_acosh(e.get_root())
  {
    e_acosh->inc_refcount();
    GAOL_DEBUG(3,std::cout << "acosh_node created" << std::endl);
  }

  acosh_node::~acosh_node()
  {
    if (e_acosh->dec_refcount() == 0) {
      delete e_acosh;
    }
    GAOL_DEBUG(3,std::cout << "acosh_node destroyed" << std::endl);
  }

  std::ostream& acosh_node::display(std::ostream& os) const
  {
    os << "acosh(";
    e_acosh->display(os);
    os.put(')');
    return os;
  }

  expr_node* acosh_node::clone() const
  {
    acosh_node* e = new acosh_node(*e_acosh);
    return e;
  }

  unsigned int acosh_node::get_precedence() const
  {
    return precedence;
  }

  /*
    asinh_node
  */
  asinh_node::asinh_node(const expression& e) : e_asinh(e.get_root())
  {
    e_asinh->inc_refcount();
    GAOL_DEBUG(3,std::cout << "asinh_node created" << std::endl);
  }

  asinh_node::~asinh_node()
  {
    if (e_asinh->dec_refcount() == 0) {
      delete e_asinh;
    }
    GAOL_DEBUG(3,std::cout << "asinh_node destroyed" << std::endl);
  }

  std::ostream& asinh_node::display(std::ostream& os) const
  {
    os << "asinh(";
    e_asinh->display(os);
    os.put(')');
    return os;
  }

  expr_node* asinh_node::clone() const
  {
    asinh_node* e = new asinh_node(*e_asinh);
    return e;
  }

  unsigned int asinh_node::get_precedence() const
  {
    return precedence;
  }

  /*
    atanh_node
  */
  atanh_node::atanh_node(const expression& e) : e_atanh(e.get_root())
  {
    e_atanh->inc_refcount();
    GAOL_DEBUG(3,std::cout << "atanh_node created" << std::endl);
  }

  atanh_node::~atanh_node()
  {
    if (e_atanh->dec_refcount() == 0) {
      delete e_atanh;
    }
    GAOL_DEBUG(3,std::cout << "atanh_node destroyed" << std::endl);
  }

  std::ostream& atanh_node::display(std::ostream& os) const
  {
    os << "atanh(";
    e_atanh->display(os);
    os.put(')');
    return os;
  }

  expr_node* atanh_node::clone() const
  {
    atanh_node* e = new atanh_node(*e_atanh);
    return e;
  }

  unsigned int atanh_node::get_precedence() const
  {
    return precedence;
  }


  /*
    exp_node
  */
  exp_node::exp_node(const expression& e) : e_exp(e.get_root())
  {
    e_exp->inc_refcount();
    GAOL_DEBUG(3,std::cout << "exp_node created" << std::endl);
  }

  exp_node::~exp_node()
  {
    if (e_exp->dec_refcount() == 0) {
      delete e_exp;
    }
    GAOL_DEBUG(3,std::cout << "exp_node destroyed" << std::endl);
  }

  std::ostream& exp_node::display(std::ostream& os) const
  {
    os << "exp(";
    e_exp->display(os);
    os.put(')');
    return os;
  }

  expr_node* exp_node::clone() const
  {
    exp_node* e = new exp_node(*e_exp);
    return e;
  }

  unsigned int exp_node::get_precedence() const
  {
    return precedence;
  }

  /*
    log_node
  */
  log_node::log_node(const expression& e) : e_log(e.get_root())
  {
    e_log->inc_refcount();
    GAOL_DEBUG(3,std::cout << "log_node created" << std::endl);
  }

  log_node::~log_node()
  {
    if (e_log->dec_refcount() == 0) {
      delete e_log;
    }
    GAOL_DEBUG(3,std::cout << "log_node destroyed" << std::endl);
  }

  std::ostream& log_node::display(std::ostream& os) const
  {
    os << "log(";
    e_log->display(os);
    os.put(')');
    return os;
  }

  expr_node* log_node::clone() const
  {
    log_node* e = new log_node(*e_log);
    return e;
  }

  unsigned int log_node::get_precedence() const
  {
    return precedence;
  }

  /*
   * Construction operators --
   */

  const expression operator+(const expression& el, const expression& er)
  {
    return *(new add_node(el,er));
  }

  const expression operator-(const expression& e)
  {
    return *(new unary_minus_node(e));
  }

  const expression operator-(const expression& el, const expression& er)
  {
    return *(new sub_node(el,er));
  }

  const expression operator*(const expression& el, const expression& er)
  {
    return *(new mult_node(el,er));
  }

  const expression operator/(const expression& el, const expression& er)
  {
    return *(new div_node(el,er));
  }

  const expression pow(const expression& e, unsigned int n)
  {
    return *(new pow_node(e,n));
  }

  const expression pow(const expression& e1, const expression &e2)
  {
    return *(new pow_itv_node(e1,e2));
  }

  const expression nth_root(const expression& e, unsigned int n)
  {
    return *(new nth_root_node(e,n));
  }

  const expression cos(const expression& e)
  {
    return *(new cos_node(e));
  }
  const expression sin(const expression& e)
  {
    return *(new sin_node(e));
  }
  const expression tan(const expression& e)
  {
    return *(new tan_node(e));
  }

  const expression atan2(const expression& e1, const expression& e2)
  {
    return *(new atan2_node(e1,e2));
  }

  const expression acos(const expression& e)
  {
    return *(new acos_node(e));
  }
  const expression asin(const expression& e)
  {
    return *(new asin_node(e));
  }
  const expression atan(const expression& e)
  {
    return *(new atan_node(e));
  }

  const expression cosh(const expression& e)
  {
    return *(new cosh_node(e));
  }
  const expression sinh(const expression& e)
  {
    return *(new sinh_node(e));
  }
  const expression tanh(const expression& e)
  {
    return *(new tanh_node(e));
  }

  const expression acosh(const expression& e)
  {
    return *(new acosh_node(e));
  }
  const expression asinh(const expression& e)
  {
    return *(new asinh_node(e));
  }
  const expression atanh(const expression& e)
  {
    return *(new atanh_node(e));
  }

  const expression exp(const expression& e)
  {
    return *(new exp_node(e));
  }
  const expression log(const expression& e)
  {
    return *(new log_node(e));
  }

  bool evaluate_left_right(const expression& el, const expression& er,
			   interval* itv)
  {
    expr_eval eval;
    (el.get_root())->accept(eval);
    interval tmp_l = eval.result();
    if (eval.error_occurred()) {
      return false;
    }
    (er.get_root())->accept(eval);
    interval tmp_r = eval.result();
    if (eval.error_occurred()) {
      return false;
    }
    *itv = interval(tmp_l.left(),tmp_r.right());
    return true;
  }

  bool evaluate_left_right(const expression& e, interval* itv)
  {
    expr_eval eval;
    (e.get_root())->accept(eval);
    if (eval.error_occurred()) {
      return false;
    }
    *itv = eval.result();
    return true;

  }

  bool evaluate_expr(const expression& e, interval& itv)
  {
    expr_eval eval;
    (e.get_root())->accept(eval);
    if (eval.error_occurred()) {
      return false;
    }
    itv = eval.result();
    return true;
  }

} // namespace gaol
