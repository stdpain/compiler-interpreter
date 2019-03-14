#include "../include/MEM.h"
#include "../include/interpreter.h"
#include "../include/eval.h"
#include <string.h>

void Release_IfMString(Value *v)
{
    if (v->type == ValueType_String)
        MStringRelease(v->u.mstring);
}
void Retain_IfMString(Value *v)
{
    if (v->type == ValueType_String)
        MStringRetain(v->u.mstring);
}

void alloc_Environment(Environment *env)
{
    env->storage = MEM_open_storage(0);
    env->list = NULL;
    env->outervar = NULL;
}
void disposite_Environment(Environment *env)
{
    MEM_dispose_storage(env->storage);
    env->list = NULL;
    env->outervar = NULL;
}
void clearParams(Environment *env)
{
    Variable_list *pos;
    for (pos = env->list; pos; pos = pos->next)
    {
        Release_IfMString(&(pos->variable->v));
    }
}
void Environment_Varlist_add(Environment *env, Variable *variable)
{
    if (!env->list)
    {
        env->list = MEM_storage_malloc(env->storage, sizeof(Variable_list));
        env->list->next = NULL;
        env->list->variable = variable;
    }
    else
    {
        Variable_list *pos;
        for (pos = env->list; pos->next; pos = pos->next)
            ;
        pos->next = MEM_storage_malloc(env->storage, sizeof(Variable_list));
        pos->next->next = NULL;
        pos->next->variable = variable;
    }
}
Variable *allocVariable(Environment *env, char *identifier)
{
    Variable *variable = MEM_storage_malloc(env->storage, sizeof(Variable));
    variable->identifier = identifier;
    variable->v.type = ValueType_INVALID;
    Environment_Varlist_add(env, variable);
    return variable;
}
Variable *Variable_Find(Environment *env, char *identifier)
{
    Variable_list *pos;
    //当前环境
    for (pos = env->list; pos; pos = pos->next)
    {
        if (!strcmp(identifier, pos->variable->identifier))
        {
            return pos->variable;
        }
    }
    //当前环境保留的外部变量
    for (pos = env->outervar; pos; pos = pos->next)
    {
        if (!strcmp(identifier, pos->variable->identifier))
        {
            return pos->variable;
        }
    }
    return NULL;
}

Variable *
eval_getVariable(Interpreter *inter, Environment *localenv, char *identifier)
{
    Variable *variable = NULL;
    if (localenv)
    {
        variable = Variable_Find(localenv, identifier);
    }
    else
    {
        variable = Variable_Find(&(inter->globalEnvironment), identifier);
    }
    return variable;
}

Variable *Variable_FindorCreate(Interpreter *inter, Environment *env, char *identifier)
{
    Variable *variable = eval_getVariable(inter, env, identifier);
    if (!variable)
    {
        if (!env)
        {
            variable = allocVariable(&(inter->globalEnvironment), identifier);
        }
        else
        {
            variable = allocVariable(env, identifier);
        }
    }
    return variable;
}
//paramlist 形参
void ParamListAssign(Interpreter *inter, Environment *outerEnv,
                     Environment *innerEnv, ParamList *paramlist, Function_t *func)
{
    ParamList *outer_pos = paramlist;
    ParamList *inner_pos = func->paramlist;
    while (outer_pos && inner_pos)
    {
        Variable *variable = eval_getVariable(inter, outerEnv, outer_pos->identifier);
        Variable *realVariable = allocVariable(innerEnv, variable->identifier);
        realVariable->v = variable->v;
        Retain_IfMString(&(realVariable->v));
        outer_pos = outer_pos->next;
        inner_pos = inner_pos->next;
    }
}
//

