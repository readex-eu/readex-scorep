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




/* Copy the first part of user declarations.  */
#line 39 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:339  */


/*----------------------------------------------------------------------------*/
/* Loads system interface modules.                                            */
/*----------------------------------------------------------------------------*/
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*----------------------------------------------------------------------------*/
/* Loads own interface modules.                                               */
/*----------------------------------------------------------------------------*/
#include "scorep_oa_mri_control.h"
#include "SCOREP_OA_Request.h"
#include "scorep_oa_connection.h"
#include "SCOREP_OA_RuntimeManagement.h"
#include <UTILS_Debug.h>
#include <UTILS_CStr.h>
#include <scorep_substrates_definition.h>


/*----------------------------------------------------------------------------*/
/* Defines Macros                                                             */
/*----------------------------------------------------------------------------*/
#define DEFAULT_BUFFER_SIZE 10000


/*----------------------------------------------------------------------------*/
/* Declaration of types                                                       */
/*----------------------------------------------------------------------------*/
struct
{
    int   region_id;
    int   kind;
    char* action_name;
    int   value;
} tuning_action;

//TODO: Find another way of passing tuning action type. -RM
//This is dangerous as it is already defined in the tuning substrate plugin. -RM
enum
{
    SCOREP_TUNING_VARIABLE = 0,
    SCOREP_TUNING_FUNCTION,
    SCOREP_TUNING_UNDEFINED
} SCOREP_TuningActionKind;

/*----------------------------------------------------------------------------*/
/* Variable declarations:                                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Declaration of Procedures/Functions prototypes:                            */
/*----------------------------------------------------------------------------*/
void yyerror( char *s );
int yylex( void );

char* json = NULL;

#line 127 "scorep_oa_scanner.c" /* yacc.c:339  */

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
#ifndef YY_YY_SCOREP_OA_SCANNER_H_INCLUDED
# define YY_YY_SCOREP_OA_SCANNER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_NUMBER = 258,
    T_QSTRING = 259,
    T_STRING = 260,
    T_RUNTOSTART = 261,
    T_RUNTOEND = 262,
    T_TERMINATE = 263,
    T_SETNUMITERATIONS = 264,
    T_BEGINREQUESTS = 265,
    T_MEASUREMENTREQUEST = 266,
    T_TUNINGREQUEST = 267,
    T_RTSTUNINGREQUESTS = 268,
    T_ENDREQUESTS = 269,
    T_GETSUMMARYDATA = 270,
    T_GLOBAL = 271,
    T_MPI = 272,
    T_METRIC = 273,
    T_EXECUTION_TIME = 274,
    T_PAPI = 275,
    T_PERF = 276,
    T_RUSAGE = 277,
    T_PLUGIN = 278,
    T_OTHER = 279,
    T_VARIABLE = 280,
    T_FUNCTION = 281,
    T_INTPARAMS = 282,
    T_UINTPARAMS = 283,
    T_STRINGPARAMS = 284
  };
#endif
/* Tokens.  */
#define T_NUMBER 258
#define T_QSTRING 259
#define T_STRING 260
#define T_RUNTOSTART 261
#define T_RUNTOEND 262
#define T_TERMINATE 263
#define T_SETNUMITERATIONS 264
#define T_BEGINREQUESTS 265
#define T_MEASUREMENTREQUEST 266
#define T_TUNINGREQUEST 267
#define T_RTSTUNINGREQUESTS 268
#define T_ENDREQUESTS 269
#define T_GETSUMMARYDATA 270
#define T_GLOBAL 271
#define T_MPI 272
#define T_METRIC 273
#define T_EXECUTION_TIME 274
#define T_PAPI 275
#define T_PERF 276
#define T_RUSAGE 277
#define T_PLUGIN 278
#define T_OTHER 279
#define T_VARIABLE 280
#define T_FUNCTION 281
#define T_INTPARAMS 282
#define T_UINTPARAMS 283
#define T_STRINGPARAMS 284

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 106 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:355  */

  int   Number;
  char *String;

