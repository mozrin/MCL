#include "../evaluator.h"
#include "../../parser/ast.h"
#include "../../common/value.h"
#include "../../common/token.h"
#include <stdexcept>
#include <variant>
#include <sstream>
#include <cmath>
#include <limits>

Value Evaluator::performNumericBinaryOp(const Value &left_val, const Value &right_val, TokenType op)
{
    long long left_ll = 0;
    bool left_is_ll_candidate = false;
    if (std::holds_alternative<long long>(left_val))
    {
        left_ll = std::get<long long>(left_val);
        left_is_ll_candidate = true;
    }
    else if (std::holds_alternative<bool>(left_val))
    {
        left_ll = std::get<bool>(left_val) ? 1LL : 0LL;
        left_is_ll_candidate = true;
    }
    else if (std::holds_alternative<double>(left_val))
    {
        left_is_ll_candidate = false;
    }

    long long right_ll = 0;
    bool right_is_ll_candidate = false;
    if (std::holds_alternative<long long>(right_val))
    {
        right_ll = std::get<long long>(right_val);
        right_is_ll_candidate = true;
    }
    else if (std::holds_alternative<bool>(right_val))
    {
        right_ll = std::get<bool>(right_val) ? 1LL : 0LL;
        right_is_ll_candidate = true;
    }
    else if (std::holds_alternative<double>(right_val))
    {
        right_is_ll_candidate = false;
    }

    if (left_is_ll_candidate && right_is_ll_candidate)
    {
        switch (op)
        {
        case TokenType::PLUS:
            return left_ll + right_ll;
        case TokenType::MINUS:
            return left_ll - right_ll;
        case TokenType::STAR:
            return left_ll * right_ll;
        case TokenType::SLASH:
            if (right_ll == 0LL)
                throw std::runtime_error("Runtime error: Integer division by zero.");
            return left_ll / right_ll;
        default:
            break;
        }
    }

    double left_double_val;
    if (std::holds_alternative<double>(left_val))
        left_double_val = std::get<double>(left_val);
    else
        left_double_val = static_cast<double>(left_ll);

    double right_double_val;
    if (std::holds_alternative<double>(right_val))
        right_double_val = std::get<double>(right_val);
    else
        right_double_val = static_cast<double>(right_ll);

    switch (op)
    {
    case TokenType::PLUS:
        return left_double_val + right_double_val;
    case TokenType::MINUS:
        return left_double_val - right_double_val;
    case TokenType::STAR:
        return left_double_val * right_double_val;
    case TokenType::SLASH:
        if (right_double_val == 0.0)
            throw std::runtime_error("Runtime error: Floating-point division by zero.");
        return left_double_val / right_double_val;
    case TokenType::GREATER:
        return left_double_val > right_double_val;
    case TokenType::GREATER_EQUAL:
        return left_double_val >= right_double_val;
    case TokenType::LESS:
        return left_double_val < right_double_val;
    case TokenType::LESS_EQUAL:
        return left_double_val <= right_double_val;
    default:
        throw std::runtime_error("Unsupported numeric binary operator.");
    }
}

Value Evaluator::performBooleanBinaryOp(const Value &left_val, const Value &right_val, TokenType op)
{
    bool left_bool = convertToBool(left_val);
    bool right_bool = convertToBool(right_val);

    switch (op)
    {
    case TokenType::AND:
        return left_bool && right_bool;
    case TokenType::OR:
        return left_bool || right_bool;
    default:
        throw std::runtime_error("Unsupported boolean binary operator.");
    }
}

