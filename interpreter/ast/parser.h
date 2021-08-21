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
    ExpressionStatement* new_expression_statement(Expression*expr);
    AssignExpression* new_assign_expression(const char* identifier, Expression*expr);
    
    template <typename ...Args>
    PrimaryExpression* new_primary_expression(Args&&... __args) {
        return _free_list.add(new PrimaryExpression(std::forward<Args>(__args)...));
    }

    BinaryExpression* new_binary_expression(BinaryExprType binary_expr_type, Expression* left, Expression* right);

    StatementList* add_statement(StatementList*lst, Statement*statement);
    StatementList* create_statement_list();
private:
    NoLockFreeList _free_list;
    // main statement list
    StatementList _statement_list;
};
} // namespace stdpain
