#include "../include/interpreter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/MEM.h"
#include "../include/create.h"

Interpreter* interpreter;

Interpreter* getInterpreterInstance() {
    if (interpreter == NULL) {
        interpreter = MEM_malloc(sizeof(Interpreter));
        interpreter->storage = MEM_open_storage(0);
        interpreter->charpool = MEM_open_storage(0);
        interpreter->list = NULL;
        interpreter->functionlist = NULL;
        interpreter->globalEnvironment.storage = MEM_open_storage(0);
        interpreter->globalEnvironment.list = NULL;
        interpreter->globalEnvironment.outervar = NULL;
    }
    return interpreter;
}
// memory
void* Interpreter_malloc(int size) {
    Interpreter* interpreter = getInterpreterInstance();
    void* p = MEM_storage_malloc(interpreter->storage, size);
    return p;
}
char* Interpreter_str_malloc(char* str) {
    Interpreter* interpreter = getInterpreterInstance();
    void* p = MEM_storage_malloc(interpreter->storage, strlen(str) + 1);
    strcpy(p, str);
    return p;
}
//ast
void Interpreter_setlist(Statement_list* list) {
    getInterpreterInstance()->list = list;
}
Statement_list* Interpreter_getlist() {
    return getInterpreterInstance()->list;
}

void Interpreter_setFunclist(Function_t* func) {
    getInterpreterInstance()->functionlist = func;
}
Function_t* Interpreter_getFunclist() {
    return getInterpreterInstance()->functionlist;
}
//interpreter

//print
void printList(Statement_list* list);

void printPrimaryExpression(PrimaryExpression* expression) {
    switch (expression->type) {
    case DOUBLE_TYPE:
        printf("%f\n", expression->u.d);
        break;
    case INTEGER_TYPE:
        printf("%d\n", expression->u.i);
        break;
    case STRING_TYPE:
        printf("str:%s\n", expression->u.mstring->str);
        break;
    case IDENTIFIER_TYPE:
        printf("id:%s\n", expression->u.identifier);
        break;
    default:
        printf("error\n");
        abort();
        break;
    }
}

void printExpression(Expression* expression) {
    switch (expression->type) {
    case Expression_type_assign: {
        printExpression(expression->u.a->expression);
        printf("%s\n", expression->u.a->identifier);
        printf("assign\n");
    } break;
    case Expression_type_binary: {
        printExpression(expression->u.b->left);
        printExpression(expression->u.b->right);
        printf("action:%d\n", expression->u.b->action);
    } break;
    case Expression_type_inv: {
        printExpression(expression->u.e);
        printf("inv \n");
    } break;
    case Expression_type_primary: {
        printPrimaryExpression(expression->u.p);
    } break;
    case Expression_type_func: {
        printf("call function: %s\n", expression->u.func->funname);
        break;
    }
    default:
        printf("error\n");
        abort();
        break;
    }
}

void printForstatement(For_Statement* statement) {
    printf("for\n");
    if (statement->before == NULL) {
        printf("before:NULL\n");
    } else {
        printExpression(statement->before);
    }
    printf("condition:\n");
    printExpression(statement->condition);
    printf("after:\n");
    if (statement->after) {
        printExpression(statement->after);
    } else {
        printf("NULL\n");
    }
    printf("inner statement:\n");
    printList(statement->statementlist);
}

void printIfStatus(If_Statement* statement) {
    printf("if \n");
    printExpression(statement->expression);
    printf("ef\n");
    printList(statement->statementlist);
    printf("fi\n");
}

void printInfo(Statement* statement) {
    switch (statement->type) {
    case Statement_Type_Expression:
        printExpression(statement->u.e->expression);
        break;
    case Statement_Type_For:
        printForstatement(statement->u.f);
        break;
    case Statement_Type_IF:
        printIfStatus(statement->u.i);
        break;
    case Statement_Type_Break:
        printf("break\n");
    case Statement_Type_Continue:
        printf("continue\n");
    case Statement_Type_Rtn:
        printf("Return\n");
        break;
    default:
        printf("error\n");
        abort();
        break;
    }
}
void printList(Statement_list* list) {
    Statement_list* pos;
    for (pos = list; pos; pos = pos->next) {
        printInfo(pos->statement);
    }
}
void printParams(ParamList* list) {
    printf("params:");
    while (list) {
        printf("%s,", list->identifier);
        list = list->next;
    }
    printf("\n");
}
void printFunc(Function_t* func) {
    printf("funcname:%s\n", func->identifier);
    printParams(func->paramlist);
    printList(func->list);
}
void printFucList(Function_t* funlist) {
    Function_t* pos;
    printf("will print func list\n");
    for (pos = funlist; pos; pos = pos->next) {
        printFunc(pos);
    }
}

void Interpreter_printRoot() {
    Interpreter* interpreter = getInterpreterInstance();
    Statement_list* list = interpreter->list;
    printf("start print %p\n", list);
    printList(list);
    printFucList(interpreter->functionlist);
}

//exec
void execute() {
    Interpreter* interpreter = getInterpreterInstance();
    Statement_list* list = interpreter->list;
    printf("\nwill execute \n");
    execute_StatementList(interpreter, NULL, list);
}