Value execute_Expression_Primary(Interpreter *inter,
                                 Environment *local, Expression *expression)
{
    PrimaryExpression *primaryExpression = expression->u.p;
    Value v;
    switch (primaryExpression->type)
    {
    case INTEGER_TYPE:
        v.type = ValueType_Integer;
        v.u.i = primaryExpression->u.i;
        break;
    case DOUBLE_TYPE:
        v.type = ValueType_Double;
        v.u.d = primaryExpression->u.d;
        break;
    case STRING_TYPE:
        v.type = ValueType_String;
        v.u.mstring = primaryExpression->u.mstring;
        break;
    case IDENTIFIER_TYPE:
    {
        Variable *variable = eval_getVariable(inter, local, primaryExpression->u.identifier);
        if (variable)
        {
            v.type = variable->v.type;
            v.u = variable->v.u;
            Retain_IfMString(&v);
        }
        else
        {
            v.type = ValueType_INVALID;
            //
            printf("can not find var %s\n", primaryExpression->u.mstring->str);
            printf("need code int line:%d\n", __LINE__);
        }
    }
    break;
    case INVALID_TYPE:
        v.type = ValueType_INVALID;
        break;
    default:
        printf("error in execute expression statement in line:%d type:%d\n", __LINE__, primaryExpression->type);
        break;
    }
    return v;
}

#define allInteger(leftv, rightv) ((leftv).type == ValueType_Integer && (rightv).type == ValueType_Integer)

#define orString(leftv, rightv) ((leftv).type == ValueType_String || (rightv).type == ValueType_String)

#define allNum(leftv, rightv)                                                   \
    (((leftv).type == ValueType_Integer || (leftv).type == ValueType_Double) && \
     ((rightv).type == ValueType_Integer || (rightv).type == ValueType_Double))

#define BUFFER_SIZE 1024

