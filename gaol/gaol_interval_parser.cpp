/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         gaol_parse
#define yylex           gaol_lex
#define yyerror         gaol_error
#define yydebug         gaol_debug
#define yynerrs         gaol_nerrs

#define yylval          gaol_lval
#define yychar          gaol_char

/* Copy the first part of user declarations.  */
#line 1 "gaol_interval_parser.ypp" /* yacc.c:339  */

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
 * CVS: $Id: gaol_interval_parser.ypp 191 2012-03-06 17:08:58Z goualard $
 * Last modified:
 * By:
 *--------------------------------------------------------------------------*/

#include <cstdarg>
#include <cstdlib>
#include <cmath>
#include <string>
#include "gaol/gaol_common.h"
#include "gaol/gaol_interval.h"
#include "gaol/gaol_expression.h"
#include "gaol/gaol_limits.h"
#include "gaol/gaol_exceptions.h"

#undef yyerror
#define yyerror gaol_error_bison

int gaol_error_bison(char *s ...);
int gaol_lex(void);

using namespace gaol;
extern interval *gaol_result_of_parsing;
extern interval gaol_tmp_itv;
extern bool gaol_global_parsing_flag;

#line 117 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
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
#line 44 "gaol_interval_parser.ypp" /* yacc.c:355  */

  int i;
  double d;
  Interval_struct itv;
  expr_node* expr;

#line 224 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE gaol_lval;

int gaol_parse (void);

#endif /* !YY_GAOL_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 241 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  85
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   574

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  7
/* YYNRULES -- Number of rules.  */
#define YYNRULES  67
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  235

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   285

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      35,    36,    31,    29,    37,    30,     2,    32,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      40,     2,    41,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    38,     2,    39,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    33,    34
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    74,    74,    78,    79,    81,    83,    85,    87,    89,
      90,    91,    96,    98,   100,   102,   105,   107,   109,   111,
     113,   115,   117,   119,   121,   123,   125,   127,   130,   132,
     145,   152,   158,   164,   172,   177,   191,   193,   195,   197,
     199,   201,   203,   205,   207,   209,   211,   213,   214,   215,
     219,   221,   223,   225,   227,   229,   231,   233,   235,   237,
     239,   241,   243,   245,   247,   249,   281,   284
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "EMPTY_STR", "INFINITY_STR", "DMIN_STR",
  "DMAX_STR", "PI_STR", "EXP_STR", "LOG_STR", "POW_STR", "NTH_ROOT_STR",
  "SQRT_STR", "COS_STR", "SIN_STR", "TAN_STR", "ATAN2_STR", "COSH_STR",
  "SINH_STR", "TANH_STR", "ACOS_STR", "ASIN_STR", "ATAN_STR", "ACOSH_STR",
  "ASINH_STR", "ATANH_STR", "UNEXPECTED_CHAR", "NUMBER", "INTERVAL_CST",
  "'+'", "'-'", "'*'", "'/'", "UMINUS", "UPLUS", "'('", "')'", "','",
  "'['", "']'", "'<'", "'>'", "$accept", "itv_expr", "itv_expr_aux",
  "itv_function_call", "parsed_interval", "expression", "function_call", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,    43,
      45,    42,    47,   284,   285,    40,    41,    44,    91,    93,
      60,    62
};
# endif

