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
    FunctionCall,
    Empty,
};

struct Expression {
    ExpressionType type;
    Expression(ExpressionType type_) : type(type_) {}
};

using ExpressionList = std::vector<Expression*>;

struct EmptyExpression : Expression {
    EmptyExpression() : Expression(ExpressionType::Empty) {}
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

enum class UnaryExprType { INV };

struct UnaryExpression : Expression {
    UnaryExprType unary_expr_type;
    Expression* expression;
    UnaryExpression(UnaryExprType unary_expr_type_, Expression* expression_)
            : Expression(ExpressionType::Unary),
              unary_expr_type(unary_expr_type_),
              expression(expression_) {}
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

struct FunctionCallExpression : Expression {
    const char* function_name;
    ExpressionList* parameters;

    FunctionCallExpression(const char* function_name_, ExpressionList* expressions_)
            : Expression(ExpressionType::FunctionCall),
              function_name(function_name_),
              parameters(expressions_) {}
};

enum class StatementType {
    Expression,
    For,
    If,
    Break,
    Continue,
    Return,
};

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

struct ForStatement : Statement {
    Expression* init_expression;
    Expression* cond_expression;
    Expression* inc_expression;
    StatementList* loop_body;

    ForStatement(Expression* init_expression_, Expression* cond_expression_,
                 Expression* inc_expression_, StatementList* statements)
            : Statement(StatementType::For),
              init_expression(init_expression_),
              cond_expression(cond_expression_),
              inc_expression(inc_expression_),
              loop_body(statements) {}
};

struct IfStatement : Statement {
    Expression* cond_expression;
    StatementList* statements;

    IfStatement(Expression* cond_expression_, StatementList* statements_)
            : Statement(StatementType::If),
              cond_expression(cond_expression_),
              statements(statements_) {}
};

struct BreakStatement : Statement {
    BreakStatement() : Statement(StatementType::Break) {}
};

struct ContinueStatement : Statement {
    ContinueStatement() : Statement(StatementType::Continue) {}
};

struct ReturnStatement : Statement {
    ReturnStatement() : Statement(StatementType::Return) {}
};

struct Param {
    const char* identifier;
};

using ParamList = std::vector<Param*>;

struct Function {
    const char* name;
    ParamList* parameters;
    StatementList* statements;

    Function(const char* name_, ParamList* parameters_, StatementList* statements_)
            : name(name_), parameters(parameters_), statements(statements_) {}
};
using FunctionList = std::vector<Function*>;