#line 230 "scorep_oa_scanner.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SCOREP_OA_SCANNER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 247 "scorep_oa_scanner.c" /* yacc.c:358  */

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
#define YYFINAL  32
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   122

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  72
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  157

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   284

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
      31,    33,    37,     2,    32,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    30,
       2,    36,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    34,     2,    35,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   155,   155,   156,   157,   159,   163,   167,   173,   178,
     179,   180,   182,   188,   189,   190,   191,   193,   199,   200,
     202,   203,   205,   210,   220,   210,   243,   244,   246,   246,
     254,   258,   263,   254,   272,   275,   277,   280,   282,   285,
     287,   288,   290,   291,   293,   294,   296,   300,   304,   308,
     309,   311,   316,   321,   322,   326,   330,   332,   333,   335,
     335,   367,   372,   373,   377,   380,   383,   386,   389,   393,
     398,   404,   408
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_NUMBER", "T_QSTRING", "T_STRING",
  "T_RUNTOSTART", "T_RUNTOEND", "T_TERMINATE", "T_SETNUMITERATIONS",
  "T_BEGINREQUESTS", "T_MEASUREMENTREQUEST", "T_TUNINGREQUEST",
  "T_RTSTUNINGREQUESTS", "T_ENDREQUESTS", "T_GETSUMMARYDATA", "T_GLOBAL",
  "T_MPI", "T_METRIC", "T_EXECUTION_TIME", "T_PAPI", "T_PERF", "T_RUSAGE",
  "T_PLUGIN", "T_OTHER", "T_VARIABLE", "T_FUNCTION", "T_INTPARAMS",
  "T_UINTPARAMS", "T_STRINGPARAMS", "';'", "'('", "','", "')'", "'['",
  "']'", "'='", "'*'", "$accept", "Command", "ExecutionControl",
  "Configuration", "Requests", "BeginRequests", "RequestCommand",
  "EndRequests", "MeasurementRequests", "TuningRequests",
  "MeasurementRequest", "RTSTuningRequests", "$@1", "$@2",
  "RegionCallPaths", "RegionCallPath", "$@3", "ParameterIdentifier", "$@4",
  "$@5", "$@6", "ParamIntSpec", "ParamUIntSpec", "ParamStringSpec",
  "ParameterInts", "ParameterUInts", "ParameterStrings", "ParameterInt",
  "ParameterUInt", "ParameterString", "TuningParameters",
  "TuningParameter", "SummaryRequest", "NodeList", "NodeNrs",
  "RequestKind", "MeasurementTypeList", "TuningRequest", "$@7",
  "MeasurementType", "MetricRequest", "RegionId", "TuningAction",
  "TuningActionKind", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
      59,    40,    44,    41,    91,    93,    61,    42
};
# endif

