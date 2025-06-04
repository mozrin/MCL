#include "evaluator.h"
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <variant>
#include <limits>

Evaluator::Evaluator()
{
    enterScope();
}

void Evaluator::registerNativeFunction(const std::string &name, NativeFunction func)
{
    nativeFunctions[name] = std::move(func);
}

void Evaluator::registerConstant(const std::string &name, Value value)
{
    if (findVariableInScope(name).first != nullptr)
    {
        throw std::runtime_error("Internal error: Attempted to re-register existing constant/variable '" + name + "'.");
    }

    scopeStack.front()[name] = {value, valueTypeToDeclaredType(value)};
}

void Evaluator::interpret(std::unique_ptr<ProgramNode> ast)
{
    evaluate(ast.get());
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
    else if (auto *funcDecl = dynamic_cast<FunctionDeclaration *>(node))
    {
        return evaluateFunctionDeclaration(funcDecl);
    }
    else if (auto *block = dynamic_cast<BlockStatement *>(node))
    {
        return evaluateBlockStatement(block);
    }
    else if (auto *ret = dynamic_cast<ReturnStatement *>(node))
    {
        return evaluateReturnStatement(ret);
    }
    else if (auto *decl = dynamic_cast<DeclarationStatement *>(node))
    {
        return evaluateDeclarationStatement(decl);
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
    else if (auto *call = dynamic_cast<CallExpr *>(node))
    {
        return evaluateCallExpr(call);
    }
    else
    {
        throw std::runtime_error("Unknown AST node type during evaluation.");
    }
}

Value Evaluator::callNativeFunctionByName(const std::string &name, const std::vector<Value> &args)
{
    auto it = nativeFunctions.find(name);
    if (it == nativeFunctions.end())
    {
        throw std::runtime_error("Runtime error: Attempted to call unknown native function '" + name + "'.");
    }
    return it->second(args);
}

Value Evaluator::getConstant(const std::string &name)
{
    auto [val_ptr, type_ptr] = findVariableInScope(name);
    if (val_ptr == nullptr)
    {
        throw std::runtime_error("Runtime error: Undefined constant '" + name + "'.");
    }
    return *val_ptr;
}