%{
#include <stdio.h>
#include "ast/exprs.h"
#include "ast/ast_builder.h"
#include "gen/ms_yacc.y.hpp"
#include "gen/ms_lex.l.h"
#include "logging/interpreter_logger.hpp"

#define YYDEBUG 1
%}

%code requires {
#include "parser/context.h"
}


%{
int yylex (MSSTYPE * yylval_param , yyscan_t yyscanner);
void yyerror(MSParseContext*, const char *s);

using ASTBuilder = ms::ASTBuilder;
#define MSLEX_PARAM ctx->scanner
%}

%define api.prefix {ms}
%define api.pure full
%parse-param { MSParseContext* ctx }
%lex-param { MSLEX_PARAM }

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
        EQ NE GT GE LT LE LP RP LC RC LSB RSB SEMICOLON IDENTIFIER 
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
                ctx->ast_builder->append_function($1);
        }
        |statement
        {
                ctx->ast_builder->append_statement($1);
        }
        ;
function_definition: FUNCTION IDENTIFIER LP arglist RP block
        {
                ctx->ast_builder->new_function($2, $4, $6);
        }
        |
        FUNCTION IDENTIFIER LP RP block
        {
                auto instance = ctx->ast_builder;
                instance->new_function($2, instance->create_param_list(), $5);
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
                $$ = ctx->ast_builder->new_expression_statement($1);
        }
        ;
expression: value_expression
        |IDENTIFIER ASSIGN expression
        {
                $$ = ctx->ast_builder->new_assign_expression($1, $3);
        }
        ;
value_expression: compare_expression
        |value_expression EQ compare_expression
        {
                $$ = ctx->ast_builder->new_binary_expression(BinaryExprType::EQ, $1, $3);
        }
        |value_expression NE compare_expression
        {
                $$ = ctx->ast_builder->new_binary_expression(BinaryExprType::NE, $1, $3);
        }
        ;
compare_expression:add_sub_expression 
        |compare_expression GT add_sub_expression
        {
                $$ = ctx->ast_builder->new_binary_expression(BinaryExprType::GT, $1, $3);
        }
        |compare_expression GE add_sub_expression
        {
                $$ = ctx->ast_builder->new_binary_expression(BinaryExprType::GE, $1, $3);
        }
        |compare_expression LT add_sub_expression
        {
                $$ = ctx->ast_builder->new_binary_expression(BinaryExprType::LT, $1, $3);
        }
        |compare_expression LE add_sub_expression
        {
                $$ = ctx->ast_builder->new_binary_expression(BinaryExprType::LE, $1, $3);
        }
        ;
add_sub_expression:mul_div_expression 
        |add_sub_expression ADD mul_div_expression
        {
                $$ = ctx->ast_builder->new_binary_expression(BinaryExprType::ADD, $1, $3);
        }
        |add_sub_expression SUB mul_div_expression
        {
                $$ = ctx->ast_builder->new_binary_expression(BinaryExprType::SUB, $1, $3);
        }
        ;
mul_div_expression:primary_expression
        {
                $$=$1;
        }
        |mul_div_expression DIV primary_expression
        {
                $$ = ctx->ast_builder->new_binary_expression(BinaryExprType::DIV, $1, $3);
        }
        |mul_div_expression MUL primary_expression
        {
                $$ = ctx->ast_builder->new_binary_expression(BinaryExprType::MUL, $1, $3);
        }
        ;
primary_expression:SUB primary_expression
        {
                $$ = ctx->ast_builder->new_unary_expression(UnaryExprType::INV, $2);
        }
        |LP expression RP
        {
                $$ = $2;
        }
        |IDENTIFIER
        {
                $$ = ctx->ast_builder->new_primary_expression($1, true);
        }
        |STRING_LITERAL
        {
                $$ = ctx->ast_builder->new_primary_expression($1, false);
        }
        |INT_LITERAL
        {
                $$ = ctx->ast_builder->new_primary_expression($1);
        }
        |DOUBLE_LITERAL
        {
                $$ = ctx->ast_builder->new_primary_expression($1);
        }
        |IDENTIFIER LP expressions RP
        {
                auto instance = ctx->ast_builder;
                $$ = instance->new_functioncall_expression($1, $3);
        }
        |IDENTIFIER LSB expression RSB
        {
                auto instance = ctx->ast_builder;
                $$ = instance->new_expression<ArrayAccessExpresion>($1, $3);
        }
        ;
expressions: 
        {
                auto instance = ctx->ast_builder;
                $$ = instance->create_expression_list();
        }
        | expressions COMMA expression
        {
                auto instance = ctx->ast_builder;
                $$ = instance->add_expression($1, $3);
        }
        | expression
        {
                auto instance = ctx->ast_builder;
                $$ = instance->add_expression(instance->create_expression_list(), $1);
        }
        ;
statement_list:statement_list statement
        {
                $$ = ctx->ast_builder->add_statement($1, $2);
        }
        |statement
        {
                $$ = ctx->ast_builder->create_statement_list();
        }
        ;
block:LC RC
        {
                $$ = ctx->ast_builder->create_statement_list();
        }
        |LC statement_list RC
        {
                $$ = $2;
        }
        ;
arglist:arglist COMMA arg
        {
                $$ = ctx->ast_builder->add_param($1, $3);
        }
        |
        arg
        {
                auto instance = ctx->ast_builder;
                auto param_list = instance->create_param_list();
                $$ = instance->add_param(param_list, $1);
        }
        ;
arg:    IDENTIFIER
        {
                $$ = ctx->ast_builder->create_param($1);
        }
        ;
expression_option: { $$ = ctx->ast_builder->new_empty_expression(); }
        |expression
        ;
for_statement: FOR LP expression_option SEMICOLON expression_option SEMICOLON expression_option RP block
        {
                $$ = ctx->ast_builder->new_statement<ForStatement>($3, $5, $7, $9);
        }       
        ;
if_statement: IF LP expression RP block
        {
                $$ = ctx->ast_builder->new_statement<IfStatement>($3, $5);
        }
        ;
break_statement:BREAK SEMICOLON
        {
                $$ = ctx->ast_builder->new_statement<BreakStatement>();
        }
        ;
continue_statement:CONTINUE SEMICOLON
        {
                $$ = ctx->ast_builder->new_statement<ContinueStatement>();
        }
        ;
return_statement:RETURN IDENTIFIER
        {
                $$ = ctx->ast_builder->new_statement<ReturnStatement>();
        }
        ;
global_declaration:GLOBAL IDENTIFIER
        {
                // $$=NULL;
        }
        ;
%%