char *value_str_cat(Value *l, Value *r)
{
    char buffer[BUFFER_SIZE];
    char *pbuffer = NULL;
    if (l->type == ValueType_Integer)
    {
        sprintf(buffer, "%d", l->u.i);
        strcat(buffer, r->u.mstring->str);
    }
    else if (l->type == ValueType_Double)
    {
        sprintf(buffer, "%f", l->u.d);
        strcat(buffer, r->u.mstring->str);
    }
    else if (l->type == ValueType_INVALID)
    {
        sprintf(buffer, "[invalid]");
        strcat(buffer, r->u.mstring->str);
    }
    else if (l->type == ValueType_String)
    {
        sprintf(buffer, "%s", l->u.mstring->str);
        pbuffer = buffer + strlen(buffer);
        if (r->type == ValueType_Integer)
        {
            sprintf(pbuffer, "%d", r->u.i);
        }
        else if (r->type == ValueType_Double)
        {
            sprintf(pbuffer, "%f", r->u.d);
        }
        else if (r->type == ValueType_INVALID)
        {
            sprintf(pbuffer, "[INVALID]");
        }
        else if (r->type == ValueType_String)
        {
            sprintf(pbuffer, "%s", r->u.mstring->str);
        }
        else
        {
            //error
        }
    }
    else
    {
        //error
    }
    // void *p = Interpreter_str_malloc(buffer);
    int len = strlen(buffer);
    void *p = MEM_malloc(len + 1);
    memcpy(p, buffer, len + 1);
    return p;
}
Value value_add_option(Value *l, Value *r)
{
    Value v;
    if (l->type == ValueType_Integer && r->type == ValueType_Integer)
    {
        v.type = ValueType_Integer;
        v.u.i = l->u.i + r->u.i;
    }
    else if (l->type == ValueType_Integer)
    {
        v.type = ValueType_Double;
        v.u.d = l->u.i + r->u.d;
    }
    else if (r->type == ValueType_Integer)
    {
        v.type = ValueType_Double;
        v.u.d = l->u.d + r->u.i;
    }
    else
    {
        v.type = ValueType_Double;
        v.u.d = l->u.d + r->u.d;
    }
    return v;
}
Value value_sub_option(Value *l, Value *r)
{
    Value v;
    if (l->type == ValueType_Integer && r->type == ValueType_Integer)
    {
        v.type = ValueType_Integer;
        v.u.i = l->u.i - r->u.i;
    }
    else if (l->type == ValueType_Integer)
    {
        v.type = ValueType_Double;
        v.u.d = l->u.i - r->u.d;
    }
    else if (r->type == ValueType_Integer)
    {
        v.type = ValueType_Double;
        v.u.d = l->u.d - r->u.i;
    }
    else
    {
        v.type = ValueType_Double;
        v.u.d = l->u.d - r->u.d;
    }
    return v;
}
Value value_mul_option(Value *l, Value *r)
{
    Value v;
    if (l->type == ValueType_Integer && r->type == ValueType_Integer)
    {
        v.type = ValueType_Integer;
        v.u.i = l->u.i * r->u.i;
    }
    else if (l->type == ValueType_Integer)
    {
        v.type = ValueType_Double;
        v.u.d = l->u.i * r->u.d;
    }
    else if (r->type == ValueType_Integer)
    {
        v.type = ValueType_Double;
        v.u.d = l->u.d * r->u.i;
    }
    else
    {
        v.type = ValueType_Double;
        v.u.d = l->u.d * r->u.d;
    }
    return v;
}
Value value_div_option(Value *l, Value *r)
{
    Value v;
    if (l->type == ValueType_Integer && r->type == ValueType_Integer)
    {
        v.type = ValueType_Integer;
        v.u.i = l->u.i / r->u.i;
    }
    else if (l->type == ValueType_Integer)
    {
        v.type = ValueType_Double;
        v.u.d = l->u.i / r->u.d;
    }
    else if (r->type == ValueType_Integer)
    {
        v.type = ValueType_Double;
        v.u.d = l->u.d / r->u.i;
    }
    else
    {
        v.type = ValueType_Double;
        v.u.d = l->u.d / r->u.d;
    }
    return v;
}
Value value_gt_option(Value *l, Value *r)
{
    Value v;
    if (l->type == ValueType_Integer && r->type == ValueType_Integer)
    {
        v.type = ValueType_Integer;
        v.u.i = l->u.i > r->u.i;
    }
    else if (l->type == ValueType_Integer)
    {
        v.type = ValueType_Integer;
        v.u.d = l->u.i > r->u.d;
    }
    else if (r->type == ValueType_Integer)
    {
        v.type = ValueType_Integer;
        v.u.d = l->u.d > r->u.i;
    }
    else
    {
        v.type = ValueType_Integer;
        v.u.d = l->u.d > r->u.d;
    }
    return v;
}
Value value_ge_option(Value *l, Value *r)
{
    Value v;
    if (l->type == ValueType_Integer && r->type == ValueType_Integer)
    {
        v.type = ValueType_Integer;
        v.u.i = l->u.i >= r->u.i;
    }
    else if (l->type == ValueType_Integer)
    {
        v.type = ValueType_Integer;
        v.u.d = l->u.i >= r->u.d;
    }
    else if (r->type == ValueType_Integer)
    {
        v.type = ValueType_Integer;
        v.u.d = l->u.d >= r->u.i;
    }
    else
    {
        v.type = ValueType_Integer;
        v.u.d = l->u.d >= r->u.d;
    }
    return v;
}
Value value_lt_option(Value *l, Value *r)
{
    Value v;
    if (l->type == ValueType_Integer && r->type == ValueType_Integer)
    {
        v.type = ValueType_Integer;
        v.u.i = l->u.i < r->u.i;
    }
    else if (l->type == ValueType_Integer)
    {
        v.type = ValueType_Integer;
        v.u.d = l->u.i < r->u.d;
    }
    else if (r->type == ValueType_Integer)
    {
        v.type = ValueType_Integer;
        v.u.d = l->u.d < r->u.i;
    }
    else
    {
        v.type = ValueType_Integer;
        v.u.d = l->u.d < r->u.d;
    }
    return v;
}
Value value_le_option(Value *l, Value *r)
{
    Value v;
    if (l->type == ValueType_Integer && r->type == ValueType_Integer)
    {
        v.type = ValueType_Integer;
        v.u.i = l->u.i <= r->u.i;
    }
    else if (l->type == ValueType_Integer)
    {
        v.type = ValueType_Integer;
        v.u.d = l->u.i <= r->u.d;
    }
    else if (r->type == ValueType_Integer)
    {
        v.type = ValueType_Integer;
        v.u.d = l->u.d <= r->u.i;
    }
    else
    {
        v.type = ValueType_Integer;
        v.u.d = l->u.d <= r->u.d;
    }
    return v;
}
Value value_eq_option(Value *l, Value *r)
{
    Value v;
    v.type = ValueType_Integer;
    if (l->type != r->type)
    {
        v.u.i = 0;
        return v;
    }
    else
    {
        if (l->type == ValueType_Integer)
        {
            v.u.i = (l->u.i == r->u.i);
            return v;
        }
        if (l->type == ValueType_Double)
        {
            v.u.i = (l->u.d == r->u.d);
            return v;
        }
        if (l->type == ValueType_String)
        {
            v.u.i = (!strcmp(l->u.mstring->str, r->u.mstring->str));
            return v;
        }
        if (l->type == ValueType_Native)
        {
            v.u.i = (l->u.p->p == r->u.p->p);
        }
    }
    return v;
}
Value value_ne_option(Value *l, Value *r)
{
    Value v;
    v = value_eq_option(l, r);
    v.u.i = !v.u.i;
    return v;
}

