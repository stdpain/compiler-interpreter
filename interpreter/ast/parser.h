#pragma once
#include "ast/exprs.h"
#include "common/free_list.hpp"
namespace stdpain {
class Parser {
public:
    Parser* getInstance() {
        static Parser instance;
        return &instance;
    }

    Statement* new_statement();
private:
    NoLockFreeList _list;
};
} // namespace stdpain
