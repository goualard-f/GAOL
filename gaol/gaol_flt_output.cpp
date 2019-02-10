/*--------------------------------------------------------------------------
 * gaol -- Just Another Interval Library
 *--------------------------------------------------------------------------
 * This file is part of the gaol distribution. Gaol was primarily 
 * developed at the Swiss Federal Institute of Technology, Lausanne, 
 * Switzerland, and is now developed at the Institut de Recherche 
 * en Informatique de Nantes, France.
 *
 * Copyright (c) 2001 Swiss Federal Institute of Technology, Switzerland
 * Copyright (c) 2002 Institut de Recherche en Informatique de Nantes, France
 *--------------------------------------------------------------------------
 * gaol is a software distributed WITHOUT ANY WARRANTY. Read the associated 
 * COPYING file for information.
 *--------------------------------------------------------------------------
 * CVS: $Id: gaol_flt_output.cpp 191 2012-03-06 17:08:58Z goualard $PhD
 * Last modified: Mon Apr 17 16:14:50 2006 on proust
 * By: Frédéric Goualard <Frederic.Goualard@univ-nantes.fr>
 *--------------------------------------------------------------------------*/

/*!
  \file   gaol_flt_output.cpp
  \brief  double to ASCII conversion


  Uses the dtoa() function written by Gay (see library gdtoa) to convert doubles 
  into ASCII strings while respecting the current rounding direction.

  \author Frederic Goualard
  \date   2002-04-16
*/


#include <iostream>
#include <cstdio>
#include <string.h>
#include "gdtoa/gdtoa.h"
extern int Flt_Rounds; // Defined in libgdtoa
#include "gaol/gaol_config.h"
#include "gaol/gaol_common.h"
#include "gaol/gaol_flt_output.h"

#if defined (_MSC_VER)
#  pragma warning( disable : 4996)
#endif


namespace gaol {

  /*!
  \brief Creates the ASCII representation of a number in fixed-point
  representation
  \param bufin the string of digits without a decimal point
  \param decpt the position of the decimal point
  \param sign the sign of the number: zero if positive and any non-zero
  value otherwise
  \param bufout the string representing the number in fixed-point notation
  \note bufout is owned by the caller. It is then assumed that enough space 
  has already been allocated to hold the result.
 */
void fp_fixed_point(char *bufin, int decpt, int sign, char *bufout)
{
  if (decpt > 0) {
    size_t lenbuf=strlen(bufin);
    char *bufout_as=bufout+1; // Position after the sign '+' or '-'
    bufout[0]=(sign==0 ? '+' : '-');
    if (decpt>=lenbuf) { // Should we add zeros?
      strncpy(bufout_as,bufin,lenbuf);
      memset(bufout_as+lenbuf,'0',decpt-lenbuf);
      bufout[lenbuf+decpt]='\0';
    } else { // Just add the decimal point
      strncpy(bufout_as,bufin,decpt);
      bufout[decpt+1]='.';
      bufout[decpt+2]='\0';
      //      std::cout << ">" << bufout << "<" << std::endl;
      strcat(bufout_as,bufin+decpt);
    }
  } else{
    decpt= -decpt;
    std::sprintf(bufout,"%c0.",(sign==0)?'+':'-');
    memset(bufout+3,'0',decpt);
    *(bufout+3+decpt)='\0';
    strcat(bufout,bufin);
  }
}

/*!
  \brief Creates the ASCII representation of a number in floating-point
  representation
  \param bufin the string of digits without a decimal point
  \param decpt the position of the decimal point
  \param sign the sign of the number: zero if positive and any non-zero
  value otherwise
  \param bufout the string representing the number in floating-point notation
  \note bufout is owned by the caller. It is then assumed that enough space 
  has already been allocated to hold the result.
 */
void fp_floating_point(char *bufin, int decpt, int sign, char *bufout)
{
  int expo=decpt-1;
  GAOL_DEBUG(1,std::cout << "bufin = " << bufin << ", decpt = " << decpt << ", sign = " << 
    sign << std::endl);
  std::sprintf(bufout,"%c%c.%se%s%d",(sign==0)?'+':'-',bufin[0],bufin+1,
	  (expo>-10 && expo<10)?((expo<0)?"-0":"+0"):((expo<0)?"-":"+"),expo);
}

/*!
  \brief Prints the double d to the stream os, respecting the current
  rounding direction (given by Flt_Round)  and number of digits to display
  \warning The string returned is owned by the function
*/
char *dtoa_down_or_up(double d, std::ostream& os)
{
  static char buffer[50]; // 50 should be enough to hold doubles !!
  int decpt,sign;
  char *buf;

  // 2 means: return max(1,ndigits) significant digits
  buf=dtoa(d,2,int(os.precision()),&decpt,&sign,NULL);
  if (decpt == 9999) { // Float is either +/-Infinity or NaN
    return buf;
  }
  
  // Printing in %g style
  // See the GNU libc doc for the meaning of the 'magic' -3 value 
  if (decpt < -3 || decpt-1 >= os.precision()) { // Should we print in %e style?
    fp_floating_point(buf,decpt,sign,buffer);
  } else { // Printing in %f style
    fp_fixed_point(buf, decpt, sign, buffer);
  }
  return buffer;
}

/*!
  \brief Prints the double d to the stream os with downward rounding for
  the conversion digits->ASCII
*/
char* dtoa_downward(double d, std::ostream& os)
{
  Flt_Rounds=3;
  return dtoa_down_or_up(d,os);
}

/*!
  \brief Prints the double d to the stream os with upward rounding for
  the conversion digits->ASCII
*/
char* dtoa_upward(double d, std::ostream& os)
{
  Flt_Rounds=2;
  return dtoa_down_or_up(d,os);
}



} // namespace gaol
