#ifndef HELPERS_H
#define HELPERS_H

#include "../core/runtime/evaluator.h"
#include "../core/common/value.h"
#include "../core/common/token.h"
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <sstream>
#include <cmath>

enum class ParameterKind
{
    REQUIRED,
    OPTIONAL,
    FIXED
};

struct ParameterDefinition
{
    ParameterKind kind;
    DeclaredType expected_type;
    std::string config_value_str;
    Value resolved_value;

    ParameterDefinition() : kind(ParameterKind::REQUIRED), expected_type(DeclaredType::ANY) {}
};

struct HelperDefinition
{
    std::string helper_name;
    std::string extension_name;
    std::vector<ParameterDefinition> parameters;
};

void registerAllHelpers(Evaluator &eval);

inline std::string get_string_arg(const std::vector<Value> &args, size_t index, const std::string &func_name, const std::string &arg_name)
{
    if (index >= args.size())
    {
        throw std::runtime_error("Function '" + func_name + "': Missing argument " + arg_name + " at index " + std::to_string(index) + ".");
    }
    if (std::holds_alternative<std::string>(args[index]))
    {
        return std::get<std::string>(args[index]);
    }
    std::stringstream ss;
    ss << "Function '" << func_name << "': Argument " << arg_name << " must be a string, but got value of type ";
    switch (valueTypeToDeclaredType(args[index]))
    {
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
        ss << "unknown";
        break;
    }
    ss << ".";
    throw std::runtime_error(ss.str());
}

inline long long get_integer_arg(const std::vector<Value> &args, size_t index, const std::string &func_name, const std::string &arg_name)
{
    if (index >= args.size())
    {
        throw std::runtime_error("Function '" + func_name + "': Missing argument " + arg_name + " at index " + std::to_string(index) + ".");
    }
    if (std::holds_alternative<long long>(args[index]))
    {
        return std::get<long long>(args[index]);
    }
    else if (std::holds_alternative<double>(args[index]))
    {
        double d_val = std::get<double>(args[index]);
        if (d_val != std::floor(d_val))
        {
            std::stringstream ss;
            ss << "Function '" << func_name << "': Argument " << arg_name << " must be an integer, but got " << d_val << ".";
            throw std::runtime_error(ss.str());
        }
        return static_cast<long long>(d_val);
    }
    std::stringstream ss;
    ss << "Function '" << func_name << "': Argument " << arg_name << " must be an integer (integer or number), but got value of type ";
    switch (valueTypeToDeclaredType(args[index]))
    {
    case DeclaredType::STRING:
        ss << "string";
        break;
    case DeclaredType::BOOLEAN:
        ss << "boolean";
        break;
    default:
        ss << "unknown";
        break;
    }
    ss << ".";
    throw std::runtime_error(ss.str());
}

inline Value get_number_arg(const std::vector<Value> &args, size_t index, const std::string &func_name, const std::string &arg_name)
{
    if (index >= args.size())
    {
        throw std::runtime_error("Function '" + func_name + "': Missing argument " + arg_name + " at index " + std::to_string(index) + ".");
    }
    if (std::holds_alternative<long long>(args[index]) || std::holds_alternative<double>(args[index]))
    {
        return args[index];
    }
    std::stringstream ss;
    ss << "Function '" << func_name << "': Argument " << arg_name << " must be a number (integer or number), but got value of type ";
    switch (valueTypeToDeclaredType(args[index]))
    {
    case DeclaredType::STRING:
        ss << "string";
        break;
    case DeclaredType::BOOLEAN:
        ss << "boolean";
        break;
    default:
        ss << "unknown";
        break;
    }
    ss << ".";
    throw std::runtime_error(ss.str());
}

#endif