#define YYPACT_NINF -43

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-43)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
       1,   -25,   -12,   -43,    26,    16,    13,   -43,    17,    19,
     -43,    30,    20,    21,   -43,   -43,   -43,   -43,   -10,    -7,
     -43,    22,    23,   -43,    51,    52,   -43,   -43,    -3,    25,
     -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,
     -43,    27,    28,   -43,   -43,    29,   -43,    54,    31,    55,
      58,    47,   -43,    32,   -43,   -15,   -43,    33,    34,    18,
     -43,    35,    54,    31,    36,   -43,   -43,   -43,     4,   -43,
     -43,    37,    39,    41,   -43,    43,    64,    71,    72,    73,
      74,   -43,    18,    -4,   -43,   -43,   -43,   -43,   -43,   -43,
      75,   -43,   -43,   -43,   -43,    48,    49,    56,    77,   -43,
      80,   -43,    50,    53,    57,     6,   -43,    59,    61,   -43,
      84,    77,   -43,    85,    -2,    62,   -43,   -43,    63,    65,
     -43,     8,   -43,    66,    67,   -43,    86,    90,   -43,    69,
     -43,   -43,   -43,    -1,    68,    70,   -43,    10,   -43,    76,
     -43,    92,    94,   -43,    78,   -43,   -43,     0,    79,   -43,
      12,   -43,    99,   100,   -43,   -43,   -43
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     7,     0,     0,     0,    59,     0,     0,
      52,     0,     0,     0,     4,     9,    10,    11,     0,     0,
      19,     0,     0,    21,     0,     0,     8,    12,     0,     0,
      23,    17,     1,     2,     3,    13,    18,    14,    20,    15,
      16,     0,     0,    55,    54,     0,    53,     0,     0,     0,
       0,     0,    69,     0,    28,     0,    27,     0,     0,     0,
      22,     0,     0,     0,     0,     5,     6,    61,     0,    63,
      56,    58,     0,     0,    26,     0,     0,     0,     0,     0,
       0,    62,     0,     0,    60,    30,    24,    64,    65,    66,
       0,    68,    57,    71,    72,     0,     0,     0,     0,    67,
       0,    29,     0,     0,     0,     0,    50,     0,     0,    31,
       0,     0,    25,     0,     0,     0,    51,    49,     0,     0,
      35,     0,    41,     0,     0,    70,     0,     0,    34,     0,
      32,    46,    40,     0,     0,     0,    37,     0,    43,     0,
      33,     0,     0,    36,     0,    47,    42,     0,     0,    39,
       0,    45,     0,     0,    38,    48,    44
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,
      87,   -43,   -43,   -43,   -43,    44,   -43,   -43,   -43,   -43,
     -43,   -43,   -43,   -43,   -43,   -43,   -43,   -19,   -32,   -42,
     -43,     2,   -43,   -43,   -43,   -43,    38,    95,   -43,   -43,
     -43,    60,   -43,   -43
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    48,    98,    55,    56,    62,    96,    97,   115,
     134,   103,   124,   140,   121,   137,   150,   122,   138,   151,
     105,   106,    22,    45,    46,    60,    70,    23,    29,    71,
      81,    53,    84,    95
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      43,     6,   119,   135,   148,     7,    24,     1,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    63,    64,    25,
      35,    93,    94,    37,    76,    77,    78,    79,    80,    26,
      32,   120,   136,   149,    44,    67,    68,    69,   111,   112,
     127,   128,   142,   143,   153,   154,    27,    28,    30,    31,
      33,    34,    39,    40,    41,    42,    47,    52,    57,    49,
      50,    58,    54,    59,    51,    61,    65,    66,    87,    82,
      83,    72,    75,    85,    86,    88,    89,    90,    91,    99,
     100,   104,   101,   102,   107,   109,   108,   116,   118,   131,
     123,   113,   114,   110,   119,   145,   125,   139,   135,   130,
     133,   126,   129,   155,   148,    36,   141,    74,   132,   147,
     146,   156,   144,   117,    38,   152,     0,     0,     0,     0,
      92,     0,    73
};

