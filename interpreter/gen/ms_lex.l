%{
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "ast/creator.h"
#include "ast/exprs.h"
#include "ast/parser.h"
#include "gen/ms_yacc.y.hpp"


/* 一个功能性函数-都要加 */
extern "C" {
int yywrap(void)
{
    return 1;
}
}


%}

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
<INITIAL>";"         return SEMICOLON;
<INITIAL>"break"     return BREAK;
<INITIAL>"continue"  return CONTINUE;
<INITIAL>"return"    return RETURN;
<INITIAL>","         return COMMA;

<INITIAL>[A-Za-z_][A-Za-z_0-9]* {
    yylval.identifier = stdpain::Parser::getInstance()->new_str(yytext);
    return IDENTIFIER;
}
<INITIAL>([1-9][0-9]*)|"0" {
    int temp;
    sscanf(yytext, "%d", &temp);
    yylval.integer = temp;
    return INT_LITERAL;
}
<INITIAL>[0-9]+\.[0-9]+ {
    double temp;
    sscanf(yytext, "%lf", &temp);
    yylval.db = temp;
    return DOUBLE_LITERAL;
}

<INITIAL>\" {
    charbuffer_open();
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
    auto parser = stdpain::Parser::getInstance();
    yylval.identifier = parser->new_str(charbuffer_flush());
    BEGIN INITIAL;
    return STRING_LITERAL;
}
<STRING_LITERAL_STATE>\n        {
    charbuffer_append('\n');
}
<STRING_LITERAL_STATE>\\\"      charbuffer_append('"');
<STRING_LITERAL_STATE>\\n       charbuffer_append('\n');
<STRING_LITERAL_STATE>\\t       charbuffer_append('\t');
<STRING_LITERAL_STATE>\\\\      charbuffer_append('\\');
<STRING_LITERAL_STATE>.         charbuffer_append(yytext[0]);
%%
