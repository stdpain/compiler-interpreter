#include "../include/create.h"
#include "../include/interpreter.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

PrimaryExpression *createInteger(int i)
{
    PrimaryExpression *exp = Interpreter_malloc(sizeof(PrimaryExpression));
    exp->type = INTEGER_TYPE;
    exp->u.i = i;
    return exp;
}

PrimaryExpression *createStr_ID(char *p, int type)
{
    PrimaryExpression *exp = Interpreter_malloc(sizeof(PrimaryExpression));
    if (type == 0)
    {
        exp->type = STRING_TYPE;
        // exp->u.str = p;
        exp->u.mstring = allocMString(p, 1);
    }
    else
    {
        exp->type = IDENTIFIER_TYPE;
        exp->u.identifier = p;
    }
    return exp;
}

PrimaryExpression *createDouble(double d)
{
    PrimaryExpression *exp = Interpreter_malloc(sizeof(PrimaryExpression));
    exp->type = DOUBLE_TYPE;
    exp->u.d = d;
    return exp;
}

Expression *create_IntergerExpression(int i)
{
    Expression *exp = Interpreter_malloc(sizeof(Expression));
    exp->type = Expression_type_primary;
    exp->u.p = createInteger(i);
    return exp;
}

Expression *create_DoubleExpression(double i)
{
    Expression *exp = Interpreter_malloc(sizeof(Expression));
    exp->type = Expression_type_primary;
    exp->u.p = createDouble(i);
    return exp;
}

Expression *create_StrExpression(char *p)
{
    Expression *exp = Interpreter_malloc(sizeof(Expression));
    exp->type = Expression_type_primary;
    exp->u.p = createStr_ID(p, 0);
    return exp;
}

Expression *create_IDExpression(char *p)
{
    Expression *exp = Interpreter_malloc(sizeof(Expression));
    exp->type = Expression_type_primary;
    exp->u.p = createStr_ID(p, 1);
    return exp;
}

Binary_Expression *createBinaryExpression(enum ExpressionAction action, Expression *left, Expression *right)
{
    Binary_Expression *exp = Interpreter_malloc(sizeof(Binary_Expression));
    exp->action = action;
    exp->left = left;
    exp->right = right;
    return exp;
}

Assign_Expression *createAssignExpression(char *c, Expression *expression)
{
    Assign_Expression *exp = Interpreter_malloc(sizeof(Assign_Expression));
    exp->expression = expression;
    exp->identifier = c;
    return exp;
}

Expression *binExpressionWarpper(Binary_Expression *expression)
{
    Expression *exp = Interpreter_malloc(sizeof(Expression));
    exp->type = Expression_type_binary;
    exp->u.b = expression;
    return exp;
}

Expression *AssignExpressionWarpper(Assign_Expression *expression)
{
    Expression *exp = Interpreter_malloc(sizeof(Assign_Expression));
    exp->type = Expression_type_assign;
    exp->u.a = expression;
    return exp;
}

Expression *create_INVExpression(Expression *expression)
{
    expression = Interpreter_malloc(sizeof(Expression));
    expression->type = Expression_type_inv;
    expression->u.e = expression;
    return expression;
}

Expression *create_FuncCallExpression(char *identifier, ParamList *params)
{
    Expression *expression = Interpreter_malloc(sizeof(Expression));
    expression->type = Expression_type_func;
    expression->u.func = Interpreter_malloc(sizeof(FuncCallExpression));
    expression->u.func->funname = identifier;
    expression->u.func->params = params;
    expression->u.func->func = NULL;
    return expression;
}

Statement_list *createStatementList(Statement *statement)
{
    Statement_list *list = Interpreter_malloc(sizeof(Statement_list));
    list->next = NULL;
    list->statement = statement;
    return list;
}

Statement *create_Statement(enum StatementType type)
{
    Statement *statement = Interpreter_malloc(sizeof(Statement));
    statement->type = type;
    statement->u.e = NULL;
    return statement;
}

Statement *create_ExpressionStatement(Expression *expression)
{
    Statement *s = Interpreter_malloc(sizeof(Statement));
    s->type = Statement_Type_Expression;
    s->u.e = Interpreter_malloc(sizeof(ExpressionStatement));
    s->u.e->expression = expression;
    return s;
}

Statement *create_IFStatement(Expression *condition, Statement_list *list)
{
    Statement *statement = Interpreter_malloc(sizeof(Statement));
    statement->type = Statement_Type_IF;
    statement->u.i = Interpreter_malloc(sizeof(For_Statement));
    statement->u.i->expression = condition;
    statement->u.i->statementlist = list;
    return statement;
}

Statement *create_FORStatement(Expression *before, Expression *condition,
                               Expression *after, Statement_list *list)
{
    Statement *statement = Interpreter_malloc(sizeof(Statement));
    statement->type = Statement_Type_For;
    statement->u.f = Interpreter_malloc(sizeof(For_Statement));
    statement->u.f->before = before;
    statement->u.f->condition = condition;
    statement->u.f->after = after;
    statement->u.f->statementlist = list;
    return statement;
}

Statement_list *StatementList_add(Statement_list *list, Statement *statement)
{
    if (list == NULL)
    {
        list = createStatementList(statement);
    }
    else
    {
        Statement_list *pos;
        for (pos = list; pos->next; pos = pos->next)
            ;
        pos->next = createStatementList(statement);
    }
    return list;
}

ParamList *createParamList(char *identifier)
{
    ParamList *list = Interpreter_malloc(sizeof(ParamList));
    list->identifier = identifier;
    list->next = NULL;
    return list;
}
ParamList *ParamList_add(ParamList *list, ParamList *param)
{
    if (list == NULL)
    {
        list = param;
    }
    else
    {
        ParamList *pos;
        for (pos = list; pos->next; pos = pos->next)
            ;
        pos->next = param;
    }
    return list;
}

Function_t *Functon_define(char *identifier, ParamList *params, Statement_list *list)
{
    Function_t *func = Interpreter_malloc(sizeof(Function_t));
    func->identifier = identifier;
    func->list = list;
    func->paramlist = params;
    func->next = NULL;
    func->isNative = 0;
    func->funcAddr = NULL;
    return func;
}
Function_t *Function_add(Function_t *functionlist, Function_t *func)
{
    if (functionlist == NULL)
    {
        functionlist = func;
    }
    else
    {
        Function_t *pos;
        for (pos = functionlist; pos->next; pos = pos->next)
            ;
        pos->next = func;
    }
    return functionlist;
}
Function_t *Function_find(Function_t *funclist, char *identifier)
{
    Function_t *pos;
    for (pos = funclist; pos; pos = pos->next)
    {
        if (!strcmp(identifier, pos->identifier))
            return pos;
    }
    return NULL;
}

static char *buffer;
int pos;

void open_charbuffer()
{
    buffer = Interpreter_malloc(100);
    pos = 0;
}
void addc_charbuffer(char c)
{
    assert(buffer);
    assert(pos < 100);
    buffer[pos++] = c;
}
char *flush_charbuffer()
{
    char *p = buffer;
    buffer[pos] = 0;
    buffer = NULL;
    return p;
}
