
/*  A Bison parser, made from yaccsrc.yy
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	VARIABLE	257
#define	MENU	258
#define	SHORTCUT	259
#define	APP	260
#define	KEY	261
#define	IND	262
#define	EXITDLG	263
#define	STARTUP	264
#define	ACC	265
#define	TBSC	266
#define	VAR	267
#define	STRING	268
#define	FUNC	269
#define	PLUS	270
#define	MINUS	271

#line 21 "yaccsrc.yy"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "window/qvwm.h"
#include "parser/parse.h"
#include "lib/util.h"

extern int line;
extern char filename[256];
extern int yylex();
extern int yyerror(char* error);

#define YYDEBUG 0

#line 41 "yaccsrc.yy"
typedef union {
  char* str;
  MenuElem* mItem;
  AttrStream* aStream;
  unsigned int modifier;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		101
#define	YYFLAG		-32768
#define	YYNTBASE	21

#define YYTRANSLATE(x) ((unsigned)(x) <= 271 ? yytranslate[x] : 46)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,    19,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    18,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,    20,     2,     2,     2,     2,     2,     2,
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
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     3,     6,     8,    11,    13,    16,    18,    21,
    23,    26,    28,    31,    33,    36,    38,    41,    43,    46,
    48,    51,    53,    56,    58,    61,    63,    67,    71,    74,
    76,    80,    84,    90,    93,    95,   102,   107,   113,   117,
   124,   129,   135,   139,   142,   144,   147,   151,   153,   159,
   163,   166,   168,   172,   176,   179,   182,   184,   188,   192,
   196,   198,   201,   203,   206,   209,   211,   213,   216,   218,
   222,   225,   227,   231
};

static const short yyrhs[] = {    22,
     0,     0,    23,    22,     0,    23,     0,     3,    24,     0,
     3,     0,     6,    30,     0,     6,     0,     4,    26,     0,
     4,     0,     5,    28,     0,     5,     0,     7,    35,     0,
     7,     0,     8,    38,     0,     8,     0,     9,    33,     0,
     9,     0,    10,    40,     0,    10,     0,    11,    42,     0,
    11,     0,    12,    44,     0,    12,     0,    25,    24,     0,
    25,     0,    13,    18,    14,     0,    13,    18,    13,     0,
    27,    26,     0,    27,     0,    14,    14,    14,     0,    14,
    14,    15,     0,    14,    14,    16,    26,    17,     0,    29,
    28,     0,    29,     0,    14,    14,    14,    13,    19,    13,
     0,    14,    14,    14,    13,     0,    14,    14,    14,    19,
    13,     0,    14,    14,    14,     0,    14,    14,    15,    13,
    19,    13,     0,    14,    14,    15,    13,     0,    14,    14,
    15,    19,    13,     0,    14,    14,    15,     0,    31,    30,
     0,    31,     0,    14,    32,     0,    13,    19,    32,     0,
    13,     0,    13,    18,    14,    19,    32,     0,    13,    18,
    14,     0,    34,    33,     0,    34,     0,    13,    14,    15,
     0,    13,    14,    14,     0,    13,    14,     0,    36,    35,
     0,    36,     0,    13,    37,    14,     0,    13,    37,    15,
     0,    13,    20,    37,     0,    13,     0,    39,    38,     0,
    39,     0,    14,    14,     0,    41,    40,     0,    41,     0,
    14,     0,    43,    42,     0,    43,     0,    14,    13,    13,
     0,    45,    44,     0,    45,     0,    14,    14,    14,     0,
    14,    14,    15,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    59,    60,    63,    64,    67,    68,    69,    70,    71,    72,
    73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
    83,    84,    85,    86,    89,    90,    93,    94,    97,    98,
   101,   102,   103,   106,   107,   110,   112,   114,   116,   118,
   120,   122,   124,   128,   129,   132,   135,   136,   137,   138,
   141,   142,   145,   146,   147,   150,   151,   154,   155,   158,
   159,   162,   163,   166,   169,   170,   173,   176,   177,   180,
   183,   184,   187,   189
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","VARIABLE",
"MENU","SHORTCUT","APP","KEY","IND","EXITDLG","STARTUP","ACC","TBSC","VAR","STRING",
"FUNC","PLUS","MINUS","'='","','","'|'","qvwmrc","sessions","session","vars",
"var","items","item","scs","sc","apps","app","stream","dlgitems","dlgitem","keys",
"key","mod","inds","ind","cmds","cmd","accs","acc","tbscs","tbsc", NULL
};
#endif

static const short yyr1[] = {     0,
    21,    21,    22,    22,    23,    23,    23,    23,    23,    23,
    23,    23,    23,    23,    23,    23,    23,    23,    23,    23,
    23,    23,    23,    23,    24,    24,    25,    25,    26,    26,
    27,    27,    27,    28,    28,    29,    29,    29,    29,    29,
    29,    29,    29,    30,    30,    31,    32,    32,    32,    32,
    33,    33,    34,    34,    34,    35,    35,    36,    36,    37,
    37,    38,    38,    39,    40,    40,    41,    42,    42,    43,
    44,    44,    45,    45
};

static const short yyr2[] = {     0,
     1,     0,     2,     1,     2,     1,     2,     1,     2,     1,
     2,     1,     2,     1,     2,     1,     2,     1,     2,     1,
     2,     1,     2,     1,     2,     1,     3,     3,     2,     1,
     3,     3,     5,     2,     1,     6,     4,     5,     3,     6,
     4,     5,     3,     2,     1,     2,     3,     1,     5,     3,
     2,     1,     3,     3,     2,     2,     1,     3,     3,     3,
     1,     2,     1,     2,     2,     1,     1,     2,     1,     3,
     2,     1,     3,     3
};

static const short yydefact[] = {     2,
     6,    10,    12,     8,    14,    16,    18,    20,    22,    24,
     1,     4,     0,     5,    26,     0,     9,    30,     0,    11,
    35,     0,     7,    45,     0,    13,    57,     0,    15,    63,
     0,    17,    52,    67,    19,    66,     0,    21,    69,     0,
    23,    72,     3,     0,    25,     0,    29,     0,    34,    48,
    46,    44,    61,     0,    56,    64,    62,    55,    51,    65,
     0,    68,     0,    71,    28,    27,    31,    32,     0,    39,
    43,     0,     0,     0,    58,    59,    54,    53,    70,    73,
    74,     0,    37,     0,    41,     0,    50,    47,    60,    33,
     0,    38,     0,    42,     0,    36,    40,    49,     0,     0,
     0
};

static const short yydefgoto[] = {    99,
    11,    12,    14,    15,    17,    18,    20,    21,    23,    24,
    51,    32,    33,    26,    27,    54,    29,    30,    35,    36,
    38,    39,    41,    42
};

static const short yypact[] = {    -1,
    13,    -2,    21,    22,    24,    25,    27,    28,    29,    30,
-32768,    -1,    -3,-32768,    13,    31,-32768,    -2,    32,-32768,
    21,    34,-32768,    22,    35,-32768,    24,    36,-32768,    25,
    38,-32768,    27,-32768,-32768,    28,    40,-32768,    29,    41,
-32768,    30,-32768,     8,-32768,     2,-32768,    10,-32768,     9,
-32768,-32768,    18,    15,-32768,-32768,-32768,    17,-32768,-32768,
    43,-32768,    19,-32768,-32768,-32768,-32768,-32768,    -2,     0,
     1,    44,    34,    35,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,    37,    42,    46,    45,    47,    48,-32768,-32768,-32768,
    49,-32768,    50,-32768,    34,-32768,-32768,-32768,    57,    65,
-32768
};

static const short yypgoto[] = {-32768,
    54,-32768,    26,-32768,   -18,-32768,    51,-32768,    52,-32768,
   -72,    16,-32768,    53,-32768,    -6,    39,-32768,    55,-32768,
    56,-32768,    33,-32768
};


#define	YYLAST		95


static const short yytable[] = {    47,
    88,     1,     2,     3,     4,     5,     6,     7,     8,     9,
    10,    16,    83,    85,    44,    67,    68,    69,    84,    86,
    65,    66,    98,    70,    71,    13,    72,    73,    75,    76,
    77,    78,    80,    81,    19,    22,    25,    74,    28,    31,
    45,    34,    37,    40,    46,    48,    50,    53,    59,    56,
    82,    58,    61,    90,    63,    79,   100,    87,    92,    94,
    91,    96,    97,    93,   101,    43,    95,    89,    57,     0,
     0,    49,     0,     0,    64,    52,     0,     0,     0,    55,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    60,     0,     0,     0,    62
};

static const short yycheck[] = {    18,
    73,     3,     4,     5,     6,     7,     8,     9,    10,    11,
    12,    14,    13,    13,    18,    14,    15,    16,    19,    19,
    13,    14,    95,    14,    15,    13,    18,    19,    14,    15,
    14,    15,    14,    15,    14,    14,    13,    20,    14,    13,
    15,    14,    14,    14,    14,    14,    13,    13,    33,    14,
    69,    14,    13,    17,    14,    13,     0,    14,    13,    13,
    19,    13,    13,    19,     0,    12,    19,    74,    30,    -1,
    -1,    21,    -1,    -1,    42,    24,    -1,    -1,    -1,    27,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    36,    -1,    -1,    -1,    39
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/lib/bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "/usr/lib/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 59 "yaccsrc.yy"
{ DoAllSetting(); ;
    break;}
case 2:
#line 60 "yaccsrc.yy"
{ DoAllSetting(); ;
    break;}
case 9:
#line 71 "yaccsrc.yy"
{ CompleteMenu(yyvsp[-1].str, yyvsp[0].mItem); ;
    break;}
case 11:
#line 73 "yaccsrc.yy"
{ CompleteMenu(yyvsp[-1].str, yyvsp[0].mItem); ;
    break;}
case 17:
#line 79 "yaccsrc.yy"
{ CompleteMenu(yyvsp[-1].str, yyvsp[0].mItem); ;
    break;}
case 23:
#line 85 "yaccsrc.yy"
{ CompleteMenu(yyvsp[-1].str, yyvsp[0].mItem); ;
    break;}
case 27:
#line 93 "yaccsrc.yy"
{ AssignVariable(yyvsp[-2].str, yyvsp[0].str); ;
    break;}
case 28:
#line 94 "yaccsrc.yy"
{ AssignVariable(yyvsp[-2].str, yyvsp[0].str); ;
    break;}
case 29:
#line 97 "yaccsrc.yy"
{ yyval.mItem = ChainMenuItem(yyvsp[-1].mItem, yyvsp[0].mItem); ;
    break;}
case 30:
#line 98 "yaccsrc.yy"
{ yyval.mItem = ChainMenuItem(yyvsp[0].mItem, NULL); ;
    break;}
case 31:
#line 101 "yaccsrc.yy"
{ yyval.mItem = MakeExecItem(yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); ;
    break;}
case 32:
#line 102 "yaccsrc.yy"
{ yyval.mItem = MakeFuncItem(yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); ;
    break;}
case 33:
#line 103 "yaccsrc.yy"
{ yyval.mItem = MakeDirItem(yyvsp[-4].str, yyvsp[-3].str, yyvsp[-1].mItem); ;
    break;}
case 34:
#line 106 "yaccsrc.yy"
{ yyval.mItem = ChainMenuItem(yyvsp[-1].mItem, yyvsp[0].mItem); ;
    break;}
case 35:
#line 107 "yaccsrc.yy"
{ yyval.mItem = ChainMenuItem(yyvsp[0].mItem, NULL); ;
    break;}
case 36:
#line 111 "yaccsrc.yy"
{ yyval.mItem = MakeDesktopItem(yyvsp[-5].str, yyvsp[-4].str, yyvsp[-3].str, yyvsp[-2].str, yyvsp[0].str); ;
    break;}
case 37:
#line 113 "yaccsrc.yy"
{ yyval.mItem = MakeDesktopItem(yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str, NULL); ;
    break;}
case 38:
#line 115 "yaccsrc.yy"
{ yyval.mItem = MakeDesktopItem(yyvsp[-4].str, yyvsp[-3].str, yyvsp[-2].str, NULL, yyvsp[0].str); ;
    break;}
case 39:
#line 117 "yaccsrc.yy"
{ yyval.mItem = MakeDesktopItem(yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str, NULL, NULL); ;
    break;}
case 40:
#line 119 "yaccsrc.yy"
{ yyval.mItem = MakeDesktopFuncItem(yyvsp[-5].str, yyvsp[-4].str, yyvsp[-3].str, yyvsp[-2].str, yyvsp[0].str); ;
    break;}
case 41:
#line 121 "yaccsrc.yy"
{ yyval.mItem = MakeDesktopFuncItem(yyvsp[-3].str, yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str, NULL); ;
    break;}
case 42:
#line 123 "yaccsrc.yy"
{ yyval.mItem = MakeDesktopFuncItem(yyvsp[-4].str, yyvsp[-3].str, yyvsp[-2].str, NULL, yyvsp[0].str); ;
    break;}
case 43:
#line 125 "yaccsrc.yy"
{ yyval.mItem = MakeDesktopFuncItem(yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str, NULL, NULL); ;
    break;}
case 46:
#line 132 "yaccsrc.yy"
{ CreateAppHash(yyvsp[-1].str, yyvsp[0].aStream); ;
    break;}
case 47:
#line 135 "yaccsrc.yy"
{ yyval.aStream = MakeStream(yyvsp[-2].str, NULL, yyvsp[0].aStream); ;
    break;}
case 48:
#line 136 "yaccsrc.yy"
{ yyval.aStream = MakeStream(yyvsp[0].str, NULL, NULL); ;
    break;}
case 49:
#line 137 "yaccsrc.yy"
{ yyval.aStream = MakeStream(yyvsp[-4].str, yyvsp[-2].str, yyvsp[0].aStream); ;
    break;}
case 50:
#line 138 "yaccsrc.yy"
{ yyval.aStream = MakeStream(yyvsp[-2].str, yyvsp[0].str, NULL); ;
    break;}
case 51:
#line 141 "yaccsrc.yy"
{ yyval.mItem = ChainMenuItem(yyvsp[-1].mItem, yyvsp[0].mItem); ;
    break;}
case 52:
#line 142 "yaccsrc.yy"
{ yyval.mItem = ChainMenuItem(yyvsp[0].mItem, NULL); ;
    break;}
case 53:
#line 145 "yaccsrc.yy"
{ yyval.mItem = MakeDlgFuncItem(yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); ;
    break;}
case 54:
#line 146 "yaccsrc.yy"
{ yyval.mItem = MakeDlgItem(yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); ;
    break;}
case 55:
#line 147 "yaccsrc.yy"
{ yyval.mItem = MakeDlgFuncItem(yyvsp[-1].str, yyvsp[0].str, NULL); ;
    break;}
case 58:
#line 154 "yaccsrc.yy"
{ CreateSCKey(yyvsp[-2].str, yyvsp[-1].modifier, yyvsp[0].str); ;
    break;}
case 59:
#line 155 "yaccsrc.yy"
{ CreateSCKeyFunc(yyvsp[-2].str, yyvsp[-1].modifier, yyvsp[0].str); ;
    break;}
case 60:
#line 158 "yaccsrc.yy"
{ yyval.modifier = MakeModifier(yyvsp[-2].str, yyvsp[0].modifier); ;
    break;}
case 61:
#line 159 "yaccsrc.yy"
{ yyval.modifier = MakeModifier(yyvsp[0].str, 0); ;
    break;}
case 64:
#line 166 "yaccsrc.yy"
{ CreateIndicator(yyvsp[-1].str, yyvsp[0].str); ;
    break;}
case 67:
#line 173 "yaccsrc.yy"
{ if (!restart) ExecCommand(yyvsp[0].str); ;
    break;}
case 70:
#line 180 "yaccsrc.yy"
{ CreateAccessory(yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); ;
    break;}
case 71:
#line 183 "yaccsrc.yy"
{ yyval.mItem = ChainMenuItem(yyvsp[-1].mItem, yyvsp[0].mItem); ;
    break;}
case 72:
#line 184 "yaccsrc.yy"
{ yyval.mItem = ChainMenuItem(yyvsp[0].mItem, NULL); ;
    break;}
case 73:
#line 188 "yaccsrc.yy"
{ yyval.mItem = MakeExecItem(yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); ;
    break;}
case 74:
#line 190 "yaccsrc.yy"
{ yyval.mItem = MakeFuncItem(yyvsp[-2].str, yyvsp[-1].str, yyvsp[0].str); ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "/usr/lib/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 193 "yaccsrc.yy"

int yyerror(char* error)
{
  QvwmError("%s: %d: %s", filename, line, error);
  QvwmError("Restarting with the minimum configuration...");

  // restart without parsing any configuration files
  RestartQvwm(True);

  return 0;  // dummy
}
