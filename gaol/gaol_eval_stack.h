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
 * CVS: $Id: gaol_eval_stack.h 54 2009-03-01 13:53:01Z goualard $
 * Last modified: 
 * By: 
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_eval_stack.h
  \brief  Stack for performing evaluation of expressions

  An eval_stack is a stack for which underflow is supposed never to occur,
  since the arity of the operators is known in advance.

  \author Frédéric Goualard
  \date   2001-10-20
*/


#ifndef __gaol_eval_stack_h__
#define __gaol_eval_stack_h__

#include "gaol/gaol_common.h"

namespace gaol {

  /*!
    \brief stack for evaluation of expressions
    
    \warning Overflows and underflows are reported only if copra was 
    compiled in debug mode 

    \param T type of the elements on the stack
    \param SZ size of the stack
  */
  template <typename T, unsigned int SZ = 8>
    class eval_stack {
      public:
      eval_stack();
      void push(const T& e);
      T pop();
      
      private:
      //! Next position available on the stack
      unsigned int next_pos;
      unsigned int size;
      T the_stack[SZ];
    };
  
  template<typename T, unsigned int SZ>
    eval_stack<T,SZ>::eval_stack()
    {
      size = SZ;
      next_pos = 0;
    }
  
  template<typename T, unsigned int SZ>
    void eval_stack<T,SZ>::push(const T& e)
    {
      the_stack[next_pos++] = T(e);
    }
  
  template<typename T, unsigned int SZ>
    T eval_stack<T,SZ>::pop()
    {
      /*
	Case of underflow: only happens if the type of the result of 
	the evaluation is not the expected one. We return 0, knowing that
	the visitor MUST somehow set the error variable to true.
      */
      if (next_pos == 0) {
	return the_stack[0];
      } else {
	return the_stack[--next_pos]; 
      }
    }
  
} // namespace gaol

#endif /* __gaol_eval_stack_h__ */
