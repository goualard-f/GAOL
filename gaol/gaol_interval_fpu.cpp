/*-*-C++-*------------------------------------------------------------------
 * gaol -- NOT Just Another Interval Library
 *--------------------------------------------------------------------------
 * This file is part of the gaol distribution. Gaol was primarily
 * developed at the Swiss Federal Institute of Technology, Lausanne,
 * Switzerland, and is now developed at the Laboratoire d'Informatique de
 * Nantes-Atlantique, France.
 *
 * Copyright (c) 2001 Swiss Federal Institute of Technology, Switzerland
 * Copyright (c) 2002-2009 Laboratoire d'Informatique de
 *                         Nantes-Atlantique, France
 *--------------------------------------------------------------------------
 * gaol is a software distributed WITHOUT ANY WARRANTY. Read the associated
 * COPYING file for information.
 *--------------------------------------------------------------------------
 * SVN: $Id: gaol_interval_fpu.cpp 247 2015-05-21 07:00:45Z goualard $
 * Last modified:
 * By: Frederic Goualard <Frederic.Goualard@univ-nantes.fr>
 *--------------------------------------------------------------------------*/

/*
    This file is not meant to be compiled. It is only to be included
        by gaol/gaol_interval.cpp.
*/


//#include "cycle.h"

  const interval interval::cst_emptyset(GAOL_NAN,GAOL_NAN);
  const interval interval::cst_universe(-GAOL_INFINITY,GAOL_INFINITY);
  const interval interval::cst_zero(0.0);
  const interval interval::cst_positive(0.0,GAOL_INFINITY);
  const interval interval::cst_negative(-GAOL_INFINITY,0.0);

  double hausdorff(const interval &I1, const interval &I2)
  {
    if (I1.is_empty() || I2.is_empty()) {
      return GAOL_NAN;
    } else {
      if (std::isinf(I1.left()) || std::isinf(I1.right()) || std::isinf(I2.left()) || std::isinf(I2.right())) {
	if (I1.set_eq(interval::universe()) && I2.set_eq(interval::universe())) {
	  // hausdorff([-oo, +oo],[-oo,+oo]) = 0
	  return 0.0;
	}	else {
	  return GAOL_INFINITY;
	}
      } else {
	return maximum(std::fabs(I1.left()-I2.left()),std::fabs(I1.right()-I2.right()));
      }
    }
  }

	interval uipow_dnup(const interval& I, unsigned int n)
	{
		double ly = 1.0, ry = 1.0;
		double lz2 = I.lb_, rz2 = I.rb_;
		double lz1 = -lz2;
		for (;;) {
			if (odd(n)) {
				ry *= rz2;
				ly = -(ly*lz2);
				if (n==1) { // On input, n is assumed to be non-null
					return interval(ly, ry);
				}
			}
			n >>= 1;
			rz2 *= rz2;
			lz2 *= lz1;
			lz1 = -lz2;
		}
	}

	interval uipow_upup(const interval& I, unsigned int n)
	{
		double ly = 1.0, ry = 1.0;

		double lz = I.lb_, rz = I.rb_;

		for (;;) {
			if (odd(n)) {
				ly *= lz;
				ry *= rz;
				if (n==1) { // On input, n is assumed to be non-null
					return interval(-ly,ry);
				}
			}
			n >>= 1;
			lz *= lz;
			rz *= rz;
		}
	}

	interval uipow(const interval& I, unsigned int e)
	{
		GAOL_RND_ENTER();
		interval res;

		int signI = ((I.rb_ < 0) << 1) |  (I.lb_ < 0);
		switch (signI) { // Remember that the sign of the left bound is negated
			case 0: // 00: I.straddles_zero()
			{
				res = uipow_upup(I,e);
				if (!odd(e)) {
					res = interval(0.0,fmax(res.lb_,res.rb_));
				}
				break;
			}
			case 1: // 01: I.positive()
			{
				res = uipow_dnup(I,e);
				break;
			}
			case 2: // 10: I.negative()
			{
				res = uipow_dnup(-I,e);
				if (odd(e)) {
					res = -res;
				}
				break;
			}
			case 3: // 11: I=[0, -0] (empty intervals have been filtered already)
			{
				res = interval(0.0);
				break;
			}
			// No other meaningful cases
		}

		GAOL_RND_LEAVE();
		return res;
	}





  interval& interval::operator+=(const interval& I)
  {
    GAOL_RND_ENTER();
		lb_ += I.lb_;
		rb_ += I.rb_;

    GAOL_RND_LEAVE();
    return *this;

  }


  interval& interval::operator-=(const interval& I)
  {
    GAOL_RND_ENTER();
		lb_ += I.rb_;
		rb_ += I.lb_;
    GAOL_RND_LEAVE();
    return *this;
  }


  interval& interval::operator+=(double d)
  {
    GAOL_RND_ENTER();
		lb_ = lb_-d;
		rb_ += d;
    GAOL_RND_LEAVE();
    return *this;
  }


  interval& interval::operator-=(double d)
  {
    GAOL_RND_ENTER();
		lb_ += d;
		rb_ -= d;
    GAOL_RND_LEAVE();
    return *this;
  }


  interval& interval::operator*=(double d)
  {
    if (is_empty()) {
      *this=interval::emptyset();
      return *this;
    }
    if (d==0.0) {
      *this=interval::zero();
      return *this;
    }

    if (d>0.0) {
			GAOL_RND_ENTER();
			lb_ *= d;
			rb_ *= d;
      GAOL_RND_LEAVE();
      return *this;
    } else {
			GAOL_RND_ENTER();
			double tmp = (-d)*rb_;
			rb_ = (-d)*lb_;
			lb_ = tmp;
      GAOL_RND_LEAVE();
      return *this;
    }
  }


  interval& interval::operator/=(double d)
  {
    if (is_empty()) {
      *this = interval::emptyset();
      return *this;
    }
    if (d == 0.0) {
      *this = interval::emptyset();
      return *this;
    }

    if (d > 0.0) {
		GAOL_RND_ENTER();
		lb_ /= d;
		rb_ /= d;
      	GAOL_RND_LEAVE();
      	return *this;
    } else { // d < 0.0
		GAOL_RND_ENTER();
		double tmp = rb_/(-d);
		rb_ = lb_/(-d);
		lb_ = tmp;
      	GAOL_RND_LEAVE();
      	return *this;
    }
  }


  interval& interval::operator%=(double d)
  {
    if (is_empty()) {
      *this = interval::emptyset();
      return *this;
    }

    if (d == 0.0) {
      if (straddles_zero()) {
		*this = interval::universe();
		return *this;
      } else { // this does not contain 0
		*this = interval::emptyset();
		return *this;
      }
    }


    if (d > 0.0) {
			GAOL_RND_ENTER();
			lb_ /= d;
			rb_ /= d;
      GAOL_RND_LEAVE();
      return *this;
    } else { // d < 0.0
			GAOL_RND_ENTER();
			double tmp = rb_/(-d);
			rb_ = lb_/(-d);
			lb_ = tmp;
      GAOL_RND_LEAVE();
      return *this;
    }
  }










  interval& interval::operator*=(const interval& I)
  {
    if (is_empty() || I.is_empty()) {
      *this=interval::emptyset();
      return *this;
    }

    if (is_zero()) {
      return *this;
    }
    if (I.is_zero()) {
      *this=I;
      return *this;
    }

		GAOL_RND_ENTER();
    /* Multiplication --
       Algorithm from:
       Interval Arithmetic: from Principles to Implementation
       Timothy J. Hickey, Qun Ju, and Maarten H. van Emden
       J. ACM 48:5, p. 1038--1068, sep. 2001
    */
    if (certainly_positive()) { // P ?
      if (I.certainly_positive()) { // P P
				lb_ *= -I.lb_;
				rb_ *= I.rb_;
      } else {
				if (I.strictly_straddles_zero()) { // P M
					lb_ = rb_*I.lb_;
					rb_ *= I.rb_;
				} else { // P N
					double tmp = rb_*I.lb_;
					rb_ = (-lb_)*I.rb_;
					lb_ = tmp;
				}
      }
    } else {
      if (strictly_straddles_zero()) { // M ?
				if (I.certainly_positive()) { // M P
					lb_ *= I.rb_;
					rb_ *= I.rb_;
				} else {
	  			if (I.strictly_straddles_zero()) { // M M
						double l1 = lb_*I.rb_;
						double l2 = rb_*I.lb_;
						double r1 = lb_*I.lb_;
						double r2 = rb_*I.rb_;
						lb_ = (l1 > l2) ? l1 : l2;
						rb_ = (r1 > r2) ? r1 : r2;
	  			} else { // M N
						double tmp = rb_*I.lb_;
						rb_ = lb_*I.lb_;
						lb_ = tmp;
	  			}
				}
      } else { // N  (case Z already tested)
				if (I.certainly_positive()) { // N P
					lb_ *= I.rb_;
					rb_ *= -I.lb_;
				} else {
	  			if (I.strictly_straddles_zero()) { // N M
						double tmp = lb_*I.rb_;
						rb_ = lb_*I.lb_;
						lb_ = tmp;
	  			} else { // N N
						double tmp = rb_*(-I.rb_);
						rb_ = lb_*I.lb_;
						lb_ = tmp;
	  			}
				}
      }
    }
    GAOL_RND_LEAVE();
    return *this;
  }


  interval interval::inverse() const
  {
    if (is_empty()) {
      return interval::emptyset();
    }

    interval res;
    GAOL_RND_ENTER();
    if (right() <= 0.0) { // (Z N0 N1)
      if (left() < 0) { // (N0 N1)
        if (right() < 0) { // N1
          res.lb_ = (-1.0)/rb_;
          res.rb_ = (-1.0)/lb_;
        } else { // N0
          res.lb_ = GAOL_INFINITY;
          res.rb_ = (-1.0)/lb_;
        }
      } else {  // Z
        res = interval::emptyset();
      }
    } else { // (M P0 P1)
      if (left() < 0) { // M
        res = interval::universe();
      } else { // (P0 P1)
        if (left() == 0.0) { // P0
          res.lb_ = (-1.0)/rb_;
          res.rb_ = GAOL_INFINITY;
        } else { // P1
          res.lb_ = (-1.0)/rb_;
          res.rb_ = (-1.0)/lb_;
        }
      }
    }
    GAOL_RND_LEAVE();
    return res;
  }

  interval& interval::operator/=(const interval& I)
  {
    if (is_empty() || I.is_empty()) {
      *this = interval::emptyset();
      return *this;
    }
    
    if (right() < 0.0) { // [this] N1
      if (I.right() < 0.0) { // [I] N1
		  GAOL_RND_ENTER();
		  double tmp = rb_/I.lb_;
		  rb_ = lb_/(-I.rb_);
		  lb_ = tmp;
		  GAOL_RND_LEAVE();
		  return *this; // N1 N1
      } else { // [I] P or Z
		  if (I.right() == 0.0) { // [I] N0 or Z
			 if ( I.left() == 0.0) { // [I] Z
				*this = interval::emptyset(); // N1 Z
				return *this;
			 }
			 GAOL_RND_ENTER();
			 lb_ = rb_/I.lb_;
			 rb_ = GAOL_INFINITY;
			 GAOL_RND_LEAVE();
			 return *this; // N1 N0
		  } else { // [I] P or M
			 if (I.left() < 0.0) { // [I] M
				*this = interval::universe(); // N1 M
				return *this;
			 } else { // [I] P0 or P1
				if (I.left() == 0.0) { // [I] P0
				  GAOL_RND_ENTER();
				  *this = interval(-GAOL_INFINITY,right()/I.right()); // N1 P0
				  GAOL_RND_LEAVE();
				  return *this;
				} else { // [I] P1
				  GAOL_RND_ENTER();
				  lb_ /= -I.lb_;
				  rb_ /= I.rb_;
				  GAOL_RND_LEAVE();
				  return *this; // N1 P1
				}
			 }
		  }
      }
    } else { // [this] N0, Z, M, or P
      if (right() == 0.0) { // [this] N0 or Z
		  if (left() == 0.0) { // [this] Z
			 if (I.left() == 0.0 && I.right() == 0.0) {
				*this = interval::emptyset(); // Z Z
				return *this;
			 } else {
				*this = interval::zero(); // Z (N,M,P)
				return *this;
			 }
		  } else { // [this] N0
			 if (I.right() < 0.0) { // [I] N1
				GAOL_RND_ENTER();
				*this = interval(0.0,left()/I.right()); // N0 N1
				GAOL_RND_LEAVE();
				return *this;
			 } else { // [I] P or Z
				if (I.right() == 0.0) { // [I] N0 or Z
				  if (I.left() == 0.0) { // [I] Z
					 *this = interval::emptyset(); // N0 Z
					 return *this;
				  }
				  *this = interval(0.0,GAOL_INFINITY); // N0 N0
				  return *this;
				} else { // [I] P or M
				  if (I.left() < 0.0) { // [I] M
					 *this = interval::universe(); // N0 M
					 return *this;
				  } else { // [I] P0 or P1
					 if (I.left() == 0.0) { // [I] P0
						*this = interval(-GAOL_INFINITY,0.0); // N0 P0
						return *this;
					 } else { // [I] P1
						GAOL_RND_ENTER();
						lb_ /= (-I.lb_);
						rb_ = 0.0;
						GAOL_RND_LEAVE();
						return *this; // N0 P1
					 }
				  }
				}
			 }
		  }
      } else { // [this] M, P0, or P1
		  if (left() < 0.0) { // [this] M
			 if (I.right() < 0.0) { // [I] N1
				GAOL_RND_ENTER();
				double tmp = rb_/(-I.rb_);
				rb_ = lb_/(-I.rb_);
				lb_ = tmp;
				GAOL_RND_LEAVE();
				return *this; // M N1
			 } else { // [I] P or Z
				if (I.right() == 0.0) { // [I] N0 or Z
				  if (I.left() == 0.0) { // [I] Z
					 *this = interval::emptyset(); // M Z
					 return *this;
				  }
				  *this = interval::universe(); // M N0
				  return *this;
				} else { // [I] P or M
				  if (I.left() < 0.0) { // [I] M
					 *this = interval::universe(); // M M
					 return *this;
				  } else { // [I] P0 or P1
					 if (I.left() == 0.0) { // [I] P0
						*this = interval::universe(); // M P0
						return *this;
					 } else { // [I] P1
						GAOL_RND_ENTER();
						lb_ = lb_/(-I.lb_);
						rb_ /= -I.lb_;
						GAOL_RND_LEAVE();
						return *this; // M P1
					 }
				  }
				}
			 }
		  } else { // [this] P0 or P1
			 if (left() == 0.0) { // [this] P0
				if (I.right() < 0.0) { // [I] N1
				  GAOL_RND_ENTER();
				  lb_ = rb_/(-I.rb_);
				  rb_ = 0.0;
				  GAOL_RND_LEAVE();
				  return *this; // P0 N1
				} else { // [I] P or Z
				  if (I.right() == 0.0) { // [I] N0 or Z
					 if (I.left() == 0.0) { // [I] Z
						*this = interval::emptyset(); // P0 Z
						return *this;
					 }
					 lb_ = GAOL_INFINITY;
					 rb_ = 0.0;
					 return *this; // P0 N0
				  } else { // [I] P or M
					 if (I.left() < 0.0) { // [I] M
						*this = interval::universe(); // P0 M
						return *this;
					 } else { // [I] P0 or P1
						if (I.left() == 0.0) { // [I] P0
						  lb_ = 0.0;
						  rb_ = GAOL_INFINITY;
						  return *this; // P0 P0
						} else { // [I] P1
						  GAOL_RND_ENTER();
						  *this = interval(0.0,right()/I.left()); // P0 P1
						  GAOL_RND_LEAVE();
						  return *this;
						}
					 }
				  }
				}
			 } else { // [this] P1
				if (I.right() < 0.0) { // [I] N1
				  GAOL_RND_ENTER();
				  double tmp = (-rb_)/I.rb_;
				  rb_ = lb_/I.lb_;
				  lb_ = tmp;
				  GAOL_RND_LEAVE();
				  return *this; // P1 N1
				} else { // [I] P or Z
				  if (I.right() == 0.0) { // [I] N0 or Z
					 if (I.left() == 0.0) { // [I] Z
						*this = interval::emptyset(); // P1 Z
						return *this;
					 }
					 GAOL_RND_ENTER();
					 rb_ = left()/I.left(); // beware: rb_ uses left()
					 lb_ = GAOL_INFINITY;
					 GAOL_RND_LEAVE();
					 return *this; // P1 N0
				  } else { // [I] P or M
					 if (I.left() < 0.0) { // [I] M
						*this = interval::universe(); // P1 M
						return *this;
					 } else { // [I] P0 or P1
						if (I.left() == 0.0) { // [I] P0
						  GAOL_RND_ENTER();
						  lb_ /= I.rb_;
						  rb_ = GAOL_INFINITY;
						  GAOL_RND_LEAVE();
						  return *this; // P1 P0
						} else { // [I] P1
						  GAOL_RND_ENTER();
						  lb_ /= I.rb_;
						  rb_ /=  -I.lb_;
						  GAOL_RND_LEAVE();
						  return *this; // P1 P1
						}
					 }
				  }
				}
			 }
		  }
      }
    }
  }

  interval& interval::operator%=(const interval& I)
  {
    if (is_empty() || I.is_empty()) {
      *this = interval::emptyset();
      return *this;
    }


    if (right() < 0.0) { // [this] N1
      if (I.right() < 0.0) { // [I] N1
				GAOL_RND_ENTER();
				double tmp = rb_/I.lb_;
				rb_ = lb_/(-I.rb_);
				lb_ = tmp;
				GAOL_RND_LEAVE();
				return *this; // N1 N1
      } else { // [I] P or Z
				if (I.right() == 0.0) { // [I] N0 or Z
	  			if ( I.left() == 0.0) { // [I] Z
	    			*this = interval::emptyset(); // N1 Z
	    			return *this;
	  			}
					GAOL_RND_ENTER();
					lb_ = rb_/I.lb_;
					rb_ = GAOL_INFINITY;
	  			GAOL_RND_LEAVE();
	  			return *this; // N1 N0
				} else { // [I] P or M
	  			if (I.left() < 0.0) { // [I] M
	    			*this = interval::universe(); // N1 M
	    			return *this;
	  			} else { // [I] P0 or P1
	    			if (I.left() == 0.0) { // [I] P0
							GAOL_RND_ENTER();
							*this = interval(-GAOL_INFINITY,right()/I.right()); // N1 P0
	      			GAOL_RND_LEAVE();
	      			return *this;
	    			} else { // [I] P1
							GAOL_RND_ENTER();
							lb_ = lb_/(-I.lb_);
							rb_ /= I.rb_;
	      			GAOL_RND_LEAVE();
	      			return *this; // N1 P1
	    			}
	  			}
				}
      }
    } else { // [this] N0, Z, M, or P
      if (right() == 0.0) { // [this] N0 or Z
				if (left() == 0.0) { // [this] Z
	  			if (I.straddles_zero()) {
	    			*this = interval::universe(); // Z (N0, M, P0)
	    			return *this;
	  			} else {
	    			*this = interval::zero(); // Z (N1, P1)
	    			return *this;
	  			}
				} else { // [this] N0
	  			if (I.right() < 0.0) { // [I] N1
						GAOL_RND_ENTER();
						*this = interval(0.0,left()/I.right()); // N0 N1
	    			GAOL_RND_LEAVE();
	    			return *this;
	  			} else { // [I] P or Z
	    			if (I.right() == 0.0) { // [I] N0 or Z
	      			if (I.left() == 0.0) { // [I] Z
								*this = interval::universe(); // N0 Z
								return *this;
	      			}
	      			*this = interval::universe(); // N0 N0
	      			return *this;
	    			} else { // [I] P or M
	      			if (I.left() < 0.0) { // [I] M
								*this = interval::universe(); // N0 M
								return *this;
	      			} else { // [I] P0 or P1
								if (I.left() == 0.0) { // [I] P0
		  						*this = interval::universe(); // N0 P0
		  						return *this;
								} else { // [I] P1
									GAOL_RND_ENTER();
									lb_ = lb_/(-I.lb_);
									rb_ = 0.0;
								  GAOL_RND_LEAVE();
		  						return *this; // N0 P1
								}
	      			}
	    			}
	  			}
				}
      } else { // [this] M, P0, or P1
				if (left() < 0.0) { // [this] M
	  			if (I.right() < 0.0) { // [I] N1
						GAOL_RND_ENTER();
						double tmp = rb_/(-I.rb_);
						rb_ = lb_/(-I.rb_);
						lb_ = tmp;
	    			GAOL_RND_LEAVE();
	    			return *this; // M N1
	  			} else { // [I] P or Z
	    			if (I.right() == 0.0) { // [I] N0 or Z
	      			*this = interval::universe(); // M N0
	      			return *this;
	    			} else { // [I] P or M
	      			if (I.left() < 0.0) { // [I] M
								*this = interval::universe(); // M M
								return *this;
	      			} else { // [I] P0 or P1
								if (I.left() == 0.0) { // [I] P0
		  						*this = interval::universe(); // M P0
		  						return *this;
								} else { // [I] P1
									GAOL_RND_ENTER();
									lb_ = lb_/(-I.lb_);
									rb_ = rb_/(-I.lb_);
		  						GAOL_RND_LEAVE();
		  						return *this; // M P1
								}
	      			}
	    			}
	  			}
				} else { // [this] P0 or P1
	  			if (left() == 0.0) { // [this] P0
	    			if (I.right() < 0.0) { // [I] N1
							GAOL_RND_ENTER();
							lb_ = rb_/(-I.rb_);
							rb_ = 0.0;
	      			GAOL_RND_LEAVE();
	      			return *this; // P0 N1
	    			} else { // [I] P or Z
	      			if (I.right() == 0.0) { // [I] N0 or Z
								*this = interval::universe(); // P0 Z
								return *this; // P0 N0
	      			} else { // [I] P or M
								if (I.left() < 0.0) { // [I] M
		  						*this = interval::universe(); // P0 M
		  						return *this;
								} else { // [I] P0 or P1
		  						if (I.left() == 0.0) { // [I] P0
		    						*this = interval::universe();
		    						return *this;
		  						} else { // [I] P1
										GAOL_RND_ENTER();
										*this = interval(0.0,right()/I.left()); // P0 P1
		    						GAOL_RND_LEAVE();
		    						return *this;
		  						}
								}
	      			}
	    			}
	  			} else { // [this] P1
	    			if (I.right() < 0.0) { // [I] N1
							GAOL_RND_ENTER();
							double tmp = rb_/(-I.rb_);
							rb_ = lb_/I.lb_;
							lb_ = tmp;
				      GAOL_RND_LEAVE();
	      			return *this; // P1 N1
	    			} else { // [I] P or Z
	      			if (I.right() == 0.0) { // [I] N0 or Z
								if (I.left() == 0.0) { // [I] Z
		  						*this = interval::emptyset(); // P1 Z
		  						return *this;
								}
								GAOL_RND_ENTER();
								rb_ = left()/I.left(); // beware: rb_ uses lb_
								lb_ = GAOL_INFINITY;
								GAOL_RND_LEAVE();
								return *this; // P1 N0
	      			} else { // [I] P or M
								if (I.left() < 0.0) { // [I] M
		  						*this = interval::universe(); // P1 M
		  						return *this;
								} else { // [I] P0 or P1
		  						if (I.left() == 0.0) { // [I] P0
										GAOL_RND_ENTER();
										lb_ = lb_/I.rb_;
										rb_ = GAOL_INFINITY;
								    GAOL_RND_LEAVE();
		    						return *this; // P1 P0
		  						} else { // [I] P1
										GAOL_RND_ENTER();
										lb_ = lb_/I.rb_;
										rb_ /= -I.lb_;
								    GAOL_RND_LEAVE();
		    						return *this; // P1 P1
		  						}
								}
	      			}
	    			}
	  			}
				}
      }
    }
  }

  interval div_rel(const interval &K, const interval &J, const interval &I)
  {
    if (K.is_empty() || J.is_empty() || I.is_empty()) {
      return interval::emptyset();
    }
    
    if (K.right() < 0.0) { // [this] N1
      if (J.right() < 0.0) { // [J] N1
	// I & [K.r/J.l, K.l/J.r]
	GAOL_RND_ENTER();
	interval tmp(-(K.rb_/J.lb_),K.lb_/(-J.rb_));
	GAOL_RND_LEAVE();
	return I & tmp; // N1 N1
      } else { // [J] P or Z
	if (J.right() == 0.0) { // [J] N0 or Z
	  if (J.left() == 0.0) { // [J] Z
	    return interval::emptyset(); // N1 Z
	  }
	  // I & [K.r/J.l,+oo]
	  GAOL_RND_ENTER();
	  interval tmp(-(K.rb_/J.lb_),GAOL_INFINITY);
	  GAOL_RND_LEAVE();
	  return I & tmp; // N1 N0
	} else { // [I] P or M
	  if (J.left() < 0.0) { // [J] M
	    GAOL_RND_ENTER();
	    interval tmp(-GAOL_INFINITY,K.rb_/J.rb_);
	    interval tmp2(K.rb_/(-J.lb_),GAOL_INFINITY);
	    GAOL_RND_LEAVE();
	    return (I&tmp) | (I&tmp2); // N1 M
	  } else { // [J] P0 or P1
	    if (J.left() == 0.0) { // [J] P0
	      GAOL_RND_ENTER();
	      interval tmp2 = I & interval(-GAOL_INFINITY,K.right()/J.right()); // N1 P0
	      GAOL_RND_LEAVE();
	      return tmp2; // N1 P0
	    } else { // [J] P1
	      GAOL_RND_ENTER();
	      interval tmp(-(K.lb_/(-J.lb_)),K.rb_/J.rb_);
	      GAOL_RND_LEAVE();
	      return I & tmp; // N1 P1
	    }
	  }
	}
      }
    } else { // [this] N0, Z, M, or P
      if (K.right() == 0.0) { // [this] N0 or Z
	if (K.left() == 0.0) { // [this] Z
	  if (J.straddles_zero()) {
	    return I; // Z (N0, M, P0)
	  } else {
	    return I & interval::zero(); // Z (N1, P1)
	  }
	} else { // [this] N0
	  if (J.right() < 0.0) { // [J] N1
	    GAOL_RND_ENTER();
	    interval tmp2 = I & interval(0.0,K.left()/J.right()); // N0 N1
	    GAOL_RND_LEAVE();
	    return tmp2;
	  } else { // [J] P or Z
	    if (J.right() == 0.0) { // [J] N0 or Z
	      if (J.left() == 0.0) { // [J] Z
		return I; // N0 Z
	      }
	      return I; // N0 N0
	    } else { // [J] P or M
	      if (J.left() < 0.0) { // [J] M
		return I; // N0 M
	      } else { // [J] P0 or P1
		if (J.left() == 0.0) { // [J] P0
		  return I; // N0 P0
		} else { // [J] P1
		  GAOL_RND_ENTER();
		  interval tmp(-(K.lb_/(-J.lb_)),0.0);
		  GAOL_RND_LEAVE();
		  return I & tmp; // N0 P1
		}
	      }
	    }
	  }
	}
      } else { // [this] M, P0, or P1
	if (K.left() < 0.0) { // [this] M
	  if (J.right() < 0.0) { // [J] N1
	    GAOL_RND_ENTER();
	    interval tmp(-(K.rb_/(-J.rb_)),K.lb_/(-J.rb_));
	    GAOL_RND_LEAVE();
	    return I & tmp; // M N1
	  } else { // [J] P or Z
	    if (J.right() == 0.0) { // [J] N0 or Z
	      return I; // M N0
	    } else { // [J] P or M
	      if (J.left() < 0.0) { // [J] M
		return I; // M M
	      } else { // [J] P0 or P1
		if (J.left() == 0.0) { // [J] P0
		  return I; // M P0
		} else { // [J] P1
		  GAOL_RND_ENTER();
		  interval tmp(-(K.lb_/(-J.lb_)),K.rb_/(-J.lb_));
		  GAOL_RND_LEAVE();
		  return I & tmp; // M P1
		}
	      }
	    }
	  }
	} else { // [this] P0 or P1
	  if (K.left() == 0.0) { // [this] P0
	    if (J.right() < 0.0) { // [J] N1
	      GAOL_RND_ENTER();
	      interval tmp(-(K.rb_/(-J.rb_)),0.0);
	      GAOL_RND_LEAVE();
	      return I & tmp; // P0 N1
	    } else { // [J] P or Z
	      if (J.right() == 0.0) { // [J] N0 or Z
		return I; // P0 (Z or N0)
	      } else { // [J] P or M
		if (J.left() < 0.0) { // [J] M
		  return I; // P0 M
		} else { // [J] P0 or P1
		  if (J.left() == 0.0) { // [J] P0
		    return I; // P0 P0
		  } else { // [J] P1
		    GAOL_RND_ENTER();
		    interval tmp2 = I & interval(0.0,K.right()/J.left()); // P0 P1
		    GAOL_RND_LEAVE();
		    return tmp2;
		  }
		}
	      }
	    }
	  } else { // [this] P1
	    if (J.right() < 0.0) { // [J] N1
	      GAOL_RND_ENTER();
	      interval tmp(-(K.rb_/(-J.rb_)),K.lb_/J.lb_);
	      GAOL_RND_LEAVE();
	      return I & tmp; // P1 N1
	    } else { // [J] P or Z
	      if (J.right() == 0.0) { // [J] N0 or Z
		if (J.left() == 0.0) { // [J] Z
		  return interval::emptyset(); // P1 Z
		}
		GAOL_RND_ENTER();
		interval tmp2 = I & interval(-GAOL_INFINITY,K.lb_/J.lb_); // P1 N0
		GAOL_RND_LEAVE();
		return tmp2;
	      } else { // [J] P or M
		if (J.left() < 0.0) { // [J] M
		  GAOL_RND_ENTER();
		  interval tmp(-GAOL_INFINITY,K.lb_/J.lb_);
		  interval tmp2 = interval(-(K.lb_/J.rb_),GAOL_INFINITY);
		  GAOL_RND_LEAVE();
		  return (I&tmp) | (I&tmp2); // P1 M
		} else { // [J] P0 or P1
		  if (J.left() == 0.0) { // [J] P0
		    GAOL_RND_ENTER();
		    interval tmp(-(K.lb_/J.rb_),GAOL_INFINITY);
		    GAOL_RND_LEAVE();
		    return I & tmp; // P1 P0
		  } else { // [J] P1
		    GAOL_RND_ENTER();
		    interval tmp(-(K.lb_/J.rb_),K.rb_/(-J.lb_));
		    GAOL_RND_LEAVE();
		    return I & tmp; // P1 P1
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }


  interval sqr(const interval& I)
  {
    if (I.is_empty()) {
      return interval::emptyset();
    }

    if (I.certainly_positive()) {
		GAOL_RND_ENTER();
		double l = I.lb_*(-I.lb_);
		double r = I.rb_*I.rb_;
      	GAOL_RND_LEAVE();
        return interval(-l,r);
    }

    if (I.certainly_negative()) {
		GAOL_RND_ENTER();
		double l = I.rb_*(-I.rb_);
		double r = I.lb_*I.lb_;
      	GAOL_RND_LEAVE();
        return interval(-l,r);
    }

    if (fabs(I.left()) >= fabs(I.right())) {
		GAOL_RND_ENTER();
		interval tmp = interval(0.0,I.left()*I.left());
        GAOL_RND_LEAVE();
        return tmp;
    } else {
		GAOL_RND_ENTER();
		interval tmp = interval(0.0,I.right()*I.right());
        GAOL_RND_LEAVE();
        return tmp;
    }
  }


  interval interval::mid() const
  {
    if (is_empty()) {
      return interval::emptyset();
    }
    if (is_symmetric()) { // symmetric case handles [-oo, +oo]
      return 0.0;
    }
    if (left() == -GAOL_INFINITY) {
      return interval(-std::numeric_limits<double>::max());
    }
    if (right() == GAOL_INFINITY) {
      return interval(std::numeric_limits<double>::max());
    }
    GAOL_RND_ENTER();
    double mid_left  = -(.5*lb_ - .5*rb_);
    double mid_right = (-.5*lb_) + .5*rb_;
    GAOL_RND_LEAVE();
    return interval(mid_left,mid_right);
  }


