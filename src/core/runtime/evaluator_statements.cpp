#include "evaluator.h"
#include "../parser/ast.h"
#include "../common/value.h"
#include "../common/token.h"
#include <stdexcept>
#include <string>
#include <sstream>

Value Evaluator::evaluateProgramNode(ProgramNode *node)
{
    for (const auto &stmt : node->statements)
    {
        evaluate(stmt.get());
    }
    return std::monostate{};
}

Value Evaluator::evaluateFunctionDeclaration(FunctionDeclaration *node)
{
    if (userFunctions.count(node->name) || nativeFunctions.count(node->name))
    {
        throw std::runtime_error("Runtime error: Function or native function '" + node->name + "' already declared.");
    }
    userFunctions[node->name] = node;
    return std::monostate{};
}

Value Evaluator::evaluateBlockStatement(BlockStatement *node)
{
    for (const auto &stmt : node->statements)
    {
        try
        {
            evaluate(stmt.get());
        }
        catch (const FunctionReturnException &)
        {
            throw;
        }
    }
    return std::monostate{};
}

Value Evaluator::evaluateReturnStatement(ReturnStatement *node)
{
    Value ret_val = std::monostate{};
    if (node->expression)
    {
        ret_val = evaluate(node->expression.get());
    }
    throw FunctionReturnException(std::move(ret_val));
}

Value Evaluator::evaluateDeclarationStatement(DeclarationStatement *node)
{
    std::string var_name = node->target->name;
    DeclaredType declared_type = tokenTypeToDeclaredType(node->declared_type);

    if (scopeStack.back().count(var_name))
    {
        throw std::runtime_error("Runtime error: Variable '" + var_name + "' already declared in this scope.");
    }

    Value initial_value_resolved = std::monostate{};
    if (node->value)
    {
        initial_value_resolved = evaluate(node->value.get());
    }
    else
    {
        initial_value_resolved = getDefaultValueForType(node->declared_type);
    }

    enforceType(var_name, declared_type, initial_value_resolved);
    scopeStack.back()[var_name] = {initial_value_resolved, declared_type};
    return initial_value_resolved;
}

Value Evaluator::evaluateAssignmentStatement(AssignmentStatement *node)
{
    Value value = evaluate(node->value.get());
    std::string var_name = node->target->name;

    auto [val_ptr, type_ptr] = findVariableInScope(var_name);

    if (val_ptr == nullptr)
    {
        scopeStack.back()[var_name] = {value, DeclaredType::ANY};
    }
    else
    {
        enforceType(var_name, *type_ptr, value);
        *val_ptr = value;
    }
    return value;
}

Value Evaluator::evaluateEchoStatement(EchoStatement *node)
{
    Value result = evaluate(node->expression.get());
    std::cout << result << std::endl;
    return std::monostate{};
}