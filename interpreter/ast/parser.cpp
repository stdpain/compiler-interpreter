#include "ast/parser.h"

namespace stdpain
{
void Parser::append_statement(Statement* statement) {
    PAIN_LOG(statement);
    _statement_list.emplace_back(statement);
}

StatementList* Parser::add_statement(StatementList*lst, Statement* statement) {
    lst->push_back(statement);
    return lst;
}

StatementList* Parser::create_statement_list() {
    return _free_list.add(new StatementList());
}

ExpressionStatement* Parser::new_expression_statement(Expression* expression) {
    return _free_list.add(new ExpressionStatement(expression));
}

AssignExpression* Parser::new_assign_expression(const char* identifier, Expression*expr) {
    return _free_list.add(new AssignExpression(identifier, expr));
}

BinaryExpression* Parser::new_binary_expression(BinaryExprType binary_expr_type, Expression* left, Expression* right) {
    return _free_list.add(new BinaryExpression(binary_expr_type, left, right));
}


} // namespace stdpain