Value execute_BinaryExpression(Interpreter *inter, Environment *env, Expression *expression)
{
    Value v;
    Value l;
    Value r;
    // printf("will execute bin line:%d\n", __LINE__);
    Binary_Expression *binexpression = expression->u.b;
    l = execute_Expression(inter, env, binexpression->left);
    r = execute_Expression(inter, env, binexpression->right);
    switch (binexpression->action)
    {
    case ADD_OPERATOR:
    {
        if (allNum(l, r))
        {
            v = value_add_option(&l, &r);
        }
        else if (orString(l, r))
        {
            char *p = value_str_cat(&l, &r);
            v.type = ValueType_String;
            v.u.mstring = allocMString(p, 0);
            Release_IfMString(&l);
            Release_IfMString(&r);
        }
    }
    break;
    case SUB_OPERATOR:
    {
        if (!allNum(l, r))
        {
            //error
            printf("error %d\n", __LINE__);
            printf("l.type %d\n", l.type);
            printf("r.type %d\n", l.type);
            abort();
            break;
        }
        v = value_sub_option(&l, &r);
        // printf("sub result type %d line:%d\n", v.type, __LINE__);
    }
    break;
    case MUL_OPERATOR:
    {
        if (!allNum(l, r))
        {
            //error
            printf("error %d\n", __LINE__);
            printf("l.type %d\n", l.type);
            printf("r.type %d\n", l.type);
            abort();
            break;
        }
        v = value_mul_option(&l, &r);
    }
    break;
    case DIV_OPERATOR:
    {
        if (!allNum(l, r))
        {
            //error
            printf("error %d\n", __LINE__);
            break;
        }
        v = value_mul_option(&l, &r);
    }
    break;
    case GT_OPERATOR:
    {
        if (!allNum(l, r))
        {
            //error
            printf("error %d\n", __LINE__);
            printf("l.type %d\n", l.type);
            printf("r.type %d\n", l.type);
            abort();
            break;
        }
        v = value_gt_option(&l, &r);
    }
    break;
    case GE_OPERATOR:
    {
        if (!allNum(l, r))
        {
            //error
            printf("error %d\n", __LINE__);
            printf("l.type %d\n", l.type);
            printf("r.type %d\n", l.type);
            abort();
            break;
        }
        v = value_ge_option(&l, &r);
    }
    break;
    case LT_OPERATOR:
    {
        if (!allNum(l, r))
        {
            //error
            printf("error %d\n", __LINE__);
            printf("l.type %d\n", l.type);
            printf("r.type %d\n", l.type);
            abort();
            break;
        }
        v = value_lt_option(&l, &r);
    }
    break;
    case LE_OPERATOR:
    {
        if (!allNum(l, r))
        {
            //error
            printf("error %d\n", __LINE__);
            printf("l.type %d\n", l.type);
            printf("r.type %d\n", l.type);
            abort();
            break;
        }
        v = value_le_option(&l, &r);
    }
    break;
    case EQ_OPERATOR:
    {
        v = value_eq_option(&l, &r);
        Release_IfMString(&l);
        Release_IfMString(&r);
    }
    break;
    case NE_OPERATOR:
        v = value_ne_option(&l, &r);
        Release_IfMString(&l);
        Release_IfMString(&r);
        break;
    default:
        break;
    }
    return v;
}

