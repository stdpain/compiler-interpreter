#pragma once
#include <vector>

enum ValueType {
    Integer,
    Double,
    String,
    Identifier,
    Invalid,
};

union ValueVariant {
    int i;
    double d;
    const char* identifier;
};

enum class ExpressionType {
    Primary,
    Unary,
    Binary,
    Assign,
};

struct Expression {
    ExpressionType type;
    Expression(ExpressionType type_) : type(type_) {}
};

struct PrimaryExpression : Expression {
    ValueType value_type;
    ValueVariant value;

    PrimaryExpression(int i) : Expression(ExpressionType::Primary) {
        value_type = ValueType::Integer;
        value.i = i;
    }
    PrimaryExpression(double d) : Expression(ExpressionType::Primary) {
        value_type = ValueType::Double;
        value.d = d;
    }
    PrimaryExpression(const char* str, bool is_identifier) : Expression(ExpressionType::Primary) {
        if (is_identifier) {
            value_type = ValueType::Identifier;
            value.identifier = str;
        } else {
            // TODO:
            value_type = ValueType::String;
        }
    }
};

struct AssignExpression : Expression {
    const char* identifier;
    Expression* expression;
    AssignExpression(const char* identifier_, Expression* expression_)
            : Expression(ExpressionType::Assign),
              identifier(identifier_),
              expression(expression_) {}
};

struct UnaryExpression : Expression {
    Expression* expression;
    UnaryExpression(Expression* expression_)
            : Expression(ExpressionType::Unary), expression(expression_) {}
};

enum class BinaryExprType { ADD, SUB, MUL, DIV, LT, LE, EQ, NE, GT, GE };

struct BinaryExpression : Expression {
    BinaryExprType binary_expr_type;
    Expression* left_expression;
    Expression* right_expression;

    BinaryExpression(BinaryExprType binary_expr_type_, Expression* left, Expression* right)
            : Expression(ExpressionType::Binary),
              binary_expr_type(binary_expr_type_),
              left_expression(left),
              right_expression(right) {}
};

enum class StatementType { Expression };

struct Statement {
    StatementType type;
    Statement(StatementType type_) : type(type_) {}
};

using StatementList = std::vector<Statement*>;

struct ExpressionStatement : Statement {
    ExpressionStatement(Expression* expression_)
            : expression(expression_), Statement(StatementType::Expression) {}
    Expression* expression;
};

struct Function {};