static const yytype_int16 yycheck[] =
{
       3,    11,     4,     4,     4,    12,    31,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    32,    33,    31,
      30,    25,    26,    30,    20,    21,    22,    23,    24,     3,
       0,    33,    33,    33,    37,    17,    18,    19,    32,    33,
      32,    33,    32,    33,    32,    33,    30,    34,    31,    30,
      30,    30,    30,    30,     3,     3,    31,     3,     3,    32,
      32,     3,    31,    16,    35,    33,    33,    33,     4,    32,
      31,    36,    36,    32,    31,     4,     4,     4,     4,     4,
      32,     4,    33,    27,     4,    32,    36,     3,     3,     3,
      28,    32,    31,    36,     4,     3,    33,    29,     4,    32,
      31,    36,    36,     4,     4,    18,    36,    63,   127,    31,
     142,   153,    36,   111,    19,    36,    -1,    -1,    -1,    -1,
      82,    -1,    62
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    70,    75,    31,    31,     3,    30,    34,    76,
      31,    30,     0,    30,    30,    30,    48,    30,    75,    30,
      30,     3,     3,     3,    37,    71,    72,    31,    50,    32,
      32,    35,     3,    79,    31,    52,    53,     3,     3,    16,
      73,    33,    54,    32,    33,    33,    33,    17,    18,    19,
      74,    77,    36,    79,    53,    36,    20,    21,    22,    23,
      24,    78,    32,    31,    80,    32,    31,     4,     4,     4,
       4,     4,    74,    25,    26,    81,    55,    56,    51,     4,
      32,    33,    27,    59,     4,    68,    69,     4,    36,    32,
      36,    32,    33,    32,    31,    57,     3,    69,     3,     4,
      33,    62,    65,    28,    60,    33,    36,    32,    33,    36,
      32,     3,    65,    31,    58,     4,    33,    63,    66,    29,
      61,    36,    32,    33,    36,     3,    66,    31,     4,    33,
      64,    67,    36,    32,    33,     4,    67
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    38,    39,    39,    39,    40,    40,    40,    41,    42,
      42,    42,    43,    44,    44,    44,    44,    45,    46,    46,
      47,    47,    48,    50,    51,    49,    52,    52,    54,    53,
      56,    57,    58,    55,    59,    59,    60,    60,    61,    61,
      62,    62,    63,    63,    64,    64,    65,    66,    67,    68,
      68,    69,    70,    71,    71,    72,    73,    74,    74,    76,
      75,    77,    77,    77,    78,    78,    78,    78,    78,    79,
      80,    81,    81
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     1,     6,     6,     1,     2,     1,
       1,     1,     2,     2,     2,     2,     2,     2,     2,     1,
       2,     1,     5,     0,     0,    10,     3,     1,     0,     6,
       0,     0,     0,     8,     5,     4,     5,     4,     5,     4,
       3,     1,     3,     1,     3,     1,     3,     3,     3,     3,
       1,     3,     1,     1,     1,     1,     2,     3,     1,     0,
       7,     1,     2,     1,     2,     2,     2,     3,     2,     1,
       7,     1,     1
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
        case 5:
#line 159 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Execution request run_to_beginning received" );
                                                                              scorep_oa_mri_set_appl_control( SCOREP_OA_MRI_STATUS_RUNNING_TO_BEGINNING );
                                                                          }
#line 1436 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 6:
#line 163 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Execution request run_to_end received" );
                                                                              scorep_oa_mri_set_appl_control( SCOREP_OA_MRI_STATUS_RUNNING_TO_END );
                                                                          }
#line 1445 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 7:
#line 167 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Termination request received" );
                                                                              scorep_oa_mri_set_appl_control( SCOREP_OA_MRI_EXEC_REQUEST_TERMINATE );
                                                                              scorep_oa_connection_send_string( scorep_oa_connection, "OK\n" );
                                                                          }
#line 1455 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 8:
#line 173 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Request to set number of iterations" );
                                                                              scorep_oa_mri_set_num_iterations( (yyvsp[0].Number) );
                                                                          }
#line 1464 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 12:
#line 182 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Standby for requests..." );
                                                                              scorep_oa_requests_begin();
                                                                              scorep_oa_connection_send_string( scorep_oa_connection, "OK\n" );
                                                                          }
#line 1474 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 17:
#line 193 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Requests submitted" );
                                                                              scorep_oa_requests_end();
                                                                              scorep_oa_connection_send_string( scorep_oa_connection, "OK\n" );
                                                                          }
#line 1484 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 22:
#line 205 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Measurements are requested" );
                                                                              scorep_oa_connection_send_string( scorep_oa_connection, "OK\n" );
                                                                          }
#line 1493 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 23:
#line 210 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              json = ( char* ) calloc(1, DEFAULT_BUFFER_SIZE );
                                                                              sprintf( json, "{\n"
                                                                                             "    \"genericEventType\":\"tuningRequest\",\n"
                                                                                             "    \"genericEventTypeVersion\":0.2,\n"
                                                                                             "    \"data\":\n"
                                                                                             "    {\n"
                                                                                             "        \"RegionCallpath\":\n"
                                                                                             "        [\n" );
                                                                          }
#line 1508 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 24:
#line 220 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              sprintf( json + strlen( json ) - 2, "\n"
                                                                                                                  "        ],\n"
                                                                                                                  "        \"TuningParameters\":\n" 
                                                                                                                  "        {\n" );
                                                                                                                  
                                                                          }
