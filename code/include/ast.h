// include/ast.h
#ifndef AST_H
#define AST_H

#include <memory> // For std::unique_ptr, essential for managing tree ownership
#include <string>
#include <vector>
#include "token.h" // To use TokenType for operators (like DOT)

// Base class for all Abstract Syntax Tree nodes.
// Virtual destructor ensures proper cleanup when unique_ptr deletes derived objects.
struct ASTNode
{
    virtual ~ASTNode() = default;
};

// --- Expression Nodes ---
// Expressions produce a value (e.g., "hello", $name, "a" . "b")

// Represents a literal string value (e.g., "hello", "World!")
struct StringLiteralExpr : public ASTNode
{
    std::string value; // The actual string content
    StringLiteralExpr(std::string val) : value(std::move(val)) {}
};

// Represents a variable (e.g., $name, $my_variable)
struct VariableExpr : public ASTNode
{
    std::string name; // The variable's name including the '$' (e.g., "$name")
    VariableExpr(std::string name) : name(std::move(name)) {}
};

// Represents a binary operation (an operation with two operands and an operator between them)
// For our MVI, this will be concatenation ('.')
struct BinaryOpExpr : public ASTNode
{
    TokenType op;                   // The type of the operator (e.g., TokenType::DOT for concatenation)
    std::unique_ptr<ASTNode> left;  // The left-hand side expression
    std::unique_ptr<ASTNode> right; // The right-hand side expression

    // Constructor to initialize the operator and its operands
    BinaryOpExpr(TokenType op_type, std::unique_ptr<ASTNode> left_expr, std::unique_ptr<ASTNode> right_expr)
        : op(op_type), left(std::move(left_expr)), right(std::move(right_expr)) {}
};

// --- Statement Nodes ---
// Statements perform an action and typically don't produce a value (e.g., $x = 1;, echo "hi";)

// Represents a variable assignment statement (e.g., $name = "World";)
struct AssignmentStatement : public ASTNode
{
    std::unique_ptr<VariableExpr> target; // The variable on the left side of '='
    std::unique_ptr<ASTNode> value;       // The expression on the right side of '='

    // Constructor to initialize the target variable and the assigned value
    AssignmentStatement(std::unique_ptr<VariableExpr> target_var, std::unique_ptr<ASTNode> assigned_value)
        : target(std::move(target_var)), value(std::move(assigned_value)) {}
};

// Represents an 'echo' statement (e.g., echo "Hello!";)
struct EchoStatement : public ASTNode
{
    std::unique_ptr<ASTNode> expression; // The expression whose value will be printed

    // Constructor to initialize the expression to be echoed
    EchoStatement(std::unique_ptr<ASTNode> expr_to_echo) : expression(std::move(expr_to_echo)) {}
};

// --- Program Node ---
// The root node of the entire AST, representing the whole program.
struct ProgramNode : public ASTNode
{
    std::vector<std::unique_ptr<ASTNode>> statements; // A list of all statements in the program

    // Constructor (no arguments needed, just an empty list of statements)
    ProgramNode() = default;
};

#endif // AST_H