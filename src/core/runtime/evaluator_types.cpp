#include "evaluator.h"
#include "../common/value.h"
#include "../common/token.h"
#include <stdexcept>
#include <variant>
#include <sstream>
#include <cmath>
#include <limits>

bool Evaluator::convertToBool(const Value &val)
{
    return std::visit([](auto &&arg) -> bool
                      {
using T = std::decay_t<decltype(arg)>;
if constexpr (std::is_same_v<T, std::string>) {
return !arg.empty();
} else if constexpr (std::is_same_v<T, long long>) {
return arg != 0LL;
} else if constexpr (std::is_same_v<T, double>) {
return arg != 0.0;
} else if constexpr (std::is_same_v<T, bool>) {
return arg;
} else if constexpr (std::is_same_v<T, std::monostate>) {
return false;
} else {
throw std::runtime_error("Internal error: convertToBool received unsupported type.");
} }, val);
}

void Evaluator::enforceType(const std::string &var_name, DeclaredType declared_type, const Value &assigned_value)
{
    DeclaredType actual_type = valueTypeToDeclaredType(assigned_value);

    if (declared_type == DeclaredType::ANY)
    {
        return;
    }

    bool type_matches = false;
    switch (declared_type)
    {
    case DeclaredType::STRING:
        type_matches = (actual_type == DeclaredType::STRING);
        break;
    case DeclaredType::INTEGER:
        type_matches = (actual_type == DeclaredType::INTEGER || actual_type == DeclaredType::BOOLEAN);
        if (actual_type == DeclaredType::NUMBER)
        {
            double d_val = std::get<double>(assigned_value);
            type_matches = (d_val == std::floor(d_val) && d_val >= std::numeric_limits<long long>::min() && d_val <= std::numeric_limits<long long>::max());
        }
        break;
    case DeclaredType::NUMBER:
        type_matches = (actual_type == DeclaredType::NUMBER || actual_type == DeclaredType::INTEGER || actual_type == DeclaredType::BOOLEAN);
        break;
    case DeclaredType::BOOLEAN:
        type_matches = (actual_type == DeclaredType::BOOLEAN);
        if (actual_type == DeclaredType::INTEGER)
        {
            long long ll_val = std::get<long long>(assigned_value);
            type_matches = (ll_val == 0LL || ll_val == 1LL);
        }
        break;
    default:
        throw std::runtime_error("Internal error: Unknown declared type for variable '" + var_name + "'.");
    }

    if (!type_matches)
    {
        std::stringstream ss;
        ss << "Type mismatch for variable '" << var_name << "'. Expected ";
        switch (declared_type)
        {
        case DeclaredType::STRING:
            ss << "string";
            break;
        case DeclaredType::INTEGER:
            ss << "integer";
            break;
        case DeclaredType::NUMBER:
            ss << "number";
            break;
        case DeclaredType::BOOLEAN:
            ss << "boolean";
            break;
        default:
            ss << "unknown declared type";
            break;
        }
        ss << ", but got value of type ";
        switch (actual_type)
        {
        case DeclaredType::STRING:
            ss << "string";
            break;
        case DeclaredType::INTEGER:
            ss << "integer";
            break;
        case DeclaredType::NUMBER:
            ss << "number";
            break;
        case DeclaredType::BOOLEAN:
            ss << "boolean";
            break;
        case DeclaredType::ANY:
            ss << "null";
            break;
        default:
            ss << "unknown actual type";
            break;
        }
        throw std::runtime_error(ss.str());
    }
}

Value Evaluator::getDefaultValueForType(TokenType type_token)
{
    switch (type_token)
    {
    case TokenType::STRING_KEYWORD:
        return std::string("");
    case TokenType::INTEGER_KEYWORD:
        return 0LL;
    case TokenType::NUMBER_KEYWORD:
        return 0.0;
    case TokenType::BOOLEAN_KEYWORD:
        return false;
    default:
        return std::monostate{};
    }
}