#define YYPACT_NINF -27

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-27)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     154,   -27,   -27,   -27,   -27,   -27,    -2,     8,    10,    11,
      16,    17,    23,    24,    34,    35,    39,    44,    65,    66,
      67,    68,    69,    70,   -27,   -27,   154,   154,   154,   224,
     256,    40,   -24,   -27,   -27,    46,   -27,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   -27,   -27,   -27,   -27,   119,
     189,    14,    71,    90,    91,    92,    94,   117,   118,   145,
     155,   169,   171,   173,   178,   187,   188,   215,   220,   221,
     256,   256,   256,     5,    31,   -27,   154,   154,   154,   154,
     256,   256,   256,   256,   258,   266,   274,   282,   106,   110,
     156,   166,   290,   298,   306,   314,   322,   330,   338,   346,
     170,   180,   354,   362,   370,   378,   386,   394,   402,   410,
     418,   426,   434,   442,   450,   458,   466,   474,   482,   490,
     -27,   -27,   -27,   256,   256,   256,   256,   256,   256,   256,
     256,   256,   256,   256,   256,   256,   256,   256,   256,   256,
     256,   -27,   -27,   189,   256,   -27,   256,   -22,   -22,   -27,
     -27,     7,     7,   -27,   -27,   -27,   -27,   -27,   -27,   154,
     256,   154,   256,   -27,   -27,   -27,   -27,   -27,   -27,   -27,
     -27,   154,   256,   -27,   -27,   -27,   -27,   -27,   -27,   -27,
     -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,
     -27,   266,   282,   110,   166,   298,   314,   330,   346,   180,
     362,   378,   394,   410,   426,   442,   458,   474,   490,    18,
       0,   498,   506,   514,   522,   530,   538,   -27,   -27,   -27,
     -27,   -27,   -27,   -27,   -27
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    31,    41,    37,    38,    39,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    36,    40,     0,     0,     0,     0,
       0,     0,     2,    10,     3,    30,    48,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     9,    30,     8,    30,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     1,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      11,    49,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    47,    46,     0,     0,    32,     0,     4,     5,     6,
       7,    42,    43,    44,    45,    25,    63,    26,    64,     0,
       0,     0,     0,    28,    66,    12,    50,    13,    51,    14,
      52,     0,     0,    19,    57,    20,    58,    21,    59,    16,
      54,    17,    55,    18,    56,    22,    60,    23,    61,    24,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    33,    35,    27,
      65,    29,    67,    15,    53
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -27,   -27,    45,   -27,   -27,   -26,   -27
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    31,    32,    33,    34,    35,    36
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      56,    58,    60,    83,    84,    86,    87,    88,    89,    88,
      89,    95,    97,    99,   101,   103,   105,   107,   109,   111,
     113,   115,   117,   119,   121,   123,   125,   127,   129,    90,
      91,    92,    93,    37,    90,    91,    92,    93,    92,    93,
      85,   228,   154,    38,   155,    39,    40,    90,    91,    92,
      93,    41,    42,   132,   151,   152,   153,   227,    43,    44,
      90,    91,    92,    93,   161,   162,   163,   164,   156,    45,
      46,    55,    57,    59,    47,    90,    91,    92,    93,    48,
       0,     0,    94,    96,    98,   100,   102,   104,   106,   108,
     110,   112,   114,   116,   118,   120,   122,   124,   126,   128,
      49,    50,    51,    52,    53,    54,   133,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   134,   135,   136,   219,   137,
     220,   157,   158,   159,   160,    86,    87,    88,    89,    90,
      91,    92,    93,   169,   222,     0,   224,   170,    86,    87,
      88,    89,   138,   139,     0,   130,   226,     1,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
     140,    24,    25,    26,    27,    86,    87,    88,    89,    28,
     141,     0,    29,   171,    30,    90,    91,    92,    93,    86,
      87,    88,    89,   172,   142,     0,   143,   181,   144,    90,
      91,    92,    93,   145,   221,     0,   223,   182,    90,    91,
      92,    93,   146,   147,     0,   131,   225,    61,     2,     3,
       4,     5,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
     148,    24,    25,    80,    81,   149,   150,     0,     0,    82,
       2,     3,     4,     5,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,     0,    24,    25,    80,    81,    86,    87,    88,
      89,    82,     0,     0,   165,    90,    91,    92,    93,     0,
       0,     0,   166,    86,    87,    88,    89,     0,     0,     0,
     167,    90,    91,    92,    93,     0,     0,     0,   168,    86,
      87,    88,    89,     0,     0,     0,   173,    90,    91,    92,
      93,     0,     0,     0,   174,    86,    87,    88,    89,     0,
       0,     0,   175,    90,    91,    92,    93,     0,     0,     0,
     176,    86,    87,    88,    89,     0,     0,     0,   177,    90,
      91,    92,    93,     0,     0,     0,   178,    86,    87,    88,
      89,     0,     0,     0,   179,    90,    91,    92,    93,     0,
       0,     0,   180,    86,    87,    88,    89,     0,     0,     0,
     183,    90,    91,    92,    93,     0,     0,     0,   184,    86,
      87,    88,    89,     0,     0,     0,   185,    90,    91,    92,
      93,     0,     0,     0,   186,    86,    87,    88,    89,     0,
       0,     0,   187,    90,    91,    92,    93,     0,     0,     0,
     188,    86,    87,    88,    89,     0,     0,     0,   189,    90,
      91,    92,    93,     0,     0,     0,   190,    86,    87,    88,
      89,     0,     0,     0,   191,    90,    91,    92,    93,     0,
       0,     0,   192,    86,    87,    88,    89,     0,     0,     0,
     193,    90,    91,    92,    93,     0,     0,     0,   194,    86,
      87,    88,    89,     0,     0,     0,   195,    90,    91,    92,
      93,     0,     0,     0,   196,    86,    87,    88,    89,     0,
       0,     0,   197,    90,    91,    92,    93,     0,     0,     0,
     198,    86,    87,    88,    89,     0,     0,     0,   199,    90,
      91,    92,    93,     0,     0,     0,   200,    86,    87,    88,
      89,     0,     0,     0,   229,    90,    91,    92,    93,     0,
       0,     0,   230,    86,    87,    88,    89,     0,     0,     0,
     231,    90,    91,    92,    93,     0,     0,     0,   232,    86,
      87,    88,    89,     0,     0,     0,   233,    90,    91,    92,
      93,     0,     0,     0,   234
};

