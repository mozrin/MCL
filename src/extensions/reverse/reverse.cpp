#include "reverse.h"
#include <string>
#include <vector>
#include <variant>
#include <stdexcept>
#include <sstream>
#include <algorithm>

Value mcl_reverse(const std::vector<Value> &args)
{
    if (args.empty())
    {
        throw std::runtime_error("Function 'reverse' expects at least 1 argument.");
    }

    std::string combined_string = "";
    for (size_t i = 0; i < args.size(); ++i)
    {
        if (std::holds_alternative<std::string>(args[i]))
        {
            combined_string += std::get<std::string>(args[i]);
        }
        else
        {
            std::stringstream ss;
            ss << "Function 'reverse': Argument " << (i + 1) << " must be a string, but got value of type ";
            switch (valueTypeToDeclaredType(args[i]))
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
    }

    std::reverse(combined_string.begin(), combined_string.end());

    return combined_string;
}

void register_reverse_extension(Evaluator &eval)
{
    eval.registerNativeFunction("reverse", mcl_reverse);
}