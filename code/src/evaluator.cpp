// /nirvana/prep_ai/../code/src/evaluator.cpp
#include "../include/evaluator.h"
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <sstream>

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
    else if (auto *numLit = dynamic_cast<NumberLiteralExpr *>(node))
    {
        return evaluateNumberLiteralExpr(numLit);
    }
    else if (auto *boolLit = dynamic_cast<BooleanLiteralExpr *>(node))
    {
        return evaluateBooleanLiteralExpr(boolLit);
    }
    else if (auto *var = dynamic_cast<VariableExpr *>(node))
    {
        return evaluateVariableExpr(var);
    }
    else if (auto *binOp = dynamic_cast<BinaryOpExpr *>(node))
    {
        return evaluateBinaryOpExpr(binOp);
    }
    else if (auto *unaryOp = dynamic_cast<UnaryOpExpr *>(node))
    {
        return evaluateUnaryOpExpr(unaryOp);
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

Value Evaluator::evaluateNumberLiteralExpr(NumberLiteralExpr *node)
{
    return node->value;
}

Value Evaluator::evaluateBooleanLiteralExpr(BooleanLiteralExpr *node)
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

Value Evaluator::evaluateUnaryOpExpr(UnaryOpExpr *node)
{
    Value right_val = evaluate(node->right.get());

    if (node->op == TokenType::MINUS)
    {
        if (std::holds_alternative<double>(right_val))
        {
            return -std::get<double>(right_val);
        }
        else
        {
            throw std::runtime_error("Type error: Unary '-' operator can only be applied to numbers.");
        }
    }
    else if (node->op == TokenType::BANG || node->op == TokenType::NOT)
    {
        if (std::holds_alternative<bool>(right_val))
        {
            return !std::get<bool>(right_val);
        }
        else
        {
            throw std::runtime_error("Type error: Unary '!' or 'not' operator can only be applied to booleans.");
        }
    }
    throw std::runtime_error("Unsupported unary operator.");
}

Value Evaluator::evaluateBinaryOpExpr(BinaryOpExpr *node)
{
    Value left_val = evaluate(node->left.get());
    Value right_val = evaluate(node->right.get());

    if (node->op == TokenType::DOT)
    {
        std::string s_left, s_right;
        if (std::holds_alternative<std::string>(left_val))
        {
            s_left = std::get<std::string>(left_val);
        }
        else
        {
            std::stringstream ss;
            ss << left_val;
            s_left = ss.str();
        }

        if (std::holds_alternative<std::string>(right_val))
        {
            s_right = std::get<std::string>(right_val);
        }
        else
        {
            std::stringstream ss;
            ss << right_val;
            s_right = ss.str();
        }
        return s_left + s_right;
    }
    else if (node->op == TokenType::PLUS)
    {
        if (std::holds_alternative<double>(left_val) && std::holds_alternative<double>(right_val))
        {
            return std::get<double>(left_val) + std::get<double>(right_val);
        }
        else
        {
            throw std::runtime_error("Type error: '+' operator requires two numbers.");
        }
    }
    else if (node->op == TokenType::MINUS)
    {
        if (std::holds_alternative<double>(left_val) && std::holds_alternative<double>(right_val))
        {
            return std::get<double>(left_val) - std::get<double>(right_val);
        }
        else
        {
            throw std::runtime_error("Type error: '-' operator requires two numbers.");
        }
    }
    else if (node->op == TokenType::STAR)
    {
        if (std::holds_alternative<double>(left_val) && std::holds_alternative<double>(right_val))
        {
            return std::get<double>(left_val) * std::get<double>(right_val);
        }
        else
        {
            throw std::runtime_error("Type error: '*' operator requires two numbers.");
        }
    }
    else if (node->op == TokenType::SLASH)
    {
        if (std::holds_alternative<double>(left_val) && std::holds_alternative<double>(right_val))
        {
            if (std::get<double>(right_val) == 0.0)
            {
                throw std::runtime_error("Runtime error: Division by zero.");
            }
            return std::get<double>(left_val) / std::get<double>(right_val);
        }
        else
        {
            throw std::runtime_error("Type error: '/' operator requires two numbers.");
        }
    }
    else if (node->op == TokenType::GREATER || node->op == TokenType::GREATER_EQUAL ||
             node->op == TokenType::LESS || node->op == TokenType::LESS_EQUAL)
    {
        if (std::holds_alternative<double>(left_val) && std::holds_alternative<double>(right_val))
        {
            double l = std::get<double>(left_val);
            double r = std::get<double>(right_val);
            switch (node->op)
            {
            case TokenType::GREATER:
                return l > r;
            case TokenType::GREATER_EQUAL:
                return l >= r;
            case TokenType::LESS:
                return l < r;
            case TokenType::LESS_EQUAL:
                return l <= r;
            default:
                break;
            }
        }
        else
        {
            throw std::runtime_error("Type error: Comparison operators (>, >=, <, <=) require two numbers.");
        }
    }
    else if (node->op == TokenType::EQUAL_EQUAL || node->op == TokenType::BANG_EQUAL)
    {
        bool are_equal;
        if (left_val.index() != right_val.index())
        {
            are_equal = false;
        }
        else if (std::holds_alternative<std::string>(left_val))
        {
            are_equal = (std::get<std::string>(left_val) == std::get<std::string>(right_val));
        }
        else if (std::holds_alternative<double>(left_val))
        {
            are_equal = (std::fabs(std::get<double>(left_val) - std::get<double>(right_val)) < 0.000001);
        }
        else if (std::holds_alternative<bool>(left_val))
        {
            are_equal = (std::get<bool>(left_val) == std::get<bool>(right_val));
        }
        else if (std::holds_alternative<std::monostate>(left_val))
        {
            are_equal = true;
        }
        else
        {
            throw std::runtime_error("Type error: Unsupported types for equality comparison.");
        }

        return (node->op == TokenType::EQUAL_EQUAL) ? are_equal : !are_equal;
    }
    else if (node->op == TokenType::AND || node->op == TokenType::OR)
    {
        if (std::holds_alternative<bool>(left_val) && std::holds_alternative<bool>(right_val))
        {
            bool l = std::get<bool>(left_val);
            bool r = std::get<bool>(right_val);
            if (node->op == TokenType::AND)
            {
                return l && r;
            }
            else
            {
                return l || r;
            }
        }
        else
        {
            throw std::runtime_error("Type error: Logical operators (and, or) require two booleans.");
        }
    }

    throw std::runtime_error("Unsupported binary operator.");
}

void Evaluator::interpret(std::unique_ptr<ProgramNode> ast)
{
    evaluate(ast.get());
}