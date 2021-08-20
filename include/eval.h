#ifndef __EVAL_H
#define __EVAL_H
#include "MEM.h"
#include "create.h"
#include "interpreter.h"
#include "string_pool.h"

typedef struct interpreter_tag Interpreter;

typedef struct NativePointer_tag NativePointer;
typedef struct Variable_tag Variable;
typedef struct Value_tag Value;
typedef struct Variable_list_tag Variable_list;
typedef struct StatementResult_tag StatementResult;

enum eval_ValueType {
    ValueType_Integer = 1,
    ValueType_String,
    ValueType_Double,
    ValueType_Native,
    ValueType_INVALID
};
struct NativePointer_tag {
    void* p;
    char* info;
};
struct Value_tag {
    enum eval_ValueType type;
    union {
        int i;
        double d;
        // char *str;
        MString* mstring;
        NativePointer* p;
    } u;
};
struct Variable_tag {
    char* identifier;
    Value v;
};
struct Variable_list_tag {
    Variable* variable;
    Variable_list* next;
};
enum StatementResultType {
    StatementResultType_Normal = 1,
    StatementResultType_Break,
    StatementResultType_Continue,
    StatementResultType_Rtn
};
struct StatementResult_tag {
    enum StatementResultType result;
    Value v;
};

StatementResult execute_ExpressionStatement(Interpreter* inter, Environment* local,
                                            Statement* statement);
StatementResult execute_StatementList(Interpreter* inter, Environment* local,
                                      Statement_list* statementlist);
Value execute_Expression(Interpreter* inter, Environment* local, Expression* expression);

#endif