#line 1520 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 25:
#line 227 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                      
                                                                              sprintf( json + strlen( json ) - 2, "\n"
                                                                                                                  "        }\n"
                                                                                                                  "    }\n"
                                                                                                                  "}\n" );
                                                                              size_t size  = strlen( json ) + 1;
                                                                              json[ size ] = '\0';
                                                                              //printf( "%s\n", json );
                                                                              SCOREP_CALL_SUBSTRATE( GenericCommand, GENERIC_COMMAND, ( json, size ) );
                                  
                                                                              json = NULL;
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Runtime situation tuning actions are requested" );
                                                                              scorep_oa_connection_send_string( scorep_oa_connection, "OK\n" );
                                                                          }
#line 1540 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 28:
#line 246 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              sprintf( json + strlen( json ), "            {\n"
                                                                                                              "        " );
                                                                          }
#line 1549 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 29:
#line 250 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              sprintf( json + strlen( json ), "            },\n" );
                                                                          }
#line 1557 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 30:
#line 254 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              sprintf( json + strlen( json ), "                \"AdditionalIdentifiersInt\":\n"
                                                                                                              "                {\n" );
                                                                          }
#line 1566 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 31:
#line 258 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              sprintf( json + strlen( json ), "                },\n"
                                                                                                              "                \"AdditionalIdentifiersUInt\":\n"
                                                                                                              "                {\n" );
                                                                          }
#line 1576 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 32:
#line 263 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              sprintf( json + strlen( json ), "                },\n"
                                                                                                              "                \"AdditionalIdentifiersString\":\n"
                                                                                                              "                {\n" );
                                                                          }
#line 1586 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 33:
#line 268 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              sprintf( json + strlen( json ), "                }\n" );
                                                                          }
#line 1594 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 34:
#line 272 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              sprintf( json + strlen( json ) - 2, "\n" );
                                                                          }
#line 1602 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 36:
#line 277 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              sprintf( json + strlen( json ) - 2, "\n" );
                                                                          }
#line 1610 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 38:
#line 282 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              sprintf( json + strlen( json ) - 2, "\n" );
                                                                          }
#line 1618 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 46:
#line 296 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              sprintf( json + strlen( json ), "                    \"%s\":%d,\n", (yyvsp[-2].String), (yyvsp[0].Number) );
                                                                          }
#line 1626 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 47:
#line 300 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              sprintf( json + strlen( json ), "                    \"%s\":%d,\n", (yyvsp[-2].String), (yyvsp[0].Number) );
                                                                          }
#line 1634 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 48:
#line 304 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              sprintf( json + strlen( json ), "                    \"%s\":%s,\n", (yyvsp[-2].String), (yyvsp[0].String) );
                                                                          }
#line 1642 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 51:
#line 311 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {

                                                                              sprintf( json + strlen( json ), "                    \"%s\":%d,\n", (yyvsp[-2].String), (yyvsp[0].Number) );
                                                                           }
#line 1651 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 52:
#line 316 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Measured data requested" );
                                                                              scorep_oa_mri_return_summary_data( scorep_oa_connection );
                                                                          }
#line 1660 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 54:
#line 322 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Node lists are ignored" );
                                                                          }
#line 1668 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 55:
#line 326 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Node lists are ignored" );
                                                                          }
#line 1676 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 59:
#line 335 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              json = ( char* ) malloc( DEFAULT_BUFFER_SIZE );
                                                                              sprintf( json, "{\n"
                                                                                             "    \"genericEventType\":\"tuningRequest\",\n"
                                                                                             "    \"genericEventTypeVersion\":0.1,\n"
                                                                                             "    \"data\":\n"
                                                                                             "    {\n"
                                                                                             "        \"RegionName\":\"%s\",\n",
                                                                                             "Name" );
                                                                          }
