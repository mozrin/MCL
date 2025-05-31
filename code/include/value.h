#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <variant>
#include <iostream>

using Value = std::variant<std::string, std::monostate>;

inline std::ostream &operator<<(std::ostream &os, const Value &val)
{
    if (std::holds_alternative<std::string>(val))
    {
        os << std::get<std::string>(val);
    }
    else if (std::holds_alternative<std::monostate>(val))
    {
        os << "null";
    }
    return os;
}

#endif