static const yytype_int16 yycheck[] =
{
      26,    27,    28,    29,    30,    29,    30,    31,    32,    31,
      32,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    29,
      30,    31,    32,    35,    29,    30,    31,    32,    31,    32,
       0,    41,    37,    35,    39,    35,    35,    29,    30,    31,
      32,    35,    35,    39,    80,    81,    82,    39,    35,    35,
      29,    30,    31,    32,    90,    91,    92,    93,    37,    35,
      35,    26,    27,    28,    35,    29,    30,    31,    32,    35,
      -1,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      35,    35,    35,    35,    35,    35,    35,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,    35,    35,    35,   154,    35,
     156,    86,    87,    88,    89,    29,    30,    31,    32,    29,
      30,    31,    32,    37,   170,    -1,   172,    37,    29,    30,
      31,    32,    35,    35,    -1,    36,   182,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      35,    27,    28,    29,    30,    29,    30,    31,    32,    35,
      35,    -1,    38,    37,    40,    29,    30,    31,    32,    29,
      30,    31,    32,    37,    35,    -1,    35,    37,    35,    29,
      30,    31,    32,    35,   169,    -1,   171,    37,    29,    30,
      31,    32,    35,    35,    -1,    36,   181,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      35,    27,    28,    29,    30,    35,    35,    -1,    -1,    35,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    -1,    27,    28,    29,    30,    29,    30,    31,
      32,    35,    -1,    -1,    36,    29,    30,    31,    32,    -1,
      -1,    -1,    36,    29,    30,    31,    32,    -1,    -1,    -1,
      36,    29,    30,    31,    32,    -1,    -1,    -1,    36,    29,
      30,    31,    32,    -1,    -1,    -1,    36,    29,    30,    31,
      32,    -1,    -1,    -1,    36,    29,    30,    31,    32,    -1,
      -1,    -1,    36,    29,    30,    31,    32,    -1,    -1,    -1,
      36,    29,    30,    31,    32,    -1,    -1,    -1,    36,    29,
      30,    31,    32,    -1,    -1,    -1,    36,    29,    30,    31,
      32,    -1,    -1,    -1,    36,    29,    30,    31,    32,    -1,
      -1,    -1,    36,    29,    30,    31,    32,    -1,    -1,    -1,
      36,    29,    30,    31,    32,    -1,    -1,    -1,    36,    29,
      30,    31,    32,    -1,    -1,    -1,    36,    29,    30,    31,
      32,    -1,    -1,    -1,    36,    29,    30,    31,    32,    -1,
      -1,    -1,    36,    29,    30,    31,    32,    -1,    -1,    -1,
      36,    29,    30,    31,    32,    -1,    -1,    -1,    36,    29,
      30,    31,    32,    -1,    -1,    -1,    36,    29,    30,    31,
      32,    -1,    -1,    -1,    36,    29,    30,    31,    32,    -1,
      -1,    -1,    36,    29,    30,    31,    32,    -1,    -1,    -1,
      36,    29,    30,    31,    32,    -1,    -1,    -1,    36,    29,
      30,    31,    32,    -1,    -1,    -1,    36,    29,    30,    31,
      32,    -1,    -1,    -1,    36,    29,    30,    31,    32,    -1,
      -1,    -1,    36,    29,    30,    31,    32,    -1,    -1,    -1,
      36,    29,    30,    31,    32,    -1,    -1,    -1,    36,    29,
      30,    31,    32,    -1,    -1,    -1,    36,    29,    30,    31,
      32,    -1,    -1,    -1,    36,    29,    30,    31,    32,    -1,
      -1,    -1,    36,    29,    30,    31,    32,    -1,    -1,    -1,
      36,    29,    30,    31,    32,    -1,    -1,    -1,    36,    29,
      30,    31,    32,    -1,    -1,    -1,    36,    29,    30,    31,
      32,    -1,    -1,    -1,    36
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    27,    28,    29,    30,    35,    38,
      40,    43,    44,    45,    46,    47,    48,    35,    35,    35,
      35,    35,    35,    35,    35,    35,    35,    35,    35,    35,
      35,    35,    35,    35,    35,    44,    47,    44,    47,    44,
      47,     3,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      29,    30,    35,    47,    47,     0,    29,    30,    31,    32,
      29,    30,    31,    32,    44,    47,    44,    47,    44,    47,
      44,    47,    44,    47,    44,    47,    44,    47,    44,    47,
      44,    47,    44,    47,    44,    47,    44,    47,    44,    47,
      44,    47,    44,    47,    44,    47,    44,    47,    44,    47,
      36,    36,    39,    35,    35,    35,    35,    35,    35,    35,
      35,    35,    35,    35,    35,    35,    35,    35,    35,    35,
      35,    47,    47,    47,    37,    39,    37,    44,    44,    44,
      44,    47,    47,    47,    47,    36,    36,    36,    36,    37,
      37,    37,    37,    36,    36,    36,    36,    36,    36,    36,
      36,    37,    37,    36,    36,    36,    36,    36,    36,    36,
      36,    36,    36,    36,    36,    36,    36,    36,    36,    36,
      36,    47,    47,    47,    47,    47,    47,    47,    47,    47,
      47,    47,    47,    47,    47,    47,    47,    47,    47,    47,
      47,    44,    47,    44,    47,    44,    47,    39,    41,    36,
      36,    36,    36,    36,    36
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    42,    43,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    45,    45,    45,    45,    45,    45,    45,    45,
      45,    45,    45,    45,    45,    45,    45,    45,    45,    45,
      46,    46,    46,    46,    46,    46,    47,    47,    47,    47,
      47,    47,    47,    47,    47,    47,    47,    47,    47,    47,
      48,    48,    48,    48,    48,    48,    48,    48,    48,    48,
      48,    48,    48,    48,    48,    48,    48,    48
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     3,     3,     3,     3,     2,     2,
       1,     3,     4,     4,     4,     6,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     6,     4,     6,
       1,     1,     3,     5,     3,     5,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     2,     2,     1,     3,
       4,     4,     4,     6,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     6,     4,     6
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 74 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { *gaol_result_of_parsing = interval((yyvsp[0].itv).l,(yyvsp[0].itv).r); }
#line 1520 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 78 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { (yyval.itv) = (yyvsp[0].itv); }
#line 1526 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 79 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=interval((yyvsp[-2].itv).l,(yyvsp[-2].itv).r)+interval((yyvsp[0].itv).l,(yyvsp[0].itv).r);
						  					(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1533 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 81 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=interval((yyvsp[-2].itv).l,(yyvsp[-2].itv).r)-interval((yyvsp[0].itv).l,(yyvsp[0].itv).r);
						  					(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1540 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 83 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=interval((yyvsp[-2].itv).l,(yyvsp[-2].itv).r)*interval((yyvsp[0].itv).l,(yyvsp[0].itv).r);
						  					(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1547 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 85 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=interval((yyvsp[-2].itv).l,(yyvsp[-2].itv).r)/interval((yyvsp[0].itv).l,(yyvsp[0].itv).r);
						  					(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1554 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 87 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv= -interval((yyvsp[0].itv).l,(yyvsp[0].itv).r);
						  					(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1561 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 89 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { (yyval.itv) = (yyvsp[0].itv); }
#line 1567 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 90 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { (yyval.itv) = (yyvsp[0].itv); }
#line 1573 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 91 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { (yyval.itv) = (yyvsp[-1].itv); }
#line 1579 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 96 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=cos(interval((yyvsp[-1].itv).l,(yyvsp[-1].itv).r));
						  									(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1586 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 98 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=sin(interval((yyvsp[-1].itv).l,(yyvsp[-1].itv).r));
						  									(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1593 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 100 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=tan(interval((yyvsp[-1].itv).l,(yyvsp[-1].itv).r));
						  									(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1600 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 102 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=atan2(interval((yyvsp[-3].itv).l,(yyvsp[-3].itv).r),
							          						interval((yyvsp[-1].itv).l,(yyvsp[-1].itv).r));
						    								(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1608 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 105 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=acos(interval((yyvsp[-1].itv).l,(yyvsp[-1].itv).r));
						  									(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1615 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 107 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=asin(interval((yyvsp[-1].itv).l,(yyvsp[-1].itv).r));
						  									(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1622 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 109 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=atan(interval((yyvsp[-1].itv).l,(yyvsp[-1].itv).r));
						  									(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1629 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 111 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=cosh(interval((yyvsp[-1].itv).l,(yyvsp[-1].itv).r));
						  									(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1636 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 113 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=sinh(interval((yyvsp[-1].itv).l,(yyvsp[-1].itv).r));
						  									(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1643 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 115 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=tanh(interval((yyvsp[-1].itv).l,(yyvsp[-1].itv).r));
						  									(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1650 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 117 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=acosh(interval((yyvsp[-1].itv).l,(yyvsp[-1].itv).r));
						  									(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1657 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 119 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=asinh(interval((yyvsp[-1].itv).l,(yyvsp[-1].itv).r));
						  									(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1664 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 121 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=atanh(interval((yyvsp[-1].itv).l,(yyvsp[-1].itv).r));
						  									(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1671 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 123 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=exp(interval((yyvsp[-1].itv).l,(yyvsp[-1].itv).r));
						  									(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1678 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 125 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=log(interval((yyvsp[-1].itv).l,(yyvsp[-1].itv).r));
						  									(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1685 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 127 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=
						   									pow(interval((yyvsp[-3].itv).l,(yyvsp[-3].itv).r),interval((yyvsp[-1].itv).l,(yyvsp[-1].itv).r));
						   									(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1693 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 130 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { gaol_tmp_itv=sqrt(interval((yyvsp[-1].itv).l,(yyvsp[-1].itv).r));
						  									(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
#line 1700 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 132 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { interval tmp((yyvsp[-1].itv).l,(yyvsp[-1].itv).r);
														   if (!tmp.is_an_int()) {
					                                       		// not an int?
																(yyval.itv).l=1; (yyval.itv).r=-1;
																gaol_ERROR(invalid_action_error,
																	"nth_root used with non integral 2nd arg.");
														    } else {
					     										gaol_tmp_itv=nth_root(interval((yyvsp[-3].itv).l,(yyvsp[-3].itv).r),int((yyvsp[-1].itv).l));
						       										(yyval.itv).l=gaol_tmp_itv.left(); (yyval.itv).r=gaol_tmp_itv.right(); }
														    }
#line 1715 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 145 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	gaol_global_parsing_flag = evaluate_left_right(expression(*(yyvsp[0].expr)),
																gaol_result_of_parsing);
											delete (yyvsp[0].expr);
							  				(yyval.itv).l=gaol_result_of_parsing->left();
					          				(yyval.itv).r=gaol_result_of_parsing->right();
										}
#line 1726 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 152 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	*gaol_result_of_parsing = interval::emptyset();
					  						gaol_global_parsing_flag = true;
											(yyval.itv).l=gaol_result_of_parsing->left();
					  						(yyval.itv).r=gaol_result_of_parsing->right();
										}
#line 1736 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 158 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    {	gaol_global_parsing_flag = evaluate_left_right(expression(*(yyvsp[-1].expr)),
																gaol_result_of_parsing);
					  						delete (yyvsp[-1].expr);
											(yyval.itv).l=gaol_result_of_parsing->left();
					  						(yyval.itv).r=gaol_result_of_parsing->right();
										}
#line 1747 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 164 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	gaol_global_parsing_flag = evaluate_left_right(expression(*(yyvsp[-3].expr)),
																			expression(*(yyvsp[-1].expr)),
																			gaol_result_of_parsing);
					  						delete (yyvsp[-3].expr);
					  						delete (yyvsp[-1].expr);
                                          	(yyval.itv).l=gaol_result_of_parsing->left();
					  						(yyval.itv).r=gaol_result_of_parsing->right();
										}
#line 1760 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 172 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	*gaol_result_of_parsing = interval::emptyset();
					  						gaol_global_parsing_flag = true;
                                          	(yyval.itv).l=gaol_result_of_parsing->left();
					  						(yyval.itv).r=gaol_result_of_parsing->right();
										}
#line 1770 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 177 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	gaol_global_parsing_flag = evaluate_left_right(expression(*(yyvsp[-3].expr)),
																							expression(*(yyvsp[-1].expr)),
																							gaol_result_of_parsing);
											delete (yyvsp[-3].expr);
											delete (yyvsp[-1].expr);
                                          	(yyval.itv).l=gaol_result_of_parsing->left();
					  						(yyval.itv).r=gaol_result_of_parsing->right();
											if ((yyval.itv).l != (yyval.itv).r) {
												gaol_ERROR(input_format_error, std::string("bounds of degenerate interval do not evaluate to the same value"));
											}
										}
#line 1786 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 191 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new double_node((yyvsp[0].d));
					  				(yyval.expr)->inc_refcount(); }
#line 1793 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 193 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new double_node(std::numeric_limits<double>::min());
					  				(yyval.expr)->inc_refcount(); }
#line 1800 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 195 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new double_node(std::numeric_limits<double>::max());
					  				(yyval.expr)->inc_refcount(); }
#line 1807 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 197 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new interval_node(interval::pi());
	                                (yyval.expr)->inc_refcount(); }
#line 1814 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 199 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new interval_node(interval((yyvsp[0].itv).l,(yyvsp[0].itv).r));
					  				(yyval.expr)->inc_refcount(); }
#line 1821 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 201 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new double_node(GAOL_INFINITY);
					  				(yyval.expr)->inc_refcount(); }
#line 1828 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 203 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new add_node(*(yyvsp[-2].expr),*(yyvsp[0].expr));
					  				(yyval.expr)->inc_refcount(); }
#line 1835 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 205 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new sub_node(*(yyvsp[-2].expr),*(yyvsp[0].expr));
					  				(yyval.expr)->inc_refcount(); }
#line 1842 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 207 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new mult_node(*(yyvsp[-2].expr),*(yyvsp[0].expr));
					  				(yyval.expr)->inc_refcount(); }
#line 1849 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 209 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    {	(yyval.expr) = new div_node(*(yyvsp[-2].expr),*(yyvsp[0].expr));
					  				(yyval.expr)->inc_refcount(); }
#line 1856 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 211 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { (yyval.expr) = new unary_minus_node(*(yyvsp[0].expr));
					  				(yyval.expr)->inc_refcount(); }
#line 1863 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 213 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 1869 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 214 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 1875 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 215 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[-1].expr); }
#line 1881 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 219 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new cos_node(*(yyvsp[-1].expr));
					  									(yyval.expr)->inc_refcount(); }
#line 1888 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 221 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new sin_node(*(yyvsp[-1].expr));
					  									(yyval.expr)->inc_refcount(); }
