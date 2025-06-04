#include "../evaluator.h"
#include "../../parser/ast.h"
#include "../../common/value.h"
#include "../../common/token.h"
#include <stdexcept>
#include <variant>
#include <sstream>
#include <cmath>

Value Evaluator::evaluateUnaryOpExpr(UnaryOpExpr *node)
{
    Value right_val = evaluate(node->right.get());

    if (node->op == TokenType::MINUS)
    {
        return std::visit([](auto &&arg) -> Value
                          {
using T = std::decay_t<decltype(arg)>;
if constexpr (std::is_same_v<T, long long>) {
return -arg;
} else if constexpr (std::is_same_v<T, double>) {
return -arg;
} else if constexpr (std::is_same_v<T, bool>) {
return static_cast<long long>(arg ? -1 : 0);
} else {
throw std::runtime_error("Type error: Unary '-' operator can only be applied to numbers.");
} }, right_val);
    }
    else if (node->op == TokenType::BANG || node->op == TokenType::NOT)
    {
        return !convertToBool(right_val);
    }
    throw std::runtime_error("Unsupported unary operator.");
}