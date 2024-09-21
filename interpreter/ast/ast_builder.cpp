#include "ast/ast_builder.h"

#include <cstring>
#include <memory>

#include "logging/interpreter_logger.hpp"

namespace ms {

const char* ASTBuilder::new_str(const char* str) {
    size_t sz = strlen(str);
    char* mem = _free_list.add(new char[sz + 1]);
    memcpy(mem, str, sz + 1);
    return mem;
}

Param* ASTBuilder::create_param(const char* name) {
    auto param = _free_list.add(new Param());
    param->identifier = name;
    return param;
}

ParamList* ASTBuilder::create_param_list() {
    return _free_list.add(new ParamList());
}

ParamList* ASTBuilder::add_param(ParamList* param_list, Param* param) {
    param_list->push_back(param);
    return param_list;
}

FunctionCallExpression* ASTBuilder::new_functioncall_expression(const char* name,
                                                                ExpressionList* expressions) {
    return _free_list.add(new FunctionCallExpression(name, expressions));
}

Function* ASTBuilder::new_function(const char* name, ParamList* parameters,
                                   StatementList* statements) {
    return _free_list.add(new Function(name, parameters, statements));
}

void ASTBuilder::append_function(Function* function) {
    _function_list.push_back(function);
}

void ASTBuilder::append_statement(Statement* statement) {
    LOG(INFO) << "TRACE:" << (statement);
    _statement_list.emplace_back(statement);
}

StatementList* ASTBuilder::add_statement(StatementList* lst, Statement* statement) {
    lst->push_back(statement);
    return lst;
}

StatementList* ASTBuilder::create_statement_list() {
    return _free_list.add(new StatementList());
}

ExpressionStatement* ASTBuilder::new_expression_statement(Expression* expression) {
    return _free_list.add(new ExpressionStatement(expression));
}

ExpressionList* ASTBuilder::create_expression_list() {
    return _free_list.add(new ExpressionList());
}

ExpressionList* ASTBuilder::add_expression(ExpressionList* lst, Expression* expression) {
    lst->push_back(expression);
    return lst;
}

EmptyExpression* ASTBuilder::new_empty_expression() {
    return _free_list.add(new EmptyExpression());
}

AssignExpression* ASTBuilder::new_assign_expression(const char* identifier, Expression* expr) {
    return _free_list.add(new AssignExpression(identifier, expr));
}

UnaryExpression* ASTBuilder::new_unary_expression(UnaryExprType unary_expr_type,
                                                  Expression* expression) {
    return _free_list.add(new UnaryExpression(unary_expr_type, expression));
}

BinaryExpression* ASTBuilder::new_binary_expression(BinaryExprType binary_expr_type,
                                                    Expression* left, Expression* right) {
    return _free_list.add(new BinaryExpression(binary_expr_type, left, right));
}

} // namespace ms
