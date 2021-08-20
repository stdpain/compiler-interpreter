#pragma once
#include "MEM.h"

typedef struct Variable_list_tag Variable_list;

struct Environment_tag {
    MEM_Storage storage;
    Variable_list* list;
    Variable_list* outervar;
};

typedef struct Environment_tag Environment;
