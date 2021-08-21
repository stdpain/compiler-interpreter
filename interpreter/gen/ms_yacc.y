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
};

%token <integer> INT_LITERAL
%token <db> DOUBLE_LITERAL

%token  <identifier>VAR FUNCTION GLOBAL FOR IF ELIF ELSE ADD SUB MUL DIV ASSIGN
        EQ NE GT GE LT LE LP RP LC RC SEMICOLON IDENTIFIER 
        BREAK CONTINUE RETURN COMMA STRING_LITERAL

%type <paramlist>arglist arg
%type <function>function_definition
        
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
                // Function_t*funclist = Interpreter_getFunclist();
                // funclist = Function_add(funclist, $1);
                // Interpreter_setFunclist(funclist);
        }
        |statement
        {
                Parser::getInstance()->append_statement($1);
        }
        ;
function_definition: FUNCTION IDENTIFIER LP arglist RP block
        {
                // $$=Functon_define($2,$4,$6);
        }
        |
        FUNCTION IDENTIFIER LP RP block
        {
                // $$=Functon_define($2,NULL,$5);
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
                $$ = stdpain::Parser::getInstance()->new_expression_statement($1);
        }
        ;
expression: value_expression
        |IDENTIFIER ASSIGN expression
        {
                $$ = stdpain::Parser::getInstance()->new_assign_expression($1, $3);
        }
        ;
value_expression: compare_expression
        |value_expression EQ compare_expression
        {
                $$ = stdpain::Parser::getInstance()->new_binary_expression(BinaryExprType::EQ, $1, $3);
        }
        |value_expression NE compare_expression
        {
                $$ = stdpain::Parser::getInstance()->new_binary_expression(BinaryExprType::NE, $1, $3);
        }
        ;
compare_expression:add_sub_expression 
        |compare_expression GT add_sub_expression
        {
                $$ = stdpain::Parser::getInstance()->new_binary_expression(BinaryExprType::GT, $1, $3);
        }
        |compare_expression GE add_sub_expression
        {
                $$ = stdpain::Parser::getInstance()->new_binary_expression(BinaryExprType::GE, $1, $3);
        }
        |compare_expression LT add_sub_expression
        {
                $$ = stdpain::Parser::getInstance()->new_binary_expression(BinaryExprType::LT, $1, $3);
        }
        |compare_expression LE add_sub_expression
        {
                $$ = stdpain::Parser::getInstance()->new_binary_expression(BinaryExprType::LE, $1, $3);
        }
        ;
add_sub_expression:mul_div_expression 
        |add_sub_expression ADD mul_div_expression
        {
                $$ = stdpain::Parser::getInstance()->new_binary_expression(BinaryExprType::ADD, $1, $3);
        }
        |add_sub_expression SUB mul_div_expression
        {
                $$ = stdpain::Parser::getInstance()->new_binary_expression(BinaryExprType::SUB, $1, $3);
        }
        ;
mul_div_expression:primary_expression
        |mul_div_expression DIV primary_expression
        {
                $$ = stdpain::Parser::getInstance()->new_binary_expression(BinaryExprType::DIV, $1, $3);
        }
        |mul_div_expression MUL primary_expression
        {
                $$ = stdpain::Parser::getInstance()->new_binary_expression(BinaryExprType::MUL, $1, $3);
        }
        ;
primary_expression:SUB primary_expression
        {
                // $$=$2;
        }
        |LP expression RP
        {
                $$ = $2;
        }
        |IDENTIFIER
        {
                $$ = stdpain::Parser::getInstance()->new_primary_expression($1, true);
        }
        |STRING_LITERAL
        {
                $$ = stdpain::Parser::getInstance()->new_primary_expression($1, false);
        }
        |INT_LITERAL
        {
                $$ = stdpain::Parser::getInstance()->new_primary_expression($1);
        }
        |DOUBLE_LITERAL
        {
                $$ = stdpain::Parser::getInstance()->new_primary_expression($1);
        }
        |IDENTIFIER LP RP
        {
                // $$=create_FuncCallExpression($1, NULL);
        }
        |IDENTIFIER LP arglist RP
        {
                // $$=create_FuncCallExpression($1, $3);
        }
        ;
statement_list:statement_list statement
        {
                $$ = stdpain::Parser::getInstance()->add_statement($1, $2);
        }
        |statement
        {
                $$ = stdpain::Parser::getInstance()->create_statement_list();
        }
        ;
block:LC RC
        {
                $$ = NULL;
        }
        |LC statement_list RC
        {
                $$ = $2;
        }
        ;
arglist:arglist COMMA arg
        {
                // $$=ParamList_add($1,$3);
        }
        |
        arg
        ;
arg:    IDENTIFIER
        {
                // $$=createParamList($1);
        }
        ;
expression_option:{$$=NULL;}
        |expression
        ;
for_statement: FOR LP expression_option SEMICOLON expression_option SEMICOLON expression_option  RP block
        {
                // $$=create_FORStatement($3,$5,$7,$9);
        }       
        ;
if_statement: IF LP expression RP block
        {
                // $$=create_IFStatement($3,$5);
        }
        ;
break_statement:BREAK SEMICOLON
        {
                // $$=create_Statement(Statement_Type_Break);
        }
        ;
continue_statement:CONTINUE SEMICOLON
        {
                // $$=create_Statement(Statement_Type_Continue);
        }
        ;
return_statement:RETURN IDENTIFIER
        {
                // $$=create_Statement(Statement_Type_Rtn);
        }
        ;
global_declaration:GLOBAL IDENTIFIER
        {
                // $$=NULL;
        }
        ;

