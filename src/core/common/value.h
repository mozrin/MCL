#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <variant>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

enum class DeclaredType
{
    ANY,
    STRING,
    INTEGER,
    NUMBER,
    BOOLEAN
};

inline DeclaredType valueTypeToDeclaredType(const std::variant<std::string, long long, double, bool, std::monostate> &val)
{
    if (std::holds_alternative<std::string>(val))
        return DeclaredType::STRING;
    if (std::holds_alternative<long long>(val))
        return DeclaredType::INTEGER;
    if (std::holds_alternative<double>(val))
        return DeclaredType::NUMBER;
    if (std::holds_alternative<bool>(val))
        return DeclaredType::BOOLEAN;
    return DeclaredType::ANY;
}

using Value = std::variant<std::string, long long, double, bool, std::monostate>;

inline std::ostream &operator<<(std::ostream &os, const Value &val)
{
    if (std::holds_alternative<std::string>(val))
    {
        os << std::get<std::string>(val);
    }
    else if (std::holds_alternative<long long>(val))
    {
        os << std::get<long long>(val);
    }
    else if (std::holds_alternative<double>(val))
    {
        std::stringstream ss;
        ss << std::setprecision(14) << std::get<double>(val);
        os << ss.str();
    }
    else if (std::holds_alternative<bool>(val))
    {
        os << (std::get<bool>(val) ? "true" : "false");
    }
    else if (std::holds_alternative<std::monostate>(val))
    {
        os << "null";
    }
    return os;
}

#endif