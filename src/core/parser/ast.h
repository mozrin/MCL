#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <vector>
#include "../common/token.h"
#include "../common/value.h"

struct ASTNode
{
    virtual ~ASTNode() = default;
};

struct StringLiteralExpr : public ASTNode
{
    std::string value;
    StringLiteralExpr(std::string val) : value(std::move(val)) {}
};

struct NumberLiteralExpr : public ASTNode
{
    double value;
    NumberLiteralExpr(double val) : value(val) {}
};

struct BooleanLiteralExpr : public ASTNode
{
    bool value;
    BooleanLiteralExpr(bool val) : value(val) {}
};

struct VariableExpr : public ASTNode
{
    std::string name;
    VariableExpr(std::string name) : name(std::move(name)) {}
};

struct BinaryOpExpr : public ASTNode
{
    TokenType op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    BinaryOpExpr(TokenType op_type, std::unique_ptr<ASTNode> left_expr, std::unique_ptr<ASTNode> right_expr)
        : op(op_type), left(std::move(left_expr)), right(std::move(right_expr)) {}
};

struct UnaryOpExpr : public ASTNode
{
    TokenType op;
    std::unique_ptr<ASTNode> right;

    UnaryOpExpr(TokenType op_type, std::unique_ptr<ASTNode> right_expr)
        : op(op_type), right(std::move(right_expr)) {}
};

struct CallExpr : public ASTNode
{
    std::unique_ptr<ASTNode> callee;
    std::vector<std::unique_ptr<ASTNode>> arguments;

    CallExpr(std::unique_ptr<ASTNode> callee_expr, std::vector<std::unique_ptr<ASTNode>> args)
        : callee(std::move(callee_expr)), arguments(std::move(args)) {}
};

struct AssignmentStatement : public ASTNode
{
    std::unique_ptr<VariableExpr> target;
    std::unique_ptr<ASTNode> value;
    bool is_declaration;

    AssignmentStatement(std::unique_ptr<VariableExpr> target_var, std::unique_ptr<ASTNode> assigned_value, bool is_decl = false)
        : target(std::move(target_var)), value(std::move(assigned_value)), is_declaration(is_decl) {}
};

struct DeclarationStatement : public ASTNode
{
    TokenType declared_type;
    std::unique_ptr<VariableExpr> target;
    std::unique_ptr<ASTNode> value;

    DeclarationStatement(TokenType type, std::unique_ptr<VariableExpr> target_var, std::unique_ptr<ASTNode> initial_value)
        : declared_type(type), target(std::move(target_var)), value(std::move(initial_value)) {}
};

struct EchoStatement : public ASTNode
{
    std::unique_ptr<ASTNode> expression;

    EchoStatement(std::unique_ptr<ASTNode> expr_to_echo) : expression(std::move(expr_to_echo)) {}
};

struct ReturnStatement : public ASTNode
{
    std::unique_ptr<ASTNode> expression;
    ReturnStatement(std::unique_ptr<ASTNode> expr) : expression(std::move(expr)) {}
};

struct BlockStatement : public ASTNode
{
    std::vector<std::unique_ptr<ASTNode>> statements;
    BlockStatement() = default;
};

struct ParameterDeclaration
{
    TokenType declared_type;
    std::string name;
    std::unique_ptr<ASTNode> default_value;

    ParameterDeclaration(TokenType type, std::string param_name, std::unique_ptr<ASTNode> def_val = nullptr)
        : declared_type(type), name(std::move(param_name)), default_value(std::move(def_val)) {}

    ParameterDeclaration(const ParameterDeclaration &) = delete;
    ParameterDeclaration &operator=(const ParameterDeclaration &) = delete;
    ParameterDeclaration(ParameterDeclaration &&) = default;
    ParameterDeclaration &operator=(ParameterDeclaration &&) = default;
};

struct FunctionDeclaration : public ASTNode
{
    std::string name;
    std::vector<ParameterDeclaration> parameters;
    TokenType return_type;
    std::unique_ptr<BlockStatement> body;

    FunctionDeclaration(std::string func_name, std::vector<ParameterDeclaration> &&params, TokenType ret_type, std::unique_ptr<BlockStatement> func_body)
        : name(std::move(func_name)), parameters(std::move(params)), return_type(ret_type), body(std::move(func_body)) {}
};

struct ProgramNode : public ASTNode
{
    std::vector<std::unique_ptr<ASTNode>> statements;
    ProgramNode() = default;
};

#endif