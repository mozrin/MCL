#include "max.h"
#include <cmath>
#include <stdexcept>
#include <variant>
#include <sstream>

Value mcl_max(const std::vector<Value> &args)
{
    if (args.empty())
    {
        throw std::runtime_error("Function 'max' expects at least 1 argument.");
    }

    Value current_max = args[0];
    
    if (!std::holds_alternative<long long>(current_max) && !std::holds_alternative<double>(current_max))
    {
        std::stringstream ss;
        ss << "Function 'max': All arguments must be numbers (integer or number), but argument 1 got value of type ";
        switch (valueTypeToDeclaredType(args[0]))
        {
        case DeclaredType::STRING:
            ss << "string";
            break;
        case DeclaredType::BOOLEAN:
            ss << "boolean";
            break;
        case DeclaredType::ANY:
            ss << "null";
            break;
        default:
            ss << "unknown";
            break;
        }
        ss << ".";
        throw std::runtime_error(ss.str());
    }

    bool result_is_double = std::holds_alternative<double>(current_max);
    double current_max_d = result_is_double ? std::get<double>(current_max) : static_cast<double>(std::get<long long>(current_max));

    for (size_t i = 1; i < args.size(); ++i)
    {
        const Value &next_val = args[i];
        
        double next_val_d;
        if (std::holds_alternative<long long>(next_val))
        {
            next_val_d = static_cast<double>(std::get<long long>(next_val));
        }
        else if (std::holds_alternative<double>(next_val))
        {
            next_val_d = std::get<double>(next_val);
            result_is_double = true;
        }
        else
        {
            std::stringstream ss;
            ss << "Function 'max': All arguments must be numbers (integer or number), but argument " << (i + 1) << " got value of type ";
            switch (valueTypeToDeclaredType(next_val))
            {
            case DeclaredType::STRING:
                ss << "string";
                break;
            case DeclaredType::BOOLEAN:
                ss << "boolean";
                break;
            case DeclaredType::ANY:
                ss << "null";
                break;
            default:
                ss << "unknown";
                break;
            }
            ss << ".";
            throw std::runtime_error(ss.str());
        }

        if (next_val_d > current_max_d)
        {
            current_max_d = next_val_d;
            current_max = next_val;
            result_is_double = std::holds_alternative<double>(next_val);
        }
        else if (next_val_d == current_max_d && std::holds_alternative<double>(next_val))
        {
            result_is_double = true;
        }
    }
    
    if (result_is_double && std::holds_alternative<long long>(current_max)) {
        return static_cast<double>(std::get<long long>(current_max));
    }

    return current_max;
}

void register_max_extension(Evaluator &eval)
{
    eval.registerNativeFunction("max", mcl_max);
}