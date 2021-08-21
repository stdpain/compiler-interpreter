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

    const char* new_str(const char* str);
    // Function
    Param* create_param(const char* name);
    ParamList* create_param_list();
    ParamList* add_param(ParamList* param_list, Param* param);
    FunctionCallExpression* new_functioncall_expression(const char* name,
                                                        ExpressionList* expressions);

    Function* new_function(const char* name, ParamList* parameters, StatementList* statements);
    void append_function(Function* function);

    // Expression
    ExpressionList* create_expression_list();
    ExpressionList* add_expression(ExpressionList* lst, Expression* expression);
    
    EmptyExpression* new_empty_expression();
    AssignExpression* new_assign_expression(const char* identifier, Expression* expr);

    template <typename... Args>
    PrimaryExpression* new_primary_expression(Args&&... args) {
        return _free_list.add(new PrimaryExpression(std::forward<Args>(args)...));
    }
    UnaryExpression* new_unary_expression(UnaryExprType unary_expr_type, Expression* expression);
    BinaryExpression* new_binary_expression(BinaryExprType binary_expr_type, Expression* left,
                                            Expression* right);
    // Statement
    ExpressionStatement* new_expression_statement(Expression* expr);
    
    template<typename SpecStatement, typename... Args>
    SpecStatement* new_statement(Args&&... args) {
        return _free_list.add(new SpecStatement(std::forward<Args>(args)...));
    }

    void append_statement(Statement* statement);
    StatementList* add_statement(StatementList* lst, Statement* statement);
    StatementList* create_statement_list();

private:
    NoLockFreeList _free_list;
    // main statement list
    StatementList _statement_list;
    FunctionList _function_list;
};
} // namespace stdpain
