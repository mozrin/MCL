#include "../include/evaluator.h"
#include <iostream>
#include <stdexcept>

Evaluator::Evaluator()
{
}

Value Evaluator::evaluate(ASTNode *node)
{
    if (!node)
    {
        return std::monostate{};
    }

    if (auto *prog = dynamic_cast<ProgramNode *>(node))
    {
        return evaluateProgramNode(prog);
    }
    else if (auto *assign = dynamic_cast<AssignmentStatement *>(node))
    {
        return evaluateAssignmentStatement(assign);
    }
    else if (auto *echo = dynamic_cast<EchoStatement *>(node))
    {
        return evaluateEchoStatement(echo);
    }
    else if (auto *strLit = dynamic_cast<StringLiteralExpr *>(node))
    {
        return evaluateStringLiteralExpr(strLit);
    }
    else if (auto *var = dynamic_cast<VariableExpr *>(node))
    {
        return evaluateVariableExpr(var);
    }
    else if (auto *binOp = dynamic_cast<BinaryOpExpr *>(node))
    {
        return evaluateBinaryOpExpr(binOp);
    }
    else
    {
        throw std::runtime_error("Unknown AST node type during evaluation.");
    }
}

Value Evaluator::evaluateProgramNode(ProgramNode *node)
{
    for (const auto &stmt : node->statements)
    {
        evaluate(stmt.get());
    }
    return std::monostate{};
}

Value Evaluator::evaluateAssignmentStatement(AssignmentStatement *node)
{
    Value value = evaluate(node->value.get());
    variables[node->target->name] = value;
    return value;
}

Value Evaluator::evaluateEchoStatement(EchoStatement *node)
{
    Value result = evaluate(node->expression.get());
    std::cout << result << std::endl;
    return std::monostate{};
}

Value Evaluator::evaluateStringLiteralExpr(StringLiteralExpr *node)
{
    return node->value;
}

Value Evaluator::evaluateVariableExpr(VariableExpr *node)
{
    auto it = variables.find(node->name);
    if (it != variables.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("Undefined variable: " + node->name);
    }
}

Value Evaluator::evaluateBinaryOpExpr(BinaryOpExpr *node)
{
    Value left_val = evaluate(node->left.get());
    Value right_val = evaluate(node->right.get());

    if (node->op == TokenType::DOT)
    {
        if (std::holds_alternative<std::string>(left_val) && std::holds_alternative<std::string>(right_val))
        {
            return std::get<std::string>(left_val) + std::get<std::string>(right_val);
        }
        else
        {
            throw std::runtime_error("Type error: Only string concatenation is supported with '.' operator.");
        }
    }
    throw std::runtime_error("Unsupported binary operator.");
}

void Evaluator::interpret(std::unique_ptr<ProgramNode> ast)
{
    evaluate(ast.get());
}