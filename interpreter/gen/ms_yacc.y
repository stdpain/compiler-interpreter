%{
#include <stdio.h>
#include "ast/creator.h"
#include "ast/exprs.h"
#include "ast/parser.h"
#define YYDEBUG 1
%}

%{
using Parser = stdpain::Parser;
// https://stackoverflow.com/questions/23717039/generating-a-compiler-from-lex-and-yacc-grammar
int yylex();
void yyerror(const char *s);
%}

%union {
    const char* identifier;
    Expression* expression;
    int integer;
    double db;
    Statement* statement;
    StatementList* statement_list;
    Function* function;
    ParamList* paramlist;
    Param* param;
    ExpressionList* expression_list; 
};

%token <integer> INT_LITERAL
%token <db> DOUBLE_LITERAL

%token  <identifier>VAR FUNCTION GLOBAL FOR IF ELIF ELSE ADD SUB MUL DIV ASSIGN
        EQ NE GT GE LT LE LP RP LC RC SEMICOLON IDENTIFIER 
        BREAK CONTINUE RETURN COMMA STRING_LITERAL

%type <param>arg
%type <paramlist>arglist
%type <function>function_definition
        
%type <expression_list> expressions
%type <expression> definition_or_statement
        expression value_expression compare_expression add_sub_expression mul_div_expression
        primary_expression expression_option

%type <statement_list> block statement_list

%type <statement> expression_statement return_statement continue_statement break_statement
                for_statement statement if_statement global_declaration

%%
translation_unit: definition_or_statement 
        | translation_unit definition_or_statement
        ;
definition_or_statement:function_definition
        {
                auto instance = Parser::getInstance();
                instance->append_function($1);
        }
        |statement
        {
                Parser::getInstance()->append_statement($1);
        }
        ;
function_definition: FUNCTION IDENTIFIER LP arglist RP block
        {
                auto parser = Parser::getInstance();
                $$ = parser->new_function($2, $4, $6);
        }
        |
        FUNCTION IDENTIFIER LP RP block
        {
                auto parser = Parser::getInstance();
                $$ = parser->new_function($2, parser->create_param_list(), $5);
        }
        ;
statement:expression_statement
        |global_declaration
        |for_statement 
        |if_statement
        |break_statement
        |continue_statement
        |return_statement
        ;
expression_statement:expression SEMICOLON
        {
                $$ = Parser::getInstance()->new_expression_statement($1);
        }
        ;
expression: value_expression
        |IDENTIFIER ASSIGN expression
        {
                $$ = Parser::getInstance()->new_assign_expression($1, $3);
        }
        ;
value_expression: compare_expression
        |value_expression EQ compare_expression
        {
                $$ = Parser::getInstance()->new_binary_expression(BinaryExprType::EQ, $1, $3);
        }
        |value_expression NE compare_expression
        {
                $$ = Parser::getInstance()->new_binary_expression(BinaryExprType::NE, $1, $3);
        }
        ;
compare_expression:add_sub_expression 
        |compare_expression GT add_sub_expression
        {
                $$ = Parser::getInstance()->new_binary_expression(BinaryExprType::GT, $1, $3);
        }
        |compare_expression GE add_sub_expression
        {
                $$ = Parser::getInstance()->new_binary_expression(BinaryExprType::GE, $1, $3);
        }
        |compare_expression LT add_sub_expression
        {
                $$ = Parser::getInstance()->new_binary_expression(BinaryExprType::LT, $1, $3);
        }
        |compare_expression LE add_sub_expression
        {
                $$ = Parser::getInstance()->new_binary_expression(BinaryExprType::LE, $1, $3);
        }
        ;
add_sub_expression:mul_div_expression 
        |add_sub_expression ADD mul_div_expression
        {
                $$ = Parser::getInstance()->new_binary_expression(BinaryExprType::ADD, $1, $3);
        }
        |add_sub_expression SUB mul_div_expression
        {
                $$ = Parser::getInstance()->new_binary_expression(BinaryExprType::SUB, $1, $3);
        }
        ;
mul_div_expression:primary_expression
        |mul_div_expression DIV primary_expression
        {
                $$ = Parser::getInstance()->new_binary_expression(BinaryExprType::DIV, $1, $3);
        }
        |mul_div_expression MUL primary_expression
        {
                $$ = Parser::getInstance()->new_binary_expression(BinaryExprType::MUL, $1, $3);
        }
        ;
primary_expression:SUB primary_expression
        {
                $$ = Parser::getInstance()->new_unary_expression(UnaryExprType::INV, $2);
        }
        |LP expression RP
        {
                $$ = $2;
        }
        |IDENTIFIER
        {
                $$ = Parser::getInstance()->new_primary_expression($1, true);
        }
        |STRING_LITERAL
        {
                $$ = Parser::getInstance()->new_primary_expression($1, false);
        }
        |INT_LITERAL
        {
                $$ = Parser::getInstance()->new_primary_expression($1);
        }
        |DOUBLE_LITERAL
        {
                $$ = Parser::getInstance()->new_primary_expression($1);
        }
        |IDENTIFIER LP RP
        {
                auto instance = Parser::getInstance();
                auto empty_expression = instance->create_expression_list();
                $$ = instance->new_functioncall_expression($1, empty_expression);
        }
        |IDENTIFIER LP expressions RP
        {
                auto instance = Parser::getInstance();
                $$ = instance->new_functioncall_expression($1, $3);
        }
        ;
expressions:expressions COMMA expression
        {
                auto instance = Parser::getInstance();
                $$ = instance->add_expression($1, $3);
        }
        |expression
        {
                auto instance = Parser::getInstance();
                auto expressions = instance->create_expression_list();
                $$ = instance->add_expression(expressions, $1);
        }
        ;
statement_list:statement_list statement
        {
                $$ = Parser::getInstance()->add_statement($1, $2);
        }
        |statement
        {
                $$ = Parser::getInstance()->create_statement_list();
        }
        ;
block:LC RC
        {
                $$ = Parser::getInstance()->create_statement_list();
        }
        |LC statement_list RC
        {
                $$ = $2;
        }
        ;
arglist:arglist COMMA arg
        {
                $$ = Parser::getInstance()->add_param($1, $3);
        }
        |
        arg
        {
                auto instance = Parser::getInstance();
                auto param_list = instance->create_param_list();
                $$ = instance->add_param(param_list, $1);
        }
        ;
arg:    IDENTIFIER
        {
                $$ = Parser::getInstance()->create_param($1);
        }
        ;
expression_option: { $$ = Parser::getInstance()->new_empty_expression(); }
        |expression
        ;
for_statement: FOR LP expression_option SEMICOLON expression_option SEMICOLON expression_option RP block
        {
                $$ = Parser::getInstance()->new_statement<ForStatement>($3, $5, $7, $9);
        }       
        ;
if_statement: IF LP expression RP block
        {
                $$ = Parser::getInstance()->new_statement<IfStatement>($3, $5);
        }
        ;
break_statement:BREAK SEMICOLON
        {
                $$ = Parser::getInstance()->new_statement<BreakStatement>();
        }
        ;
continue_statement:CONTINUE SEMICOLON
        {
                $$ = Parser::getInstance()->new_statement<ContinueStatement>();
        }
        ;
return_statement:RETURN IDENTIFIER
        {
                $$ = Parser::getInstance()->new_statement<ReturnStatement>();
        }
        ;
global_declaration:GLOBAL IDENTIFIER
        {
                // $$=NULL;
        }
        ;

