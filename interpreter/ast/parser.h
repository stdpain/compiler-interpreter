#pragma once
#include "ast/exprs.h"
#include "common/free_list.hpp"
#include "logging/interpreter_logger.hpp"

namespace stdpain {
class Parser {
public:
    static Parser* getInstance() {
        static Parser instance;
        return &instance;
    }

    void append_statement(Statement* statement);
private:
    NoLockFreeList _list;
    std::vector<Statement*> _statement_list;
};
} // namespace stdpain
