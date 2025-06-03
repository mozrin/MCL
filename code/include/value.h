// /nirvana/prep_ai/../code/include/value.h
#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <variant>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

// Define the declared types for static checking
enum class DeclaredType
{
    ANY, // For dynamically typed variables
    STRING,
    INTEGER,
    NUMBER, // Floating point
    BOOLEAN
};

// Map Value actual type to DeclaredType
inline DeclaredType valueTypeToDeclaredType(const std::variant<std::string, long long, double, bool, std::monostate>& val) {
    if (std::holds_alternative<std::string>(val)) return DeclaredType::STRING;
    if (std::holds_alternative<long long>(val)) return DeclaredType::INTEGER;
    if (std::holds_alternative<double>(val)) return DeclaredType::NUMBER;
    if (std::holds_alternative<bool>(val)) return DeclaredType::BOOLEAN;
    return DeclaredType::ANY; // For monostate or unknown, though monostate usually means null.
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
ss << std::fixed << std::setprecision(5) << std::get<double>(val);
std::string s = ss.str();
s.erase(s.find_last_not_of('0') + 1, std::string::npos);
if (s.back() == '.')
{
s += '0';
}
os << s;
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