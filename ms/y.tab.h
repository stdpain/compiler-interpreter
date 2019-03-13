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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    INT_LITERAL = 258,
    DOUBLE_LITERAL = 259,
    VAR = 260,
    FUNCTION = 261,
    GLOBAL = 262,
    FOR = 263,
    IF = 264,
    ELIF = 265,
    ELSE = 266,
    ADD = 267,
    SUB = 268,
    MUL = 269,
    DIV = 270,
    ASSIGN = 271,
    EQ = 272,
    NE = 273,
    GT = 274,
    GE = 275,
    LT = 276,
    LE = 277,
    LP = 278,
    RP = 279,
    LC = 280,
    RC = 281,
    SEMICOLON = 282,
    IDENTIFIER = 283,
    BREAK = 284,
    CONTINUE = 285,
    RETURN = 286,
    COMMA = 287,
    STRING_LITERAL = 288
  };
#endif
/* Tokens.  */
#define INT_LITERAL 258
#define DOUBLE_LITERAL 259
#define VAR 260
#define FUNCTION 261
#define GLOBAL 262
#define FOR 263
#define IF 264
#define ELIF 265
#define ELSE 266
#define ADD 267
#define SUB 268
#define MUL 269
#define DIV 270
#define ASSIGN 271
#define EQ 272
#define NE 273
#define GT 274
#define GE 275
#define LT 276
#define LE 277
#define LP 278
#define RP 279
#define LC 280
#define RC 281
#define SEMICOLON 282
#define IDENTIFIER 283
#define BREAK 284
#define CONTINUE 285
#define RETURN 286
#define COMMA 287
#define STRING_LITERAL 288

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 7 "ms.y" /* yacc.c:1909  */

    char                *identifier;
    Expression          *expression;
    int integer;
    double db;
    Statement*statement;
    Statement_list*statement_list;
    Function_t*function;
    ParamList*paramlist;

#line 131 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