Value Evaluator::evaluateBinaryOpExpr(BinaryOpExpr *node)
{
    Value left_val = evaluate(node->left.get());
    Value right_val = evaluate(node->right.get());

    if (node->op == TokenType::DOT)
    {
        std::string s_left, s_right;
        std::visit([&](auto &&arg)
                   {
using T = std::decay_t<decltype(arg)>;
if constexpr (std::is_same_v<T, std::string>) {
s_left = arg;
} else {
std::stringstream ss;
ss << arg;
s_left = ss.str();
} }, left_val);
        std::visit([&](auto &&arg)
                   {
using T = std::decay_t<decltype(arg)>;
if constexpr (std::is_same_v<T, std::string>) {
s_right = arg;
} else {
std::stringstream ss;
ss << arg;
s_right = ss.str();
} }, right_val);
        return s_left + s_right;
    }
    else if (node->op == TokenType::PLUS || node->op == TokenType::MINUS ||
             node->op == TokenType::STAR || node->op == TokenType::SLASH ||
             node->op == TokenType::GREATER || node->op == TokenType::GREATER_EQUAL ||
             node->op == TokenType::LESS || node->op == TokenType::LESS_EQUAL)
    {

        bool left_is_numeric_like = std::holds_alternative<long long>(left_val) || std::holds_alternative<double>(left_val) || std::holds_alternative<bool>(left_val);
        bool right_is_numeric_like = std::holds_alternative<long long>(right_val) || std::holds_alternative<double>(right_val) || std::holds_alternative<bool>(right_val);

        if (!left_is_numeric_like || !right_is_numeric_like)
        {
            throw std::runtime_error("Type error: Arithmetic/comparison operators can only be applied to numbers.");
        }

        return performNumericBinaryOp(left_val, right_val, node->op);
    }
    else if (node->op == TokenType::EQUAL_EQUAL || node->op == TokenType::BANG_EQUAL)
    {

        bool are_equal = std::visit([](auto &&l_arg, auto &&r_arg) -> bool
                                    {

using L = std::decay_t<decltype(l_arg)>;
using R = std::decay_t<decltype(r_arg)>;

if constexpr (std::is_same_v<L, R>) {
if constexpr (std::is_same_v<L, std::string>) return l_arg == r_arg;
else if constexpr (std::is_same_v<L, long long>) return l_arg == r_arg;
else if constexpr (std::is_same_v<L, double>) return std::abs(l_arg - r_arg) < 0.000001;
else if constexpr (std::is_same_v<L, bool>) return l_arg == r_arg;
else if constexpr (std::is_same_v<L, std::monostate>) return true;
}

else if constexpr (
(std::is_same_v<L, long long> || std::is_same_v<L, double> || std::is_same_v<L, bool>) &&
(std::is_same_v<R, long long> || std::is_same_v<R, double> || std::is_same_v<R, bool>)
) {
double l_double, r_double;
if constexpr (std::is_same_v<L, long long>) l_double = static_cast<double>(l_arg);
else if constexpr (std::is_same_v<L, double>) l_double = l_arg;
else if constexpr (std::is_same_v<L, bool>) l_double = l_arg ? 1.0 : 0.0;
else l_double = 0.0;

if constexpr (std::is_same_v<R, long long>) r_double = static_cast<double>(r_arg);
else if constexpr (std::is_same_v<R, double>) r_double = r_arg;
else if constexpr (std::is_same_v<R, bool>) r_double = r_arg ? 1.0 : 0.0;
else r_double = 0.0;

return std::abs(l_double - r_double) < 0.000001;
}

return false; }, left_val, right_val);

        return (node->op == TokenType::EQUAL_EQUAL) ? are_equal : !are_equal;
    }
    else if (node->op == TokenType::AND || node->op == TokenType::OR)
    {

        return performBooleanBinaryOp(left_val, right_val, node->op);
    }
    else if (node->op == TokenType::PIPE)
    {
        long long left_int_val;
        long long right_int_val;

        if (std::holds_alternative<long long>(left_val))
        {
            left_int_val = std::get<long long>(left_val);
        }
        else if (std::holds_alternative<double>(left_val))
        {
            double d_val = std::get<double>(left_val);
            long long ll_val = static_cast<long long>(d_val);
            if (d_val != static_cast<double>(ll_val))
            {
                throw std::runtime_error("Type error: Bitwise OR operator '|' requires integer operands. Left operand is a non-integer number.");
            }
            left_int_val = ll_val;
        }
        else if (std::holds_alternative<bool>(left_val))
        {
            left_int_val = std::get<bool>(left_val) ? 1LL : 0LL;
        }
        else
        {
            throw std::runtime_error("Type error: Bitwise OR operator '|' can only be applied to integer or number (convertible to integer) operands. Left operand is of invalid type.");
        }

        if (std::holds_alternative<long long>(right_val))
        {
            right_int_val = std::get<long long>(right_val);
        }
        else if (std::holds_alternative<double>(right_val))
        {
            double d_val = std::get<double>(right_val);
            long long ll_val = static_cast<long long>(d_val);
            if (d_val != static_cast<double>(ll_val))
            {
                throw std::runtime_error("Type error: Bitwise OR operator '|' requires integer operands. Right operand is a non-integer number.");
            }
            right_int_val = ll_val;
        }
        else if (std::holds_alternative<bool>(right_val))
        {
            right_int_val = std::get<bool>(right_val) ? 1LL : 0LL;
        }
        else
        {
            throw std::runtime_error("Type error: Bitwise OR operator '|' can only be applied to integer or number (convertible to integer) operands. Right operand is of invalid type.");
        }

        return left_int_val | right_int_val;
    }

    throw std::runtime_error("Unsupported binary operator.");
}