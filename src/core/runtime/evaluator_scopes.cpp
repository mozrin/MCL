#include "evaluator.h"
#include <stdexcept>
#include <map>
#include <string>
#include <utility>

void Evaluator::enterScope()
{
    scopeStack.emplace_back();
}

void Evaluator::exitScope()
{
    if (scopeStack.empty())
    {
        throw std::runtime_error("Internal error: Attempted to exit empty scope stack.");
    }
    scopeStack.pop_back();
}

std::pair<Value *, DeclaredType *> Evaluator::findVariableInScope(const std::string &name)
{
    for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it)
    {
        auto var_it = it->find(name);
        if (var_it != it->end())
        {
            return {&var_it->second.first, &var_it->second.second};
        }
    }
    return {nullptr, nullptr};
}