#include "../evaluator.h"
#include "../../parser/ast.h"
#include "../../common/value.h"
#include "../../common/token.h"
#include <stdexcept>
#include <variant>
#include <sstream>
#include <cmath>

Value Evaluator::evaluateVariableExpr(VariableExpr *node)
{
    auto [val_ptr, type_ptr] = findVariableInScope(node->name);
    if (val_ptr != nullptr)
    {
        return *val_ptr;
    }
    else
    {
        throw std::runtime_error("Undefined variable: " + node->name);
    }
}