#line 1895 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 223 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new tan_node(*(yyvsp[-1].expr));
					  									(yyval.expr)->inc_refcount(); }
#line 1902 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 225 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new atan2_node(*(yyvsp[-3].expr), *(yyvsp[-1].expr));
					  									(yyval.expr)->inc_refcount(); }
#line 1909 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 227 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new acos_node(*(yyvsp[-1].expr));
					  									(yyval.expr)->inc_refcount(); }
#line 1916 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 229 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new asin_node(*(yyvsp[-1].expr));
					  									(yyval.expr)->inc_refcount(); }
#line 1923 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 231 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new atan_node(*(yyvsp[-1].expr));
					  									(yyval.expr)->inc_refcount(); }
#line 1930 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 233 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new cosh_node(*(yyvsp[-1].expr));
														(yyval.expr)->inc_refcount(); }
#line 1937 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 235 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new sinh_node(*(yyvsp[-1].expr));
					  									(yyval.expr)->inc_refcount(); }
#line 1944 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 237 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new tanh_node(*(yyvsp[-1].expr));
					  									(yyval.expr)->inc_refcount(); }
#line 1951 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 239 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new acosh_node(*(yyvsp[-1].expr));
					  									(yyval.expr)->inc_refcount(); }
#line 1958 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 241 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new asinh_node(*(yyvsp[-1].expr));
					  									(yyval.expr)->inc_refcount(); }
