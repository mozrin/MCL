#include "contains.h"
#include <string>
#include <vector>
#include <variant>
#include <stdexcept>
#include <sstream>

Value mcl_contains(const std::vector<Value> &args)
{
    if (args.size() != 2)
    {
        throw std::runtime_error("Function 'contains' expects exactly 2 arguments: contains(string $str, string $substring).");
    }

    std::string str;
    if (std::holds_alternative<std::string>(args[0]))
    {
        str = std::get<std::string>(args[0]);
    }
    else
    {
        std::stringstream ss;
        ss << "Function 'contains': Argument 1 ($str) must be a string, but got value of type ";
        switch (valueTypeToDeclaredType(args[0]))
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

    std::string substring;
    if (std::holds_alternative<std::string>(args[1]))
    {
        substring = std::get<std::string>(args[1]);
    }
    else
    {
        std::stringstream ss;
        ss << "Function 'contains': Argument 2 ($substring) must be a string, but got value of type ";
        switch (valueTypeToDeclaredType(args[1]))
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

    return str.find(substring) != std::string::npos;
}

void register_contains_extension(Evaluator &eval)
{
    eval.registerNativeFunction("contains", mcl_contains);
}