Value execute_AssignExpresion(Interpreter *inter,
                              Environment *local, Expression *expression)
{
    Value v;
    v = execute_Expression(inter, local, expression->u.a->expression);
    Variable *variable = Variable_FindorCreate(inter, local, expression->u.a->identifier);
    printf("will release left\n");
    Release_IfMString(&(variable->v));
    variable->v = v;
    Retain_IfMString(&(variable->v));
    // printf("assign var:%s v:%s type:%d\n", variable->identifier, variable->v.u.str, variable->v.type);
    return v;
}
Function_t *find_function(char *funname)
{
    Interpreter *inter = getInterpreterInstance();
    Function_t *pos;
    for (pos = inter->functionlist; pos; pos = pos->next)
    {
        if (!strcmp(pos->identifier, funname))
        {
            return pos;
        }
    }
    return NULL;
}

typedef Value NativerFuncProc(Interpreter *inter, Environment *env);

Value nativeFuncCall(Interpreter *inter, Environment *local, Function_t *func)
{
    NativerFuncProc *proc = func->funcAddr;
    return proc(inter, local);
}

Value execute_FuctionExpression(Interpreter *inter, Environment *local, Expression *expression)
{
    Value v;
    FuncCallExpression *funcexpress = expression->u.func;
    // printf("will call func:%s in line:%d\n", funcexpress->funname, __LINE__);
    if (funcexpress->func == NULL)
    {
        funcexpress->func = find_function(funcexpress->funname);
    }
    if (funcexpress->func == NULL)
    {
        //error
    }
    //call
    Environment funcEnvironment;
    alloc_Environment(&funcEnvironment);
    //param copy
    Function_t *funct = funcexpress->func;
    ParamListAssign(inter, local, &funcEnvironment, funcexpress->params, funcexpress->func);
    if (funct->isNative)
    {
        nativeFuncCall(inter, &funcEnvironment, funct);
    }
    else
    {
        execute_StatementList(inter, &funcEnvironment, funct->list);
    }
    clearParams(&funcEnvironment);
    disposite_Environment(&funcEnvironment);
    return v;
}

Value execute_Expression(Interpreter *inter, Environment *local, Expression *expression)
{
    Value v;
    switch (expression->type)
    {
    case Expression_type_assign:
        v = execute_AssignExpresion(inter, local, expression);
        break;
    case Expression_type_binary:
        v = execute_BinaryExpression(inter, local, expression);
        break;
    case Expression_type_func:
        v = execute_FuctionExpression(inter, local, expression);
        break;
    case Expression_type_inv:
        break;
    case Expression_type_primary:
        v = execute_Expression_Primary(inter, local, expression);
        break;
    default:
        printf("error in execute expression statement %d\n", __LINE__);
        abort();
        break;
    }
    return v;
}
StatementResult
execute_ExpressionStatement(Interpreter *inter, Environment *local, Statement *statement)
{
    StatementResult result;
    result.v = execute_Expression(inter, local, statement->u.e->expression);
    Release_IfMString(&result.v);
    result.result = StatementResultType_Normal;
    return result;
}

StatementResult
execute_ForStatement(Interpreter *inter, Environment *local, Statement *statement)
{
    StatementResult result;
    result.result = StatementResultType_Normal;
    For_Statement *forstatement = statement->u.f;
    if (forstatement->before)
        execute_Expression(inter, local, forstatement->before);
    while (1)
    {
        // printf("for :%d\n", __LINE__);
        if (forstatement->condition)
        {
            Value v = execute_Expression(inter, local, forstatement->condition);
            if (v.type == ValueType_Integer && v.u.i == 0)
                break;
            if (v.type == INVALID_TYPE)
                break;
        }
        // printf("stp:%p\n", forstatement->statementlist);
        StatementResult exeresult = execute_StatementList(inter, local, forstatement->statementlist);
        if (exeresult.result == StatementResultType_Break)
            break;
        if (exeresult.result == StatementResultType_Rtn)
            return exeresult;
        if (forstatement->after)
            execute_Expression(inter, local, forstatement->after);
    }
    return result;
}

