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
 * CVS: $Id: gaol_expr_visitor.h 54 2009-03-01 13:53:01Z goualard $
 * Last modified: 
 * By: 
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_expr_visitor.h
  \brief  Base classes to represent visitors manipulating interval expressions


  \author Frédéric Goualard
  \date   2001-10-30
*/


#ifndef __gaol_expr_visitor_h__
#define __gaol_expr_visitor_h__

namespace gaol {
  // Forward declarations
  class null_node;
  class double_node;
  class interval_node;
  class add_node;
  class unary_minus_node;
  class sub_node;
  class mult_node;
  class pow_node;
  class pow_itv_node;
  class nth_root_node;
  class div_node;
  class sin_node;
  class cos_node;
  class tan_node;
  class atan2_node;
  class asin_node;
  class acos_node;
  class atan_node;
  class sinh_node;
  class cosh_node;
  class tanh_node;
  class asinh_node;
  class acosh_node;
  class atanh_node;
  class exp_node;
  class log_node;

  /*!
    \brief Base class for all visitors that want to manipulate expressions
    
    A derived class can override only a subset of the visit() methods. It
    is then assumed that all the non-overrided ones are illegal in the
    context of this visitor. The error attribute is used to report such an
    error.
  */
  class expr_visitor {
  public:
    expr_visitor() {
      error = false;
    }
    virtual ~expr_visitor() {}
    bool error_occurred() const {
      return error;
    }
    void reset() {
      error = false;
    }
    virtual void visit(null_node* node) {
      error = true;
    }
    virtual void visit(double_node* node) {
      error = true;
    }
    virtual void visit(interval_node* node) {
      error = true;
    }
    virtual void visit(add_node* node) {
      error = true;
    }
    virtual void visit(unary_minus_node* node) {
      error = true;
    }
    virtual void visit(sub_node* node) {
      error = true;
    }
    virtual void visit(mult_node* node) {
      error = true;
    }
    virtual void visit(div_node* node) {
      error = true;
    }
    virtual void visit(pow_node* node) {
      error = true;
    }
    virtual void visit(pow_itv_node* node) {
      error = true;
    }
    virtual void visit(nth_root_node* node) {
      error = true;
    }
    virtual void visit(cos_node* node) {
      error = true;
    }
    virtual void visit(sin_node* node) {
      error = true;
    }
    virtual void visit(tan_node* node) {
      error = true;
    }
    virtual void visit(atan2_node* node) {
      error = true;
    }
    virtual void visit(acos_node* node) {
      error = true;
    }
    virtual void visit(asin_node* node) {
      error = true;
    }
    virtual void visit(atan_node* node) {
      error = true;
    }
    virtual void visit(cosh_node* node) {
      error = true;
    }
    virtual void visit(sinh_node* node) {
      error = true;
    }
    virtual void visit(tanh_node* node) {
      error = true;
    }
    virtual void visit(acosh_node* node) {
      error = true;
    }
    virtual void visit(asinh_node* node) {
      error = true;
    }
    virtual void visit(atanh_node* node) {
      error = true;
    }
    virtual void visit(log_node* node) {
      error = true;
    }
    virtual void visit(exp_node* node) {
      error = true;
    }
  protected:
    //! True if an error occurred during the last visit
    bool error;
  };

} // namespace gaol

#endif /* __gaol_expr_visitor_h__ */