#line 1691 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 60:
#line 345 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              SCOREP_CALL_SUBSTRATE( AddTuningAction, ADD_TUNING_ACTION,
                                                                                                     ( tuning_action.region_id,
                                                                                                     tuning_action.kind,
                                                                                                     tuning_action.action_name,
                                                                                                     tuning_action.value ) );
                                                                              sprintf( json + strlen( json ), "        \"TuningParameters\":\n"
                                                                                                              "        {\n"
                                                                                                              "            \"%s\":%d\n"
                                                                                                              "        }\n"
                                                                                                              "    }\n"
                                                                                                              "}\n",
                                                                                                              tuning_action.action_name,
                                                                                                              tuning_action.value );
                                                                              size_t size  = strlen( json ) + 1;
                                                                              json[ size ] = '\0';
                                                                              //printf( "%s\n", json );
                                                                              SCOREP_CALL_SUBSTRATE( GenericCommand, GENERIC_COMMAND, ( json, size ) );
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Tuning actions are requested" );
                                                                              scorep_oa_connection_send_string( scorep_oa_connection, "OK\n" );
                                                                          }
#line 1717 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 61:
#line 367 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              scorep_oa_mri_set_mpiprofiling( 1 );
                                                                              scorep_oa_requests_add_metric_by_name( UTILS_CStr_dup( "late_send" ), NULL, SCOREP_METRIC_SOURCE_MPI_PROFILING );
                                                                              scorep_oa_requests_add_metric_by_name( UTILS_CStr_dup( "late_receive" ), NULL, SCOREP_METRIC_SOURCE_MPI_PROFILING );
                                                                          }
#line 1727 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 63:
#line 373 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              scorep_oa_requests_add_metric_by_name( UTILS_CStr_dup( "execution_time" ), NULL, SCOREP_METRIC_TIMER );
                                                                          }
#line 1735 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 64:
#line 377 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              scorep_oa_requests_add_metric_by_name( (yyvsp[0].String), NULL, SCOREP_METRIC_SOURCE_PAPI );
                                                                          }
#line 1743 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 65:
#line 380 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              scorep_oa_requests_add_metric_by_name( (yyvsp[0].String), NULL, SCOREP_METRIC_SOURCE_PERF );
                                                                          }
#line 1751 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 66:
#line 383 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              scorep_oa_requests_add_metric_by_name( (yyvsp[0].String), NULL, SCOREP_METRIC_SOURCE_RUSAGE );
                                                                          }
#line 1759 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 67:
#line 386 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              scorep_oa_requests_add_metric_by_name( (yyvsp[0].String), (yyvsp[-1].String), SCOREP_METRIC_SOURCE_PLUGIN );
                                                                          }
#line 1767 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 68:
#line 389 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              scorep_oa_requests_add_metric_by_name( (yyvsp[0].String), NULL, SCOREP_METRIC_OTHER );
                                                                          }
#line 1775 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 69:
#line 393 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              sprintf( json + strlen( json ), "        \"RegionID\":%d,\n", (yyvsp[0].Number) );
                                                                              tuning_action.region_id = (yyvsp[0].Number);
                                                                          }
#line 1784 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 70:
#line 398 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Tuning action: %s = %d", (yyvsp[-3].String), (yyvsp[-1].Number) );
                                                                              tuning_action.action_name = (yyvsp[-3].String);
                                                                              tuning_action.value = (yyvsp[-1].Number);
                                                                          }
#line 1794 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 71:
#line 404 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Tuning action type: VARIABLE" );
                                                                              tuning_action.kind = SCOREP_TUNING_VARIABLE;
                                                                          }
#line 1803 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;

  case 72:
#line 408 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1646  */
    {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Tuning action type: FUNCTION" );
                                                                              tuning_action.kind = SCOREP_TUNING_FUNCTION;
                                                                          }
#line 1812 "scorep_oa_scanner.c" /* yacc.c:1646  */
    break;


#line 1816 "scorep_oa_scanner.c" /* yacc.c:1646  */
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
#line 412 "../../build-mpi/../src/measurement/online_access/scorep_oa_scanner.y" /* yacc.c:1906  */



/******************************************************************************/
/* Error handling                                                             */
/******************************************************************************/
void yyerror( char* s )
{
    fprintf( stderr, "Unknown online access request: %s\n", s );
    fprintf( stderr, "Aborting...\n" );
    abort();
}