StatementResult
execute_IFStatement(Interpreter *inter, Environment *local, Statement *statement)
{
    StatementResult result;
    If_Statement *if_Statement = statement->u.i;
    Expression *expression = if_Statement->expression;
    Value v = execute_Expression(inter, local, expression);
    if (v.type == ValueType_Integer)
    {
        if (v.u.i != 0)
            execute_StatementList(inter, local, if_Statement->statementlist);
    }
    else if (v.type == ValueType_Double)
    {
        if (v.u.d != 0)
            execute_StatementList(inter, local, if_Statement->statementlist);
    }
    else if (v.type == ValueType_Native)
    {
        if (v.u.p != NULL)
            execute_StatementList(inter, local, if_Statement->statementlist);
    }
    result.result = StatementResultType_Normal;
}

StatementResult
execute_Statement(Interpreter *inter, Environment *local, Statement *statement)
{
    StatementResult result;
    switch (statement->type)
    {
    case Statement_Type_Expression:
        result = execute_ExpressionStatement(inter, local, statement);
        break;
    case Statement_Type_For:
        result = execute_ForStatement(inter, local, statement);
        break;
    case Statement_Type_IF:
        result = execute_IFStatement(inter, local, statement);
        break;
    case Statement_Type_Break:
        result.result = StatementResultType_Break;
        break;
    case Statement_Type_Rtn:
        result.result = StatementResultType_Rtn;
        break;
    case Statement_Type_Continue:
        result.result = Statement_Type_Continue;
        break;
    default:
        printf("error in execute\n");
        abort();
        break;
    }
    return result;
}

StatementResult
execute_StatementList(Interpreter *inter, Environment *local, Statement_list *statementlist)
{
    StatementResult result;
    Statement_list *pos;
    for (pos = statementlist; pos; pos = pos->next)
    {
        result = execute_Statement(inter, local, pos->statement);
        if (result.result != StatementResultType_Normal)
            break;
    }
    return result;
}

void mprint(Value *v)
{
    switch (v->type)
    {
    case ValueType_Integer:
        printf("%d", v->u.i);
        break;
    case ValueType_Double:
        printf("%f", v->u.d);
        break;
    case ValueType_String:
        printf("%s", v->u.mstring->str);
        break;
    case ValueType_INVALID:
        printf("[INVALID]", v->u.mstring->str);
        break;
    case ValueType_Native:
        printf("[Native]", v->u.mstring->str);
        break;
    default:
        printf("[ERROR TYPE]", v->u.mstring->str);
        abort();
        break;
    }
}

Value NativeFunc_print(Interpreter *inter, Environment *env)
{
    Value v;
    v.type = ValueType_Integer;
    v.u.i = 1;
    Variable_list *pos = env->list;
    // pos->variable->v;
    mprint(&(pos->variable->v));
    return v;
}

void RegistNativeProc(Interpreter *inter, NativerFuncProc *nativefunc, char *identifier, int argsnum)
{
    Function_t *func = Interpreter_malloc(sizeof(Function_t));
    func->list = NULL;
    func->isNative = 1;
    func->funcAddr = nativefunc;
    func->next = NULL;
    func->paramlist = NULL;
    func->identifier = identifier;
    inter->functionlist = Function_add(inter->functionlist, func);
    ParamList *list = NULL;
    for (int i = 0; i < argsnum; i++)
    {
        list = ParamList_add(list, createParamList("tmp"));
    }
    func->paramlist = list;
}
void initProcs()
{
    RegistNativeProc(getInterpreterInstance(), NativeFunc_print, "print", 1);
}