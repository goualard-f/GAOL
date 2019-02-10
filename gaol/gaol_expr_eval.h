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
 * CVS: $Id: gaol_expr_eval.h 54 2009-03-01 13:53:01Z goualard $
 * Last modified: 
 * By: 
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_expr_eval.h
  \brief  Evaluation of an interval represented internally via an expression

  <long description>

  \author Frédéric Goualard
  \date   2001-10-30
*/


#ifndef __gaol_expr_eval_h__
#define __gaol_expr_eval_h__

#include "gaol/gaol_eval_stack.h"
#include "gaol/gaol_interval.h"
#include "gaol/gaol_expression.h"
#include "gaol/gaol_expr_visitor.h"

namespace gaol {
  // Forward declarations
  /*!
    \brief Base class for all visitors that want to manipulate expressions
    
    A derived class can override only a subset of the visit() methods. It
    is then assumed that all the non-overrided ones are illegal in the
    context of this visitor. The error attribute is used to report such an
    error.
  */
  class expr_eval : public expr_visitor {
  public:
    virtual void visit(null_node* node) {
      error = true;
    }
    virtual void visit(double_node* node) {
      stack.push(node->get_val());
    }
    virtual void visit(interval_node* node) {
      stack.push(node->get_val());
    }
    virtual void visit(add_node* node) {
      (node->get_left())->accept(*this);
      interval l=stack.pop();
      (node->get_right())->accept(*this);
      interval r=stack.pop();
      stack.push(l+r);
    }
    virtual void visit(unary_minus_node* node) {
      (node->get_subexpr())->accept(*this);
      stack.push(-stack.pop());
    }
    virtual void visit(sub_node* node) {
      (node->get_left())->accept(*this);
      interval l=stack.pop();
      (node->get_right())->accept(*this);
      interval r=stack.pop();
      stack.push(l-r);
    }
    virtual void visit(mult_node* node) {
      (node->get_left())->accept(*this);
      interval l=stack.pop();
      (node->get_right())->accept(*this);
      interval r=stack.pop();
      stack.push(l*r);
    }
    virtual void visit(div_node* node) {
      (node->get_left())->accept(*this);
      interval l=stack.pop();
      (node->get_right())->accept(*this);
      interval r=stack.pop();
      stack.push(l/r);
    }
    virtual void visit(pow_node* node) {
      (node->get_subexpr())->accept(*this);
      stack.push(gaol::pow(stack.pop(),node->get_exponent()));
    }
    virtual void visit(pow_itv_node* node) {
      (node->get_left())->accept(*this);
      interval l = stack.pop();
      (node->get_right())->accept(*this);
      stack.push(gaol::pow(l,stack.pop()));
    }
    virtual void visit(nth_root_node* node) {
      (node->get_subexpr())->accept(*this);
      stack.push(gaol::nth_root(stack.pop(),node->get_exponent()));
    }
    virtual void visit(cos_node* node) {
      (node->get_subexpr())->accept(*this);
      stack.push(gaol::cos(stack.pop()));
    }
    virtual void visit(sin_node* node) {
      (node->get_subexpr())->accept(*this);
      stack.push(gaol::sin(stack.pop()));
    }
    virtual void visit(tan_node* node) {
      (node->get_subexpr())->accept(*this);
      stack.push(gaol::tan(stack.pop()));
    }
    virtual void visit(atan2_node* node) {
      (node->get_Y())->accept(*this);
      interval Y=stack.pop();
      (node->get_X())->accept(*this);
      stack.push(gaol::atan2(Y,stack.pop()));
    }
    virtual void visit(acos_node* node) {
      (node->get_subexpr())->accept(*this);
      stack.push(gaol::acos(stack.pop()));
    }
    virtual void visit(asin_node* node) {
      (node->get_subexpr())->accept(*this);
      stack.push(gaol::asin(stack.pop()));
    }
    virtual void visit(atan_node* node) {
      (node->get_subexpr())->accept(*this);
      stack.push(gaol::atan(stack.pop()));
    }
    virtual void visit(cosh_node* node) {
      (node->get_subexpr())->accept(*this);
      stack.push(gaol::cosh(stack.pop()));
    }
    virtual void visit(sinh_node* node) {
      (node->get_subexpr())->accept(*this);
      stack.push(gaol::sinh(stack.pop()));
    }
    virtual void visit(tanh_node* node) {
      (node->get_subexpr())->accept(*this);
      stack.push(gaol::tanh(stack.pop()));
    }
    virtual void visit(acosh_node* node) {
      (node->get_subexpr())->accept(*this);
      stack.push(gaol::acosh(stack.pop()));
    }
    virtual void visit(asinh_node* node) {
      (node->get_subexpr())->accept(*this);
      stack.push(gaol::asinh(stack.pop()));
    }
    virtual void visit(atanh_node* node) {
      (node->get_subexpr())->accept(*this);
      stack.push(gaol::atanh(stack.pop()));
    }
    virtual void visit(log_node* node) {
      (node->get_subexpr())->accept(*this);
      stack.push(gaol::log(stack.pop()));
    }
    virtual void visit(exp_node* node) {
      (node->get_subexpr())->accept(*this);
      stack.push(gaol::exp(stack.pop()));
    }
    interval result() {
      return stack.pop();
    }
  private:
    eval_stack<interval> stack;
  };

} // namespace gaol

#endif /* __gaol_expr_eval_h__ */
