#ifndef __CREATE_H__
#define __CREATE_H__

typedef struct PrimaryExpression_tag PrimaryExpression;
typedef struct Binary_Expression_tag Binary_Expression;
typedef struct Expression_tag Expression;
typedef union Expression_uni Expression_u;
typedef struct Assign_Expression_tag Assign_Expression;
typedef struct FuncCallExpression_tag FuncCallExpression;

typedef struct Expression_Statement_tag ExpressionStatement;
typedef struct For_Statement_tag For_Statement;
typedef struct If_Statement_tag If_Statement;
typedef struct Statement_tag Statement;
typedef struct Statement_list_tag Statement_list;

typedef struct ParamList_tag ParamList;
typedef struct Function_tag Function_t;

union Expression_uni {
    PrimaryExpression *p;
    Binary_Expression *b;
    Assign_Expression *a;
    Expression *e;
    FuncCallExpression *func;
};

enum Expression_type
{
    Expression_type_primary,
    Expression_type_binary,
    Expression_type_assign,
    Expression_type_inv,
    Expression_type_func
};

struct Expression_tag
{
    enum Expression_type type;
    Expression_u u;
};

enum ValueType
{
    INTEGER_TYPE = 1,
    DOUBLE_TYPE,
    STRING_TYPE,
    IDENTIFIER_TYPE,
    INVALID_TYPE
};

struct FuncCallExpression_tag
{
    char *funname;
    Function_t *func;
    ParamList *params;
};

struct PrimaryExpression_tag
{
    enum ValueType type;
    union {
        int i;
        double d;
        char *str;
        char *identifier;
    } u;
};

enum ExpressionAction
{
    ADD_OPERATOR = 1,
    SUB_OPERATOR,
    MUL_OPERATOR,
    DIV_OPERATOR,
    GT_OPERATOR,
    GE_OPERATOR,
    LT_OPERATOR,
    LE_OPERATOR,
    EQ_OPERATOR,
    NE_OPERATOR
};

struct Binary_Expression_tag
{
    enum ExpressionAction action;
    Expression *left;
    Expression *right;
};

struct Assign_Expression_tag
{
    char *identifier;
    Expression *expression;
};

enum StatementType
{
    Statement_Type_Expression = 1,
    Statement_Type_IF,
    Statement_Type_For,
    Statement_Type_Break,
    Statement_Type_Continue,
    Statement_Type_Rtn
};

struct Expression_Statement_tag
{
    Expression *expression;
};
struct For_Statement_tag
{
    Expression *before;
    Expression *condition;
    Expression *after;
    Statement_list *statementlist;
};
struct If_Statement_tag
{
    Expression *expression;
    Statement_list *statementlist;
};

struct Statement_tag
{
    enum StatementType type;
    union {
        ExpressionStatement *e;
        For_Statement *f;
        If_Statement *i;
    } u;
};
struct Statement_list_tag
{
    Statement_list *next;
    Statement *statement;
};

struct ParamList_tag
{
    char *identifier;
    ParamList *next;
};

struct Function_tag
{
    char *identifier;
    int isNative;
    void *funcAddr;
    Statement_list *list;
    ParamList *paramlist;
    Function_t *next;
};

Statement *create_Statement(enum StatementType type);
Statement_list *createStatementList(Statement *statement);
Statement *create_ExpressionStatement(Expression *expression);
Statement *create_IFStatement(Expression *condition, Statement_list *list);
Statement *create_FORStatement(Expression *before, Expression *condition,
                               Expression *after, Statement_list *list);
Statement_list *StatementList_add(Statement_list *list, Statement *statement);

Expression *create_IntergerExpression(int i);
Expression *create_DoubleExpression(double i);
Expression *create_StrExpression(char *p);
Expression *create_IDExpression(char *p);
Binary_Expression *createBinaryExpression(enum ExpressionAction action, Expression *left, Expression *right);
Assign_Expression *createAssignExpression(char *c, Expression *expression);
Expression *binExpressionWarpper(Binary_Expression *expression);
Expression *AssignExpressionWarpper(Assign_Expression *expression);
Expression *create_FuncCallExpression(char *identifier, ParamList *params);

ParamList *createParamList(char *identifier);
ParamList *ParamList_add(ParamList *list, ParamList *param);
Function_t *Functon_define(char *identifier, ParamList *params, Statement_list *list);
Function_t *Function_add(Function_t *functionlist, Function_t *func);
Function_t *Function_find(Function_t *funclist, char *identifier);

void open_charbuffer();
void addc_charbuffer(char c);
char *flush_charbuffer();
char *Interpreter_str_malloc(char *str);
void Interpreter_setlist(Statement_list *list);
Statement_list *Interpreter_getlist();

void Interpreter_setFunclist(Function_t *func);
Function_t *Interpreter_getFunclist();

void Interpreter_printRoot();
#endif