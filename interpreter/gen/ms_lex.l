%{
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "ast/exprs.h"
#include "ast/ast_builder.h"
#include "gen/ms_yacc.y.hpp"
#include "parser/context.h"
#include "logging/interpreter_logger.hpp"
#define YYSTYPE MSSTYPE
#define YY_EXTRA_TYPE MSParseContext*
using namespace ms;
%}

%option reentrant
%option noyywrap
%option bison-bridge
%option prefix="ms"

%start COMMENT STRING_LITERAL_STATE

%%
<INITIAL>"function"  return FUNCTION;
<INITIAL>"global"    return GLOBAL;
<INITIAL>"for"       return FOR;
<INITIAL>"if"        return IF;
<INITIAL>"elif"      return ELIF;
<INITIAL>"else"      return ELSE;
<INITIAL>"+"         return ADD;
<INITIAL>"-"         return SUB;
<INITIAL>"*"         return MUL;
<INITIAL>"/"         return DIV;
<INITIAL>"="         return ASSIGN;
<INITIAL>"=="        return EQ;
<INITIAL>"!="        return NE;
<INITIAL>">"         return GT;
<INITIAL>">="        return GE;
<INITIAL>"<"         return LT;
<INITIAL>"<="        return LE;
<INITIAL>"("         return LP;
<INITIAL>")"         return RP;
<INITIAL>"{"         return LC;
<INITIAL>"}"         return RC;
<INITIAL>"["         return LSB;
<INITIAL>"]"         return RSB;
<INITIAL>";"         return SEMICOLON;
<INITIAL>"break"     return BREAK;
<INITIAL>"continue"  return CONTINUE;
<INITIAL>"return"    return RETURN;
<INITIAL>","         return COMMA;

<INITIAL>[A-Za-z_][A-Za-z_0-9]* {
    msget_lval(yyscanner)->identifier = yyget_extra(yyscanner)->ast_builder->new_str(yytext);
    return IDENTIFIER;
}
<INITIAL>([1-9][0-9]*) {
    int temp;
    sscanf(yytext, "%d", &temp);
    msget_lval(yyscanner)->integer = temp;
    return INT_LITERAL;
}
<INITIAL>[0-9]+\.[0-9]+ {
    double temp;
    sscanf(yytext, "%lf", &temp);
    msget_lval(yyscanner)->db = temp;
    return DOUBLE_LITERAL;
}

<INITIAL>\" {
    yyget_extra(yyscanner)->ast_builder->buffer_open();
    BEGIN STRING_LITERAL_STATE;
}
<INITIAL>[ \t] ;
<INITIAL>\n ;
<INITIAL>#     BEGIN COMMENT;
<INITIAL>.      {
    //error
    char buf[1024];
    if (isprint(yytext[0])) {
        buf[0] = yytext[0];
        buf[1] = '\0';
    } else {
        sprintf(buf, "0x%02x", (unsigned char)yytext[0]);
    }
    printf("meet:%s",buf);
    abort();
}

<COMMENT>\n     {
    BEGIN INITIAL;
}
<COMMENT>.      ;

<STRING_LITERAL_STATE>\"        {
    msget_lval(yyscanner)->identifier = yyget_extra(yyscanner)->ast_builder->charbuffer_flush();
    BEGIN INITIAL;
    return STRING_LITERAL;
}
<STRING_LITERAL_STATE>\n        {
    yyget_extra(yyscanner)->ast_builder->charbuffer_append('\n');
}
<STRING_LITERAL_STATE>\\\"      yyget_extra(yyscanner)->ast_builder->charbuffer_append('"');
<STRING_LITERAL_STATE>\\n       yyget_extra(yyscanner)->ast_builder->charbuffer_append('\n');
<STRING_LITERAL_STATE>\\t       yyget_extra(yyscanner)->ast_builder->charbuffer_append('\t');
<STRING_LITERAL_STATE>\\\\      yyget_extra(yyscanner)->ast_builder->charbuffer_append('\\');
<STRING_LITERAL_STATE>.         yyget_extra(yyscanner)->ast_builder->charbuffer_append(yytext[0]);
%%