#line 1965 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 243 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new atanh_node(*(yyvsp[-1].expr));
					  									(yyval.expr)->inc_refcount(); }
#line 1972 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 245 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new exp_node(*(yyvsp[-1].expr));
					  									(yyval.expr)->inc_refcount(); }
#line 1979 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 247 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	(yyval.expr) = new log_node(*(yyvsp[-1].expr));
					  									(yyval.expr)->inc_refcount(); }
#line 1986 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 249 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	interval tmp;
					  									gaol_global_parsing_flag =
					    									evaluate_expr(expression(*(yyvsp[-1].expr)),tmp);
														if (!gaol_global_parsing_flag) { // Error?
					     									(yyval.expr) = the_null_expr;
					     									(yyval.expr)->inc_refcount();
                                          				} else {
					     									if (!tmp.is_an_int()) {
					        									// not an int in disguise?
																(yyval.expr) = new pow_itv_node(*(yyvsp[-3].expr),*(yyvsp[-1].expr));
																(yyval.expr)->inc_refcount();
       					     								} else {
					        									if (tmp.left() > 0) {
					           										(yyval.expr) = new pow_node(*(yyvsp[-3].expr),(unsigned int)(tmp.left()));
	 				           										(yyval.expr)->inc_refcount();
                                               					} else {
						  											expr_node *one = new double_node(1.0);
					          										one->inc_refcount();
						  											if (tmp.left() < 0) {
					           											// translating x^(-a) into 1/(x^a)
						     											expr_node *e = new pow_node(*(yyvsp[-3].expr),
																							(unsigned int)(-tmp.left()));
						     											e->inc_refcount();
						     											(yyval.expr) = new div_node(*one,*e);
						     											(yyval.expr)->inc_refcount();
																	} else {  // tmp == 0.0
						     											(yyval.expr) = one;
						 											}
                                                				}
					     									}
                                          				}
                                        			}
#line 2023 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 281 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    {	(yyval.expr) = new nth_root_node(*(yyvsp[-1].expr),2);
					  									(yyval.expr)->inc_refcount();
													}
#line 2031 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 284 "gaol_interval_parser.ypp" /* yacc.c:1646  */
    { 	interval tmp;
					  									gaol_global_parsing_flag =
					    									evaluate_expr(expression(*(yyvsp[-1].expr)),tmp);
														if (!tmp.is_an_int() || !gaol_global_parsing_flag) {
					     									// not an int?
															(yyval.expr) = the_null_expr;
															(yyval.expr)->inc_refcount();
       					  								} else {
					     									(yyval.expr) = new nth_root_node(*(yyvsp[-3].expr),int(tmp.left()));
					     									(yyval.expr)->inc_refcount();
					 									}
													  }
#line 2048 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2052 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 298 "gaol_interval_parser.ypp" /* yacc.c:1906  */


int gaol_error_bison(char *s ...)
{
  gaol_global_parsing_flag = false;
  return 0; // An error occurred
}
