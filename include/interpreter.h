#ifndef INTERPRETER
#define INTERPRETER

#include "../include/eval.h"

typedef struct interpreter_tag Interpreter;

struct interpreter_tag
{
    MEM_Storage storage;
    MEM_Storage charpool;
    Statement_list *list;
    Function_t *functionlist;
    Environment globalEnvironment;
};
Interpreter *getInterpreterInstance();
void *Interpreter_malloc(int size);
char *Interpreter_str_malloc(char *str);

#endif