/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_GAOL_Y_TAB_H_INCLUDED
# define YY_GAOL_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int gaol_debug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    EMPTY_STR = 258,
    INFINITY_STR = 259,
    DMIN_STR = 260,
    DMAX_STR = 261,
    PI_STR = 262,
    EXP_STR = 263,
    LOG_STR = 264,
    POW_STR = 265,
    NTH_ROOT_STR = 266,
    SQRT_STR = 267,
    COS_STR = 268,
    SIN_STR = 269,
    TAN_STR = 270,
    ATAN2_STR = 271,
    COSH_STR = 272,
    SINH_STR = 273,
    TANH_STR = 274,
    ACOS_STR = 275,
    ASIN_STR = 276,
    ATAN_STR = 277,
    ACOSH_STR = 278,
    ASINH_STR = 279,
    ATANH_STR = 280,
    UNEXPECTED_CHAR = 281,
    NUMBER = 282,
    INTERVAL_CST = 283,
    UMINUS = 284,
    UPLUS = 285
  };
#endif
/* Tokens.  */
#define EMPTY_STR 258
#define INFINITY_STR 259
#define DMIN_STR 260
#define DMAX_STR 261
#define PI_STR 262
#define EXP_STR 263
#define LOG_STR 264
#define POW_STR 265
#define NTH_ROOT_STR 266
#define SQRT_STR 267
#define COS_STR 268
#define SIN_STR 269
#define TAN_STR 270
#define ATAN2_STR 271
#define COSH_STR 272
#define SINH_STR 273
#define TANH_STR 274
#define ACOS_STR 275
#define ASIN_STR 276
#define ATAN_STR 277
#define ACOSH_STR 278
#define ASINH_STR 279
#define ATANH_STR 280
#define UNEXPECTED_CHAR 281
#define NUMBER 282
#define INTERVAL_CST 283
#define UMINUS 284
#define UPLUS 285

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 44 "gaol_interval_parser.ypp" /* yacc.c:1909  */

  int i;
  double d;
  Interval_struct itv;
  expr_node* expr;

#line 121 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE gaol_lval;

int gaol_parse (void);

#endif /* !YY_GAOL_Y_TAB_H_INCLUDED  */
